# Upstream audit: Loaram/TamaPoke_ko 3.9.3

Step 0 of the plan to rebase this repo on Loaram/TamaPoke_ko (a Korean fork of
DylanPDao/TamaPoke, itself a fork of socquique/TamaPoke) and translate it into
Hong Kong Traditional Chinese. No behaviour changes here.

Decisions: host the sprite packs like upstream (option a); drop 1.75C support;
saves need not stay compatible; use Hong Kong Pokédex names.

## Licence and attribution

- Code: MIT, © 2026 Quique Tortosa. Keep the `LICENSE` notice and the upstream
  `CREDITS.md` chain (socquique → DylanPDao → Loaram).
- Sprites: PMD SpriteCollab, CC BY-NC 4.0. Hosting them requires crediting
  them, keeping the non-commercial notice and carrying over
  `web/credits-galar-paldea.md` (per-species artists for Galar/Paldea).
- Badges: SteGriff/pokemon-badges, CC BY 3.0.
- Font: upstream bundles Galmuri11 (SIL OFL 1.1); not reusable for Chinese. This
  repo already has a Noto Sans CJK subset (`licenses/NotoSansCJK-OFL.txt`).
- Pokémon is © Nintendo / Game Freak / The Pokémon Company; keep the
  non-commercial, unofficial notice.
- `LEGAL.md` and the README here currently say the repo ships no sprites; they
  must be rewritten when sprites are hosted.

## Upstream layout (flat root, not `firmware/`)

`TamaPoke.ino`, `i18n.cpp/.h`, `dex.h`, `moves.h`, `forms*.h`, `save.cpp`,
`party.cpp`, `trade.cpp`, `web/` (installer, `manifest.json`, `packs.json`,
nine `sprites-*.pak` + `forms.pak`, about 470 MB), `tools/` (Python build and
check scripts), `localization/`, `docs/`, Android and Wear OS sources.

## Korean-only items to replace

| Item | Location | Count |
|---|---|---|
| UI strings | `localization/ko.json`, row 7 of `STRINGS` in `i18n.cpp` | 194 |
| Language enum / default | `i18n.h` (`LANG_KO`, `LANG_DEFAULT`) | 1 |
| Names (species, moves, types, regions, trainers, places) | `localization/names-ko.json`, `korean_names.h` | 1968 entries (1025 species, 695 moves, rest labels) |
| Font subset | `korean_font.h`, `tools/gen_korean_font.py`, `tools/fonts/Galmuri11.ttf` | 1 |
| Text renderer | `korean_text.h` | 1 |
| Checks | `tools/check_korean.py`, `gen_korean_names.py`, `font-coverage.json` | 3 |
| Installer page | `web/index.html`, `installer.js`, `packs.json` region names | all |
| Docs | `README*.md`, `docs/*.ko.md`, guide PDFs | many |

## Keep from this repo

- zh-HK names for species 1–151 (`firmware/generated/dex_names_hk.h`) and their
  moves and UI strings (`i18n_strings_hk.h`, `help_hk.h`).
- The `chinese_canvas` glyph approach and Noto subset pipeline, adapted to
  upstream's renderer.
- Hong Kong wording, legal notes, credits and third-party notices.
- Dropped: 1.75C support, the old firmware layout, Windows flasher, the
  `Build-Public.ps1` flow, and old save compatibility.

## Hong Kong names source (important)

PokeAPI, which upstream used for Korean, has no Hong Kong language: language 4
(`zh-hant`) is **Taiwan** (妙蛙種子, whereas Hong Kong uses 奇異種子). This repo's
151 names are already Hong Kong names, so PokeAPI Traditional Chinese cannot be
used for 152–1025, forms and moves without conversion errors.

From the build sandbox, Hong Kong sources (zh-yue Wikipedia, 52poke, Bulbapedia,
Wikidata, Fandom) are blocked; only GitHub raw and PokeAPI are reachable.

Options:
1. You provide a Hong Kong list (CSV/JSON of dex number → name, plus moves), or
   the allowlist is opened for a source such as zh-yue Wikipedia or Bulbapedia.
2. Fall back to PokeAPI zh-hant (Taiwan) for entries with no Hong Kong name,
   flagged in a report for manual fixing. Hong Kong names exist officially
   only for Gen 1 to about Gen 8; newer species and moves use Taiwan/mainland
   style in HK media, so the fallback is partly unavoidable.

I recommend 1 for species and 2 for the remainder. Please say which.

## Next

Step 1 (installer PR) after you approve this audit.
