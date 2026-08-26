#pragma once

#include "driver/spi_master.h"

#if defined(__cplusplus)
extern "C" {
#endif

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
#define LCD_BL_GPIO_NUM         GPIO_NUM_2      // GPIO for backlight control
#define LCD_RST_GPIO_NUM        GPIO_NUM_8      // GPIO for LCD reset
#define LCD_SPI_CS_GPIO_NUM     GPIO_NUM_6      // GPIO for SPI CS
#define LCD_SPI_DC_GPIO_NUM     GPIO_NUM_NC     // GPIO for SPI DC (Data/Command) (not used)
#define LCD_SPI_SCLK_GPIO_NUM   GPIO_NUM_5      // GPIO for SPI SCLK
#define LCD_QSPI_DAT0_GPIO_NUM  GPIO_NUM_9      // GPIO for QSPI DATA0
#define LCD_QSPI_DAT1_GPIO_NUM  GPIO_NUM_7      // GPIO for QSPI DATA1
#define LCD_QSPI_DAT2_GPIO_NUM  GPIO_NUM_4      // GPIO for QSPI DATA2
#define LCD_QSPI_DAT3_GPIO_NUM  GPIO_NUM_3      // GPIO for QSPI DATA3
#define LCD_SPI_PCLK_MHZ        30              // QSPI clock frequency in MHz
#define LCD_TOUCH_SDA_GPIO_NUM  GPIO_NUM_17
#define LCD_TOUCH_SCL_GPIO_NUM  GPIO_NUM_18
#define LCD_TOUCH_INT_GPIO_NUM  GPIO_NUM_42
#define LCD_TOUCH_RST_GPIO_NUM  GPIO_NUM_41
#else
#define LCD_BL_GPIO_NUM         GPIO_NUM_4       // GPIO for backlight control
#define LCD_RST_GPIO_NUM        GPIO_NUM_7       // GPIO for LCD reset
#define LCD_SPI_CS_GPIO_NUM     GPIO_NUM_NC      // GPIO for SPI CS (not used)
#define LCD_SPI_DC_GPIO_NUM     GPIO_NUM_21      // GPIO for SPI DC (Data/Command)
#define LCD_SPI_SCLK_GPIO_NUM   GPIO_NUM_14      // GPIO for SPI SCLK
#define LCD_SPI_MISO_GPIO_NUM   GPIO_NUM_12      // GPIO for SPI MISO
#define LCD_SPI_MOSI_GPIO_NUM   GPIO_NUM_13      // GPIO for SPI MOSI
#define LCD_SPI_PCLK_MHZ        60               // SPI clock frequency in MHz
#define LCD_TOUCH_SDA_GPIO_NUM  GPIO_NUM_10
#define LCD_TOUCH_SCL_GPIO_NUM  GPIO_NUM_9
#define LCD_TOUCH_INT_GPIO_NUM  GPIO_NUM_8
#define LCD_TOUCH_RST_GPIO_NUM  GPIO_NUM_11
#endif

#define LCD_BL_PWM_FREQ_HZ      25000    // PWM frequency (25kHz)

#if defined(CONFIG_ESP32S3_LCD_SPI2)
#define LCD_SPI_HOST    SPI2_HOST
#else // CONFIG_ESP32S3_LCD_SPI3
#define LCD_SPI_HOST    SPI3_HOST
#endif

// Touch related parameters FT5446 series
#define LCD_TOUCH_SWAP_MAX_XY    0
#if defined(CONFIG_ESP32S3_LCD_43Q)
#define LCD_TOUCH_SWAP_XY        1 // only 4.3QSPI needs swapping
#define LCD_TOUCH_MIRROR_X       0
#define LCD_TOUCH_MIRROR_Y       0
#elif defined(CONFIG_ESP32S3_LCD_35) || defined(CONFIG_ESP32S3_LCD_32)
#define LCD_TOUCH_SWAP_XY        0
#define LCD_TOUCH_MIRROR_X       0
#define LCD_TOUCH_MIRROR_Y       1
#else // 2.4-inch and 2.8-inch
#define LCD_TOUCH_SWAP_XY        0
#define LCD_TOUCH_MIRROR_X       0
#define LCD_TOUCH_MIRROR_Y       0
#endif

#if defined(__cplusplus)
}
#endif