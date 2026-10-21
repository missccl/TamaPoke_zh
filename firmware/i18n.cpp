#include "i18n.h"

#include "dex.h"
#include "generated/dex_names_hk.h"
#include "generated/help_hk.h"
#include "generated/i18n_strings_hk.h"
#include "pet.h"

Lang gLang = LANG_DEFAULT;

const char *T(StrId id) {
  if (id >= STR_COUNT) return "?";
  return STRINGS_HK[id];
}

const char *dexName(int16_t dex) {
  if (dex < 1 || dex > DEX_COUNT) return "?";
  return DEX_NAMES_HK[dex];
}

const char *medalName(int i) {
  if (i < 0 || i >= MED_COUNT) return "?";
  return MED_NAME_HK[i];
}

const char *medalLabel(int i) {
  if (i < 0 || i >= MED_COUNT) return "?";
  return MED_LBL_HK[i];
}

const char *medalDesc(int i) {
  if (i < 0 || i >= MED_COUNT) return "?";
  return MED_DSC_HK[i];
}

const char *typeNameHk(uint8_t type) {
  if (type >= 19) return "-";
  return TYPE_NAMES_HK[type];
}

const char *helpWordHk() { return HELP_WORD_HK; }

const char *helpOkHk() { return HELP_OK_HK; }

const char *helpTitleHk(uint8_t page) {
  if (page >= 8) return "?";
  return HELP_TITLES_HK[page];
}

const char *helpLineHk(uint8_t page, uint8_t line) {
  if (page >= 8 || line >= 6) return "?";
  return HELP_LINES_HK[page][line];
}

void loadLang() { gLang = LANG_DEFAULT; }

void setLang(Lang) { gLang = LANG_DEFAULT; }
