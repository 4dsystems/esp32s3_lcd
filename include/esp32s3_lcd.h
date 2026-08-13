/*
 * 4D Systems Pty Ltd
 * www.4dsystems.com.au
 *
 * SPDX-FileCopyrightText: 
 *   - 2022-2023 Espressif Systems (Shanghai) CO LTD
 *   - 4D Systems Pty Ltd
 * SPDX-License-Identifier: Apache-2.0
 */
/**
 * @file
 * @brief ESP LCD: 4D Systems' ESP32-S3 Series
 */

#pragma once

#include "sdkconfig.h"

#if defined(CONFIG_ESP32S3_LCD_ALLOW_NOPSRAM) && \
    !(defined(CONFIG_LCD_INTERFACE_SPI) || defined(CONFIG_LCD_INTERFACE_QSPI))
#error "CONFIG_ESP32S3_LCD_ALLOW_NOPSRAM is only valid on SPI and QSPI displays - the selected display's framebuffer requires PSRAM."
#endif

#if !defined(CONFIG_ESP32S3_LCD_ALLOW_NOPSRAM) && !defined(CONFIG_SPIRAM)
#error "This board requires PSRAM. Enable CONFIG_SPIRAM, or enable CONFIG_ESP32S3_LCD_ALLOW_NOPSRAM (SPI and QSPI displays only) to opt out."
#endif

#if defined(CONFIG_SPIRAM) && !defined(CONFIG_SPIRAM_MODE_OCT)
#error "This board's PSRAM is octal, not quad. Select Octal Mode PSRAM."
#endif

#if defined(CONFIG_SPIRAM) && (defined(CONFIG_ESPTOOLPY_FLASHMODE_DIO) || defined(CONFIG_ESPTOOLPY_FLASHMODE_DOUT))
#error "This board's flash chip (GD25Q127C) is quad-SPI. DIO/DOUT flash mode combined with octal PSRAM crashes at boot during MSPI timing tuning."
#endif

#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_io_spi.h"
#include "esp_check.h"
#include "driver/ledc.h"

#include "esp_lcd_touch.h"

#if defined(CONFIG_LCD_INTERFACE_RGB)
#include "rgb_lcd.h"
#else // CONFIG_LCD_INTERFACE_QSPI or CONFIG_LCD_INTERFACE_SPI
#include "spi_lcd.h"
#endif // LCD_INTERFACE

#if defined(CONFIG_ESP32S3_LCD_I2C0)
#define LCD_I2C_NUM     I2C_NUM_0
#else // CONFIG_ESP32S3_LCD_I2C1
#define LCD_I2C_NUM     I2C_NUM_1
#endif

#define LCD_TOUCH_AREA_MAX_X        (CONFIG_ESP32S3_4D_LCD_WIDTH - 1)
#define LCD_TOUCH_AREA_MAX_Y        (CONFIG_ESP32S3_4D_LCD_HEIGHT - 1)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief LCD panel initialization commands.
 *
 */
typedef struct {
    int cmd;                /*<! The specific LCD command */
    const void *data;       /*<! Buffer that holds the command specific data */
    size_t data_bytes;      /*<! Size of `data` in memory, in bytes */
    unsigned int delay_ms;  /*<! Delay in milliseconds after this command */
} esp32s3_lcd_init_cmd_t;

/**
 * @brief LCD panel vendor configuration.
 *
 * @note  This structure needs to be passed to the `vendor_config` field in `esp_lcd_panel_dev_config_t`.
 *
 */
typedef struct {
    const esp32s3_lcd_init_cmd_t *init_cmds;      /*!< Pointer to initialization commands array. Set to NULL if using default commands.
                                                         *   The array should be declared as `static const` and positioned outside the function.
                                                         *   Please refer to `vendor_specific_init_default` in source file.
                                                         */
    uint16_t init_cmds_size;                            /*<! Number of commands in above array */
} esp32s3_lcd_vendor_config_t;

/**
 * @brief Create LCD panel for 4D Systems ESP32-S3 series of displays
 *
 * @note  Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for initialization sequence code.
 *
 * @param[in] io LCD panel IO handle
 * @param[out] ret_panel Returned LCD panel handle
 * @return
 *          - ESP_ERR_INVALID_ARG   if parameter is invalid
 *          - ESP_ERR_NO_MEM        if out of memory
 *          - ESP_OK                on success
 */
esp_err_t esp_lcd_new_esp32s3_lcd(const esp_lcd_panel_io_handle_t io, esp_lcd_panel_handle_t *ret_panel);

/**
 * @brief LCD panel bus configuration structure
 *
 * @param[in] max_trans_sz Maximum transfer size in bytes
 *
 */
#if defined(CONFIG_LCD_INTERFACE_SPI)
#define ESP32S3_LCD_BUS_SPI_CONFIG(max_trans_sz)                \
    {                                                           \
        .sclk_io_num = LCD_SPI_SCLK_GPIO_NUM,                   \
        .mosi_io_num = LCD_SPI_MOSI_GPIO_NUM,                   \
        .miso_io_num = LCD_SPI_MISO_GPIO_NUM,                   \
        .quadhd_io_num = -1,                                    \
        .quadwp_io_num = -1,                                    \
        .max_transfer_sz = max_trans_sz,                        \
    }
