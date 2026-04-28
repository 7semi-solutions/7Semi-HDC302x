/**
 * HDC302x Alert Example
 *
 * - Demonstrates alert threshold configuration
 * - Shows how to set trigger and clear limits
 * - Reads status register to detect alert condition
 *
 * Features:
 * - High / Low threshold alerts
 * - Hysteresis (clear thresholds)
 * - Status monitoring
 *
 * Hardware:
 * - Sensor: HDC302x
 * - Interface: I2C
 */

#include <Wire.h>
#include <7Semi_HDC302x.h>


HDC302x_7Semi sensor;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("HDC302x Alert Example");

    // Initialize sensor
    if (!sensor.begin())
    {
        Serial.println("Sensor init failed!");
        while (1);
    }

    Serial.println("Sensor initialized");

    /**
     * Configure ALERT thresholds
     *
     * Trigger conditions:
     * - Temp > 40°C OR Temp < 10°C
     * - Hum  > 80% OR Hum  < 20%
     */
    if (!sensor.setAlert(40.0, 10.0, 80.0, 20.0))
    {
        Serial.println("Failed to set alert!");
        while (1);
    }

    /**
     * Configure CLEAR thresholds (hysteresis)
     *
     * Alert will clear when:
     * - Temp goes below 38°C or above 12°C
     * - Hum goes below 75% or above 25%
     */
    if (!sensor.clearAlert(38.0, 12.0, 75.0, 25.0))
    {
        Serial.println("Failed to set clear alert!");
        while (1);
    }
}

void loop()
{
    float temp, hum;

    // Read current values
    if (sensor.readTemperatureHumidity(temp, hum))
    {
        Serial.print("T: ");
        Serial.print(temp);
        Serial.print(" °C  | H: ");
        Serial.print(hum);
        Serial.println(" %RH");
    }

    // Read status register
    uint16_t status;
    if (sensor.getStatus(status))
    {
        // Serial.print("Status: 0x");
        // Serial.println(status, HEX);

        if (status & HDC302X_ALERT_T_LOW)
        {
            Serial.println("Temperature LOW alert!");
        }

        if (status & HDC302X_ALERT_T_HIGH)
        {
            Serial.println("Temperature HIGH alert!");
        }

        if (status & HDC302X_ALERT_RH_LOW)
        {
            Serial.println("Humidity LOW alert!");
        }

        if (status & HDC302X_ALERT_RH_HIGH)
        {
            Serial.println("Humidity HIGH alert!");
        }

        sensor.clearStatus();
    }
    else
    {
        Serial.println("Status read failed!");
    }

    delay(1000);
}