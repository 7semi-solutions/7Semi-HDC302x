#ifndef _7SEMI_HDC302X_H_ 
#define _7SEMI_HDC302X_H_

#include <Arduino.h>
#include <Wire.h>

// Default I2C address of HDC302X
#define HDC302X_DEFAULT_ADDR 0x44

// HDC302X Register Map
#define HDC302X_REG_TRIG_LPM0        0x2400  
#define HDC302X_REG_TRIG_LPM1        0x240B
#define HDC302X_REG_TRIG_LPM2        0x2416
#define HDC302X_REG_TRIG_LPM3        0x24FF  

#define HDC302X_REG_AUTO_0P5HZ_LPM0  0x2032
#define HDC302X_REG_AUTO_0P5HZ_LPM1  0x2024
#define HDC302X_REG_AUTO_0P5HZ_LPM2  0x202F
#define HDC302X_REG_AUTO_0P5HZ_LPM3  0x20FF

#define HDC302X_REG_AUTO_1HZ_LPM0    0x2130
#define HDC302X_REG_AUTO_1HZ_LPM1    0x2126
#define HDC302X_REG_AUTO_1HZ_LPM2    0x212D
#define HDC302X_REG_AUTO_1HZ_LPM3    0x21FF

#define HDC302X_REG_AUTO_2HZ_LPM0    0x2236
#define HDC302X_REG_AUTO_2HZ_LPM1    0x2220
#define HDC302X_REG_AUTO_2HZ_LPM2    0x222B
#define HDC302X_REG_AUTO_2HZ_LPM3    0x22FF

#define HDC302X_REG_AUTO_4HZ_LPM0    0x2334
#define HDC302X_REG_AUTO_4HZ_LPM1    0x2322
#define HDC302X_REG_AUTO_4HZ_LPM2    0x2329
#define HDC302X_REG_AUTO_4HZ_LPM3    0x23FF

#define HDC302X_REG_AUTO_10HZ_LPM0   0x2737
#define HDC302X_REG_AUTO_10HZ_LPM1   0x2721
#define HDC302X_REG_AUTO_10HZ_LPM2   0x272A
#define HDC302X_REG_AUTO_10HZ_LPM3   0x27FF

#define HDC302X_REG_LOW_POWER_MODE_0 0x2C06
#define HDC302X_REG_LOW_POWER_MODE_1 0x2C0D
#define HDC302X_REG_LOW_POWER_MODE_2 0x2C10

#define HDC302X_REG_EXIT_AUTO_MODE   0x3093
#define HDC302X_REG_READ_T_RH        0xE000
#define HDC302X_REG_READ_RH_ONLY     0xE001
#define HDC302X_REG_READ_MIN_T       0xE002
#define HDC302X_REG_READ_MAX_T       0xE003
#define HDC302X_REG_READ_MIN_RH      0xE004
#define HDC302X_REG_READ_MAX_RH      0xE005

#define HDC302X_REG_SET_LOW_ALERT    0x6100
#define HDC302X_REG_SET_HIGH_ALERT   0x611D
#define HDC302X_REG_CLR_LOW_ALERT    0x610B
#define HDC302X_REG_CLR_HIGH_ALERT   0x6116

#define HDC302X_REG_TEMP_HUM_OFFSET  0xA004   

#define HDC302X_REG_READ_LOW_ALERT   0xE102
#define HDC302X_REG_READ_HIGH_ALERT  0xE11F
#define HDC302X_REG_READ_CLR_LOW     0xE109
#define HDC302X_REG_READ_CLR_HIGH    0xE114

#define HDC302X_REG_HEATER_ENABLE    0x306D
#define HDC302X_REG_HEATER_DISABLE   0x3066
#define HDC302X_REG_HEATER_CONFIG    0x306E

#define HDC302X_REG_READ_STATUS      0xF32D
#define HDC302X_REG_CLEAR_STATUS     0x3041
#define HDC302X_REG_SOFT_RESET       0x30A2

