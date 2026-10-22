#pragma once

#include <Arduino.h>

// Piggy Kor care screens: sleep menu, five minigames, step walk, and the
// friend/wild visit banner. The three training games already on the train
// panel stay where they are; the play list opens from that panel's title.
bool piggyBusy();
bool piggyFast();
void piggyOpenSleep();
void piggyOpenPlay();
void piggyTick(uint32_t now, bool idleHome);
void piggyRender();
void piggyTap(int16_t x, int16_t y);
void piggySwipe();
