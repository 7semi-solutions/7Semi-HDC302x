#include "7Semi_HDC302x.h"

HDC302x_7Semi::HDC302x_7Semi()
{
}

bool HDC302x_7Semi::begin(uint8_t i2cAddress,
                          TwoWire &i2cPort,
                          uint32_t i2cClock)
{
    i2c = &i2cPort;
    i2c_address = i2cAddress;

    i2c->begin();
    i2c->setClock(i2cClock);

    delay(10);

    uint16_t id;
    if (!getManufacturerID(id))
        return false;

    if (id != HDC302X_EXPECTED_ID)
        return false;

    if(!softReset())
        return false;

    delay(100);

    return true;
}

bool HDC302x_7Semi::getManufacturerID(uint16_t &id)
{
    return readReg(HDC302X_REG_MANUFACTURER_ID, id);
}

bool HDC302x_7Semi::softReset()
{
    return writeCommand(HDC302X_REG_SOFT_RESET);
}

bool HDC302x_7Semi::getStatus(uint16_t &status)
{
    return readReg(HDC302X_REG_READ_STATUS, status);
}

bool HDC302x_7Semi::clearStatus()
{
    return writeCommand(HDC302X_REG_CLEAR_STATUS);
}

bool HDC302x_7Semi::setHeater(bool enable)
{
    uint16_t command = enable ? 
        HDC302X_REG_HEATER_ENABLE : 
        HDC302X_REG_HEATER_DISABLE;

    return writeCommand(command);
}

bool HDC302x_7Semi::setHeaterPowerRaw(uint16_t heaterPowerRaw)
{
    return writeCommand(HDC302X_REG_HEATER_CONFIG, heaterPowerRaw);
}

bool HDC302x_7Semi::setHeaterPower(float percent)
{
    if (percent > 100)
        percent = 100;

    uint16_t raw = (percent * 0x3FFF) / 100;

    return setHeaterPowerRaw(raw);
}

bool HDC302x_7Semi::getHeaterPowerRaw(uint16_t &heaterPowerRaw)
{
    return readReg(HDC302X_REG_HEATER_CONFIG, heaterPowerRaw);
}

bool HDC302x_7Semi::getHeaterPower(float &percent)
{
    uint16_t raw;

    if (!getHeaterPowerRaw(raw))
        return false;

    percent = (raw * 100) / 0x3FFF;

    return true;
}

bool HDC302x_7Semi::setAlert(
    float tempHigh,
    float tempLow,
    float humHigh,
    float humLow)
{
    uint16_t temp_high = encodeAlert(tempHigh, humHigh);
    uint16_t temp_low  = encodeAlert(tempLow, humLow);

    // Set Low Alert
    if (!writeCommand(HDC302X_REG_SET_LOW_ALERT, temp_low))
        return false;

    // Set High Alert
    if (!writeCommand(HDC302X_REG_SET_HIGH_ALERT, temp_high))
        return false;

    return true;
}

bool HDC302x_7Semi::clearAlert(
    float tempHigh,
    float tempLow,
    float humHigh,
    float humLow)
{
    uint16_t clear_low  = encodeAlert(tempLow, humLow);
    uint16_t clear_high = encodeAlert(tempHigh, humHigh);

    // CLEAR LOW ALERT
    if (!writeCommand(HDC302X_REG_CLR_LOW_ALERT, clear_low))
        return false;

    // CLEAR HIGH ALERT
    if (!writeCommand(HDC302X_REG_CLR_HIGH_ALERT, clear_high))
        return false;

    return true;
}

bool HDC302x_7Semi::getSetAlert(
    float &tempLow, float &humLow,
    float &tempHigh, float &humHigh)
{
    uint16_t lowRaw, highRaw;

    if (!readReg(HDC302X_REG_READ_LOW_ALERT, lowRaw))
        return false;

    if (!readReg(HDC302X_REG_READ_HIGH_ALERT, highRaw))
        return false;

    decodeAlert(lowRaw, tempLow, humLow);
    decodeAlert(highRaw, tempHigh, humHigh);

    return true;
}