#elif defined(CONFIG_LCD_INTERFACE_QSPI)
#define ESP32S3_LCD_BUS_SPI_CONFIG(max_trans_sz)                \
    {                                                           \
        .sclk_io_num = LCD_SPI_SCLK_GPIO_NUM,                   \
        .data0_io_num = LCD_QSPI_DAT0_GPIO_NUM,                 \
        .data1_io_num = LCD_QSPI_DAT1_GPIO_NUM,                 \
        .data2_io_num = LCD_QSPI_DAT2_GPIO_NUM,                 \
        .data3_io_num = LCD_QSPI_DAT3_GPIO_NUM,                 \
        .max_transfer_sz = max_trans_sz,                        \
    }
#else // CONFIG_ LCD_INTERFACE_RGB
// TODO
#endif // CONFIG_LCD_INTERFACE

/**
 * @brief LCD panel IO configuration structure
 *
 * @param[in] cb Callback function when SPI transfer is done
 * @param[in] cb_ctx Callback function context
 *
 */
#if defined(CONFIG_LCD_INTERFACE_SPI)
#define ESP32S3_LCD_IO_SPI_CONFIG(callback, callback_ctx)       \
    {                                                           \
        .cs_gpio_num = LCD_SPI_CS_GPIO_NUM,                     \
        .dc_gpio_num = LCD_SPI_DC_GPIO_NUM,                     \
        .spi_mode = 0,                                          \
        .pclk_hz = LCD_SPI_PCLK_MHZ * 1000 * 1000,              \
        .trans_queue_depth = 7,                                 \
        .on_color_trans_done = callback,                        \
        .user_ctx = callback_ctx,                               \
        .lcd_cmd_bits = 8,                                      \
        .lcd_param_bits = 8,                                    \
    }
#else // CONFIG_LCD_INTERFACE_QSPI
#define ESP32S3_LCD_IO_SPI_CONFIG(callback, callback_ctx)       \
    {                                                           \
        .cs_gpio_num = LCD_SPI_CS_GPIO_NUM,                     \
        .dc_gpio_num = LCD_SPI_DC_GPIO_NUM,                     \
        .spi_mode = 0,                                          \
        .pclk_hz = LCD_SPI_PCLK_MHZ * 1000 * 1000,              \
        .trans_queue_depth = 10,                                \
        .on_color_trans_done = callback,                        \
        .user_ctx = callback_ctx,                               \
        .lcd_cmd_bits = 32,                                     \
        .lcd_param_bits = 8,                                    \
        .flags.quad_mode = true                                 \
    }
#endif // CONFIG_LCD_INTERFACE

#define ESP3233_LCD_ONBOARD_I2C_CONFIG()                        \
    {                                                           \
        .clk_source = I2C_CLK_SRC_DEFAULT,                      \
        .i2c_port = LCD_I2C_NUM,                                \
        .scl_io_num = LCD_TOUCH_SCL_GPIO_NUM,                   \
        .sda_io_num = LCD_TOUCH_SDA_GPIO_NUM,                   \
        .glitch_ignore_cnt = 7,                                 \
        .flags.enable_internal_pullup = true,                   \
    }

esp_err_t backlight_init(void);
esp_err_t backlight_set(uint8_t brightness);

/**
 * @brief Bring up SPI/QSPI or RGB bus + panel IO + panel + backlight in one call.
 *
 * @param[out] ret_panel  Returned LCD panel handle, ready for esp_lcd_panel_draw_bitmap()
 * @return esp_err_t
 */
esp_err_t esp32s3_lcd_full_init(esp_lcd_panel_handle_t *ret_panel);

#if defined(CONFIG_LCD_INTERFACE_RGB)
/**
 * @brief Assign callbacks to the panel_handle using `esp_lcd_rgb_panel_register_event_callbacks`
 *
 * @param[out] result  error or success based on `esp_lcd_rgb_panel_register_event_callbacks`
 * @return esp_err_t
 */
esp_err_t esp32s3_lcd_register_event_callbacks(const esp_lcd_rgb_panel_event_callbacks_t *cbs, void *user_ctx);
#else
/**
 * @brief Assign callbacks to the io_handle using `esp_lcd_panel_io_register_event_callbacks`
 *
 * @param[out] result  error or success based on `esp_lcd_panel_io_register_event_callbacks`
 * @return esp_err_t
 */
esp_err_t esp32s3_lcd_register_event_callbacks(const esp_lcd_panel_io_callbacks_t *cbs, void *user_ctx);
#endif

/**
 * @brief Bring up I2C touch in one call.
 *
 * @param[out] ret_panel  Returned touch panel handle, ready for esp_lcd_touch functions
 * @return esp_err_t
 */
esp_err_t esp32s3_lcd_touch_init(esp_lcd_touch_handle_t *tp);

#ifdef __cplusplus
}
#endif