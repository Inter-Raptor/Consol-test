#pragma once

#include <Arduino.h>

struct InputState {
  bool up;
  bool down;
  bool left;
  bool right;
  bool a;
  bool b;
  bool start;
  bool select;
};

void retrogoHalInit();
InputState retrogoReadInputs();
void retrogoSetBacklight(bool on);
void retrogoPushAudioSample(uint8_t sample8bit);
