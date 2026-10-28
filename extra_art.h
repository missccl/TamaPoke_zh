#pragma once
#include <stdint.h>

class ZhCanvas;

// Piggy Kor's still pictures. Snorlax is four layers (S01 head, S02 body,
// S03 feet, S04 the awake picture). Diglett is D01-D04. A file of the same
// name on the microSD at /extra/<name>.tvr replaces the built-in sample.
bool drawExtraNamed(ZhCanvas *gfx, const char *name, int16_t x, int16_t y, uint8_t scale);
