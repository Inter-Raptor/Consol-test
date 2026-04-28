// Target definition
#define RG_TARGET_NAME             "CONSOL-TEST"

// Storage
#define RG_STORAGE_ROOT             "/sd"
#define RG_STORAGE_SDMMC_HOST       SDMMC_HOST_SLOT_1
#define RG_STORAGE_SDMMC_SPEED      SDMMC_FREQ_DEFAULT

// Audio
#define RG_AUDIO_USE_INT_DAC        0   // 0 = Disable, 1 = GPIO25, 2 = GPIO26, 3 = Both
#define RG_AUDIO_USE_EXT_DAC        1   // 0 = Disable, 1 = Enable

// Board-specific
#define RG_GPIO_POWER_CTRL          GPIO_NUM_1
#define RG_CUSTOM_PLATFORM_INIT()                             \
    gpio_set_direction(RG_GPIO_POWER_CTRL, GPIO_MODE_OUTPUT); \
    gpio_set_level(RG_GPIO_POWER_CTRL, 1);

// Video (ST7789)
#define RG_SCREEN_DRIVER            0   // 0 = ILI9341/ST7789
#define RG_SCREEN_HOST              SPI2_HOST
#define RG_SCREEN_SPEED             SPI_MASTER_FREQ_40M
#define RG_SCREEN_BACKLIGHT         1
#define RG_SCREEN_WIDTH             240
#define RG_SCREEN_HEIGHT            240
#define RG_SCREEN_ROTATE            0
#define RG_SCREEN_VISIBLE_AREA      {0, 0, 0, 0}
#define RG_SCREEN_SAFE_AREA         {0, 0, 0, 0}
#define RG_SCREEN_INIT()                                                                                         \
    ILI9341_CMD(0xCF, 0x00, 0xc3, 0x30);                                                                         \
    ILI9341_CMD(0xED, 0x64, 0x03, 0x12, 0x81);                                                                   \
    ILI9341_CMD(0xE8, 0x85, 0x00, 0x78);                                                                         \
    ILI9341_CMD(0xCB, 0x39, 0x2c, 0x00, 0x34, 0x02);                                                             \
    ILI9341_CMD(0xF7, 0x20);                                                                                     \
    ILI9341_CMD(0xEA, 0x00, 0x00);                                                                               \
    ILI9341_CMD(0xC0, 0x1B);                 /* Power control   //VRH[5:0] */                                    \
    ILI9341_CMD(0xC1, 0x12);                 /* Power control   //SAP[2:0];BT[3:0] */                            \
    ILI9341_CMD(0xC5, 0x32, 0x3C);           /* VCM control */                                                   \
    ILI9341_CMD(0xC7, 0x91);                 /* VCM control2 */                                                  \
    ILI9341_CMD(0x36, 0x08);                 /* Memory Access Control  (BGR) */                                  \
    ILI9341_CMD(0xB1, 0x00, 0x10);           /* Frame Rate Control */                                            \
    ILI9341_CMD(0xB6, 0x0A, 0xA2);           /* Display Function Control */                                      \
    ILI9341_CMD(0xF6, 0x01, 0x30);                                                                               \
    ILI9341_CMD(0xF2, 0x00);                 /* 3Gamma Function Disable */                                       \
    ILI9341_CMD(0x21);                       /* Display inversion on (matches Arduino test sketch) */            \
    ILI9341_CMD(0xE0, 0xD0, 0x00, 0x05, 0x0E, 0x15, 0x0D, 0x37, 0x43, 0x47, 0x09, 0x15, 0x12, 0x16, 0x19);       \
    ILI9341_CMD(0xE1, 0xD0, 0x00, 0x05, 0x0D, 0x0C, 0x06, 0x2D, 0x44, 0x40, 0x0E, 0x1C, 0x18, 0x16, 0x19);

// Input
#define RG_GAMEPAD_GPIO_MAP {\
    {RG_KEY_UP,     .num = GPIO_NUM_18, .pullup = 1, .level = 0},\
    {RG_KEY_LEFT,   .num = GPIO_NUM_8,  .pullup = 1, .level = 0},\
    {RG_KEY_RIGHT,  .num = GPIO_NUM_46, .pullup = 1, .level = 0},\
    {RG_KEY_DOWN,   .num = GPIO_NUM_14, .pullup = 1, .level = 0},\
    {RG_KEY_B,      .num = GPIO_NUM_45, .pullup = 1, .level = 0},\
    {RG_KEY_START,  .num = GPIO_NUM_48, .pullup = 1, .level = 0},\
    {RG_KEY_A,      .num = GPIO_NUM_47, .pullup = 1, .level = 0},\
    {RG_KEY_SELECT, .num = GPIO_NUM_21, .pullup = 1, .level = 0},\
}
#define RG_GAMEPAD_VIRT_MAP {\
    {RG_KEY_MENU,   .src = RG_KEY_START | RG_KEY_SELECT},\
    {RG_KEY_OPTION, .src = RG_KEY_START | RG_KEY_B},\
}

// Optional dedicated power switch exposed as MENU shortcut
#define RG_GPIO_POWER_SWITCH        GPIO_NUM_2

// Status LED
#define RG_GPIO_LED                 GPIO_NUM_38

// SPI Display
#define RG_GPIO_LCD_MISO            GPIO_NUM_NC
#define RG_GPIO_LCD_MOSI            GPIO_NUM_39
#define RG_GPIO_LCD_CLK             GPIO_NUM_40
#define RG_GPIO_LCD_CS              GPIO_NUM_NC
#define RG_GPIO_LCD_DC              GPIO_NUM_38
#define RG_GPIO_LCD_BCKL            GPIO_NUM_42
#define RG_GPIO_LCD_BCKL_INVERT
#define RG_GPIO_LCD_RST             GPIO_NUM_41

// SD Card (SDMMC 1-bit mode)
#define RG_GPIO_SDSPI_CLK           GPIO_NUM_10
#define RG_GPIO_SDSPI_CMD           GPIO_NUM_9
#define RG_GPIO_SDSPI_D0            GPIO_NUM_11

// External I2S DAC
#define RG_GPIO_SND_I2S_BCK         GPIO_NUM_15
#define RG_GPIO_SND_I2S_WS          GPIO_NUM_16
#define RG_GPIO_SND_I2S_DATA        GPIO_NUM_7
#define RG_GPIO_SND_AMP_ENABLE      GPIO_NUM_17
