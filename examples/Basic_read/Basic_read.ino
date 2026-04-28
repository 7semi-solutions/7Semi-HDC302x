/**
 * HDC302x Basic Example
 *
 * - Demonstrates initialization and reading temperature + humidity
 * - Uses single-shot measurement mode
 *
 * Features:
 * - I2C communication
 * - Configurable measurement rate
 * - Optional heater and offset calibration
 *
 * Hardware:
 * HDC302x → Arduino
 * VCC  → 3.3V / 5V 
 * GND  → GND
 * SDA  → SDA (e.g., A4 on UNO)
 * SCL  → SCL (e.g., A5 on UNO)
 */

#include <7Semi_HDC302x.h>

HDC302x_7Semi sensor;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("HDC302x Example");

    /**
     * Initialize sensor
     *
     * Parameters:
     * - 0x44     → I2C address
     * - Wire     → I2C interface
     * - 400000   → I2C clock (400kHz)
     */
    if (!sensor.begin(0x44, Wire, 400000))
    {
        Serial.println("Sensor init failed!");
        while (1);
    }

    Serial.println("Sensor initialized");

    /**
     * Configure measurement rate
     *
     * - ODR_1HZ → 1 reading per second
     * - LPM1    → balanced power/performance mode
     */
    sensor.setMeasurementRate(ODR_1HZ, LPM1);

    /**
     * Optional: Heater configuration
     *
     * Notes:
     * - Helps remove condensation
     * - Affects measurement accuracy while active
     */
    // sensor.setHeater(true);
    // sensor.setHeaterPower(20); // 20%

    /**
     * Optional: Calibration offset
     *
     * - tempOffset → °C correction
     * - humOffset  → %RH correction
     */
    // sensor.setOffset(0.5, -2.0);
}

void loop()
{
    float temperature, humidity;

    // Read temperature and humidity
    if (sensor.readTemperatureHumidity(temperature, humidity))
    {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C  |  ");

        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %RH");
    }
    else
    {
        Serial.println("Read failed!");
    }

    delay(1000);
}