bool HDC302x_7Semi::getClearAlert(
    float &tempLow, float &humLow,
    float &tempHigh, float &humHigh)
{
    uint16_t lowRaw, highRaw;

    if (!readReg(HDC302X_REG_READ_CLR_LOW, lowRaw))
        return false;

    if (!readReg(HDC302X_REG_READ_CLR_HIGH, highRaw))
        return false;

    decodeAlert(lowRaw, tempLow, humLow);
    decodeAlert(highRaw, tempHigh, humHigh);

    return true;
}

bool HDC302x_7Semi::setOffset(float tempOffset, float humiOffset)
{
    if ((fabs(tempOffset) > 10.9375f) || (fabs(humiOffset) > 12.5f))
        return false;

    uint16_t value = 0;

    const float TEMP_SCALE = 0.1708984375f; 
    const float HUM_SCALE  = 0.1953125f;   

    // ---- Temperature ----
    int16_t temp_raw = (int16_t)round(fabs(tempOffset) / TEMP_SCALE);

    if (temp_raw > 0x7F)
        temp_raw = 0x7F;

    if (tempOffset >= 0)
        value |= (1 << 7);

    value |= (temp_raw & 0x7F);

    // ---- Humidity ----
    int16_t hum_raw = (int16_t)round(fabs(humiOffset) / HUM_SCALE);

    if (hum_raw > 0x7F)
        hum_raw = 0x7F;

    if (humiOffset >= 0)
        value |= (1 << 15);

    value |= ((hum_raw & 0x7F) << 8);

    return writeCommand(HDC302X_REG_TEMP_HUM_OFFSET, value);
}

bool HDC302x_7Semi::getOffset(float &tempOffset, float &humiOffset)
{
    uint16_t raw;

    if (!readReg(HDC302X_REG_TEMP_HUM_OFFSET, raw)) // replace with macro
        return false;

    float TEMP_SCALE = 0.1708984375f;
    float HUM_SCALE  = 0.1953125f;

    // ---- Temperature ----
    int16_t temp_mag = raw & 0x7F;
    bool temp_positive = (raw >> 7) & 0x01;

    tempOffset = temp_mag * TEMP_SCALE;
    if (!temp_positive)
        tempOffset = -tempOffset;

    // ---- Humidity ----
    int16_t hum_mag = (raw >> 8) & 0x7F;
    bool hum_positive = (raw >> 15) & 0x01;

    humiOffset = hum_mag * HUM_SCALE;
    if (!hum_positive)
        humiOffset = -humiOffset;

    return true;
}

bool HDC302x_7Semi::setMeasurementMode(HDC302X_MeasurementMode mode)
{
    uint16_t command;

    switch (mode)
    {
    case HDC302X_MeasurementMode::TRIGGER:
        command = HDC302X_REG_EXIT_AUTO_MODE;
        break;

    case HDC302X_MeasurementMode::CONTINUOUS_RH_ONLY:
        command = HDC302X_REG_READ_RH_ONLY;
        break;

    case HDC302X_MeasurementMode::CONTINUOUS_MIN_T:
        command = HDC302X_REG_READ_MIN_T;
        break;

    case HDC302X_MeasurementMode::CONTINUOUS_MAX_T:
        command = HDC302X_REG_READ_MAX_T;
        break;

    case HDC302X_MeasurementMode::CONTINUOUS_MIN_RH:
        command = HDC302X_REG_READ_MIN_RH;
        break;

    case HDC302X_MeasurementMode::CONTINUOUS_MAX_RH:
        command = HDC302X_REG_READ_MAX_RH;
        break;

    default:
        return false;
    }

    return writeCommand(command);
}

// bool HDC302x_7Semi::readContinuousRaw(uint16_t &rawData)
// {
//     uint8_t data[3];

//     if (!readOnly(data, 3))
//         return false;

//     if (getCRC(data, 2) != data[2])
//         return false;

//     rawData = (data[0] << 8) | data[1];

//     return true;
// }

bool HDC302x_7Semi::startContinuousHumidity()
{
    return writeCommand(HDC302X_REG_READ_RH_ONLY);
}

bool HDC302x_7Semi::readContinuousRaw(uint16_t &rawData)
{
    if (!i2c)
        return false;

    uint8_t data[3];

    if (!readOnly(data, 3))
        return false;

    // CRC validation
    if (getCRC(data, 2) != data[2])
        return false;

    rawData = (data[0] << 8) | data[1];

    return true;
}

