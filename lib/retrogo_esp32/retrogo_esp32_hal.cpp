#include <Arduino.h>
#include "board_config.h"
#include "retrogo_esp32_hal.h"

namespace {
constexpr int kPwmChannel = 0;
constexpr int kPwmFreq = 5000;
constexpr int kPwmResolution = 8;

void setInputPullupIfUsed(int pin) {
  if (pin >= 0) pinMode(pin, INPUT_PULLUP);
}

bool readButtonIfUsed(int pin) {
  if (pin < 0) return false;
  return digitalRead(pin) == LOW;
}
}

void retrogoHalInit() {
  setInputPullupIfUsed(PIN_BTN_UP);
  setInputPullupIfUsed(PIN_BTN_DOWN);
  setInputPullupIfUsed(PIN_BTN_LEFT);
  setInputPullupIfUsed(PIN_BTN_RIGHT);
  setInputPullupIfUsed(PIN_BTN_A);
  setInputPullupIfUsed(PIN_BTN_B);
  setInputPullupIfUsed(PIN_BTN_START);
  setInputPullupIfUsed(PIN_BTN_SELECT);
  setInputPullupIfUsed(PIN_BTN_MENU);
  setInputPullupIfUsed(PIN_BTN_VOLUME);

  if (PIN_TFT_BL >= 0) {
    ledcSetup(kPwmChannel, kPwmFreq, kPwmResolution);
    ledcAttachPin(PIN_TFT_BL, kPwmChannel);
    retrogoSetBacklight(true);
  }
}

InputState retrogoReadInputs() {
  return {
      .up = readButtonIfUsed(PIN_BTN_UP),
      .down = readButtonIfUsed(PIN_BTN_DOWN),
      .left = readButtonIfUsed(PIN_BTN_LEFT),
      .right = readButtonIfUsed(PIN_BTN_RIGHT),
      .a = readButtonIfUsed(PIN_BTN_A),
      .b = readButtonIfUsed(PIN_BTN_B),
      .start = readButtonIfUsed(PIN_BTN_START),
      .select = readButtonIfUsed(PIN_BTN_SELECT),
  };
}

void retrogoSetBacklight(bool on) {
  if (PIN_TFT_BL < 0) return;
#if TFT_BL_ACTIVE_HIGH
  ledcWrite(kPwmChannel, on ? 255 : 0);
#else
  ledcWrite(kPwmChannel, on ? 0 : 255);
#endif
}

void retrogoPushAudioSample(uint8_t sample8bit) {
#if RETROGO_AUDIO_MODE == RETROGO_AUDIO_MODE_DAC
  if (PIN_AUDIO_DAC >= 0) {
    dacWrite(PIN_AUDIO_DAC, sample8bit);
  }
#elif RETROGO_AUDIO_MODE == RETROGO_AUDIO_MODE_I2S
  // I2S non implémenté ici: brancher le flux de samples du mixeur Retro-Go
  // vers un driver I2S (ex: MAX98357A) avec PIN_I2S_BCK/PIN_I2S_WS/PIN_I2S_DATA.
  (void)sample8bit;
#else
  (void)sample8bit;
#endif
}
