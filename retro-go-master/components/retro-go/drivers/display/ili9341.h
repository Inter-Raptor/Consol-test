#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <driver/spi_master.h>
#include <driver/gpio.h>

static spi_device_handle_t spi_dev;
static QueueHandle_t spi_buffers;

#define SPI_BUFFER_COUNT      (5)
#define SPI_BUFFER_LENGTH     (LCD_BUFFER_LENGTH * 2)

#define ILI9341_CMD(cmd, data...)                    \
    {                                                \
        const uint8_t c = cmd, x[] = {data};         \
        spi_queue_transaction(&c, 1, 0);             \
        if (sizeof(x))                               \
            spi_queue_transaction(&x, sizeof(x), 1); \
    }

static inline uint16_t *spi_take_buffer(void)
{
    uint16_t *buffer;
    if (xQueueReceive(spi_buffers, &buffer, pdMS_TO_TICKS(2500)) != pdTRUE)
        RG_PANIC("display");
    return buffer;
}

static inline void spi_give_buffer(uint16_t *buffer)
{
    xQueueSend(spi_buffers, &buffer, portMAX_DELAY);
}

static inline void spi_queue_transaction(const void *data, size_t length, uint32_t type)
{
    if (!data || !length)
        return;

    spi_transaction_t t = {
        .tx_buffer = NULL,
        .length = length * 8,
        .user = (void *)type,
        .flags = 0,
    };

    uint16_t *temp = NULL;

    if (type & 2)
    {
        t.tx_buffer = data;
    }
    else if (length < 5)
    {
        memcpy(t.tx_data, data, length);
        t.flags = SPI_TRANS_USE_TXDATA;
    }
    else
    {
        temp = spi_take_buffer();
        memcpy(temp, data, length);
        t.tx_buffer = temp;
    }

    if (spi_device_polling_transmit(spi_dev, &t) != ESP_OK)
        RG_PANIC("display");

    if (temp)
        spi_give_buffer(temp);

    if (type & 2)
        spi_give_buffer((uint16_t *)data);
}

IRAM_ATTR
static void spi_pre_transfer_cb(spi_transaction_t *t)
{
    // Set the data/command line accordingly
    gpio_set_level(RG_GPIO_LCD_DC, (int)t->user & 1);
}

static void spi_init(void)
{
    spi_buffers = xQueueCreate(SPI_BUFFER_COUNT, sizeof(uint16_t *));

    while (uxQueueSpacesAvailable(spi_buffers))
    {
        void *buffer = rg_alloc(SPI_BUFFER_LENGTH, MEM_DMA);
        xQueueSend(spi_buffers, &buffer, portMAX_DELAY);
    }

    const spi_bus_config_t buscfg = {
        .miso_io_num = RG_GPIO_LCD_MISO,
        .mosi_io_num = RG_GPIO_LCD_MOSI,
        .sclk_io_num = RG_GPIO_LCD_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    const spi_device_interface_config_t devcfg = {
        .clock_speed_hz = RG_SCREEN_SPEED,
        .mode = 0,
        .spics_io_num = -1,
        .queue_size = 1,
        .pre_cb = &spi_pre_transfer_cb,
        .flags = SPI_DEVICE_NO_DUMMY,
    };

    esp_err_t ret;

    // Initialize the SPI bus
    ret = spi_bus_initialize(RG_SCREEN_HOST, &buscfg, SPI_DMA_CH_AUTO);
    RG_ASSERT(ret == ESP_OK || ret == ESP_ERR_INVALID_STATE, "spi_bus_initialize failed.");

    ret = spi_bus_add_device(RG_SCREEN_HOST, &devcfg, &spi_dev);
    RG_ASSERT(ret == ESP_OK, "spi_bus_add_device failed.");

}

static void spi_deinit(void)
{
    // When transactions are still in flight, spi_bus_remove_device fails and spi_bus_free then crashes.
    // The real solution would be to wait for transactions to be done, but this is simpler for now...
    if (spi_bus_remove_device(spi_dev) == ESP_OK)
        spi_bus_free(RG_SCREEN_HOST);
    else
        RG_LOGE("Failed to properly terminate SPI driver!");
}

static void lcd_set_backlight(float percent)
{
#if defined(RG_GPIO_LCD_BCKL)
    float level = RG_MIN(RG_MAX(percent / 100.f, 0), 1.f);
    int value = (level > 0.f) ? 0 : 1; // HU-086 backlight is active LOW
    gpio_set_level(RG_GPIO_LCD_BCKL, value);
    RG_LOGI("backlight set to %d%%\n", (int)(100 * level));
#else
    RG_UNUSED(percent);
#endif
}

static void lcd_set_window(int left, int top, int width, int height)
{
    if (width < 1) width = 1;
    if (height < 1) height = 1;

#ifndef RG_SCREEN_OFFSET_X
#define RG_SCREEN_OFFSET_X 0
#endif
#ifndef RG_SCREEN_OFFSET_Y
#define RG_SCREEN_OFFSET_Y 0
#endif

    int x1 = left + RG_SCREEN_OFFSET_X;
    int y1 = top + RG_SCREEN_OFFSET_Y;
    int x2 = x1 + width - 1;
    int y2 = y1 + height - 1;

    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 < x1) x2 = x1;
    if (y2 < y1) y2 = y1;

    if (x2 >= display.screen.real_width) x2 = display.screen.real_width - 1;
    if (y2 >= display.screen.real_height) y2 = display.screen.real_height - 1;

    ILI9341_CMD(0x2A, x1 >> 8, x1 & 0xff, x2 >> 8, x2 & 0xff);
    ILI9341_CMD(0x2B, y1 >> 8, y1 & 0xff, y2 >> 8, y2 & 0xff);
    ILI9341_CMD(0x2C);
}


