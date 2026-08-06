#pragma once

#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_types.h"
#include "driver/gpio.h"

#if defined(__cplusplus)
extern "C" {
#endif

// Resolution and bits per pixel for 4D Systems ESP32-S3 RGB LCD models
// All RGB-interface panels (4.3", 5.0", 7.0", 9.0") share the same 800x480
// resolution - only the physical size/density and timing parameters differ.
#define LCD_BITS_PER_PIXEL      16      // RGB565 over the 16-bit parallel bus

// Pin definitions, gen4-ESP32-RGB-REV1.1
// esp_lcd_rgb_panel_config_t's GPIO fields are gpio_num_t (not int) as of
// IDF 6.0 - a plain integer literal in the designated initializer is a
// compile error (esp-idf#12592), so these are GPIO_NUM_x, not bare numbers.
#define LCD_BL_GPIO_NUM          GPIO_NUM_2     // backlight control (PWM)
#define LCD_HSYNC_GPIO_NUM       GPIO_NUM_39
#define LCD_VSYNC_GPIO_NUM       GPIO_NUM_41
#define LCD_DE_GPIO_NUM          GPIO_NUM_40
#define LCD_PCLK_GPIO_NUM        GPIO_NUM_42
#define LCD_DISP_EN_GPIO_NUM     GPIO_NUM_NC    // DISP tied to VDD-3V3 on this board, no GPIO control

#if !defined(CONFIG_ESP32S3_RGB_BYTE_SWAP)
// Data bus order matches esp_lcd_rgb_panel_config_t.data_gpio_nums[0..15]
// (D0..D15), taken directly off the schematic's RGB565 pin table.
#define LCD_DATA_GPIO_NUMS { GPIO_NUM_8, GPIO_NUM_3, GPIO_NUM_46, GPIO_NUM_9, \
                               GPIO_NUM_1, GPIO_NUM_5, GPIO_NUM_6, GPIO_NUM_7, \
                               GPIO_NUM_15, GPIO_NUM_16, GPIO_NUM_4, GPIO_NUM_45, \
                               GPIO_NUM_48, GPIO_NUM_47, GPIO_NUM_21, GPIO_NUM_14 }

#else
// This data bus order is deliberately re-configured to match the endianess needed by SPI/QSPI variants
#define LCD_DATA_GPIO_NUMS { GPIO_NUM_15, GPIO_NUM_16, GPIO_NUM_4, GPIO_NUM_45, \
                              GPIO_NUM_48, GPIO_NUM_47, GPIO_NUM_21, GPIO_NUM_14, \
                              GPIO_NUM_8, GPIO_NUM_3, GPIO_NUM_46, GPIO_NUM_9, \
                              GPIO_NUM_1, GPIO_NUM_5, GPIO_NUM_6, GPIO_NUM_7 }
#endif

// Timing parameters are panel-size specific and come from the LCD's own
// datasheet, not this schematic - see the README's 4DLCD-xxxxxx datasheet
// links. Selecting the wrong one will not "mostly work"; it'll be a blank,
// torn, or rolling image, since it's the video timing itself that's wrong.
#if defined(CONFIG_ESP32S3_LCD_43)
#define LCD_PCLK_HZ              (16 * 1000 * 1000)
#define LCD_HSYNC_PULSE_WIDTH    4
#define LCD_HSYNC_BACK_PORCH     8
#define LCD_HSYNC_FRONT_PORCH    8
#define LCD_VSYNC_PULSE_WIDTH    4
#define LCD_VSYNC_BACK_PORCH     8
#define LCD_VSYNC_FRONT_PORCH    8
#define LCD_PCLK_ACTIVE_NEG      true
#define LCD_HSYNC_IDLE_LOW       false
#define LCD_VSYNC_IDLE_LOW       true
#define LCD_DE_IDLE_HIGH         false
#elif defined(CONFIG_ESP32S3_LCD_50)
#define LCD_PCLK_HZ              (16 * 1000 * 1000)
#define LCD_HSYNC_PULSE_WIDTH    4
#define LCD_HSYNC_BACK_PORCH     8
#define LCD_HSYNC_FRONT_PORCH    8
#define LCD_VSYNC_PULSE_WIDTH    4
#define LCD_VSYNC_BACK_PORCH     8
#define LCD_VSYNC_FRONT_PORCH    8
#define LCD_PCLK_ACTIVE_NEG      true
#define LCD_HSYNC_IDLE_LOW       true
#define LCD_VSYNC_IDLE_LOW       true
#define LCD_DE_IDLE_HIGH         true
#elif defined(CONFIG_ESP32S3_LCD_70)
#define LCD_PCLK_HZ              (18 * 1000 * 1000)
#define LCD_HSYNC_PULSE_WIDTH    50
#define LCD_HSYNC_BACK_PORCH     32
#define LCD_HSYNC_FRONT_PORCH    1
#define LCD_VSYNC_PULSE_WIDTH    1
#define LCD_VSYNC_BACK_PORCH     33
#define LCD_VSYNC_FRONT_PORCH    1
#define LCD_PCLK_ACTIVE_NEG      true
#define LCD_HSYNC_IDLE_LOW       false
#define LCD_VSYNC_IDLE_LOW       true
#define LCD_DE_IDLE_HIGH         false
#elif defined(CONFIG_ESP32S3_LCD_90)
#define LCD_PCLK_HZ              (16 * 1000 * 1000)
#define LCD_HSYNC_PULSE_WIDTH    50
#define LCD_HSYNC_BACK_PORCH     32
#define LCD_HSYNC_FRONT_PORCH    1
#define LCD_VSYNC_PULSE_WIDTH    1
#define LCD_VSYNC_BACK_PORCH     33
#define LCD_VSYNC_FRONT_PORCH    1
#define LCD_PCLK_ACTIVE_NEG      true
#define LCD_HSYNC_IDLE_LOW       false
#define LCD_VSYNC_IDLE_LOW       true
#define LCD_DE_IDLE_HIGH         false
#else
#error "No valid 4D Systems RGB LCD model defined"
#endif

