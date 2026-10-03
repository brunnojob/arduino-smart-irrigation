# arduino-smart-irrigation

ESP32 irrigation controller with separate dry and wet thresholds, five-minute start lockout, 12-second pump ceiling, manual start button and boot-safe relay output. Validate thresholds for the installed capacitive sensor and drive a relay module, not the pump directly.

Select an ESP32 board in Arduino IDE, set the sensor to GPIO34, relay to GPIO26 and button to GPIO27, then upload `src/main.cpp`.

Project by [Brunno Dev](https://brunnodev.store).