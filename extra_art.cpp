#include "extra_art.h"

#include "extra_art_data.h"
#include "sdmon.h"
#include "zh_text.h"
#include <SD_MMC.h>
#include <cstring>

namespace {
struct Slot {
  const char *name = nullptr;
  bool tried = false;
  uint8_t *runs = nullptr;
  uint16_t w = 0, h = 0;
  uint32_t count = 0;
};

Slot slots[8];

uint16_t ru16(const uint8_t *p) { return (uint16_t)p[0] | ((uint16_t)p[1] << 8); }
uint32_t ru32(const uint8_t *p) {
  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

void paint(ZhCanvas *gfx, const uint8_t *p, uint16_t w, uint16_t h, uint32_t runs,
           int16_t ox, int16_t oy, uint8_t scale) {
  if (!gfx || !p || !scale) return;
  uint16_t x = 0, y = 0;
  for (uint32_t i = 0; i < runs && y < h; i++, p += 6) {
    uint16_t n = ru16(p);
    uint16_t color = ru16(p + 2);
    bool visible = ru16(p + 4) != 0;
    if (!n || (uint32_t)x + n > w) return;
    if (visible)
      gfx->fillRect(ox + (int16_t)x * scale, oy + (int16_t)y * scale, (int16_t)n * scale, scale, color);
    x = (uint16_t)(x + n);
    if (x == w) { x = 0; y++; }
  }
}

Slot *slotFor(const char *name) {
  Slot *freeSlot = nullptr;
  for (auto &slot : slots) {
    if (slot.name && strcmp(slot.name, name) == 0) return &slot;
    if (!slot.name && !freeSlot) freeSlot = &slot;
  }
  if (freeSlot) freeSlot->name = name;
  return freeSlot;
}

void trySd(Slot &slot, const char *name) {
  if (!sdReady) return;
  slot.tried = true;
  char path[32];
  snprintf(path, sizeof(path), "/extra/%s.tvr", name);
  File file = SD_MMC.open(path, FILE_READ);
  if (!file) return;
  uint8_t header[12];
  if (file.read(header, sizeof(header)) != sizeof(header) || memcmp(header, "TVR1", 4) != 0) {
    file.close();
    return;
  }
  uint16_t w = ru16(header + 4);
  uint16_t h = ru16(header + 6);
  uint32_t count = ru32(header + 8);
  uint32_t bytes = count * 6UL;
  if (!w || !h || w > 512 || h > 512 || !count || bytes > 256UL * 1024 ||
      file.size() != 12UL + bytes) {
    file.close();
    return;
  }
  uint8_t *runs = (uint8_t *)ps_malloc(bytes);
  if (!runs || file.read(runs, bytes) != bytes) {
    if (runs) free(runs);
    file.close();
    return;
  }
  file.close();
  slot.runs = runs;
  slot.w = w;
  slot.h = h;
  slot.count = count;
}

const ExtraArt *builtin(const char *name) {
  for (int i = 0; i < kExtraArtCount; i++)
    if (strcmp(kExtraArt[i].name, name) == 0) return &kExtraArt[i];
  return nullptr;
}
}

bool drawExtraNamed(ZhCanvas *gfx, const char *name, int16_t x, int16_t y, uint8_t scale) {
  Slot *slot = slotFor(name);
  if (slot && !slot->tried) trySd(*slot, name);
  if (slot && slot->runs) {
    paint(gfx, slot->runs, slot->w, slot->h, slot->count, x, y, scale);
    return true;
  }
  const ExtraArt *art = builtin(name);
  if (!art) return false;
  paint(gfx, art->data, art->w, art->h, art->runs, x, y, scale);
  return true;
}
