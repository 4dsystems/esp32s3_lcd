idf_component_register(SRCS "esp32s3_lcd.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_lcd esp_driver_ledc esp_lcd_touch_ft5x06 esp_lcd_touch_gt911 esp_io_expander_tca9554
                    PRIV_REQUIRES driver)