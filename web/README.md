# Web installer

`index.html`, `installer.js` and `packs.json` are the Traditional Chinese
installer page, adapted from Loaram/TamaPoke_ko. Large binaries are not kept in git.

Release asset `TamaPoke-Web-Installer-v<version>.zip` must contain:

- `manifest.json` (ESP Web Tools manifest; parts at offsets 0, 32768, 57344, 65536)
- `firmware/bootloader.bin`, `partitions.bin`, `boot_app0.bin`, `app.bin`, `build-info.json`
- `sprites-{kanto,johto,hoenn,sinnoh,unova,kalos,alola,galar,paldea}.pak` and `forms.pak`
  (sizes must match `packs.json`)

Then run **Actions → Deploy Web Installer** on `main` with the release tag.
`python tools/check_web.py` checks the page sources; `python tools/check_web.py SITE_DIR`
also verifies the assembled site.