#define HDC302X_REG_READ_SN_54       0x3683
#define HDC302X_REG_READ_SN_32       0x3684
#define HDC302X_REG_READ_SN_10       0x3685
#define HDC302X_REG_MANUFACTURER_ID  0x3781

#define HDC302X_EXPECTED_ID          0x3000

#define HDC302X_ALERT_T_LOW          0x0040
#define HDC302X_ALERT_T_HIGH         0x0080
#define HDC302X_ALERT_RH_LOW         0x0100
#define HDC302X_ALERT_RH_HIGH        0x0200

/**
 * Power Mode (LPM)
 *
 * - Defines measurement performance vs power consumption
 *
 * Modes:
 * - LPM0 → highest performance, fastest response, higher power
 * - LPM1 → balanced mode
 * - LPM2 → reduced power, moderate speed
 * - LPM3 → lowest power, slowest response
 *
 * Notes:
 * - Affects measurement accuracy and conversion time
 * - Used together with ODR to configure sensor behavior
 */
enum HDC302x_PowerMode
{
    LPM0     = 0,
    LPM1     = 1,
    LPM2     = 2,
    LPM3     = 3,
    LPM_MAX  = 4
};

/**
 * Output Data Rate (ODR)
 *
 * - Defines how frequently the sensor updates measurements
 *
 * Rates:
 * - ODR_0_5HZ → 0.5 measurements per second
 * - ODR_1HZ   → 1 measurement per second
 * - ODR_2HZ   → 2 measurements per second
 * - ODR_4HZ   → 4 measurements per second
 * - ODR_10HZ  → 10 measurements per second
 *
 * Notes:
 * - Higher ODR → faster updates, higher power usage
 * - Lower ODR  → lower power, slower updates
 * - Used with PowerMode for full configuration
 */
enum HDC302x_ODR
{
    ODR_0_5HZ    = 0,
    ODR_1HZ      = 1,
    ODR_2HZ      = 2,
    ODR_4HZ      = 3,
    ODR_10HZ     = 4,
    ODR_MAX      = 5
};

/**
 * Measurement Mode
 *
 * - Controls how sensor measurements are performed and accessed
 *
 * Modes:
 * - TRIGGER            → single-shot measurement (manual trigger)
 * - CONTINUOUS_RH_ONLY → continuous humidity read mode
 * - CONTINUOUS_MIN_T   → read minimum recorded temperature
 * - CONTINUOUS_MAX_T   → read maximum recorded temperature
 * - CONTINUOUS_MIN_RH  → read minimum recorded humidity
 * - CONTINUOUS_MAX_RH  → read maximum recorded humidity
 *
 * Notes:
 * - Continuous modes depend on internal sensor buffering
 * - TRIGGER mode exits automatic measurement mode
 */
enum class HDC302X_MeasurementMode : uint8_t
{
    TRIGGER            = 0,
    CONTINUOUS_RH_ONLY = 1,
    CONTINUOUS_MIN_T   = 2,
    CONTINUOUS_MAX_T   = 3,
    CONTINUOUS_MIN_RH  = 4,
    CONTINUOUS_MAX_RH  = 5
};

class HDC302x_7Semi
{
public:

    HDC302x_7Semi();

    /**
     * begin
     *
     * - Initializes I2C communication with the HDC302x sensor
     * - Assigns user-provided I2C port and address
     * - Configures I2C clock speed
     * - Verifies device presence using Manufacturer ID
     * - Prints detected ID for debugging
     *
     * Parameters:
     * - i2cAddress → I2C address of the sensor (default usually 0x44)
     * - i2cPort    → Reference to TwoWire interface (Wire, Wire1, etc.)
     * - i2cClock   → I2C clock frequency (e.g., 100000, 400000)
     *
     * Returns:
     * - true  → Sensor initialized and verified successfully
     * - false → Communication failed or wrong device detected
     */
    bool begin(uint8_t address = HDC302X_DEFAULT_ADDR,
               TwoWire &port = Wire,
               uint32_t clock = 400000);