static inline uint16_t *lcd_get_buffer(size_t length)
{
    // RG_ASSERT_ARG(length < LCD_BUFFER_LENGTH);
    return spi_take_buffer();
}

static inline void lcd_send_buffer(uint16_t *buffer, size_t length)
{
    if (length > 0)
        spi_queue_transaction(buffer, length * sizeof(*buffer), 3);
    else
        spi_give_buffer(buffer);
}

static void lcd_sync(void)
{
    // Unused for SPI LCD
}

static void lcd_init(void)
{
#ifdef RG_GPIO_LCD_BCKL
    gpio_reset_pin(RG_GPIO_LCD_BCKL);
    gpio_set_direction(RG_GPIO_LCD_BCKL, GPIO_MODE_OUTPUT);

    // Sur ta console : BL actif à LOW → écran allumé
    gpio_set_level(RG_GPIO_LCD_BCKL, 0);
#endif

    spi_init();

    // Setup Data/Command line
    gpio_set_direction(RG_GPIO_LCD_DC, GPIO_MODE_OUTPUT);
    gpio_set_level(RG_GPIO_LCD_DC, 1);

#if defined(RG_GPIO_LCD_RST)
    gpio_set_direction(RG_GPIO_LCD_RST, GPIO_MODE_OUTPUT);
    gpio_set_level(RG_GPIO_LCD_RST, 0);
    rg_usleep(100 * 1000);
    gpio_set_level(RG_GPIO_LCD_RST, 1);
    rg_usleep(10 * 1000);
#endif

    ILI9341_CMD(0x01);       // Reset
    rg_usleep(5 * 1000);     // Petit délai après reset

#ifdef RG_SCREEN_INIT
    // HU-086 : toute la séquence (0x11, 0x36, 0x3A, 0x29, 0x21, etc.)
    // est déjà définie dans RG_SCREEN_INIT de ton config.h
    RG_SCREEN_INIT();
#else
    #warning "LCD init sequence is not defined for this device!"
#endif

#ifdef RG_SCREEN_BOOT_TEST
    for (int pass = 0; pass < 3; ++pass)
    {
        const uint16_t color = (pass == 0) ? 0xF800 : (pass == 1) ? 0x07E0 : 0x001F;
        lcd_set_window(0, 0, display.screen.real_width, display.screen.real_height);
        size_t pixels = display.screen.real_width * display.screen.real_height;
        while (pixels)
        {
            size_t chunk = RG_MIN(pixels, LCD_BUFFER_LENGTH);
            uint16_t *buf = lcd_get_buffer(chunk);
            for (size_t i = 0; i < chunk; ++i)
                buf[i] = color;
            lcd_send_buffer(buf, chunk);
            pixels -= chunk;
        }
        rg_usleep(120 * 1000);
    }
#endif

}

static void lcd_deinit(void)
{
#ifdef RG_SCREEN_DEINIT
    RG_SCREEN_DEINIT();
#endif
    spi_deinit();
    // gpio_reset_pin(RG_GPIO_LCD_BCKL);
    // gpio_reset_pin(RG_GPIO_LCD_DC);
}

const rg_display_driver_t rg_display_driver_ili9341 = {
    .name = "ili9341",
};