bool HDC302x_7Semi::readHumidity(float &humidity)
{
    uint16_t raw_humidity;

    if (!readContinuousRaw(raw_humidity))
        return false;

    humidity = RawToHumidity(raw_humidity);
    return true;
}

bool HDC302x_7Semi::readContinuousHumidity(float &humidity)
{
   return  readHumidity(humidity);
}

bool HDC302x_7Semi::readMinHumidity(float &humidity)
{
    if(!writeCommand(HDC302X_REG_READ_MIN_RH))
    return false;
    if(!readHumidity(humidity))
    return false;

    return writeCommand(HDC302X_REG_READ_RH_ONLY); 
}

bool HDC302x_7Semi::readMaxHumidity(float &humidity)
{
    if(!writeCommand(HDC302X_REG_READ_MAX_RH))
    return false;
    if(!readHumidity(humidity))
    return false;

    return writeCommand(HDC302X_REG_READ_RH_ONLY); 
}

bool HDC302x_7Semi::readTemperature(float &temperature)
{
    uint16_t raw_temperature;

    if (!readContinousRaw(raw_temperature))
        return false;

    temperature = RawToTemperature(raw_temperature);

    return true;
}

bool HDC302x_7Semi::readMinTemperature(float &temperature)
{
        if(!writeCommand(HDC302X_REG_READ_MIN_T))
    return false;

        if(!readTemperature(temperature))
    return false;

    return writeCommand(HDC302X_REG_READ_RH_ONLY); 
}

bool HDC302x_7Semi::readMaxTemperature(float &temperature)
{
        if(!writeCommand(HDC302X_REG_READ_MAX_T))
    return false;

        if(!readTemperature(temperature))
    return false;

    return writeCommand(HDC302X_REG_READ_RH_ONLY); 
}


bool HDC302x_7Semi::readTemperatureHumidity(float &temperature,
                                 float &humidity,
                                 HDC302x_PowerMode mode)
{   uint16_t command;
    uint8_t data[6];

    switch (mode)
    {
        case LPM0: command = HDC302X_REG_TRIG_LPM0; break;
        case LPM1: command = HDC302X_REG_TRIG_LPM1; break;
        case LPM2: command = HDC302X_REG_TRIG_LPM2; break;
        case LPM3: command = HDC302X_REG_TRIG_LPM3; break;
        default:   command = HDC302X_REG_TRIG_LPM3; break;
    }

    if (!writeCommand(command))
        return false;

    delay(15);

    if (!readOnly(data, 6))
        return false;

    if (getCRC(data, 2) != data[2])
        return false;

    if (getCRC(data + 3, 2) != data[5])
        return false;

    uint16_t rawT = (data[0] << 8) | data[1];
    uint16_t rawH = (data[3] << 8) | data[4];

    temperature = RawToTemperature(rawT);
    humidity    = RawToHumidity(rawH);

    return true;
}

bool HDC302x_7Semi::setMeasurementRate(HDC302x_ODR odr, HDC302x_PowerMode mode)
{
    if ((odr > ODR_10HZ) || (mode > LPM3))
        return false;

    uint16_t command = measurement_command[mode][odr];

    if (!writeCommand(command))
        return false;

    return true;
}

bool HDC302x_7Semi::readReg(uint16_t cmd, uint16_t &value)
{
    uint8_t data[3];

    if (!readReg(cmd, data, 3))
        return false;

    if (getCRC(data, 2) != data[2])
        return false;

    value = (data[0] << 8) | data[1];

    return true;
}

bool HDC302x_7Semi::readReg(uint16_t cmd, uint8_t *data, uint32_t len)
{
    if (!i2c)
        return false;

    i2c->beginTransmission(i2c_address);
    i2c->write(cmd >> 8);
    i2c->write(cmd & 0xFF);

    if (i2c->endTransmission(false) != 0)
        return false;

    if (len > 255)
        return false;

    uint8_t received = i2c->requestFrom(i2c_address, (uint8_t)len);
    if (received != len)
        return false;

    for (uint32_t i = 0; i < len; i++)
    {
        if (!i2c->available())
            return false;

        data[i] = i2c->read();
    }

    return true;
}

