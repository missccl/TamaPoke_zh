#!/usr/bin/env python3
"""Build localization/names-zh.json from upstream display keys.

Usage: gen_zh_names.py UPSTREAM_DIR  (a Loaram/TamaPoke_ko checkout)
Species 1-151 keep this repo's Hong Kong names; everything else uses PokeAPI
Traditional Chinese (zh-hant, Taiwan wording). Entries without a name are
written to localization/names-zh-missing.txt for manual translation.
"""
import csv, io, json, re, sys, urllib.request
from pathlib import Path

R = Path(__file__).resolve().parents[1]
U = Path(sys.argv[1])
API = 'https://raw.githubusercontent.com/PokeAPI/pokeapi/master/data/v2/csv/'
ZH_HANT = '4'

def table(name):
    with urllib.request.urlopen(API + name, timeout=60) as r:
        return list(csv.DictReader(io.StringIO(r.read().decode('utf-8'))))

def norm(s):
    return re.sub(r'[^a-z0-9]', '', s.lower().replace('♀', 'f').replace('♂', 'm'))

keys = list(json.loads((U / 'localization/names-ko.json').read_text(encoding='utf-8')))
dex = re.findall(r'\{ "([^"]+)"', (U / 'dex.h').read_text(encoding='utf-8').split('DEX_TBL[DEX_COUNT + 1]')[1].split('\n};')[0])
moves = re.findall(r'\{ "([^"]+)"', (U / 'moves.h').read_text(encoding='utf-8').split('MOVE_TBL')[1].split('\n};')[0])

sp = {int(r['pokemon_species_id']): r['name'] for r in table('pokemon_species_names.csv') if r['local_language_id'] == ZH_HANT}
mv_id = {norm(r['identifier']): r['id'] for r in table('moves.csv')}
mv = {r['move_id']: r['name'] for r in table('move_names.csv') if r['local_language_id'] == ZH_HANT}

hk = re.findall(r'"([^"]+)"', (R / 'firmware/generated/dex_names_hk.h').read_text(encoding='utf-8').split('= {')[1])[1:152]
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
MANUAL = {
    'HI JUMP KICK': '飛膝踢', 'DAZZLE GLEAM': '魔法閃耀', 'VISE GRIP': '夾住',
    'DRAIN KISS': '吸取之吻', 'MAGIC LEAF': '魔法葉', 'MALIGN CHAIN': '邪毒鎖鏈',
    'NORMAL': '一般', 'FIRE': '火', 'WATER': '水', 'ELECTRIC': '電', 'GRASS': '草',
    'ICE': '冰', 'FIGHTING': '格鬥', 'POISON': '毒', 'GROUND': '地面', 'FLYING': '飛行',
    'BUG': '蟲', 'ROCK': '岩石', 'GHOST': '幽靈', 'DRAGON': '龍', 'DARK': '惡',
    'STEEL': '鋼', 'FAIRY': '妖精',
    'KANTO': '關都', 'JOHTO': '城都', 'HOENN': '豐緣', 'SINNOH': '神奧', 'UNOVA': '合眾',
    'KALOS': '卡洛斯', 'ALOLA': '阿羅拉', 'GALAR': '伽勒爾', 'PALDEA': '帕底亞',
    'ALL': '全部', 'OK': '確定', 'tap...': '點按…', 'STAB': '本系加成',
    'CHAMPION': '冠軍', 'ELITE 4': '四天王', 'RIVAL': '對手', 'SWORD': '劍', 'SHIELD': '盾',
}
for k, v in MANUAL.items():
    out.setdefault(k, v)
out.update({k: v for k, v in json.loads((R / 'localization/names-zh-manual.json').read_text(encoding='utf-8')).items() if k not in out})
missing = [k for k in keys if k not in out]
(R / 'localization/names-zh.json').write_text(json.dumps({k: out[k] for k in keys if k in out}, ensure_ascii=False, indent=1), encoding='utf-8')
(R / 'localization/names-zh-missing.txt').write_text('\n'.join(missing) + '\n', encoding='utf-8')
print(len(out), 'translated;', len(missing), 'missing')
