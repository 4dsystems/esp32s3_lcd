#pragma once

#include "driver/spi_master.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define LCD_COLOR_ORDER         LCD_RGB_ELEMENT_ORDER_RGB // Default color order for 4D Systems displays
#define LCD_RST_ACTIVE_HIGH     0 // Reset pin active low

// Resolution and bits per pixel for different 4D Systems ESP32-S3 LCD models
#if defined(CONFIG_ESP32S3_LCD_35)
#define LCD_BITS_PER_PIXEL      18
#elif defined(CONFIG_ESP32S3_LCD_43Q)
#define LCD_BITS_PER_PIXEL      16
#else
#define LCD_BITS_PER_PIXEL      16
#endif

// Pin definitions for SPI/QSPI, backlight, and reset
#if defined(CONFIG_ESP32S3_LCD_43Q)
#define LCD_BL_GPIO_NUM         2       // GPIO for backlight control
#define LCD_RST_GPIO_NUM        8       // GPIO for LCD reset
#define LCD_SPI_CS_GPIO_NUM     6       // GPIO for SPI CS
#define LCD_SPI_DC_GPIO_NUM     -1      // GPIO for SPI DC (Data/Command) (not used)
#define LCD_SPI_SCLK_GPIO_NUM   5       // GPIO for SPI SCLK
#define LCD_QSPI_DAT0_GPIO_NUM  9       // GPIO for QSPI DATA0
#define LCD_QSPI_DAT1_GPIO_NUM  7       // GPIO for QSPI DATA1
#define LCD_QSPI_DAT2_GPIO_NUM  4       // GPIO for QSPI DATA2
#define LCD_QSPI_DAT3_GPIO_NUM  3       // GPIO for QSPI DATA3
#define LCD_SPI_PCLK_MHZ        30      // QSPI clock frequency in MHz
#else
#define LCD_BL_GPIO_NUM         4       // GPIO for backlight control
#define LCD_RST_GPIO_NUM        7       // GPIO for LCD reset
#define LCD_SPI_CS_GPIO_NUM     -1      // GPIO for SPI CS (not used)
#define LCD_SPI_DC_GPIO_NUM     21      // GPIO for SPI DC (Data/Command)
#define LCD_SPI_SCLK_GPIO_NUM   14      // GPIO for SPI SCLK
#define LCD_SPI_MISO_GPIO_NUM   12      // GPIO for SPI MISO
#define LCD_SPI_MOSI_GPIO_NUM   13      // GPIO for SPI MOSI
#define LCD_SPI_PCLK_MHZ        60      // SPI clock frequency in MHz
#endif

#define LCD_BL_PWM_FREQ_HZ      25000    // PWM frequency (25kHz)
#define LCD_BL_PWM_RESOLUTION   LEDC_TIMER_8_BIT  // 8-bit resolution (0-255)

#if defined(CONFIG_ESP32S3_LCD_SPI2)
#define LCD_HOST    SPI2_HOST
#else // CONFIG_ESP32S3_LCD_SPI3
#define LCD_HOST    SPI3_HOST
#endif

// Touch related parameters
#define LCD_TOUCH_SWAP_MAX_XY    0
#if defined(CONFIG_ESP32S3_LCD_43Q)
#define LCD_TOUCH_SWAP_XY        1 // only 4.3QSPI needs swapping
#else
#define LCD_TOUCH_SWAP_XY        0
#endif
#define LCD_TOUCH_MIRROR_X       0
#define LCD_TOUCH_MIRROR_Y       0

#if defined(__cplusplus)
}
#endif