    /**
     * getManufacturerID
     *
     * - Reads the manufacturer ID register from the sensor
     * - Used to verify correct device connection
     *
     * Parameters:
     * - id → reference variable to store manufacturer ID
     *
     * Returns:
     * - true  → Read successful, ID updated
     * - false → I2C read failed or CRC error
     */
    bool getManufacturerID(uint16_t &id);

    /**
     * softReset
     *
     * - Performs a soft reset of the sensor
     * - Restores default configuration and clears internal state
     *
     * Returns:
     * - true  → Reset command sent successfully
     * - false → I2C communication failed
     */
    bool softReset();

    /**
     * getStatus
     *
     * - Reads the sensor status register
     * - Provides information about sensor state, alert flag and errors
     * - Status bits indicate heater state, alert flags, and errors
     *
     * Parameters:
     * - status → reference variable to store status register value
     *
     * Returns:
     * - true  → Read successful
     * - false → I2C read failed or CRC error
     */
    bool getStatus(uint16_t &status);

    /**
     * clearStatus
     *
     * - Clears status register flags inside the sensor
     * - Used to reset alert or error conditions
     * - Useful after handling alerts or faults
     *
     * Returns:
     * - true  → Command executed successfully
     * - false → I2C communication failed
     */
    bool clearStatus();

    /**
     * setHeater
     *
     * - Controls internal heater of the sensor
     * - Used to reduce condensation and improve reliability in humid environments
     * - Heater increases power consumption significantly
     * - When enabled:
     *   - Temperature reading may rise slightly (self-heating effect)
     *   - Humidity reading may drop temporarily
     * - Recommended to enable only when needed (e.g., high humidity / fog conditions)
     *
     * Usage Tips:
     * - Turn ON heater for short duration → stabilize sensor
     * - Turn OFF heater before taking measurements
     *
     * Parameters:
     * - enable →
     *   - true  → Heater ON
     *   - false → Heater OFF
     *
     * Returns:
     * - true  → Command sent successfully
     * - false → I2C communication failed
     */
    bool setHeater(bool enable);

    /**
     * setHeaterPowerRaw
     *
     * - Sets heater power using raw register value
     *
     * Notes:
     * - Raw range: 0 → 0x3FFF (14-bit resolution)
     * - 0x0000 → Heater OFF (minimum)
     * - 0x3FFF → Maximum heater power
     *
     * Warning:
     * - High values can cause:
     *   - Strong temperature offset
     *   - Reduced humidity accuracy
     *   - Increased current consumption
     *
     * Parameters:
     * - heaterPowerRaw → raw heater value (0 to 0x3FFF)
     *
     * Returns:
     * - true  → Command sent successfully
     * - false → I2C communication failed
     */
    bool setHeaterPowerRaw(uint16_t heaterPowerRaw);

    /**
     * setHeaterPower
     *
     * - Sets heater power using percentage
     *
     * Notes:
     * - Input range: 0% → 100%
     * - 0%  → Heater OFF
     * - 100% → Maximum heater power
     *
     * Recommended Ranges:
     * - 5%  – 20%  → Light stabilization
     * - 20% – 50%  → Moderate drying
     * - 50% – 100% → Heavy condensation removal
     *
     * Parameters:
     * - percent → heater power level (0 to 100)
     *
     * Returns:
     * - true  → Command sent successfully
     * - false → I2C communication failed
     */
    bool setHeaterPower(float percent);

    /**
     * getHeaterPowerRaw
     *
     * - Reads current heater power from sensor (raw register value)
     *
     * Notes:
     * - Raw range: 0 → 0x3FFF (14-bit resolution)
     * - 0x0000 → Heater OFF
     * - 0x3FFF → Maximum heater power
     *
     * Parameters:
     * - heaterPowerRaw → reference variable to store raw heater value
     *
     * Returns:
     * - true  → Read successful
     * - false → I2C read failed or CRC error
     */
    bool getHeaterPowerRaw(uint16_t &heaterPowerRaw);

