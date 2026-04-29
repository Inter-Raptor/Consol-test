# CONSOL-TEST target

Custom retro-go target for the ESP32-S3 handheld wiring used in `ConsolTest/ConsolTest.ino`.

## Pin mapping used

- LCD ST7789 SPI: MOSI=39, SCLK=40, DC=38, RST=41, BL=42 (active low), CS not connected.
- SD card via SDMMC 1-bit: CLK=10, CMD=9, D0=11.
- I2S audio DAC/amp: DATA=7, BCLK=15, WS=16, AMP_EN=17.
- Buttons: UP=18, LEFT=8, RIGHT=46, DOWN=14, B=45, START=48, A=47, SELECT=21.
- Power: POWER_CTRL=1 is forced HIGH at boot by `RG_CUSTOM_PLATFORM_INIT`.

## Build

```bash
./rg_tool.py --target=consol-test build launcher
./rg_tool.py --target=consol-test flash launcher
```

If your display orientation/colors are incorrect, adjust `RG_SCREEN_INIT` and/or `RG_SCREEN_ROTATE` in `config.h`.

## Dépannage (flash OK mais écran noir)

Si `flash` se termine sans erreur mais que l'écran reste noir, le problème est presque toujours lié à l'initialisation ST7789 (reset, backlight, séquence init, offsets visibles).

Checklist rapide:

1. **Backlight (GPIO42) actif bas**
   - Ce target est configuré en backlight inversé (`RG_GPIO_LCD_BCKL_INVERT`).
   - Vérifie au multimètre que la ligne BL descend bien à 0V juste après le boot.

2. **Reset écran (GPIO41)**
   - Vérifie que `RG_GPIO_LCD_RST` est bien câblé au pin RST du ST7789.
   - Sans reset matériel correct, l'écran peut rester allumé mais noir.

3. **Séquence d'init ST7789**
   - La macro `RG_SCREEN_INIT` envoie actuellement `0x01`, `0x11`, `0x3A`, `0x36`, `0x21`, `0x29`.
   - Si ton module diffère de la version du blog, essaye de retirer `0x21` (inversion) ou de changer la valeur de `0x36` (orientation).

4. **Zone visible / offsets**
   - Ce profil utilise `RG_SCREEN_VISIBLE_AREA {0, 80, 240, 320}`.
   - Si l'image est hors zone, teste temporairement `{0, 0, 240, 320}` pour diagnostiquer.

5. **Vérifier la cible sélectionnée**
   - Toujours compiler/flasher avec `--target=consol-test` pour éviter d'utiliser une config d'une autre carte.

Commandes utiles:

```bash
./rg_tool.py --target=consol-test clean
./rg_tool.py --target=consol-test build launcher
./rg_tool.py --target=consol-test flash launcher
```

Si tu veux reproduire exactement le câblage du billet, compare en priorité les GPIO LCD (`MOSI/SCLK/DC/RST/BL`) et la polarité du backlight: c'est la cause #1 d'un flash réussi avec écran noir.
