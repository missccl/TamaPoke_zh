'use strict';
import { openZip } from './zip.js';
const el = id => document.getElementById(id);
const enc = new TextEncoder();
const regionNames = {kanto:'關都',johto:'城都',hoenn:'豐緣',sinnoh:'神奧',unova:'合眾',kalos:'卡洛斯',alola:'阿羅拉',galar:'伽勒爾',paldea:'帕底亞'};
regionNames.forms='形態變化';
let port, reader, writer, pendingRead, decoder = new TextDecoder(), lineBuf = '', busy = false, firmwareReady = false;
let usingLocalFirmware = false, blobUrls = [];
const localPak = new Map();
const VERSION_RE = /^(?:\d+\.\d+\.\d+|zh\.\d+\.\d+\.\d+[a-z]?|\d+\.\d+-zh\.\d+)$/;
const EXPECTED_PARTS = [['firmware/bootloader.bin',0],['firmware/partitions.bin',32768],['firmware/boot_app0.bin',57344],['firmware/app.bin',65536]];
const regionButtons = [...document.querySelectorAll('[data-region]')];
const defaultLabel = new Map(regionButtons.map(button => [button.dataset.region, button.textContent]));
function log(message) { el('log').style.display='block'; el('log').textContent=(el('log').textContent+message+'\n').split('\n').slice(-301).join('\n'); el('log').scrollTop=el('log').scrollHeight; }
function status(message) { el('status').textContent=message; }
function controls() {
  const ready=!!writer && !busy;
  el('connect').disabled=!!port || busy || !('serial' in navigator) || !isSecureContext;
  el('disconnect').disabled=!port || busy;
  el('all').disabled=!ready;
  el('files').disabled=!ready;
  el('package').disabled=busy;
  el('folder').disabled=busy;
  el('local-sprites').disabled=busy;
  el('firmware').disabled=!firmwareReady || !!port || busy;
  for(const button of regionButtons) button.disabled=!ready || button.dataset.missing==='true';
}
async function disconnect() {
  const oldReader=reader, oldWriter=writer, oldPort=port;
  reader=writer=port=undefined; pendingRead=undefined; lineBuf=''; decoder=new TextDecoder();
  try { if(oldReader) { await oldReader.cancel(); oldReader.releaseLock(); } } catch (_) {}
  try { if(oldWriter) oldWriter.releaseLock(); } catch (_) {}
  try { if(oldPort) await oldPort.close(); } catch (_) {}
  controls();
}
async function readLine(timeoutMs=6000) {
  const deadline=Date.now()+timeoutMs;
  while(true) {
    const nl=lineBuf.indexOf('\n');
    if(nl>=0) { const line=lineBuf.slice(0,nl).trim();lineBuf=lineBuf.slice(nl+1);return line; }
    const remaining=deadline-Date.now();
    if(remaining<=0) throw new Error('裝置回應逾時，請重新連接。');
    if(!reader) throw new Error('裝置連線已中斷。');
    // Retain one read across waits. A timer must not leave competing reads that
    // consume a later ACK; disconnect cancels the outstanding read on failure.
    pendingRead ||= reader.read();
    let timer;
    try {
      const result=await Promise.race([pendingRead,new Promise((_,reject)=>{timer=setTimeout(()=>reject(new Error('裝置回應逾時。')),remaining);})]);
      pendingRead=undefined;
      if(result.done) throw new Error('裝置連線已中斷。');
      lineBuf+=decoder.decode(result.value,{stream:true});
      if(lineBuf.length>65536) throw new Error('裝置回應過長，請重新連接。');
    } finally { clearTimeout(timer); }
  }
}
async function waitFor(token,timeoutMs=6000) {
  const deadline=Date.now()+timeoutMs;
  while(Date.now()<deadline) {
    const line=await readLine(deadline-Date.now());
    if(line===token) return;
    if(line==='ERR' || line.startsWith('ERR ')) {
      const reasons={SD_NOT_READY:'裝置未能識別 microSD 卡。',
        INVALID_PATH:'不允許的檔案路徑。',INVALID_SIZE:'不允許的檔案大小。',
        INVALID_COMMAND:'裝置無法解析檔案傳輸指令。',
        OPEN_WRITE:'無法在 SD 卡開啟檔案，請檢查卡片連接、剩餘空間和檔案系統。',
        RX_TIMEOUT:'USB 資料未能在限時內全部送達，請檢查資料線、連接和瀏覽器狀態。',
        WRITE_SHORT:'未能把指定大小寫入 SD 卡，請檢查剩餘空間、卡片連接和檔案系統。',
        VERIFY_OPEN:'無法重新開啟已儲存的檔案。',VERIFY_SIZE:'重新讀取的檔案大小與傳輸大小不符。',
        VERIFY_READ:'無法讀完已儲存的檔案。',VERIFY_CRC:'重新讀取的檔案內容與裝置收到的資料不符。'};
      const code=line.split(/\s+/)[1];
      throw new Error((reasons[code]||'裝置拒絕了傳輸，單憑此回應無法判斷原因。')+` [裝置回應：${line}]`);
    }
    if(line) log(`[裝置] ${line.slice(0,1000)}`);
  }
  throw new Error('裝置回應逾時。');
}
function validName(name) { return /^mons\/[A-Za-z0-9_.-]+\.bin$/.test(name); }
function parsePak(buf) {
  if(buf.byteLength<6) throw new Error('精靈圖包已損毀。');
  const dv=new DataView(buf), td=new TextDecoder('utf-8',{fatal:true});
  if(td.decode(new Uint8Array(buf,0,4))!=='TPAK') throw new Error('不支援的精靈圖包。');
  const count=dv.getUint16(4,true);let off=6;const items=[],seen=new Set();
  if(!count) throw new Error('精靈圖包是空的。');
  for(let i=0;i<count;i++) {
    if(off>=buf.byteLength) throw new Error('精靈圖清單已損毀。');
    const nl=dv.getUint8(off++);
    if(!nl || off+nl+4>buf.byteLength) throw new Error('精靈圖清單已損毀。');
    const name=td.decode(new Uint8Array(buf,off,nl));off+=nl;
    const size=dv.getUint32(off,true);off+=4;
    if(!validName(name)||seen.has(name)||!size) throw new Error('精靈圖檔案資訊不正確。');
    seen.add(name);items.push({name,size});
  }
  for(const it of items) {
    if(off+it.size>buf.byteLength) throw new Error('精靈圖資料被截斷。');
    it.data=new Uint8Array(buf,off,it.size);off+=it.size;
  }
  if(off!==buf.byteLength) throw new Error('精靈圖包大小不符。');
  return items;
}
async function sendOne(name,data,onBytes) {
  if(!validName(name)||!data.length) throw new Error('請選擇有效的 .bin 檔案。');
  let acknowledged=0, phase='開啟檔案';
  try {
    await writer.write(enc.encode(`PUT ${name} ${data.length}\n`));
    await waitFor('OK',20000);
    for(let i=0;i<data.length;i+=2048) {
      phase=`寫入資料 · 區塊起點 ${i} 位元組`;
      const chunk=data.subarray(i,i+2048);await writer.write(chunk);await waitFor('#',20000);
      acknowledged+=chunk.length;onBytes(chunk.length);
    }
    phase='確認儲存完成';
    await waitFor('DONE',30000);
  } catch(e) {
    throw new Error(`${name} · ${phase} · 裝置已確認 ${acknowledged}/${data.length} 位元組\n${e.message}`);
  }
}
async function sendAll(items,label) {
  const total=items.reduce((n,it)=>n+it.data.length,0);let sent=0;
  el('progress').hidden=false;el('progress').value=0;
  for(const [i,it] of items.entries()) {
    const current=`${label} · ${i+1}/${items.length} · ${it.name}`;
    status(`${current} · 開啟檔案`);
    log(`${current} · 開始傳輸 ${it.data.length} 位元組`);
    // Stop immediately on a failed ACK; continuing the binary stream can
    // desynchronize PUT and falsely report a successful install.
    await sendOne(it.name,it.data,n=>{
      sent+=n;const percent=Math.floor(sent/total*100);el('progress').value=percent;
      status(`${current} · 正傳輸到 SD 卡 ${percent}%`);
    });
  }
  log(`${label}：已安裝 ${items.length} 個檔案`);
}
async function runInstall(action) {
  if(busy||!writer) return;
  busy=true;controls();
  try { await action();status('安裝完成！請中斷連接並重新啟動裝置。'); }
  catch(e) { status(`安裝已中止。${e.message.split('\n')[0]}`);log(e.message);log('不會自動重送。請保留錯誤內容並重新連接，切勿格式化 SD 卡或刪除存檔。');await disconnect(); }
  finally { busy=false;controls(); }
}
function sibling(manifestName, rel) {
  const slash = manifestName.lastIndexOf('/');
  return (slash >= 0 ? manifestName.slice(0, slash + 1) : '') + rel;
}
function pickManifest(names) {
  const hits = names.filter(name => name === 'manifest.json' || name.endsWith('/manifest.json'));
  if (hits.length === 1) return hits[0];
  if (!hits.length) throw new Error('這個套件沒有 manifest.json。請選擇 TamaPoke-Web-Installer zip，或解壓後含有 manifest.json 的資料夾。');
  throw new Error('找到多於一個 manifest.json。請只選擇最新的資料夾，不要選包含舊檔的上層資料夾。');
}
function assertManifest(m, info) {
  if (!VERSION_RE.test(m.version) || info.version !== m.version || m.new_install_prompt_erase !== true) {
    throw new Error('這份套件的版本資料不完整，已停止安裝。');
  }
  const parts = m.builds?.[0]?.parts;
  if (!parts || parts.length !== 4 || parts.some((part, i) => part.path !== EXPECTED_PARTS[i][0] || part.offset !== EXPECTED_PARTS[i][1])) {
    throw new Error('這份套件的韌體配置不正確，已停止安裝。');
  }
}
async function sha256(buffer) {
  const bytes = new Uint8Array(await crypto.subtle.digest('SHA-256', buffer));
  let hex = '';
  for (const b of bytes) hex += b.toString(16).padStart(2, '0');
  return hex;
}
function openFileList(files) {
  const list = [...files];
  const names = list.map(file => (file.webkitRelativePath || file.name).replaceAll('\\', '/'));
  return {
    names,
    size(name) { const i = names.indexOf(name); return i < 0 ? 0 : list[i].size; },
    async read(name) {
      const i = names.indexOf(name);
      if (i < 0) throw new Error(`找不到 ${name}。`);
      return list[i].arrayBuffer();
    },
  };
}
async function usePackage(pkg, label) {
  const manifestName = pickManifest(pkg.names);
  const manifest = JSON.parse(new TextDecoder().decode(await pkg.read(manifestName)));
  const info = JSON.parse(new TextDecoder().decode(await pkg.read(sibling(manifestName, 'firmware/build-info.json'))));
  assertManifest(manifest, info);
  const created = [];
  try {
    const flash = structuredClone(manifest);
    for (let i = 0; i < EXPECTED_PARTS.length; i++) {
      const rel = EXPECTED_PARTS[i][0];
      const data = await pkg.read(sibling(manifestName, rel));
      const base = rel.slice(rel.lastIndexOf('/') + 1);
      const meta = info.files?.[base];
      if (!meta || data.byteLength !== meta.size || (meta.sha256 && await sha256(data) !== meta.sha256)) {
        throw new Error(`${base} 和版本資料不符，已停止安裝。`);
      }
      const url = URL.createObjectURL(new Blob([data], {type: 'application/octet-stream'}));
      created.push(url);
      flash.builds[0].parts[i].path = url;
    }
    const manifestUrl = URL.createObjectURL(new Blob([JSON.stringify(flash)], {type: 'application/json'}));
    created.push(manifestUrl);
    for (const old of blobUrls) URL.revokeObjectURL(old);
    blobUrls = created;
    document.querySelector('esp-web-install-button').setAttribute('manifest', manifestUrl);
    localPak.clear();
    for (const button of regionButtons) {
      const region = button.dataset.region;
      const rel = region === 'forms' ? 'forms.pak' : `sprites-${region}.pak`;
      const full = sibling(manifestName, rel);
      if (!pkg.names.includes(full)) {
        button.dataset.missing = 'true';
        button.textContent = `${regionNames[region]} · 套件裡沒有`;
        continue;
      }
      button.dataset.missing = 'false';
      const bytes = pkg.size(full);
      button.textContent = `${regionNames[region]} · ${(bytes / 1e6).toFixed(1)}MB`;
      localPak.set(region, () => pkg.read(full));
    }
    usingLocalFirmware = true;
    firmwareReady = true;
    el('version').textContent = 'v' + manifest.version;
    el('package-name').textContent = `將燒錄：${label}（${manifestName}）`;
    el('sprite-source').textContent = `精靈圖來自本機：${label}`;
    status('已改用你選擇的套件。安裝韌體後，連接裝置即可把精靈圖寫進 microSD。');
    log(`已載入 ${label}：${manifestName}`);
    controls();
  } catch (error) {
    for (const url of created) URL.revokeObjectURL(url);
    throw error;
  }
}
async function loadPicked(action) {
  if (busy) return;
  busy = true; controls();
  try { await action(); }
  catch (error) { status(error.message); log(error.message); }
  finally { busy = false; controls(); }
}
function regionFromPakName(name) {
  const base = name.split(/[/\\]/).pop();
  if (base.toLowerCase() === 'forms.pak') return 'forms';
  const match = /^sprites-([a-z]+)\.pak$/i.exec(base);
  if (!match) return '';
  const id = match[1].toLowerCase();
  return regionNames[id] ? id : '';
}
function rememberPak(region, size, read) {
  const button = regionButtons.find(item => item.dataset.region === region);
  button.dataset.missing = 'false';
  button.textContent = `${regionNames[region]} · 本機 ${(size / 1e6).toFixed(1)}MB`;
  localPak.set(region, read);
}
async function useLocalPaks(files) {
  const skipped = [];
  localPak.clear();
  for (const button of regionButtons) {
    button.dataset.missing = 'true';
    button.textContent = defaultLabel.get(button.dataset.region);
  }
  for (const file of files) {
    const region = regionFromPakName(file.name);
    if (!region) skipped.push(file.name);
    else rememberPak(region, file.size, () => file.arrayBuffer());
  }
  if (!localPak.size) throw new Error('請選擇 sprites-地區.pak 或 forms.pak。');
  if (skipped.length) log(`略過：${skipped.join('、')}`);
  const names = [...localPak.keys()].map(id => regionNames[id]).join('、');
  el('sprite-source').textContent = `將從本機安裝：${names}`;
  status('已讀到本機精靈圖。連接裝置後即可寫進 microSD，不會再從網頁下載。');
  log(`已選擇本機精靈圖：${names}`);
}
async function loadRegion(region) {
  const name=regionNames[region];if(!name) throw new Error('未知地區。');
  let buf;
  const read = localPak.get(region);
  if (read) {
    status(`正在從本機讀取${name}…`);
    buf = await read();
  } else if (localPak.size) {
    throw new Error(`本機沒有${name}。請把該 .pak 一併選入。`);
  } else {
    status(`正在下載${name}精靈圖…`);
    // Bundles stay on the same origin. GitHub release downloads do not provide
    // the CORS headers needed by the browser, so there is no unreliable fallback.
    const resp=await fetch(region==='forms'?'forms.pak':`sprites-${region}.pak`, {cache:'no-store'});
    if(!resp.ok) throw new Error(`無法下載${name}包。（HTTP ${resp.status}）`);
    buf = await resp.arrayBuffer();
  }
  await sendAll(parsePak(buf),name);
}
el('connect').onclick=async()=>{
  if(port||busy) return;
  busy=true;controls();status('請選擇要連接的裝置。');
  try {
    port=await navigator.serial.requestPort();await port.open({baudRate:115200});
    reader=port.readable.getReader();writer=port.writable.getWriter();lineBuf='';
    status('已連接。請選擇要安裝的地區。');log('裝置已連接');
  } catch(e) { await disconnect();status(e.name==='NotFoundError'?'已取消選擇裝置。':'無法連接。請關閉其他安裝視窗後再試。');log(e.message); }
  finally {busy=false;controls();}
};
el('disconnect').onclick=async()=>{await disconnect();status('已中斷連接。請重新啟動裝置。');};
for(const button of regionButtons) button.onclick=()=>runInstall(()=>loadRegion(button.dataset.region));
el('all').onclick=()=>runInstall(async()=>{for(const b of regionButtons) {if(b.dataset.missing==='true') throw new Error(`找不到${regionNames[b.dataset.region]}包。`);await loadRegion(b.dataset.region);}});
el('package').onchange=event=>{
  const file=event.target.files?.[0];
  event.target.value='';
  if(!file) return;
  loadPicked(async()=>{ status('正在讀取壓縮檔…'); await usePackage(openZip(await file.arrayBuffer()), file.name); });
};
el('folder').onchange=event=>{
  const files=[...event.target.files];
  event.target.value='';
  if(!files.length) return;
  const label=(files[0].webkitRelativePath||files[0].name).split(/[\\/]/)[0]||'資料夾';
  loadPicked(()=>usePackage(openFileList(files), label));
};
el('local-sprites').onchange=event=>{
  const files=[...event.target.files];
  event.target.value='';
  if(!files.length) return;
  loadPicked(async()=>{
    const zips=files.filter(file=>/\.zip$/i.test(file.name));
    const paks=files.filter(file=>/\.pak$/i.test(file.name));
    if(zips.length===1 && files.length===1){
      status('正在讀取已下載的壓縮檔…');
      await usePackage(openZip(await zips[0].arrayBuffer()), zips[0].name);
      return;
    }
    if(zips.length) throw new Error('請只選一個 zip，或只選多個 .pak，不要混在一起。');
    if(!paks.length) throw new Error('請選擇 .pak 精靈圖包，或已下載的安裝 zip。');
    await useLocalPaks(paks);
  });
};
el('files').onchange=event=>runInstall(async()=>{
  const items=[];
  for(const f of event.target.files){
    if(/\.pak$/i.test(f.name)) items.push(...parsePak(await f.arrayBuffer()));
    else items.push({name:'mons/'+f.name,data:new Uint8Array(await f.arrayBuffer())});
  }
  if(!items.length) throw new Error('請選擇檔案。');await sendAll(items,'自選檔案');
});
if(!('serial' in navigator)||!isSecureContext) el('unsupported').style.display='block';
if('serial' in navigator) navigator.serial.addEventListener('disconnect',async()=>{await disconnect();status('USB 連線已中斷，請重新連接。');});
controls();
async function loadHosted() {
  let release = null;
  try {
    const response = await fetch('release.json', {cache:'no-store'});
    if (response.ok) release = await response.json();
  } catch (_) {}
  try {
    const [manifest, info] = await Promise.all(['manifest.json','firmware/build-info.json'].map(url =>
      fetch(url, {cache:'no-store'}).then(r => { if (!r.ok) throw Error(); return r.json(); })));
    if (usingLocalFirmware) return;
    assertManifest(manifest, info);
    el('version').textContent = 'v' + manifest.version;
    firmwareReady = true;
    const tag = release && /^[A-Za-z0-9._-]+$/.test(release.tag) ? release.tag : '';
    if (tag) {
      const link = document.createElement('a');
      link.href = 'https://github.com/missccl/TamaPoke_zh/releases/tag/' + tag;
      link.rel = 'noopener noreferrer';
      link.textContent = tag;
      el('package-name').textContent = '線上套件：GitHub Release ';
      el('package-name').append(link);
    } else {
      el('package-name').textContent = '線上套件已就緒。';
    }
    status('可以安裝韌體。完成後插入 microSD，連接裝置，再安裝全部地區和形態。');
    controls();
  } catch (_) {
    if (usingLocalFirmware) return;
    el('version').textContent = '需要確認版本';
    el('package-name').textContent = '網頁上沒有 GitHub Release 套件。請展開上方選項，改選電腦上的 zip。';
    status('網頁上沒有可用的韌體。請選擇電腦上已下載的 zip，或解壓後的資料夾。');
    controls();
  }
}
loadHosted();
