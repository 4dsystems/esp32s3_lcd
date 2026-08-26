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

// LCD default display orientation, with no mirroring
// useful when graphics libraries like esp_lvgl_port reinits the swapping and mirroring
// for RGB LCDs, there's no mirroring or swapping at all
#define LCD_DISP_SWAP_XY        0
#define LCD_DISP_MIRROR_X       0
#define LCD_DISP_MIRROR_Y       0

// Pin definitions, gen4-ESP32-RGB-REV1.1
#define LCD_BL_GPIO_NUM         GPIO_NUM_2     // backlight control (PWM)
#define LCD_HSYNC_GPIO_NUM      GPIO_NUM_39
#define LCD_VSYNC_GPIO_NUM      GPIO_NUM_41
#define LCD_DE_GPIO_NUM         GPIO_NUM_40
#define LCD_PCLK_GPIO_NUM       GPIO_NUM_42
#define LCD_DISP_EN_GPIO_NUM    GPIO_NUM_NC    // DISP tied to VDD-3V3 on this board, no GPIO control
#define LCD_TOUCH_SDA_GPIO_NUM  GPIO_NUM_17
#define LCD_TOUCH_SCL_GPIO_NUM  GPIO_NUM_18
#define LCD_TOUCH_INT_GPIO_NUM  GPIO_NUM_NC
#define LCD_TOUCH_RST_GPIO_NUM  GPIO_NUM_NC

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
// links.
// Note: The following timing parameters are not of standard spec, but are fine-tuned
//       for the the capabilities of the ESP32-S3 and the requirements of the LCD
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
#define LCD_TOUCH_SWAP_MAX_XY    0
#define LCD_TOUCH_SWAP_XY        1
#define LCD_TOUCH_MIRROR_X       0
#define LCD_TOUCH_MIRROR_Y       0
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
#define LCD_TOUCH_SWAP_MAX_XY    1
#define LCD_TOUCH_SWAP_XY        1
#define LCD_TOUCH_MIRROR_X       1
#define LCD_TOUCH_MIRROR_Y       0
#elif defined(CONFIG_ESP32S3_LCD_70)
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
#define LCD_TOUCH_SWAP_MAX_XY    0
#define LCD_TOUCH_SWAP_XY        1
#define LCD_TOUCH_MIRROR_X       0
#define LCD_TOUCH_MIRROR_Y       0
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
#define LCD_TOUCH_SWAP_MAX_XY    0
#define LCD_TOUCH_SWAP_XY        1
#define LCD_TOUCH_MIRROR_X       0
#define LCD_TOUCH_MIRROR_Y       0
#else
#error "No valid 4D Systems RGB LCD model defined"
#endif

#define LCD_BL_PWM_FREQ_HZ       25000    // PWM frequency (25kHz)

#if defined(__cplusplus)
}
#endif