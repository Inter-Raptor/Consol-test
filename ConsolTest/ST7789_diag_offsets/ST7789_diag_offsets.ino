#include <Arduino.h>
#include <Arduino_GFX_Library.h>

// HU-086 wiring
#define TFT_MOSI 39
#define TFT_SCK  40
#define TFT_DC   38
#define TFT_RST  41
#define TFT_BL   42
#define TFT_CS   -1

static const int kPanelW = 240;
static const int kPanelH = 320;

Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, -1);
Arduino_ST7789 *gfx = nullptr;

struct Variant {
  uint8_t madctl;
  int16_t xOff;
  int16_t yOff;
  const char *name;
};

Variant variants[] = {
  {0x00, 0,   0,  "MADCTL=0x00 off(0,0)"},
  {0x08, 0,   0,  "MADCTL=0x08 off(0,0)"},
  {0xA0, 0,   0,  "MADCTL=0xA0 off(0,0)"},
  {0x00, 0,  80,  "MADCTL=0x00 off(0,80)"},
  {0x08, 0,  80,  "MADCTL=0x08 off(0,80)"},
  {0xA0, 0,  80,  "MADCTL=0xA0 off(0,80)"},
};

void fillBars() {
  gfx->fillScreen(BLACK);
  gfx->fillRect(0, 0, kPanelW, kPanelH / 3, RED);
  gfx->fillRect(0, kPanelH / 3, kPanelW, kPanelH / 3, GREEN);
  gfx->fillRect(0, 2 * kPanelH / 3, kPanelW, kPanelH / 3, BLUE);
}

void applyVariant(int idx) {
  delete gfx;
  Variant v = variants[idx];
  gfx = new Arduino_ST7789(bus, TFT_RST, 0, false, kPanelW, kPanelH, v.xOff, v.yOff, 0, 0);

  Serial.println();
  Serial.printf("[LCD] Trying variant %d/%d: %s\n", idx + 1, (int)(sizeof(variants) / sizeof(variants[0])), v.name);

  gfx->begin();
  gfx->writeCommand(0x36);
  gfx->write(v.madctl);
  gfx->invertDisplay(true);
  fillBars();

  gfx->setCursor(8, 8);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(2);
  gfx->printf("V%d", idx + 1);
}

void setup() {
  Serial.begin(115200);
  delay(400);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW); // active LOW

  Serial.println("\n=== ST7789 HU-086 diagnostic ===");
  Serial.println("Press any key in Serial Monitor to go to next variant.");

  applyVariant(0);
}

void loop() {
  static int idx = 0;
  if (Serial.available()) {
    while (Serial.available()) Serial.read();
    idx = (idx + 1) % (sizeof(variants) / sizeof(variants[0]));
    applyVariant(idx);
  }
  delay(20);
}
