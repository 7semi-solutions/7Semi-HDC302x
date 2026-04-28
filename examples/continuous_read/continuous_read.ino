/**
 * HDC302x Continuous Humidity Example
 *
 * - Demonstrates continuous humidity measurement mode
 * - Sensor continuously updates humidity internally
 *
 * Features:
 * - Continuous read (no trigger required)
 * - Configurable data rate and power mode
 * - Low overhead for repeated readings
 *
 * Hardware:
 * - Sensor: HDC302x
 * - Interface: I2C
 */

#include <7Semi_HDC302x.h>

HDC302x_7Semi sensor;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("HDC302x Continuous Humidity Example");

    /**
     * Initialize sensor
     *
     * Default:
     * - Address → 0x44
     * - I2C     → Wire
     * - Clock   → 400kHz
     */
    if (!sensor.begin())
    {
        Serial.println("Sensor init failed!");
        while (1);
    }

    Serial.println("Sensor initialized");

    /**
     * Configure measurement mode
     *
     * - Enables continuous humidity measurement
     */
    if (!sensor.setMeasurementMode(HDC302X_MeasurementMode::CONTINUOUS_RH_ONLY))
    {
        Serial.println("Failed to set measurement mode!");
        while (1);
    }

    /**
     * Configure output data rate and power mode
     *
     * - ODR_10HZ → fast updates (10 samples/sec)
     * - LPM3     → lowest power mode
     */
    if (!sensor.setMeasurementRate(ODR_10HZ, LPM3))
    {
        Serial.println("Failed to set measurement rate!");
        while (1);
    }

    /**
     * Start continuous measurement
     *
     * Notes:
     * - Required for continuous read functions
     */
    if (!sensor.startContinuousHumidity())
    {
        Serial.println("Failed to start continuous mode!");
        while (1);
    }
}

void loop()
{
    float hum;

    if (sensor.readContinuousHumidity(hum))
    {
        Serial.print("Humidity: ");
        Serial.print(hum);
        Serial.println(" %RH");
    }
    else
    {
        Serial.println("Read failed!");
    }

    delay(200);
}