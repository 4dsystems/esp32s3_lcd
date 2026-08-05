idf_component_register(SRCS "esp32s3_4dlcd.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "esp_lcd esp_driver_ledc"
                    PRIV_REQUIRES "driver")
                    