    /**
     * getHeaterPower
     *
     * - Reads heater power as percentage
     * - Output range: 0% → 100%
     * - 0%   → Heater OFF
     * - 100% → Maximum heater power
     * - For precise control, use getHeaterPowerRaw()
     *
     * Parameters:
     * - percent → reference variable to store heater power (0 to 100)
     *
     * Returns:
     * - true  → Read successful
     * - false → I2C read failed
     */
    bool getHeaterPower(float &percent);

    /**
     * setAlert
     *
     * - Configures temperature and humidity alert thresholds
     * - Sets both LOW and HIGH alert limits inside the sensor
     * - Sensor compares live readings with these thresholds
     * - Alerts can be used for interrupt or polling-based monitoring
     *
     * Behavior:
     * - Low alert → triggered when value goes BELOW threshold
     * - High alert → triggered when value goes ABOVE threshold
     *
     * Parameters:
     * - tempHigh → high temperature threshold (°C)
     * - tempLow  → low temperature threshold (°C)
     * - humHigh  → high humidity threshold (%RH)
     * - humLow   → low humidity threshold (%RH)
     *
     * Returns:
     * - true  → Alert thresholds set successfully
     * - false → I2C communication failed
     */
    bool setAlert(
        float tempHigh,
        float tempLow,
        float humHigh,
        float humLow);

    /**
     * clearAlert
     *
     * - Configures clear (hysteresis) thresholds for alerts
     * - Defines when an active alert condition should be cleared
     * - Prevents rapid toggling of alerts
     * - Should be set slightly inside trigger thresholds
     *
     * Example:
     * - High alert at 80% → clear at 75%
     * - Low alert at 20%  → clear at 25%
     *
     * Parameters:
     * - tempHigh → high temperature clear threshold
     * - tempLow  → low temperature clear threshold
     * - humHigh  → high humidity clear threshold
     * - humLow   → low humidity clear threshold
     *
     * Returns:
     * - true  → Clear thresholds configured successfully
     * - false → I2C communication failed
     */
    bool clearAlert(
        float tempHigh,
        float tempLow,
        float humHigh,
        float humLow);

    /**
     * getSetAlert
     *
     * - Reads configured alert trigger thresholds from sensor
     * - Returns the actual values currently stored in sensor registers
     * - Values are decoded from packed raw format
     *
     * Parameters:
     * - tempLow  → output low temperature threshold
     * - humLow   → output low humidity threshold
     * - tempHigh → output high temperature threshold
     * - humHigh  → output high humidity threshold
     *
     * Returns:
     * - true  → Read successful
     * - false → I2C read failed
     */
    bool getSetAlert(
        float &tempLow,
        float &humLow,
        float &tempHigh,
        float &humHigh);

    /**
     * getClearAlert
     *
     * - Reads configured alert clear (hysteresis) thresholds
     * - Used to understand when alerts will reset
     * - Helps debug alert behavior in application
     *
     * Parameters:
     * - tempLow  → output low temperature clear threshold
     * - humLow   → output low humidity clear threshold
     * - tempHigh → output high temperature clear threshold
     * - humHigh  → output high humidity clear threshold
     *
     * Returns:
     * - true  → Read successful
     * - false → I2C read failed
     */
    bool getClearAlert(
        float &tempLow,
        float &humLow,
        float &tempHigh,
        float &humHigh);

