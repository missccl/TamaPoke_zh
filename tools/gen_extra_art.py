#!/usr/bin/env python3
"""Turn Piggy Kor still pictures into extra_art_data.h.

Snorlax layers are resized to 116x116. Diglett frames stay 22x17.
Transparent pixels (alpha below 96, the same cutoff as the Windows
installer) become invisible RGB565 runs. A picture of the same name at
/extra/<name>.tvr on the microSD replaces the built-in sample.

The sample pictures live in commit 3f31a85 as
samples/sample_Additional_assets.zip (Visual/S01.png ... Visual/D04.png).
"""
import sys
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    sys.exit("Pillow is required: pip install Pillow")

SPECS = [
    ("S01", 116, 116), ("S02", 116, 116), ("S03", 116, 116), ("S04", 116, 116),
    ("D01", 22, 17), ("D02", 22, 17), ("D03", 22, 17), ("D04", 22, 17),
]


def rgb565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def runs_for(path, w, h):
    im = Image.open(path).convert("RGBA")
    if im.size != (w, h):
        im = im.resize((w, h), Image.Resampling.LANCZOS)
    px = im.load()
    runs = []
    for y in range(h):
        x = 0
        while x < w:
            r, g, b, a = px[x, y]
            vis = a >= 96
            col = rgb565(r, g, b) if vis else 0
            n = 1
            while x + n < w:
                r2, g2, b2, a2 = px[x + n, y]
                vis2 = a2 >= 96
                col2 = rgb565(r2, g2, b2) if vis2 else 0
                if vis2 != vis or col2 != col or n == 65535:
                    break
                n += 1
            runs.append((n, col, 1 if vis else 0))
            x += n
    return runs


def c_bytes(blob):
    lines = []
    for i in range(0, len(blob), 16):
        chunk = blob[i:i + 16]
        lines.append("  " + ",".join(f"0x{b:02x}" for b in chunk) + ",")
    return "\n".join(lines)


def main():
    src = Path(sys.argv[1] if len(sys.argv) > 1 else "/tmp/piggy-assets")
    out = Path(sys.argv[2] if len(sys.argv) > 2 else "extra_art_data.h")
    parts = []
    decls = []
    for name, w, h in SPECS:
        runs = runs_for(src / f"{name}.png", w, h)
        blob = bytearray()
        for n, col, vis in runs:
            blob += bytes((n & 255, n >> 8, col & 255, col >> 8, vis, 0))
        parts.append(f"static const uint8_t kArt{name}[] = {{\n{c_bytes(blob)}\n}};")
        decls.append(f'  {{"{name}", {w}, {h}, {len(runs)}, kArt{name}}},')
        print(f"{name} {w}x{h} runs {len(runs)} bytes {len(blob)}")
    text = """#pragma once
#include <stdint.h>
// Piggy Kor sample pictures (Snorlax layers and Diglett frames).
// Same layout as /extra/S0x.tvr and /extra/D0x.tvr from the Windows installer.
struct ExtraArt {
  const char *name;
  uint16_t w, h;
  uint32_t runs;
  const uint8_t *data;
};
""" + "\n".join(parts) + "\nstatic const ExtraArt kExtraArt[] = {\n" + "\n".join(decls) + "\n};\nstatic const int kExtraArtCount = 8;\n"
    out.write_text(text)
    print("wrote", out, "chars", len(text))


if __name__ == "__main__":
    main()
