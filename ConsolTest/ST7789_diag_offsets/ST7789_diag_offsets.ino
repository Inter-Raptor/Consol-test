#include <Arduino.h>
#include <Arduino_GFX_Library.h>

// =========================
// HU-086 wiring
// =========================
#define TFT_MOSI 39
#define TFT_SCK  40
#define TFT_DC   38
#define TFT_RST  41
#define TFT_BL   42
#define TFT_CS   -1

// =========================
// Couleurs RGB565
// =========================
#define C_BLACK 0x0000
#define C_WHITE 0xFFFF
#define C_RED   0xF800
#define C_GREEN 0x07E0
#define C_BLUE  0x001F

// =========================
// Écran ST7789 HU-086
// =========================
static const int kPanelW = 240;
static const int kPanelH = 320;

Arduino_DataBus *bus = new Arduino_ESP32SPI(
  TFT_DC,
  TFT_CS,
  TFT_SCK,
  TFT_MOSI,
  -1
);

Arduino_ST7789 *gfx = nullptr;

// =========================
// Variantes à tester
// =========================
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

  {0x00, 0,  40,  "MADCTL=0x00 off(0,40)"},
  {0x08, 0,  40,  "MADCTL=0x08 off(0,40)"},
  {0xA0, 0,  40,  "MADCTL=0xA0 off(0,40)"}
};

const int variantCount = sizeof(variants) / sizeof(variants[0]);

// =========================
// Dessin test
// =========================
void drawTestPattern() {
  gfx->fillScreen(C_BLACK);

  gfx->fillRect(0, 0, kPanelW, kPanelH / 3, C_RED);
  gfx->fillRect(0, kPanelH / 3, kPanelW, kPanelH / 3, C_GREEN);
  gfx->fillRect(0, 2 * kPanelH / 3, kPanelW, kPanelH / 3, C_BLUE);

  // Coins de repère
  gfx->fillRect(0, 0, 20, 20, C_WHITE);                         // haut gauche
  gfx->fillRect(kPanelW - 20, 0, 20, 20, C_WHITE);               // haut droite
  gfx->fillRect(0, kPanelH - 20, 20, 20, C_WHITE);               // bas gauche
  gfx->fillRect(kPanelW - 20, kPanelH - 20, 20, 20, C_WHITE);    // bas droite
}

// =========================
// Appliquer variante
// =========================
void applyVariant(int idx) {
  if (gfx != nullptr) {
    delete gfx;
    gfx = nullptr;
  }

  Variant v = variants[idx];

  gfx = new Arduino_ST7789(
    bus,
    TFT_RST,
    0,      // rotation
    false,  // IPS
    kPanelW,
    kPanelH,
    v.xOff,
    v.yOff,
    0,
    0
  );

  Serial.println();
  Serial.printf("[LCD] Variant %d/%d : %s\n", idx + 1, variantCount, v.name);

  gfx->begin();

  // Commande ST7789 MADCTL = 0x36
  bus->beginWrite();
  bus->writeCommand(0x36);
  bus->write(v.madctl);
  bus->endWrite();

  gfx->invertDisplay(true);

  drawTestPattern();

  gfx->setCursor(8, 30);
  gfx->setTextColor(C_WHITE);
  gfx->setTextSize(2);
  gfx->printf("V%d", idx + 1);

  gfx->setCursor(8, 55);
  gfx->setTextSize(1);
  gfx->println(v.name);
}

// =========================
// Setup
// =========================
void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW); // rétroéclairage actif LOW

  Serial.println();
  Serial.println("=== ST7789 HU-086 diagnostic ===");
  Serial.println("Appuie sur une touche dans le moniteur serie pour changer de variante.");

  applyVariant(0);
}

// =========================
// Loop
// =========================
void loop() {
  static int idx = 0;

  if (Serial.available()) {
    while (Serial.available()) {
      Serial.read();
    }

    idx++;
    if (idx >= variantCount) {
      idx = 0;
    }

    applyVariant(idx);
  }

  delay(20);
}
