#!/usr/bin/env python3
"""Build localization/names-zh.json (display names keyed by the English names
in dex.h / moves.h plus the labels in names-zh-manual.json).

Species 1-151 use names-zh-hk-gen1.json (Hong Kong); the rest use PokeAPI
Traditional Chinese (zh-hant, Taiwan wording). names-zh-manual.json overrides
both. Anything still untranslated is written to names-zh-missing.txt.
Then run tools/gen_zh_header.py.
"""
import csv, io, json, re, sys, urllib.request
from pathlib import Path

R = Path(__file__).resolve().parents[1]
API = 'https://raw.githubusercontent.com/PokeAPI/pokeapi/master/data/v2/csv/'
ZH_HANT = '4'

def table(name):
    with urllib.request.urlopen(API + name, timeout=60) as r:
        return list(csv.DictReader(io.StringIO(r.read().decode('utf-8'))))

def norm(s):
    return re.sub(r'[^a-z0-9]', '', s.lower().replace('♀', 'f').replace('♂', 'm'))

dex = re.findall(r'\{ "([^"]+)"', (R / 'dex.h').read_text(encoding='utf-8').split('DEX_TBL[DEX_COUNT + 1]')[1].split('\n};')[0])
moves = re.findall(r'\{ "([^"]+)"', (R / 'moves.h').read_text(encoding='utf-8').split('MOVE_TBL')[1].split('\n};')[0])
manual = json.loads((R / 'localization/names-zh-manual.json').read_text(encoding='utf-8'))
keys = [k for k in dex[1:] + moves if k not in ('-', '?', 'NONE')] + list(manual)

sp = {int(r['pokemon_species_id']): r['name'] for r in table('pokemon_species_names.csv') if r['local_language_id'] == ZH_HANT}
mv_id = {norm(r['identifier']): r['id'] for r in table('moves.csv')}
mv = {r['move_id']: r['name'] for r in table('move_names.csv') if r['local_language_id'] == ZH_HANT}

hk = json.loads((R / 'localization/names-zh-hk-gen1.json').read_text(encoding='utf-8'))
out = {}
for i, k in enumerate(dex):
    if i == 0:
        continue
    n = hk[i - 1] if i <= 151 else sp.get(i)
    if n:
        out[k] = n
for k in moves:
    m = mv.get(mv_id.get(norm(k), ''))
    if m:
        out.setdefault(k, m)
# Manual names fill gaps and override PokeAPI where the 12px font lacks a glyph (戽, 獒).
out.update(json.loads((R / 'localization/names-zh-manual.json').read_text(encoding='utf-8')))
missing = [k for k in keys if k not in out]
(R / 'localization/names-zh.json').write_text(json.dumps({k: out[k] for k in keys if k in out}, ensure_ascii=False, indent=1), encoding='utf-8')
(R / 'localization/names-zh-missing.txt').write_text('\n'.join(missing) + '\n', encoding='utf-8')
print(len(out), 'translated;', len(missing), 'missing')
