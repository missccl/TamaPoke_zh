'use strict';

// Reads a zip the user already has on disk. Only the chosen entry is inflated,
// so picking a package does not unpack every sprite pack up front.
function findEocd(u8) {
  const min = Math.max(0, u8.length - 22 - 65535);
  for (let i = u8.length - 22; i >= min; i--) {
    if (u8[i] !== 0x50 || u8[i + 1] !== 0x4b || u8[i + 2] !== 0x05 || u8[i + 3] !== 0x06) continue;
    const commentLen = u8[i + 20] | (u8[i + 21] << 8);
    if (i + 22 + commentLen === u8.length) return i;
  }
  throw new Error('這不是 zip 壓縮檔。');
}

export function openZip(input) {
  const raw = input instanceof Uint8Array ? input : new Uint8Array(input);
  // Copy so a Node Buffer or a subarray cannot hand back the wrong underlying bytes.
  const u8 = new Uint8Array(raw);
  if (u8.length < 22) throw new Error('這不是 zip 壓縮檔。');
  const dv = new DataView(u8.buffer);
  const eocd = findEocd(u8);
  if (dv.getUint16(eocd + 4, true) !== 0 || dv.getUint16(eocd + 6, true) !== 0) {
    throw new Error('不支援分成多段的 zip。');
  }
  const count = dv.getUint16(eocd + 10, true);
  const cdOff = dv.getUint32(eocd + 16, true);
  if (count === 0xffff || cdOff === 0xffffffff) throw new Error('不支援 ZIP64。');
  const entries = new Map();
  let p = cdOff;
  for (let i = 0; i < count; i++) {
    if (p + 46 > u8.length || dv.getUint32(p, true) !== 0x02014b50) throw new Error('壓縮檔目錄已損毀。');
    const flags = dv.getUint16(p + 8, true);
    const method = dv.getUint16(p + 10, true);
    const compSize = dv.getUint32(p + 20, true);
    const size = dv.getUint32(p + 24, true);
    const nameLen = dv.getUint16(p + 28, true);
    const extraLen = dv.getUint16(p + 30, true);
    const commentLen = dv.getUint16(p + 32, true);
    const localOff = dv.getUint32(p + 42, true);
    if (compSize === 0xffffffff || size === 0xffffffff || localOff === 0xffffffff) throw new Error('不支援 ZIP64。');
    const name = new TextDecoder('utf-8', {fatal: false}).decode(u8.subarray(p + 46, p + 46 + nameLen)).replaceAll('\\', '/');
    p += 46 + nameLen + extraLen + commentLen;
    if (!name || name.endsWith('/')) continue;
    if (flags & 1) throw new Error('不支援加密的壓縮檔。');
    if (method !== 0 && method !== 8) throw new Error(`不支援的壓縮方式（${name}）。`);
    entries.set(name, {method, compSize, size, localOff});
  }
  return {
    names: [...entries.keys()],
    size(name) { return entries.get(name)?.size ?? 0; },
    async read(name) {
      const entry = entries.get(name);
      if (!entry) throw new Error(`找不到 ${name}。`);
      const local = entry.localOff;
      if (local + 30 > u8.length || dv.getUint32(local, true) !== 0x04034b50) throw new Error('壓縮檔內容已損毀。');
      const nameLen = dv.getUint16(local + 26, true);
      const extraLen = dv.getUint16(local + 28, true);
      const start = local + 30 + nameLen + extraLen;
      const end = start + entry.compSize;
      if (end > u8.length) throw new Error('壓縮檔被截斷。');
      if (entry.method === 0) return u8.slice(start, end).buffer;
      if (typeof DecompressionStream !== 'function') throw new Error('這個瀏覽器不能解壓 zip。');
      const stream = new Blob([u8.subarray(start, end)]).stream().pipeThrough(new DecompressionStream('deflate-raw'));
      const out = await new Response(stream).arrayBuffer();
      if (out.byteLength !== entry.size) throw new Error(`${name} 解壓後大小不符。`);
      return out;
    },
  };
}