    /**
     * setOffset
     *
     * - Applies calibration offset for temperature and humidity
     * - Corrects sensor readings to match reference measurements
     * - Offset values are limited by sensor register resolution
     * - Values outside allowed range are rejected
     *
     * Limits:
     * - Temperature offset range → ±10.9375 °C
     * - Humidity offset range   → ±12.5 %RH
     *
     * Conversion:
     * - Temperature resolution ≈ 0.1709 °C per lsb
     * - Humidity resolution   ≈ 0.1953 %RH per lsb
     *
     * Parameters:
     * - tempOffset → temperature offset in °C
     * - humiOffset → humidity offset in %RH
     *
     * Returns:
     * - true  → Offset written successfully
     * - false → Invalid input or I2C communication failed
     */
    bool setOffset(float tempOffset, float humiOffset);

    /**
     * getOffset
     *
     * - Reads temperature and humidity calibration offsets
     *
     * Parameters:
     * - tempOffset → output temperature offset (°C)
     * - humiOffset → output humidity offset (%RH)
     *
     * Returns:
     * - true  → Read and decode successful
     * - false → I2C communication failed
     */
    bool getOffset(float &tempOffset, float &humiOffset);

    /**
     * setMeasurementMode
     *
     * - Configures sensor measurement operating mode
     * - Switches between trigger mode and various continuous read modes
     *
     * Modes:
     * - TRIGGER            → Single-shot measurement (manual trigger)
     * - CONTINUOUS_RH_ONLY → Continuous humidity read mode
     * - CONTINUOUS_MIN_T   → Read minimum recorded temperature
     * - CONTINUOUS_MAX_T   → Read maximum recorded temperature
     * - CONTINUOUS_MIN_RH  → Read minimum recorded humidity
     * - CONTINUOUS_MAX_RH  → Read maximum recorded humidity
     *
     * Notes:
     * - TRIGGER mode exits automatic measurement mode
     * - Continuous modes depend on sensor internal buffering
     * - Command selection is based on datasheet-defined registers
     *
     * Parameters:
     * - mode → measurement mode selection (HDC302X_MeasurementMode)
     *
     * Returns:
     * - true  → Command sent successfully
     * - false → Invalid mode or I2C communication failed
     */
    bool setMeasurementMode(HDC302X_MeasurementMode mode);

    /**
     * readContinuousRaw
     *
     * - Reads raw 16-bit data from sensor in continuous mode
     * - Performs CRC validation to ensure data integrity
     *
     * Parameters:
     * - rawData → output raw 16-bit sensor value
     *
     * Returns:
     * - true  → Read successful and CRC valid
     * - false → I2C read failed or CRC mismatch
     */
    bool readContinuousRaw(uint16_t &rawData);

    /**
     * startContinuousHumidity
     *
     * - Starts continuous humidity measurement mode
     *
     * Notes:
     * - Sensor will continuously update humidity data
     * - Use readHumidity() to fetch values
     *
     * Returns:
     * - true  → Command sent successfully
     * - false → I2C communication failed
     */
    bool startContinuousHumidity();

    /**
     * readContinousHumidity
     *
     * - Starts continuous humidity measurement mode
     *
     * Notes:
     * - Sensor will continuously update humidity data
     * - Use readHumidity() to fetch values
     *
     * Returns:
     * - true  → Command sent successfully
     * - false → I2C communication failed
     */
    bool readContinuousHumidity(float &humidity);

    /**
     * readMinHumidity
     *
     * - Reads minimum recorded humidity value
     * - Sensor stores min values internally
     * - Returns lowest humidity recorded since last reset
     * - Value Resets on power cycle or soft reset
     *
     * Parameters:
     * - humidity → output minimum humidity (%RH)
     *
     * Returns:
     * - true  → Read successful
     * - false → Command or read failed
     */
    bool readMinHumidity(float &humidity);

    /**
     * readMaxHumidity
     *
     * - Reads maximum recorded humidity value
     * - Returns highest humidity recorded since last reset
     * - Value Resets on power cycle or soft reset
     *
     * Parameters:
     * - humidity → output maximum humidity (%RH)
     *
     * Returns:
     * - true  → Read successful
     * - false → Command or read failed
     */
    bool readMaxHumidity(float &humidity);

