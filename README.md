# Smart Irrigation

Controle de irrigação com filtro, histerese, intervalo mínimo, tempo máximo de bomba, sensor de reservatório e bloqueio por falha.

## Executar

Requisitos: ESP32, C++17 e PlatformIO.

```sh
pio run -e esp32dev
pio run -e esp32dev -t upload
python -m pip install -r cloud/requirements.txt
python cloud/serial_bridge.py /dev/ttyUSB0
```

## Funcionamento

ADC: GPIO 34. Bomba: GPIO 26. Botão: GPIO 27. Reservatório: GPIO 25. A bomba inicia desligada; partida manual respeita bloqueios. A lógica pura está em `include/irrigation.hpp`, testada sem dispositivo. Calibre o sensor e valide relé e nível ativo antes da ligação.

## Persistência de resultados

O arquivo de operações está em [vercel-home-telemetry-api.vercel.app](https://vercel-home-telemetry-api.vercel.app/laboratory.html?project=arduino-smart-irrigation). As migrações Supabase estão no [repositório da API](https://github.com/brunnojob/vercel-home-telemetry-api/tree/main/supabase/migrations).

```sh
python cloud/sync.py enqueue resultado.json --project arduino-smart-irrigation
python cloud/sync.py sync
```

Defina `BRUNNODEV_ACCESS_TOKEN` com sua sessão. A fila SQLite conserva os relatórios até confirmação do servidor; o mesmo conteúdo não gera registros duplicados. Tokens não são gravados no código.
