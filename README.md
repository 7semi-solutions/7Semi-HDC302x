# 7Semi HDC302x Arduino Library

Arduino driver for the **Texas Instruments HDC302x** high-accuracy digital temperature and humidity sensor.

The HDC302x provides precise environmental sensing with high accuracy, low power consumption, and advanced features such as configurable measurement modes, heater control, and alert thresholds.

---

## Features

- High-accuracy temperature and humidity measurement  
  - Temperature: ±0.2°C (typical)  
  - Humidity: ±2% RH (typical)

- Single-shot and continuous measurement modes  

- Configurable Output Data Rate (ODR)  
  - 0.5 Hz to 10 Hz  

- Low Power Modes (LPM0 to LPM3)

- Heater control (for condensation handling)

- Alert thresholds with hysteresis  
  - High / Low limits  
  - Clear thresholds  

- Offset calibration (temperature & humidity)

- CRC data validation

- Status register monitoring

---

## Connections / Wiring

The HDC302x uses **I²C communication**.

---

## I²C Connection

| HDC302x Pin | MCU Pin         | Notes                |
|------------|-----------------|----------------------|
| VCC        | 3.3V / 5V*      | Check module specs   |
| GND        | GND             | Common ground        |
| SDA        | SDA             | I²C data             |
| SCL        | SCL             | I²C clock            |
| ALERT      | GPIO (optional) | Alert interrupt pin  |

---

## I²C Notes

- Default I²C address: `0x44`  
- Requires pull-up resistors on SDA/SCL (4.7kΩ–10kΩ)  
- Supported bus speeds:
  - 100 kHz  
  - 400 kHz (recommended)  

---

## Installation

### Arduino Library Manager

1. Open Arduino IDE  
2. Go to Library Manager  
3. Search for **7Semi HDC302x**  
4. Click Install  

---

### Manual Installation

1. Download repository as ZIP  
2. Arduino IDE → Sketch → Include Library → Add .ZIP Library  

---

## Library Overview

---

## Reading Temperature & Humidity

```cpp
float temperature, humidity;

sensor.readTemperatureHumidity(temperature, humidity);
```

- Returns
  - temperature in °C
  - Humidity in %RH

## Setting Measurement Rate

```cpp
sensor.setMeasurementRate(ODR_1HZ, LPM1);
```

- ODR: 0.5Hz → 10Hz
- LPM: Power mode (performance vs power)

## Continuous Mode

```cpp
sensor.setMeasurementMode(HDC302X_MeasurementMode::CONTINUOUS_RH_ONLY);
sensor.startContinuousHumidity();
```

## Heater Control

```cpp
sensor.setHeater(true);
sensor.setHeaterPower(20); // %
```

## Offset Calibration

```cpp
sensor.setOffset(0.5, -2.0);
```

- Temperature offset (°C)
- Humidity offset (%RH)

## Alert Configuration

```cpp
sensor.setAlert(40.0, 10.0, 80.0, 20.0);
sensor.clearAlert(38.0, 12.0, 75.0, 25.0);
```

- High / Low thresholds
- Clear thresholds (hysteresis)

## Reading Status

```cpp
uint16_t status;

sensor.getStatus(status);
```

- Used for alert detection and diagnostics
