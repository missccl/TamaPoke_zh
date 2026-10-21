#!/usr/bin/env python3
"""Validate the web installer. Usage: check_web.py [SITE_DIR]

Without SITE_DIR only the page sources are checked (web/). With the assembled
site, the manifest, firmware parts and sprite packs are verified too.
"""
import json, re, struct, sys
from pathlib import Path

R = Path(__file__).resolve().parents[1]
W = Path(sys.argv[1]) if len(sys.argv) > 1 else R / 'web'
full = len(sys.argv) > 1
html = (W / 'index.html').read_text(encoding='utf-8')
js = (W / 'installer.js').read_text(encoding='utf-8') + '\n' + (W / 'zip.js').read_text(encoding='utf-8')
assert '<html lang="zh-HK">' in html and 'TamaPoke 香港繁體中文版' in html
assert 'Erase device' in html and 'sprites-${region}.pak' in js and "from './zip.js'" in js
assert 'id="package"' in html and 'id="folder"' in html and "cache:'no-store'" in js
assert not re.search('[\uac00-\ud7a3]', html + js), 'Korean text left in installer'
for i in ('connect', 'disconnect', 'all', 'files', 'firmware', 'version', 'status', 'progress', 'log', 'unsupported'):
    assert f'id="{i}"' in html, i
packs = json.loads((W / 'packs.json').read_text(encoding='utf-8'))
assert len(packs) == 9
regions = {p['id'] for p in packs}
assert regions == set(re.findall(r'data-region="(\w+)"', html)) - {'forms'}
if full:
    m = json.loads((W / 'manifest.json').read_text(encoding='utf-8'))
    info = json.loads((W / 'firmware/build-info.json').read_text(encoding='utf-8'))
    assert m['version'] == info['version'] and m.get('new_install_prompt_erase') is True
    for part in m['builds'][0]['parts']:
        assert (W / part['path']).is_file(), part['path']
    for p in packs + [{'path': 'forms.pak'}]:
        b = (W / p['path']).read_bytes()
        assert b[:4] == b'TPAK', p['path']
        if 'bytes' in p:
            assert len(b) == p['bytes'], p['path']
        count = struct.unpack_from('<H', b, 4)[0]
        off, total = 6, 0
        for _ in range(count):
            n = b[off]; off += 1
            assert re.fullmatch(r'mons/[A-Za-z0-9_.-]+\.bin', b[off:off + n].decode('ascii'))
            off += n
            total += struct.unpack_from('<I', b, off)[0]; off += 4
        assert off + total == len(b), p['path']
print('PASS: installer page' + (', manifest, firmware and sprite packs' if full else ''))
