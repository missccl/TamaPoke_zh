# Web installer

`index.html`, `installer.js` and `packs.json` are the Traditional Chinese
installer page, adapted from Loaram/TamaPoke_ko. Large binaries are not kept in git.

Release asset `TamaPoke-Web-Installer-v<version>.zip` must contain:

- `manifest.json` (ESP Web Tools manifest; parts at offsets 0, 32768, 57344, 65536)
- `firmware/bootloader.bin`, `partitions.bin`, `boot_app0.bin`, `app.bin`, `build-info.json`
- `sprites-{kanto,johto,hoenn,sinnoh,unova,kalos,alola,galar,paldea}.pak` and `forms.pak`
  (sizes must match `packs.json`)

The page can also flash a zip already downloaded on the computer. Choose that zip, or only the newest extracted folder. The install button then uses those files instead of the copy published on the website. Direct `file://` pages cannot open a serial port; serve `web/` from localhost.

GitHub release downloads do not send CORS headers, so the browser cannot fetch the zip from the release page. **Actions → Deploy Web Installer** on `main` downloads that zip, lifts `manifest.json`, `firmware/`, `sprites-*.pak` and `forms.pak` out of any nested folder, and publishes them beside this page.

`python tools/assemble_site.py ZIP SITE --tag TAG` does that assembly locally.
`python tools/check_web.py` checks the page sources; `python tools/check_web.py SITE_DIR`
also verifies the assembled site, including `release.json` and the sprite packs.