#define LCD_BL_PWM_FREQ_HZ       25000    // PWM frequency (25kHz)
#define LCD_BL_PWM_RESOLUTION    LEDC_TIMER_8_BIT  // 8-bit resolution (0-255)

/**
 * @brief RGB panel configuration structure for 4D Systems ESP32-S3-RGB series
 *
 * @note IDF 6.0+ layout: bits_per_pixel was removed in favor of in_color_format
 *       (LCD_COLOR_FMT_RGB565), and sram_trans_align/psram_trans_align were
 *       replaced by a single dma_burst_size. This will NOT compile against
 *       IDF <6.0 as written - those fields didn't exist yet there.
 * @note fb_in_psram is required: these boards use octal PSRAM for the frame
 *       buffer (800x480x2 = 768000 bytes), which does not fit in internal SRAM
 *       alongside WiFi/BT. CONFIG_SPIRAM must be enabled in sdkconfig.
 */
#define ESP32S3_LCD_RGB_PANEL_CONFIG()                              \
    {                                                                 \
        .clk_src = LCD_CLK_SRC_PLL160M,                               \
        .timings = {                                                  \
            .pclk_hz = LCD_PCLK_HZ,                                   \
            .h_res = CONFIG_ESP32S3_4D_LCD_WIDTH,                     \
            .v_res = CONFIG_ESP32S3_4D_LCD_HEIGHT,                    \
            .hsync_pulse_width = LCD_HSYNC_PULSE_WIDTH,               \
            .hsync_back_porch = LCD_HSYNC_BACK_PORCH,                 \
            .hsync_front_porch = LCD_HSYNC_FRONT_PORCH,               \
            .vsync_pulse_width = LCD_VSYNC_PULSE_WIDTH,               \
            .vsync_back_porch = LCD_VSYNC_BACK_PORCH,                 \
            .vsync_front_porch = LCD_VSYNC_FRONT_PORCH,               \
            .flags = {                                                \
                .hsync_idle_low = LCD_HSYNC_IDLE_LOW,                 \
                .vsync_idle_low = LCD_VSYNC_IDLE_LOW,                 \
                .de_idle_high = LCD_DE_IDLE_HIGH,                     \
                .pclk_active_neg = LCD_PCLK_ACTIVE_NEG,               \
            },                                                        \
        },                                                            \
        .data_width = 16,                                             \
        .in_color_format = LCD_COLOR_FMT_RGB565,                      \
        .num_fbs = 2,                                                 \
        .bounce_buffer_size_px = 10 * CONFIG_ESP32S3_4D_LCD_WIDTH,    \
        .dma_burst_size = 64,                                         \
        .hsync_gpio_num = LCD_HSYNC_GPIO_NUM,                         \
        .vsync_gpio_num = LCD_VSYNC_GPIO_NUM,                         \
        .de_gpio_num = LCD_DE_GPIO_NUM,                               \
        .pclk_gpio_num = LCD_PCLK_GPIO_NUM,                           \
        .disp_gpio_num = LCD_DISP_EN_GPIO_NUM,                        \
        .data_gpio_nums = LCD_DATA_GPIO_NUMS,                         \
        .flags = {                                                    \
            .fb_in_psram = true,                                      \
        },                                                            \
    }

#if defined(__cplusplus)
}
#endif