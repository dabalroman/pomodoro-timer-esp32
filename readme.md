# Pomodoro Timer made with Esp32 and OLED display

The project uses Platform.io with Arduino framework as build environment. Make sure to use latest version. 
Remember to select the correct COM port in `platformio.ini` to before trying to upload the project to the MCU.

# Features
- Timer - set a time and watch it tick away
- Presets - select a preset from multiple options
- Visual alarm - LEDs will flash once timer hits 00:00

# Hardware
- [Wemos S2 mini](https://www.wemos.cc/en/latest/s2/s2_mini.html)
- SSD1106 128x64 0.96" display
- 2x LED ARGB WS2812B

Kicad project is available in `esp-pomodoro-clock` directory.

![](assets\scheme.png)


# Software
- Custom software based on Platform.io using Arduino framework
