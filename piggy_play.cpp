#include "piggy_play.h"

#include "pet.h"
#include "i18n.h"
#include "audio.h"
#include "walk_sensor.h"
#include "zh_text.h"
#include "species.h"
#include "dex.h"
#include <math.h>

extern Pet pet;
extern ZhCanvas *gfx;

namespace play {
enum Mode : uint8_t {
  OFF = 0, SLEEP_MENU, PLAY_MENU, RUN, WAKE, CATCH, DIG, JUMP, WALK, RESULT, BANNER
};

Mode mode = OFF;
uint8_t score = 0;
uint8_t gain = 0;
uint8_t kind = PLAY_RUN;
uint8_t bannerKind = 0;
uint32_t until = 0;
uint32_t startedAt = 0;
const char *notice = nullptr;
uint32_t noticeUntil = 0;

float runX = 40;
float runSpeed = 150;
uint32_t runLast = 0;

struct Fruit { int16_t x; float y; bool live; };
Fruit fruits[3];
uint8_t spawned = 0;
uint32_t nextSpawn = 0;

uint8_t digHole = 0;
bool digUp = false;
uint8_t digPopped = 0;
uint32_t digAt = 0;

uint8_t jumpsDone = 0;
uint32_t jumpAt = 0;
bool jumpScored = false;

bool walkStarted = false;
bool walkFailed = false;
uint32_t walkSteps = 0;

const int CX = 233;
const int ROW_X = 73;
const int ROW_W = 320;
const int ROW_H = 46;

int sleepRowY(int i) { return 128 + i * 58; }
int playRowY(int i) { return 78 + i * 54; }

bool hitRow(int x, int y, int ry) {
  return x >= ROW_X && x <= ROW_X + ROW_W && y >= ry && y <= ry + ROW_H;
}

void centerText(const char *s, int y, uint8_t size, uint16_t color) {
  gfx->setTextSize(size);
  gfx->setTextColor(color);
  int w = uiTextWidth(s, size);
  gfx->setCursor(CX - w / 2, y);
  gfx->print(s);
}

void drawRow(int y, const char *label, bool live) {
  gfx->fillRoundRect(ROW_X, y, ROW_W, ROW_H, 12, live ? UI_BG_DAY : UI_TRACK);
  gfx->drawRoundRect(ROW_X, y, ROW_W, ROW_H, 12, UI_INK);
  centerText(label, y + 14, 2, live ? UI_INK : 0x7BEF);
}

void backdrop() {
  gfx->fillScreen(RGB565_BLACK);
  gfx->fillCircle(CX, 233, 231, UI_BG_DAY);
}

void showResult(uint8_t got) {
  gain = got;
  mode = RESULT;
  until = millis() + 2800;
}

void finishGame() {
  if (mode < RUN || mode > JUMP) return;
  uint8_t got = pet.applyMinigame(kind, score);
  showResult(got);
}

void stopWalk(bool reward) {
  if (walkStarted) {
    walkSensorStop();
    if (reward) pet.finishWalk(walkSteps > 1000 ? 1000 : (uint16_t)walkSteps);
    else pet.cancelWalk();
    walkStarted = false;
  }
}

void beginRun() {
  kind = PLAY_RUN;
  score = 0;
  runX = 40;
  runSpeed = 150;
  runLast = millis();
  startedAt = runLast;
  mode = RUN;
}

void beginWake() {
  kind = PLAY_WAKE;
  score = 0;
  startedAt = millis();
  mode = WAKE;
}

void beginCatch() {
  kind = PLAY_CATCH;
  score = 0;
  spawned = 0;
  nextSpawn = millis();
  for (auto &f : fruits) f.live = false;
  startedAt = millis();
  mode = CATCH;
}

void beginDig() {
  kind = PLAY_DIG;
  score = 0;
  digPopped = 0;
  digUp = false;
  digAt = millis();
  startedAt = digAt;
  mode = DIG;
}

void beginJump() {
  kind = PLAY_JUMP;
  score = 0;
  jumpsDone = 0;
  jumpScored = false;
  jumpAt = millis();
  startedAt = jumpAt;
  mode = JUMP;
}

void openWalk() {
  walkStarted = false;
  walkFailed = false;
  walkSteps = 0;
  notice = nullptr;
  mode = WALK;
}

void drawScore() {
  centerText(T(S_BACK), 24, 1, UI_INK);
  char buf[24];
  snprintf(buf, sizeof(buf), "%u", score);
  centerText(buf, 88, 3, UI_INK);
}

void showWalkResult(uint32_t now);

void tickRun(uint32_t now) {
  uint32_t dt = runLast ? now - runLast : 16;
  runLast = now;
  if (dt > 80) dt = 80;
  runX += runSpeed * (float)dt / 1000.0f;
  if (runX > 420) runX = 40;
  if (now - startedAt >= 30000UL || score >= 50) finishGame();
}

void tickCatch(uint32_t now) {
  if (spawned < 35 && now >= nextSpawn) {
    for (auto &f : fruits) {
      if (f.live) continue;
      f.live = true;
      f.x = 90 + (int16_t)random(250);
      f.y = 140;
      spawned++;
      nextSpawn = now + 700;
      break;
    }
  }
  bool any = false;
  for (auto &f : fruits) {
    if (!f.live) continue;
    any = true;
    f.y += 2.4f;
    if (f.y > 420) f.live = false;
  }
  if (spawned >= 35 && !any) finishGame();
  if (now - startedAt > 45000UL) finishGame();
}

void tickDig(uint32_t now) {
  if (!digUp) {
    if (digPopped >= 20) { finishGame(); return; }
    if (now - digAt >= 380) {
      digHole = (uint8_t)random(3);
      digUp = true;
      digAt = now;
      digPopped++;
    }
    return;
  }
  if (now - digAt >= 720) {
    digUp = false;
    digAt = now;
  }
}

void tickJump(uint32_t now) {
  if (jumpsDone >= 19) { finishGame(); return; }
  if (now - jumpAt >= 1200) {
    jumpsDone++;
    jumpAt = now;
    jumpScored = false;
  }
}

void tickWalk(uint32_t now) {
  if (!walkStarted) return;
  walkSensorUpdate(now);
  walkSteps = walkSensorSteps();
  if (walkSteps > 1000) walkSteps = 1000;
  if (walkSteps >= 1000 || now - startedAt >= 20UL * 60UL * 1000UL)
    showWalkResult(now);
}

void showWalkResult(uint32_t now) {
  uint32_t steps = walkSteps;
  stopWalk(true);
  score = steps ? 1 : 0;
  gain = 0;
  mode = RESULT;
  until = now + 2800;
}

void renderRun() {
  backdrop();
  centerText(T(S_GAME_RUN), 48, 2, UI_INK);
  drawScore();
  gfx->fillRoundRect(300, 200, 70, 70, 12, UI_BAR_OK);
  gfx->fillCircle((int)runX, 235, 22, 0xFEE0);
  gfx->fillCircle((int)runX - 6, 228, 3, UI_INK);
  gfx->fillCircle((int)runX + 8, 228, 3, UI_INK);
  gfx->flush();
}

void renderWake() {
  backdrop();
  centerText(T(S_GAME_WAKE), 52, 2, UI_INK);
  drawScore();
  uint32_t elapsed = millis() - startedAt;
  uint32_t left = elapsed >= 30000UL ? 0 : 30000UL - elapsed;
  char buf[16];
  snprintf(buf, sizeof(buf), "%lu", (unsigned long)(left / 1000UL));
  centerText(buf, 150, 2, UI_INK);
  gfx->fillCircle(CX, 280, 78, 0x5B7B);
  gfx->fillCircle(CX - 24, 268, 6, UI_INK);
  gfx->fillCircle(CX + 24, 268, 6, UI_INK);
  gfx->flush();
}

void renderCatch() {
  backdrop();
  centerText(T(S_GAME_CATCH), 48, 2, UI_INK);
  drawScore();
  for (auto &f : fruits) {
    if (!f.live) continue;
    gfx->fillCircle(f.x, (int)f.y, 16, 0xF800);
  }
  gfx->flush();
}

void renderDig() {
  backdrop();
  centerText(T(S_GAME_DIG), 48, 2, UI_INK);
  drawScore();
  const int xs[3] = { 120, 233, 346 };
  for (int i = 0; i < 3; i++) {
    gfx->fillCircle(xs[i], 300, 28, 0xB4A0);
    gfx->fillCircle(xs[i], 308, 22, 0x49A0);
    if (digUp && digHole == i)
      gfx->fillCircle(xs[i], 268, 20, 0xC340);
  }
  gfx->flush();
}

void renderJump() {
  backdrop();
  centerText(T(S_GAME_JUMP), 48, 2, UI_INK);
  drawScore();
  float t = jumpsDone >= 19 ? 0 : (millis() - jumpAt) / 1200.0f;
  if (t < 0) t = 0;
  if (t > 1) t = 1;
  float arc = sinf(t * 3.14159f);
  int y = 340 - (int)(arc * 150);
  gfx->drawFastHLine(80, 360, 306, UI_INK);
  gfx->fillCircle(CX, y, 18, 0xFD20);
  gfx->flush();
}

void renderWalk() {
  backdrop();
  centerText(T(S_WALK), 48, 3, UI_INK);
  if (walkFailed) {
    centerText(T(S_WALK_NO), 180, 2, UI_BAR_BAD);
  } else if (!walkStarted) {
    centerText(T(S_WALK_GO), 150, 2, UI_INK);
    drawRow(230, T(S_WALK_GO), pet.canStartWalk());
    if (notice && millis() < noticeUntil) centerText(notice, 300, 1, UI_BAR_BAD);
  } else {
    char buf[32];
    snprintf(buf, sizeof(buf), T(S_STEPS_FMT), (unsigned)walkSteps);
    centerText(buf, 160, 3, UI_INK);
    uint32_t left = 20UL * 60UL * 1000UL;
    uint32_t gone = millis() - startedAt;
    uint32_t sec = gone >= left ? 0 : (left - gone) / 1000UL;
    snprintf(buf, sizeof(buf), "%lu:%02lu", (unsigned long)(sec / 60UL),
             (unsigned long)(sec % 60UL));
    centerText(buf, 220, 2, UI_INK);
    centerText("150 / 300 / 600 / 1000", 280, 1, UI_INK);
  }
  centerText(T(S_BACK), 400, 2, UI_INK);
  gfx->flush();
}

void renderResult() {
  backdrop();
  if (!score) centerText(T(S_NO_SCORE), 200, 3, UI_INK);
  else {
    centerText(T(S_PLAY_DONE), 180, 3, UI_INK);
    if (gain && (kind == PLAY_RUN || kind == PLAY_WAKE || kind == PLAY_DIG)) {
      char buf[16];
      snprintf(buf, sizeof(buf), "+%u", gain);
      centerText(buf, 240, 3, UI_BAR_OK);
    }
  }
  gfx->flush();
}

void renderBanner() {
  backdrop();
  int16_t dex = pet.lastVisitSpecies();
  const char *name = (dex >= 1 && dex <= DEX_COUNT) ? localName(DEX_TBL[dex].name) : "?";
  char buf[64];
  if (bannerKind == 1) snprintf(buf, sizeof(buf), T(S_FRIEND_VISIT), name);
  else snprintf(buf, sizeof(buf), T(S_WILD_MET_FMT), name);
  centerText(buf, 210, uiTextWidth(buf, 2) > 380 ? 1 : 2, UI_INK);
  gfx->flush();
}
}

