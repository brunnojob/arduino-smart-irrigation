# arduino-smart-irrigation

ESP32 irrigation controller with dry/wet thresholds, five-minute start lockout, 12-second pump ceiling, manual start button and boot-safe relay output. Calibrate thresholds to the installed sensor; drive a relay module, not the pump directly.

Requires PlatformIO Core. Run `pio run` to build and `pio run -t upload` to flash the ESP32 DevKit. Sensor: GPIO34; relay: GPIO26; button: GPIO27.

Project by [Brunno Dev](https://brunnodev.store).