bool HDC302x_7Semi::readOnly(uint8_t *data, uint32_t len)
{
    if (!i2c)
        return false;

    if (len > 255)
        return false;

    uint8_t received = i2c->requestFrom(i2c_address, (uint8_t)len);
    if (received != len)
        return false;

    for (uint32_t i = 0; i < len; i++)
    {
        if (!i2c->available())
            return false;

        data[i] = i2c->read();
    }

    return true;
}

bool HDC302x_7Semi::writeCommand(uint16_t cmd)
{
    if (!i2c)
        return false;

    i2c->beginTransmission(i2c_address);
    i2c->write(cmd >> 8);
    i2c->write(cmd & 0xFF);

    return (i2c->endTransmission() == 0);
}

bool HDC302x_7Semi::writeCommand(uint16_t cmd, uint16_t data)
{
    if (!i2c)
        return false;

    uint8_t buff[5];

    buff[0] = cmd >> 8;
    buff[1] = cmd & 0xFF;
    buff[2] = data >> 8;
    buff[3] = data & 0xFF;
    buff[4] = getCRC(buff + 2, 2);

    if (!writeOnly(buff, 5))
        return false;

    delay(1);

    uint16_t status;
    if (!getStatus(status))
        return false;

    if (status & 0x0001)
        return false;

    return true;
}

bool HDC302x_7Semi::writeOnly(uint8_t *data, uint32_t len)
{
    if (!i2c)
        return false;

    i2c->beginTransmission(i2c_address);

    for (uint32_t i = 0; i < len; i++)
        i2c->write(data[i]);

    return (i2c->endTransmission() == 0);
}

uint16_t HDC302x_7Semi::encodeAlert(float temp, float hum)
{
    if (temp < -45) temp = -45;
    if (temp > 130) temp = 130;

    if (hum < 0) hum = 0;
    if (hum > 100) hum = 100;

    uint16_t rawT = (uint16_t)(((temp + 45.0f) / 175.0f) * 65535.0f);
    uint16_t rawH = (uint16_t)((hum / 100.0f) * 65535.0f);

    uint16_t t9 = rawT >> 7;   // 9 MSB
    uint16_t h7 = rawH >> 9;   // 7 MSB

    return (h7 << 9) | t9;
}

void HDC302x_7Semi::decodeAlert(uint16_t raw, float &temp, float &hum)
{
    uint16_t rh = (raw >> 9) & 0x7F;
    uint16_t t  = raw & 0x1FF;

    uint16_t rawRH = rh << 9;
    uint16_t rawT  = t << 7;

    hum  = RawToHumidity(rawRH);
    temp = RawToTemperature(rawT);
}

// bool HDC302x_7Semi::readContinousRaw(uint16_t &rawData)
// {
//     if (!i2c)
//         return false;

//     uint8_t len = 3;
//     uint8_t data[len];

//     if (!readOnly(data, len))
//         return false;

//     // // Debug
//     // Serial.print("Data:");
//     // for (uint8_t i = 0; i < len; i++)
//     // {
//     //     Serial.print(" ");
//     //     Serial.print(data[i], HEX);
//     // }
//     // Serial.println();

//     // CRC check
//     if (getCRC(data, 2) != data[2])
//     {
//         Serial.println("CRC mismatch!");
//         return false;
//     }

//     rawData = (data[0] << 8) | data[1];

//     return true;
// }

// bool HDC302x_7Semi::startContionousHimidity()
// {
//     return writeCommand(HDC302X_REG_READ_RH_ONLY);
// }

// bool HDC302x_7Semi::readHumidity(float &humidity)
// {
//     uint16_t raw_humidity;

//     if (!readContinousRaw(raw_humidity))
//         return false;

//     humidity = RawToHumidity(raw_humidity);
//     return true;
// }

// bool HDC302x_7Semi::readContinousHumidity(float &humidity)
// {
//    return  readHumidity(humidity);
// }

// bool HDC302x_7Semi::readMinHumidity(float &humidity)
// {
//     if (!writeCommand(HDC302X_REG_READ_MIN_RH))
//         return false;

