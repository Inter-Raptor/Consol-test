# Adapter ta copie de Retro-Go à ton montage ESP32

Tu as raison: pour une vraie compatibilité, il faut reprendre **exactement** les pins de ton montage qui fonctionne déjà.

## 1) Ce qui est prêt maintenant

- `include/board_config.h` contient une config matérielle plus complète:
  - boutons,
  - écran TFT (SPI),
  - SD,
  - audio DAC ou I2S,
  - batterie.
- `retrogo_esp32_hal.cpp` gère aussi les pins optionnelles (`-1` = non utilisé).

## 2) Comment injecter tes pins actuelles

1. Ouvre ton projet ESP32 actuel qui fonctionne.
2. Copie ses `#define` de pins dans `include/board_config.h`.
3. Garde les pins absentes à `-1`.
4. Si ton audio actuel est I2S, mets:
   - `#define RETROGO_AUDIO_MODE RETROGO_AUDIO_MODE_I2S`
   - puis renseigne `PIN_I2S_BCK`, `PIN_I2S_WS`, `PIN_I2S_DATA`.

## 3) Intégration Retro-Go

1. Copie `include/retrogo_esp32_hal.h` et `lib/retrogo_esp32/retrogo_esp32_hal.cpp` dans ta copie Retro-Go.
2. Dans la couche platform/input de Retro-Go, remplace la lecture GPIO par `retrogoReadInputs()`.
3. Dans l'init board, appelle `retrogoHalInit()`.
4. Dans l'audio, route les samples vers `retrogoPushAudioSample(...)`.

## 4) Point important

Sans les fichiers de ton firmware actuel dans ce repo, je ne peux pas deviner "toutes" tes pins réelles.
Cette version est donc prête pour être calée exactement sur ton montage dès que tu copies tes pins actuelles.
