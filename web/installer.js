'use strict';
const el = id => document.getElementById(id);
const enc = new TextEncoder();
const regionNames = {kanto:'關都',johto:'城都',hoenn:'豐緣',sinnoh:'神奧',unova:'合眾',kalos:'卡洛斯',alola:'阿羅拉',galar:'伽勒爾',paldea:'帕底亞'};
regionNames.forms='形態變化';
let port, reader, writer, pendingRead, decoder = new TextDecoder(), lineBuf = '', busy = false, firmwareReady = false;
const regionButtons = [...document.querySelectorAll('[data-region]')];
function log(message) { el('log').style.display='block'; el('log').textContent=(el('log').textContent+message+'\n').split('\n').slice(-301).join('\n'); el('log').scrollTop=el('log').scrollHeight; }
function status(message) { el('status').textContent=message; }
function controls() {
  const ready=!!writer && !busy;
  el('connect').disabled=!!port || busy || !('serial' in navigator) || !isSecureContext;
  el('disconnect').disabled=!port || busy;
  el('all').disabled=!ready;
  el('files').disabled=!ready;
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
async function loadRegion(region) {
  const name=regionNames[region];if(!name) throw new Error('未知地區。');
  status(`正在下載${name}精靈圖…`);
  // Bundles stay on the same origin. GitHub release downloads do not provide
  // the CORS headers needed by the browser, so there is no unreliable fallback.
  const resp=await fetch(region==='forms'?'forms.pak':`sprites-${region}.pak`);
  if(!resp.ok) throw new Error(`無法下載${name}包。（HTTP ${resp.status}）`);
  await sendAll(parsePak(await resp.arrayBuffer()),name);
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
el('files').onchange=event=>runInstall(async()=>{
  const items=[];for(const f of event.target.files) items.push({name:'mons/'+f.name,data:new Uint8Array(await f.arrayBuffer())});
  if(!items.length) throw new Error('請選擇檔案。');await sendAll(items,'自選檔案');
});
if(!('serial' in navigator)||!isSecureContext) el('unsupported').style.display='block';
if('serial' in navigator) navigator.serial.addEventListener('disconnect',async()=>{await disconnect();status('USB 連線已中斷，請重新連接。');});
controls();
Promise.all(['manifest.json','firmware/build-info.json'].map(url=>fetch(url).then(r=>{if(!r.ok)throw Error();return r.json();})))
  .then(([m,info])=>{
    if(!/^(?:\d+\.\d+\.\d+|zh\.\d+\.\d+\.\d+[a-z]?|\d+\.\d+-zh\.\d+)$/.test(m.version) || info.version!==m.version || m.new_install_prompt_erase!==true) throw Error();
    const expected=[['firmware/bootloader.bin',0],['firmware/partitions.bin',32768],['firmware/boot_app0.bin',57344],['firmware/app.bin',65536]];
    const parts=m.builds?.[0]?.parts;
    if(!parts || parts.length!==4 || parts.some((p,i)=>p.path!==expected[i][0]||p.offset!==expected[i][1])) throw Error();
    el('version').textContent='v'+m.version;firmwareReady=true;controls();
  }).catch(()=>{el('version').textContent='需要確認版本';status('沒有經驗證的韌體資訊，無法啟用安裝。');controls();});