//     if (!readHumidity(humidity))
//         return false;

//     return writeCommand(HDC302X_REG_READ_RH_ONLY);
// }

// bool HDC302x_7Semi::readMaxHumidity(float &humidity)
// {
//     if(!writeCommand(HDC302X_REG_READ_MAX_RH))
//     return false;
//     if(!readHumidity(humidity))
//     return false;

//     return writeCommand(HDC302X_REG_READ_RH_ONLY); 
// }

// bool HDC302x_7Semi::readTemperature(float &temperature)
// {
//     uint16_t raw_temperature;

//     if (!readContinousRaw(raw_temperature))
//         return false;

//     temperature = RawToTemperature(raw_temperature);

//     return true;
// }

// bool HDC302x_7Semi::readMinTemperature(float &temperature)
// {
//         if(!writeCommand(HDC302X_REG_READ_MIN_T))
//     return false;

//         if(!readTemperature(temperature))
//     return false;

//     return writeCommand(HDC302X_REG_READ_RH_ONLY); 
// }

// bool HDC302x_7Semi::readMaxTemperature(float &temperature)
// {
//         if(!writeCommand(HDC302X_REG_READ_MAX_T))
//     return false;

//         if(!readTemperature(temperature))
//     return false;

//     return writeCommand(HDC302X_REG_READ_RH_ONLY); 
// }

// bool HDC302x_7Semi::readTemperatureHumidity(float &temperature,
//                                  float &humidity,
//                                  HDC302x_PowerMode mode)
// {   uint16_t command;
//     uint8_t data[6];

//     switch (mode)
//     {
//         case LPM0: command = HDC302X_REG_TRIG_LPM0; break;
//         case LPM1: command = HDC302X_REG_TRIG_LPM1; break;
//         case LPM2: command = HDC302X_REG_TRIG_LPM2; break;
//         case LPM3: command = HDC302X_REG_TRIG_LPM3; break;
//         default:   command = HDC302X_REG_TRIG_LPM3; break;
//     }

//     if (!writeCommand(command))
//         return false;

//     delay(15);

//     if (!readOnly(data, 6))
//         return false;

//     if (getCRC(data, 2) != data[2])
//         return false;

//     if (getCRC(data + 3, 2) != data[5])
//         return false;

//     uint16_t rawT = (data[0] << 8) | data[1];
//     uint16_t rawH = (data[3] << 8) | data[4];

//     temperature = RawToTemperature(rawT);
//     humidity    = RawToHumidity(rawH);

//     return true;
// }

// /**
//  * setMeasurementRate
//  *
//  * - Configures sensor measurement rate and power mode
//  * - Controls how frequently new data is generated
//  *
//  * Parameters:
//  * - odr  → output data rate (0.5Hz to 10Hz)
//  * - mode → power mode (LPM0 to LPM3)ss
//  *
//  * Notes:
//  * - Higher ODR → faster updates, higher power consumption
//  * - Lower ODR → lower power, slower response
//  *
//  * Power Modes:
//  * - LPM0 → highest performance
//  * - LPM3 → lowest power consumption
//  *
//  * Behavior:
//  * - Selects command from internal lookup table
//  * - Applies configuration immediately
//  *
//  * Returns:
//  * - true  → Configuration applied successfully
//  * - false → Invalid input or I2C communication failed
//  */
// bool HDC302x_7Semi::setMeasurementRate(HDC302x_ODR odr, HDC302x_PowerMode mode)
// {
//     if ((odr > ODR_10HZ) || (mode > LPM3))
//         return false;

//     uint16_t command = measurement_command[mode][odr];

//     if (!writeCommand(command))
//         return false;

//     return true;
// }

float HDC302x_7Semi::RawToHumidity(uint16_t h_raw)
{
    return 100.0f * ((float)h_raw / 65535.0f);
}

float HDC302x_7Semi::RawToTemperature(uint16_t t_raw)
{
    return -45.0f + 175.0f * ((float)t_raw / 65535.0f);
}

uint8_t HDC302x_7Semi::getCRC(const uint8_t *data, int len)
{
    uint8_t crc = 0xFF;
    for (int i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (int b = 0; b < 8; b++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc <<= 1;
        }
    }
    return crc;
}