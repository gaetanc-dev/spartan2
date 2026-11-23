# Spartan2

Arduino / PlatformIO library for the [Spartan 2 OEM I2C](https://www.14point7.com/products/spartan-lambda-controller-2)
wideband O2 sensor controller from 14point7.

The library reads the 6-byte I2C frame and converts it into lambda, AFR, O2
percentage, pump current and sensor temperature using the lookup tables from
the 14point7 manual.

## Wiring

SDA and SCL lines require **4.7 kΩ pull-ups**.

| Board | SDA | SCL |
|---|---|---|
| ESP32 (default) | GPIO21 | GPIO22 |
| Arduino Uno / AVR | A4 | A5 |

To force or reset the board's I2C address, see the last page of the
[Spartan 2 OEM I2C manual](https://cdn.shopify.com/s/files/1/0189/1312/files/Spartan2_OEM_Manual_7b0d1d05-83e1-4863-b842-ee5017be984f.zip?11).

## Install

PlatformIO (`platformio.ini`):

```ini
lib_deps = https://github.com/gaetanc-dev/spartan2.git
```

Arduino IDE: place this folder in `~/Documents/Arduino/libraries/Spartan2`
(or use Library Manager → Add .ZIP Library).

## Usage

```cpp
#include <Arduino.h>
#include <Spartan2.h>

Spartan2 O2(0x01, 3.73, 14.7); // I2C address, K ratio, stoichiometric AFR

void setup() {
  Serial.begin(115200);
  O2.begin();
}

void loop() {
  O2.read();
  if (O2.No_I2C_Response_Flag == 0) {
    Serial.println(O2.Get.Lambda);
    Serial.println(O2.Get.Afr);
    Serial.print("O2 % = ");
    Serial.println(O2.Get.O2 * 100); // O2 is a fraction (0..1)
  }
  delay(1000);
}
```

See [`examples/Spartan2_basic/Spartan2_basic.ino`](examples/Spartan2_basic/Spartan2_basic.ino) for the full example.

## API

| Member | Description |
|---|---|
| `Spartan2(uint8_t addr, float K, float AfrStoich)` | Constructor. `K` is the sensor calibration ratio, `AfrStoich` the stoichiometric AFR (e.g. 14.7 for gasoline). |
| `void begin()` | Initializes the `Wire` I2C bus. |
| `void read()` | Reads the sensor and updates `Get`. Sets `No_I2C_Response_Flag` to 1 if the frame is missing or incomplete. |
| `void writeAddr(uint8_t addr)` | Changes the sensor's I2C address permanently; remember the new value for future use. |
| `uint8_t No_I2C_Response_Flag` | `1` = no valid data, `0` = `Get` holds a fresh measurement. |
| `Processed_Data_Struct Get` | See below. |

`Get` fields: `I2C_Addr`, `ID` (firmware), `Lambda`, `Temperature_C`, `Status`,
`Ma` (pump current in mA), `Afr`, `O2` (fraction 0..1, multiply by 100 for %).

Debug output can be enabled by building with `-DSpartan2_DEBUG=1`.

## License

MIT — see [LICENSE](LICENSE).
