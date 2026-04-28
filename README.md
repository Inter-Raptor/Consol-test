# Consol-test

Base d'adaptation Retro-Go pour ESP32.

## Important

Si tu veux un résultat 100% compatible avec ton montage, il faut reprendre les pins exactes de ton firmware actuel dans `include/board_config.h`.

## Démarrage rapide

1. Mets tes broches dans `include/board_config.h`.
2. Choisis le mode audio (`DAC` ou `I2S`).
3. Compile/flashe avec PlatformIO.
4. Intègre `retrogo_esp32_hal.*` dans ta copie Retro-Go.

Détails: `docs/retro-go-integration.md`.
