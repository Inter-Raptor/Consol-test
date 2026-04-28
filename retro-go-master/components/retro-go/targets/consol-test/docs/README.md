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
