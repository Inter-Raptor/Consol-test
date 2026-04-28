#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "SD_MMC.h"
#include "driver/i2s_std.h"

// Couleurs
#define C_BLACK   0x0000
#define C_WHITE   0xFFFF
#define C_RED     0xF800
#define C_GREEN   0x07E0
#define C_BLUE    0x001F
#define C_YELLOW  0xFFE0
#define C_CYAN    0x07FF

// LCD ST7789
#define TFT_MOSI 39
#define TFT_CS   -1
#define TFT_DC   38
#define TFT_SCK  40
#define TFT_RST  41
#define TFT_BL   42

Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, -1);
Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 0);

// SD_MMC 1-bit
#define SD_CLK 10
#define SD_CMD 9
#define SD_D0  11

// Audio I2S
#define I2S_DOUT 7
#define I2S_BCLK 15
#define I2S_LRC  16
#define PA_CTRL  17

// Power
#define POWER_SW   2
#define POWER_CTRL 1

// Boutons
int sw_pins[] = {18, 8, 46, 14, 45, 48, 47, 21};
const char *sw_name[] = {"UP", "LEFT", "RIGHT", "DOWN", "B", "STA", "A", "SEL"};

i2s_chan_handle_t tx_handle;

void txt(int x, int y, uint16_t color, const char *s, int size = 2) {
  gfx->setCursor(x, y);
  gfx->setTextColor(color);
  gfx->setTextSize(size);
  gfx->print(s);
}

void beepI2S(uint16_t freq, uint16_t ms) {
  const int sampleRate = 16000;
  const int samples = sampleRate * ms / 1000;
  int16_t buf[128 * 2];

  for (int n = 0; n < samples; n += 128) {
    for (int i = 0; i < 128; i++) {
      int16_t v = ((n + i) % (sampleRate / freq) < (sampleRate / freq) / 2) ? 9000 : -9000;
      buf[i * 2] = v;
      buf[i * 2 + 1] = v;
    }
    size_t written = 0;
    i2s_channel_write(tx_handle, buf, sizeof(buf), &written, portMAX_DELAY);
  }
}

void setupI2S() {
  i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
  i2s_new_channel(&chan_cfg, &tx_handle, NULL);

  i2s_std_config_t std_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(16000),
    .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
    .gpio_cfg = {
      .mclk = I2S_GPIO_UNUSED,
      .bclk = (gpio_num_t)I2S_BCLK,
      .ws = (gpio_num_t)I2S_LRC,
      .dout = (gpio_num_t)I2S_DOUT,
      .din = I2S_GPIO_UNUSED,
      .invert_flags = {
        .mclk_inv = false,
        .bclk_inv = false,
        .ws_inv = false,
      },
    },
  };

  i2s_channel_init_std_mode(tx_handle, &std_cfg);
  i2s_channel_enable(tx_handle);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(POWER_SW, INPUT_PULLUP);
  pinMode(POWER_CTRL, OUTPUT);
  digitalWrite(POWER_CTRL, HIGH);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW); // BL actif LOW d'après blog

  pinMode(PA_CTRL, OUTPUT);
  digitalWrite(PA_CTRL, HIGH);

  for (int i = 0; i < 8; i++) pinMode(sw_pins[i], INPUT_PULLUP);

  gfx->begin();
  gfx->invertDisplay(true);

  gfx->fillScreen(C_RED); delay(250);
  gfx->fillScreen(C_GREEN); delay(250);
  gfx->fillScreen(C_BLUE); delay(250);
  gfx->fillScreen(C_BLACK);

  txt(10, 10, C_GREEN, "HU-086 TEST", 2);
  txt(10, 40, C_WHITE, "LCD OK", 2);

  SD_MMC.setPins(SD_CLK, SD_CMD, SD_D0);
  if (SD_MMC.begin("/sdcard", true)) {
    txt(10, 70, C_GREEN, "SD OK", 2);
    Serial.println("SD OK");
  } else {
    txt(10, 70, C_RED, "SD FAIL", 2);
    Serial.println("SD FAIL");
  }

  setupI2S();
  txt(10, 100, C_CYAN, "I2S beep...", 2);
  beepI2S(1000, 200);
  beepI2S(1500, 200);

  txt(10, 135, C_YELLOW, "Boutons:", 2);
}

void loop() {
  if (!digitalRead(POWER_SW)) {
    gfx->fillScreen(C_BLACK);
    txt(20, 100, C_RED, "POWER OFF", 2);
    delay(300);
    digitalWrite(POWER_CTRL, LOW);
  }

  for (int i = 0; i < 8; i++) {
    if (digitalRead(sw_pins[i]) == LOW) {
      Serial.printf("%s GPIO %d\n", sw_name[i], sw_pins[i]);

      gfx->fillRect(0, 170, 240, 60, C_BLACK);
      gfx->setCursor(10, 170);
      gfx->setTextColor(C_YELLOW);
      gfx->setTextSize(2);
      gfx->print(sw_name[i]);

      gfx->setCursor(10, 200);
      gfx->setTextColor(C_WHITE);
      gfx->setTextSize(1);
      gfx->print("GPIO ");
      gfx->print(sw_pins[i]);

      beepI2S(800 + i * 150, 80);
      delay(180);
    }
  }
}