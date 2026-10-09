# Smart Irrigation

Irrigation control with filtering, hysteresis, a minimum interval, a maximum pump runtime, a reservoir sensor, and fault lockout.

## Run

Requirements: ESP32, C++17, and PlatformIO.

```sh
pio run -e esp32dev
pio run -e esp32dev -t upload
python -m pip install -r cloud/requirements.txt
python cloud/serial_bridge.py /dev/ttyUSB0
```

## Behavior

ADC: GPIO 34. Pump: GPIO 26. Button: GPIO 27. Reservoir: GPIO 25. The pump starts off; manual starts respect lockouts. Pure logic is in `include/irrigation.hpp` and is tested without hardware. Calibrate the sensor and validate the relay and active signal level before wiring.

## Optional report archive

Export a JSON report from the command above, then run `python cloud/sync.py enqueue result.json --project arduino-smart-irrigation` and `python cloud/sync.py sync`. Synchronization requires `BRUNNODEV_ACCESS_TOKEN` and the external operations API; the local outbox retains unacknowledged reports.

## License

Original source and documentation are MIT licensed; see [LICENSE](LICENSE). Third-party dependencies and media retain their respective terms. Maintained by [Brunno Dev](https://brunnodev.store).
