# ST7789 HU-086 Diagnostic Sketch

This sketch helps find the exact ST7789 orientation/offset combo that visually works on HU-086.

## How to use
1. Open `ST7789_diag_offsets.ino` in Arduino IDE.
2. Select ESP32-S3 board and flash.
3. Open Serial Monitor at 115200.
4. Press any key in Serial Monitor to cycle variants.
5. Note which variant displays clean RGB bars + `Vn` label in the correct place.

## What to report back
- Variant index (`Vn`)
- Whether image is fully visible or shifted/cropped
- Whether red/green/blue bars are in expected order
