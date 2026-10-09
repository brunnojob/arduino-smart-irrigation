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

## Result synchronization

The [operations archive](https://vercel-home-telemetry-api.vercel.app/laboratory.html?project=arduino-smart-irrigation) stores execution results. Supabase migrations are in the [API repository](https://github.com/brunnojob/vercel-home-telemetry-api/tree/main/supabase/migrations).

```sh
python cloud/sync.py enqueue result.json --project arduino-smart-irrigation
python cloud/sync.py sync
```

Set `BRUNNODEV_ACCESS_TOKEN` to your session token. The SQLite outbox retains reports until the server confirms persistence; identical content does not create duplicate records. Tokens are not stored in source code. To run the synchronization tests:

```sh
python -m unittest discover -s cloud
```