using namespace play;

bool piggyBusy() { return mode != OFF; }
bool piggyFast() { return mode >= RUN && mode <= WALK; }

void piggyOpenSleep() {
  if (pet.isEgg() || pet.ceremony) return;
  notice = nullptr;
  mode = SLEEP_MENU;
}

void piggyOpenPlay() {
  if (pet.isEgg() || pet.sleeping || pet.ceremony) return;
  mode = PLAY_MENU;
}

void piggyTick(uint32_t now, bool idleHome) {
  if (mode == RUN) tickRun(now);
  else if (mode == WAKE) {
    if (now - startedAt >= 30000UL || score >= 100) finishGame();
  } else if (mode == CATCH) tickCatch(now);
  else if (mode == DIG) tickDig(now);
  else if (mode == JUMP) tickJump(now);
  else if (mode == WALK) tickWalk(now);
  else if ((mode == RESULT || mode == BANNER) && (int32_t)(now - until) >= 0) mode = OFF;

  if (mode == OFF && idleHome) {
    uint8_t ev = pet.takeRandomEvent();
    if (ev) {
      bannerKind = ev;
      mode = BANNER;
      until = now + 3200;
      gain = ev == 1 ? 1 : 0;
      score = 1;
    }
  }
}

void piggyRender() {
  if (mode == SLEEP_MENU) {
    backdrop();
    centerText(pet.sleeping ? T(S_WAKE) : T(S_SLEEP), 56, 3, UI_INK);
    drawRow(sleepRowY(0), pet.sleeping ? T(S_WAKE) : T(S_SLEEP), true);
    bool sit = pet.sitterActive();
    drawRow(sleepRowY(1), sit ? T(S_SITTER_ON) : T(S_SITTER), sit || pet.canStartSitter());
    drawRow(sleepRowY(2), T(S_CLOSE), true);
    if (notice && millis() < noticeUntil) centerText(notice, 400, 1, UI_BAR_BAD);
    else if (!sit) centerText(T(S_SITTER_NO), 400, 1, UI_INK);
    gfx->flush();
    return;
  }
  if (mode == PLAY_MENU) {
    backdrop();
    centerText(T(S_PLAY), 40, 2, UI_INK);
    const char *rows[6] = {
      T(S_GAME_RUN), T(S_GAME_WAKE), T(S_GAME_CATCH),
      T(S_GAME_DIG), T(S_GAME_JUMP), T(S_WALK)
    };
    for (int i = 0; i < 6; i++) drawRow(playRowY(i), rows[i], true);
    gfx->flush();
    return;
  }
  if (mode == RUN) renderRun();
  else if (mode == WAKE) renderWake();
  else if (mode == CATCH) renderCatch();
  else if (mode == DIG) renderDig();
  else if (mode == JUMP) renderJump();
  else if (mode == WALK) renderWalk();
  else if (mode == BANNER) renderBanner();
  else if (mode == RESULT) renderResult();
}

