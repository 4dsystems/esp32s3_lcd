# 4D Systems' ESP32-S3 series displays

Implementation of the LCD controller with esp_lcd component for 4D Systems' ESP32-S3 series displays.

| 4DLCD Series                                                                                  | Supported | LCD Controller                                      | Interface | Link to LCD Datasheet |
|:--------------------------------------------------------------------------------------------- |:---------:|:---------------------------------------------------:|:---------:|:--------------------- |
| [gen4-ESP32-24 Series](https://resources.4dsystems.com.au/datasheets/esp32/gen4-esp32/)       | ✅       | [ILI9341](https://4dsystems.com.au/download/31395/) | SPI       | [Datasheet](https://resources.4dsystems.com.au/datasheets/4dlcd/4DLCD-24320240-IPS/) |
| [gen4-ESP32-28 Series](https://resources.4dsystems.com.au/datasheets/esp32/gen4-esp32/)       | ✅       | [ILI9341](https://4dsystems.com.au/download/31395/) | SPI       | [Datasheet](https://resources.4dsystems.com.au/datasheets/4dlcd/4DLCD-28320240-IPS/) |
| [gen4-ESP32-32 Series](https://resources.4dsystems.com.au/datasheets/esp32/gen4-esp32/)       | ✅       | [ILI9341](https://4dsystems.com.au/download/31395/) | SPI       | [Datasheet](https://resources.4dsystems.com.au/datasheets/4dlcd/4DLCD-32320240/)     |
| [gen4-ESP32-35 Series](https://resources.4dsystems.com.au/datasheets/esp32/gen4-esp32/)       | ✅       | [ILI9488](https://4dsystems.com.au/download/31399/) | SPI       | [Datasheet](https://resources.4dsystems.com.au/datasheets/4dlcd/4DLCD-35480320-IPS/) |
| [gen4-ESP32Q-43 Series](https://resources.4dsystems.com.au/datasheets/esp32/gen4-esp32Q-43/)  | ✅       | [NV3041A](https://4dsystems.com.au/download/31400/) | QSPI      | Not available         |
| [gen4-ESP32-43 Series](https://resources.4dsystems.com.au/datasheets/esp32/gen4-esp32-RGB/)   | ✅       |                                                     | RGB       | [Datasheet](https://resources.4dsystems.com.au/datasheets/4dlcd/4DLCD-43480272-IPS/) |
| [gen4-ESP32-50 Series](https://resources.4dsystems.com.au/datasheets/esp32/gen4-esp32-RGB/)   | ✅       |                                                     | RGB       | [Datasheet](https://resources.4dsystems.com.au/datasheets/4dlcd/4DLCD-50800480-IPS/) |
| [gen4-ESP32-70 Series](https://resources.4dsystems.com.au/datasheets/esp32/gen4-esp32-RGB/)   | ✅       |                                                     | RGB       | [Datasheet](https://resources.4dsystems.com.au/datasheets/4dlcd/4DLCD-70800480/) |
| [ESP32-90 Series](https://resources.4dsystems.com.au/datasheets/esp32/esp32-90/)              | ✅       |                                                     | RGB       | [Datasheet](https://resources.4dsystems.com.au/datasheets/4dlcd/4DLCD-90800480/) |

## Add to project

At the time of writing, this package is not yet publish in ESP-IDF Component Registry.

You can install this package by following the instructions for defining a dependency from Git repository found [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-component-manager.html#defining-dependencies-in-the-manifest).

Example:

``` yml
dependencies:
  # Define a dependency from a Git repository
  esp32s3_lcd:
    git: https://github.com/4dsystems/esp32s3_lcd.git
```

Add `version` to indicate which branch or tag to use:

``` yml
dependencies:
  # Define a dependency from a Git repository
  esp32s3_lcd:
    git: https://github.com/4dsystems/esp32s3_lcd.git
    version: v0.3.0 # define the branch/tag
```

## Selecting the Display Series

Run menuconfig and select the target module:

![Display Selection](display-selection.png)

There are also some options such as I2C number to use, SPI host number to use for SPI LCDs etc.

## Initializing the Display

``` c
esp_lcd_panel_handle_t panelHandle = NULL;
esp32s3_lcd_full_init(&panelHandle);
```

## Initializing Touch

``` c
esp_lcd_touch_handle_t tp;

i2c_master_bus_handle_t i2c_bus = NULL;
i2c_master_bus_config_t bus_config = ESP3233_LCD_ONBOARD_I2C_CONFIG();

/* Initialize the I2C bus */
if (i2c_new_master_bus(&bus_config, &i2c_bus) != ESP_OK) {
  return 0;
}
  
esp32s3_lcd_touch_init(i2c_bus, &tp);
```
