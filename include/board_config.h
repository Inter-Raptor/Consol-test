#pragma once

// -----------------------------------------------------------------------------
//  IMPORTANT
//  1) Copie ici les pins exactes de ton firmware qui fonctionne déjà.
//  2) Mets -1 pour les signaux non utilisés.
// -----------------------------------------------------------------------------

// ------------------------- Boutons (INPUT_PULLUP) -----------------------------
#ifndef PIN_BTN_UP
#define PIN_BTN_UP 32
#endif
#ifndef PIN_BTN_DOWN
#define PIN_BTN_DOWN 33
#endif
#ifndef PIN_BTN_LEFT
#define PIN_BTN_LEFT 25
#endif
#ifndef PIN_BTN_RIGHT
#define PIN_BTN_RIGHT 26
#endif
#ifndef PIN_BTN_A
#define PIN_BTN_A 27
#endif
#ifndef PIN_BTN_B
#define PIN_BTN_B 14
#endif
#ifndef PIN_BTN_START
#define PIN_BTN_START 12
#endif
#ifndef PIN_BTN_SELECT
#define PIN_BTN_SELECT 13
#endif
#ifndef PIN_BTN_MENU
#define PIN_BTN_MENU -1
#endif
#ifndef PIN_BTN_VOLUME
#define PIN_BTN_VOLUME -1
#endif

// ------------------------------ TFT / SPI -------------------------------------
#ifndef PIN_TFT_MOSI
#define PIN_TFT_MOSI 23
#endif
#ifndef PIN_TFT_MISO
#define PIN_TFT_MISO 19
#endif
#ifndef PIN_TFT_SCLK
#define PIN_TFT_SCLK 18
#endif
#ifndef PIN_TFT_CS
#define PIN_TFT_CS 5
#endif
#ifndef PIN_TFT_DC
#define PIN_TFT_DC 21
#endif
#ifndef PIN_TFT_RST
#define PIN_TFT_RST 22
#endif
#ifndef PIN_TFT_BL
#define PIN_TFT_BL 4
#endif
#ifndef TFT_BL_ACTIVE_HIGH
#define TFT_BL_ACTIVE_HIGH 1
#endif

// ------------------------------ SD card (SPI) ---------------------------------
#ifndef PIN_SD_CS
#define PIN_SD_CS -1
#endif

// ------------------------------- Audio -----------------------------------------
// DAC interne (GPIO25 ou GPIO26) si RETROGO_AUDIO_MODE_DAC
#ifndef PIN_AUDIO_DAC
#define PIN_AUDIO_DAC 25
#endif

// I2S DAC/AMP externe si RETROGO_AUDIO_MODE_I2S
#ifndef PIN_I2S_BCK
#define PIN_I2S_BCK -1
#endif
#ifndef PIN_I2S_WS
#define PIN_I2S_WS -1
#endif
#ifndef PIN_I2S_DATA
#define PIN_I2S_DATA -1
#endif

#ifndef RETROGO_AUDIO_RATE
#define RETROGO_AUDIO_RATE 22050
#endif

#define RETROGO_AUDIO_MODE_DAC 1
#define RETROGO_AUDIO_MODE_I2S 2

#ifndef RETROGO_AUDIO_MODE
#define RETROGO_AUDIO_MODE RETROGO_AUDIO_MODE_DAC
#endif

// ----------------------------- Batterie / Power -------------------------------
#ifndef PIN_BATTERY_ADC
#define PIN_BATTERY_ADC -1
#endif
#ifndef BATTERY_ADC_MAX
#define BATTERY_ADC_MAX 4095
#endif
#ifndef BATTERY_DIVIDER_RATIO
#define BATTERY_DIVIDER_RATIO 2.0f
#endif