void piggyTap(int16_t x, int16_t y) {
  if (mode == RESULT || mode == BANNER) { mode = OFF; return; }
  if (mode == SLEEP_MENU) {
    if (hitRow(x, y, sleepRowY(0))) {
      pet.toggleLight();
      sfxPlay(SFX_TAP);
      mode = OFF;
      return;
    }
    if (hitRow(x, y, sleepRowY(1))) {
      if (pet.sitterActive()) {
        pet.stopSitter();
        sfxPlay(SFX_TAP);
      } else if (pet.startSitter()) {
        sfxPlay(SFX_HEART);
        mode = OFF;
      } else {
        notice = T(S_SITTER_NO);
        noticeUntil = millis() + 2000;
        sfxPlay(SFX_DENY);
      }
      return;
    }
    if (hitRow(x, y, sleepRowY(2)) || y > 420 || x < 40) {
      mode = OFF;
      sfxPlay(SFX_TAP);
    }
    return;
  }
  if (mode == PLAY_MENU) {
    void (*start[5])() = { beginRun, beginWake, beginCatch, beginDig, beginJump };
    for (int i = 0; i < 5; i++) {
      if (!hitRow(x, y, playRowY(i))) continue;
      sfxPlay(SFX_TAP);
      start[i]();
      return;
    }
    if (hitRow(x, y, playRowY(5))) { sfxPlay(SFX_TAP); openWalk(); return; }
    if (x < 50 || x > 416 || y < 24 || y > 430) mode = OFF;
    return;
  }
  if (y < 72 && mode >= RUN && mode <= JUMP) { finishGame(); return; }
  if (mode == RUN) {
    if (runX >= 300 && runX <= 370) {
      if (score < 50) score++;
      runX = 40;
      runSpeed += 8;
      sfxPlay(SFX_TAP);
    }
    return;
  }
  if (mode == WAKE) {
    int dx = x - CX, dy = y - 280;
    if (dx * dx + dy * dy <= 78 * 78 && score < 100) {
      score++;
      sfxPlay(SFX_TAP);
    }
    return;
  }
  if (mode == CATCH) {
    for (auto &f : fruits) {
      if (!f.live) continue;
      int dx = x - f.x, dy = y - (int)f.y;
      if (dx * dx + dy * dy > 36 * 36) continue;
      f.live = false;
      if (score < 35) score++;
      sfxPlay(SFX_TAP);
      return;
    }
    return;
  }
  if (mode == DIG) {
    if (!digUp) return;
    const int xs[3] = { 120, 233, 346 };
    int dx = x - xs[digHole], dy = y - 280;
    if (dx * dx + dy * dy <= 40 * 40) {
      if (score < 20) score++;
      digUp = false;
      digAt = millis();
      sfxPlay(SFX_TAP);
    }
    return;
  }
  if (mode == JUMP) {
    if (jumpScored || jumpsDone >= 19) return;
    float t = (millis() - jumpAt) / 1200.0f;
    if (t >= 0.35f && t <= 0.65f) {
      jumpScored = true;
      if (score < 19) score++;
      sfxPlay(SFX_TAP);
    }
    return;
  }
  if (mode == WALK) {
    if (!walkStarted) {
      if (hitRow(x, y, 230)) {
        if (!pet.beginWalk()) {
          notice = T(S_EXPLORE_NEED_ENERGY);
          noticeUntil = millis() + 2000;
          sfxPlay(SFX_DENY);
          return;
        }
        if (!walkSensorStart()) {
          pet.cancelWalk();
          walkFailed = true;
          sfxPlay(SFX_DENY);
          return;
        }
        walkStarted = true;
        startedAt = millis();
        sfxPlay(SFX_TAP);
        return;
      }
      mode = OFF;
      return;
    }
    if (y > 370) showWalkResult(millis());
  }
}

void piggySwipe() {
  if (mode == WALK && walkStarted) {
    showWalkResult(millis());
    return;
  }
  if (mode >= RUN && mode <= JUMP) { finishGame(); return; }
  if (mode == WALK) mode = OFF;
  else if (mode == SLEEP_MENU || mode == PLAY_MENU || mode == RESULT || mode == BANNER)
    mode = OFF;
}
