#define RG_TARGET_NAME                 "ESP32-S3-HU086"
#define RG_ZIP_SUPPORT                 1

// Storage (SD MMC 1-bit)
#define RG_STORAGE_ROOT                "/sd"
#define RG_STORAGE_SDMMC_HOST          1
#define RG_STORAGE_SDMMC_SPEED         SDMMC_FREQ_DEFAULT

// Audio
#define RG_AUDIO_USE_INT_DAC           0
#define RG_AUDIO_USE_EXT_DAC           1

// --- DISPLAY HU-086 V1.5 : ST7789 240x320 en SPI2 ---

#define RG_SCREEN_DRIVER               0
#define RG_SCREEN_HOST                 SPI2_HOST
#define RG_SCREEN_SPEED                SPI_MASTER_FREQ_20M

// Backlight actif LOW (comme Arduino)
#define RG_SCREEN_BACKLIGHT            80
// PAS de RG_GPIO_LCD_BCKL_INVERT → on force le BL dans lcd_init()

// Dimensions PHYSIQUES du panneau ST7789
#define RG_SCREEN_WIDTH                240
#define RG_SCREEN_HEIGHT               320

// Rotation logique Retro-Go
#define RG_SCREEN_ROTATION             1
#define RG_SCREEN_RGB_BGR              1   // Arduino_GFX = true

// ST7789 address offsets (HU-086 panel uses full 240x320 area)
#define RG_SCREEN_OFFSET_X            0
#define RG_SCREEN_OFFSET_Y            0

// Marges = 0
#define RG_SCREEN_VISIBLE_AREA         {0, 0, 0, 0}
#define RG_SCREEN_SAFE_AREA            {0, 0, 0, 0}

// Séquence d'init HU-086 (celle validée par Arduino)
#define RG_SCREEN_INIT() \
    ILI9341_CMD(0x01); \
    rg_usleep(120000); \
    ILI9341_CMD(0x11); \
    rg_usleep(120000); \
    ILI9341_CMD(0x36, 0xA0); /* rotation 1 + BGR */ \
    ILI9341_CMD(0x3A, 0x55); \
    ILI9341_CMD(0xB2, 0x0C, 0x0C, 0x00, 0x33, 0x33); \
    ILI9341_CMD(0xB7, 0x35); \
    ILI9341_CMD(0xBB, 0x19); \
    ILI9341_CMD(0xC0, 0x2C); \
    ILI9341_CMD(0xC2, 0x01); \
    ILI9341_CMD(0xC3, 0x12); \
    ILI9341_CMD(0xC4, 0x20); \
    ILI9341_CMD(0xC6, 0x0F); \
    ILI9341_CMD(0xD0, 0xA4, 0xA1); \
    ILI9341_CMD(0xE0, 0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23); \
    ILI9341_CMD(0xE1, 0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23); \
    ILI9341_CMD(0x21); \
    ILI9341_CMD(0x29); \
    rg_usleep(120000);

// LCD pins (confirmés par ton test)
#define RG_GPIO_LCD_MISO               GPIO_NUM_NC
#define RG_GPIO_LCD_MOSI               GPIO_NUM_39
#define RG_GPIO_LCD_CLK                GPIO_NUM_40
#define RG_GPIO_LCD_CS                 GPIO_NUM_NC
#define RG_GPIO_LCD_DC                 GPIO_NUM_38
#define RG_GPIO_LCD_BCKL               GPIO_NUM_42
#define RG_GPIO_LCD_RST                GPIO_NUM_41

// SDMMC pins
#define RG_GPIO_SDMMC_CLK              GPIO_NUM_10
#define RG_GPIO_SDMMC_CMD              GPIO_NUM_9
#define RG_GPIO_SDMMC_D0               GPIO_NUM_11

// I2S DAC
#define RG_GPIO_SND_I2S_BCK            15
#define RG_GPIO_SND_I2S_WS             16
#define RG_GPIO_SND_I2S_DATA           7
#define RG_GPIO_SND_AMP_ENABLE         17

// Buttons
#define RG_GAMEPAD_GPIO_MAP {\
    {RG_KEY_UP,     .num = GPIO_NUM_18, .pullup = 1, .level = 0},\
    {RG_KEY_LEFT,   .num = GPIO_NUM_8,  .pullup = 1, .level = 0},\
    {RG_KEY_RIGHT,  .num = GPIO_NUM_46, .pullup = 1, .level = 0},\
    {RG_KEY_DOWN,   .num = GPIO_NUM_14, .pullup = 1, .level = 0},\
    {RG_KEY_A,      .num = GPIO_NUM_47, .pullup = 1, .level = 0},\
    {RG_KEY_B,      .num = GPIO_NUM_45, .pullup = 1, .level = 0},\
    {RG_KEY_START,  .num = GPIO_NUM_48, .pullup = 1, .level = 0},\
    {RG_KEY_SELECT, .num = GPIO_NUM_21, .pullup = 1, .level = 0},\
}

#define RG_GPIO_POWER_HOLD             GPIO_NUM_1
