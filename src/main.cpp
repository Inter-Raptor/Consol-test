#include <Arduino.h>
#include "retrogo_esp32_hal.h"

void setup() {
  Serial.begin(115200);
  retrogoHalInit();
  Serial.println("Retro-Go ESP32 HAL init OK");
}

void loop() {
  const InputState s = retrogoReadInputs();

  if (s.start && s.select) {
    retrogoSetBacklight(false);
  } else {
    retrogoSetBacklight(true);
  }

  // Ton test audio minimal (onde triangle).
  static uint8_t sample = 0;
  static int8_t delta = 1;
  retrogoPushAudioSample(sample);
  sample = static_cast<uint8_t>(sample + delta);
  if (sample == 0 || sample == 255) delta = -delta;

  delay(2);
}