    /**
     * readMinTemperature
     *
     * - Reads maximum recorded temperature value
     * - Returns highest temperature recorded since last reset
     * - Value Resets on power cycle or soft reset
     *
     * Parameters:
     * - temperature → output maximum temperature (°C)
     *
     * Returns:
     * - true  → Read successful
     * - false → Command or read failed
     */
    bool readMinTemperature(float &temperature);

    /**
     * readMaxTemperature
     *
     * - Reads maximum recorded temperature value
     * - Returns highest temperature recorded since last reset
     * - Value Resets on power cycle or soft reset
     *
     * Parameters:
     * - temperature → output maximum temperature (°C)
     *
     * Returns:
     * - true  → Read successful
     * - false → Command or read failed
     */
    bool readMaxTemperature(float &temperature);

    /**
     * readTemperatureHumidity
     *
     * - Performs single-shot measurement of temperature and humidity
     * - Supports multiple power modes
     *
     * Modes:
     * - LPM0 → highest accuracy, fastest
     * - LPM3 → lowest power, slowest
     *
     * Notes:
     * - Command triggers measurement
     * - Delay required before reading data
     * - Data includes CRC for both values
     *
     * Parameters:
     * - temperature → output temperature (°C)
     * - humidity    → output humidity (%RH)
     * - mode        → power mode selection
     *
     * Returns:
     * - true  → Measurement successful
     * - false → I2C or CRC failure
     */
    bool readTemperatureHumidity(float &temperature,
                                 float &humidity,
                                 HDC302x_PowerMode mode = LPM3);

    /**
     * setMeasurementRate
     *
     * - Configures sensor measurement rate and power mode
     * - Controls how frequently new data is generated
     *
     * Parameters:
     * - odr  → output data rate (0.5Hz to 10Hz)
     * - mode → power mode (LPM0 to LPM3)
     *
     * Notes:
     * - Higher ODR → faster updates, higher power consumption
     * - Lower ODR → lower power, slower response
     *
     * Power Modes:
     * - LPM0 → highest performance
     * - LPM3 → lowest power consumption
     *
     * Returns:
     * - true  → Configuration applied successfully
     * - false → Invalid input or I2C communication failed
     */
    bool HDC302x_7Semi::setMeasurementRate(HDC302x_ODR odr, HDC302x_PowerMode mode);

private:

    TwoWire *i2c;
    uint8_t i2c_address = 0x44;

    uint8_t power_mode = 0;
    uint8_t data_rate = 0;

    bool readReg(uint16_t cmd, uint16_t &value);

    bool readReg(uint16_t cmd, uint8_t *data, uint32_t len);

    bool readOnly(uint8_t *data, uint32_t len);

    bool writeCommand(uint16_t cmd, uint16_t data);

    bool writeCommand(uint16_t cmd);

    bool writeOnly(uint8_t *data, uint32_t len);

    uint8_t getCRC(const uint8_t *data, int len);

    bool readContinousRaw(uint16_t &rawData);

    bool readTemperature(float &temperature);

    bool readHumidity(float &humidity);

    float RawToHumidity(uint16_t h_raw);

    float RawToTemperature(uint16_t t_raw);

    uint16_t encodeAlert(float temp, float hum);

    void decodeAlert(uint16_t raw, float &temp, float &hum);

    const uint16_t measurement_command[LPM_MAX][ODR_MAX] = {
        // 0.5Hz,  1Hz,    2Hz,    4Hz,    10Hz
        {0x2032, 0x2130, 0x2236, 0x2334, 0x2737}, // LPM0
        {0x2024, 0x2126, 0x2220, 0x2322, 0x2721}, // LPM1
        {0x202F, 0x212D, 0x222B, 0x2329, 0x272A}, // LPM2
        {0x20FF, 0x21FF, 0x22FF, 0x23FF, 0x27FF}  // LPM3
    };
};

#endif