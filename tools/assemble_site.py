#!/usr/bin/env python3
"""Build the Pages site from web/ plus a GitHub release installer zip.

Release zips keep manifest.json, firmware/, sprites-*.pak and forms.pak.
Windows packs often nest that payload under one folder, or store backslash
paths. GitHub release downloads do not send the CORS headers a browser
needs, so the page and the packs have to be served from the same origin.
"""
import argparse, json, shutil, zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PAGE_FILES = {
    'index.html', 'installer.js', 'zip.js', 'packs.json', 'README.md',
    'credits-galar-paldea.md',
}

def payload(zip_path):
    with zipfile.ZipFile(zip_path) as z:
        stored = [(info, info.filename.replace('\\', '/')) for info in z.infolist()]
        manifests = [name for _, name in stored if name == 'manifest.json' or name.endswith('/manifest.json')]
        if len(manifests) != 1:
            raise SystemExit(f'expected one manifest.json, found {manifests or "none"}')
        prefix = manifests[0][:-len('manifest.json')]
        for info, name in stored:
            if not name.startswith(prefix) or name.endswith('/'):
                continue
            rel = name[len(prefix):]
            if rel in PAGE_FILES or rel == 'release.json':
                continue
            yield rel, z.read(info)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('zip_path')
    ap.add_argument('site')
    ap.add_argument('--tag', required=True)
    ap.add_argument('--web', default=str(ROOT / 'web'))
    args = ap.parse_args()
    site = Path(args.site)
    if site.exists():
        shutil.rmtree(site)
    shutil.copytree(args.web, site)
    wrote = []
    for rel, data in payload(args.zip_path):
        dest = site / rel
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(data)
        wrote.append(rel)
    if 'manifest.json' not in wrote or 'forms.pak' not in wrote:
        raise SystemExit('the release zip has no manifest.json or forms.pak')
    asset = Path(args.zip_path).name
    (site / 'release.json').write_text(json.dumps({
        'tag': args.tag,
        'asset': asset,
    }, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print(f'assembled {len(wrote)} release files from {asset} ({args.tag})')

if __name__ == '__main__':
    main()
