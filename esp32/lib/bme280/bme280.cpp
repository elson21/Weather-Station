// bme280.cpp

/**
 * BME280::begin()
 *      1. Check that the I2C bus is up
 *      2. Check that the sensor is present
 *      3. Read temperature and pressure calibration
 *      4. Read humidity calibration
 *      5. Configure oversamping and mode
 * 
 * BME280::readTemperature()
 * 
 * BME280::readPressure()
 * 
 * BME280::readHumidity
 */

#include <Arduino.h>
#include "bme280.h"

namespace{
    constexpr uint8_t REG_ID =          0xD0;   // chip ID reg
    constexpr uint8_t REG_RST =         0xE0;   // soft reset
    constexpr uint8_t REG_CTRL_HUM =    0xF2;   // control humidity reg
    constexpr uint8_t REG_STATUS =      0xF3;   // status reg
    constexpr uint8_t REG_CTRL_MEAS =   0xF4;   // control measurements reg
    constexpr uint8_t REG_CONFIG =      0xF5;   // config reg
    constexpr uint8_t REG_PRESS_MSB =   0xF7;   // pressure reg msb
    constexpr uint8_t REG_PRESS_LSB =   0xF8;   // pressure reg lsb
    constexpr uint8_t REG_PRESS_XLSB =  0xF9;   // pressure reg xlsb
    constexpr uint8_t REG_TEMP_MSB =    0xFA;   // temperature reg msb
    constexpr uint8_t REG_TEMP_LSB =    0xFB;   // temperature reg lsb
    constexpr uint8_t REG_TEMP_XLSB =   0xFC;   // temperature reg xlsb
    constexpr uint8_t REG_HUM_MSB =     0xFD;   // humidity reg msb
    constexpr uint8_t REG_HUM_LSB =     0xFE;   // humidity reg lsb
    constexpr uint8_t CHIP_ID =         0x60;   // chip ID
}   // namespace for register addresses

/**
 * @brief Initializes the BME280 sensor over I²C.
 *
 * This function sets up the I²C communication, verifies the sensor's chip ID,
 * reads the temperature, pressure, and humidity calibration data from the sensor,
 * and configures the sensor's oversampling and operating mode.
 */
bool BME280::begin(TwoWire &wire, uint8_t addr){
    _wire = &wire;
    _addr = addr;

    // check that I2C is running
    Serial.println("bme.begin: starting");
    _wire->begin();

    // check that the sensor is present and operational
    Serial.println("bme.begin: checking ID");
    _wire->beginTransmission(_addr);
    _wire->write(REG_ID);
    if(_wire->endTransmission() != 0) {
        Serial.println("bme.begin: endTransmission failed.");
        return false;
    }

    // read 1 byte and ensure the chip is present, operational and sending the correct data
    _wire->requestFrom(_addr, (uint8_t)1); 
    if (_wire->available() == 0) {
        Serial.println("bme.begin: no data available fromchip ID to read");
        return false;
    }

    uint8_t id = _wire->read();
    Serial.print("bme.begin: chip ID = 0x");
    Serial.println(id, HEX);

    if (id != CHIP_ID) {
        Serial.println("bme.begin: chip ID mismatch");
        return false;
    }

    /** 
    * read first calibration block(0x88 -> 0xA1)
    * temp 0x88 -> 0x8D
    * pressure 0x8E -> 0x9F
    * humidity 0xA1
    */
    Serial.println("bme.begin: reading calibration data");
    _wire->beginTransmission(_addr);
    _wire->write(0x88);
    _wire->endTransmission();
    _wire->requestFrom(_addr, (uint8_t)26);
    uint8_t firstCalibrationBlock[26];

    for (int i = 0; i < 26; i++){
        if (_wire->available()){
            firstCalibrationBlock[i] = _wire->read();
        } else {
            Serial.print("bme.begin: calibration byte ");
            Serial.print(i);
            Serial.println(" not available");
            return false;
        }
    }

    Serial.println("bme.begin: calibration block read successfully");

    // MSB << 8 | LSB
    dig_T1 = (uint16_t)((firstCalibrationBlock[1] << 8) | firstCalibrationBlock[0]);
    dig_T2 = (int16_t)((firstCalibrationBlock[3] << 8) | firstCalibrationBlock[2]);
    dig_T3 = (int16_t)((firstCalibrationBlock[5] << 8) | firstCalibrationBlock[4]);

    dig_P1 = (uint16_t)((firstCalibrationBlock[7] << 8) | firstCalibrationBlock[6]);
    dig_P2 = (int16_t)((firstCalibrationBlock[9] << 8) | firstCalibrationBlock[8]);
    dig_P3 = (int16_t)((firstCalibrationBlock[11] << 8) | firstCalibrationBlock[10]);
    dig_P4 = (int16_t)((firstCalibrationBlock[13] << 8) | firstCalibrationBlock[12]);
    dig_P5 = (int16_t)((firstCalibrationBlock[15] << 8) | firstCalibrationBlock[14]);
    dig_P6 = (int16_t)((firstCalibrationBlock[17] << 8) | firstCalibrationBlock[16]);
    dig_P7 = (int16_t)((firstCalibrationBlock[19] << 8) | firstCalibrationBlock[18]);
    dig_P8 = (int16_t)((firstCalibrationBlock[21] << 8) | firstCalibrationBlock[20]);
    dig_P9 = (int16_t)((firstCalibrationBlock[23] << 8) | firstCalibrationBlock[22]);

    dig_H1 = firstCalibrationBlock[25];

    // read second calibration block (0xE1 -> 0xE7)
    _wire->beginTransmission(_addr);
    _wire->write(0xE1);
    _wire->endTransmission();
    _wire->requestFrom(_addr, (uint8_t)7);
    uint8_t secondCalibrationBlock[7];

    for (int i = 0; i < 7; i++)
        secondCalibrationBlock[i] = _wire->read();

    dig_H2 = (int16_t)((secondCalibrationBlock[1] << 8) | secondCalibrationBlock[0]);
    dig_H3 = secondCalibrationBlock[2];
    dig_H4 = (int16_t)((secondCalibrationBlock[3] << 4) | secondCalibrationBlock[4] & 0x0F);
    dig_H5 = (int16_t)((secondCalibrationBlock[5] << 4) | secondCalibrationBlock[4] >> 4);
    dig_H6 = secondCalibrationBlock[6];

    // configure oversmapling and mode
    // humidity x1
    _wire->beginTransmission(_addr);
    _wire->write(REG_CTRL_HUM);
    _wire->write(0x01); // oversampling x1
    _wire->endTransmission();

    // temperature & pressure x1, normal mode
    _wire->beginTransmission(_addr);
    _wire->write(REG_CTRL_MEAS);
    _wire->write((1 << 5) | (1 << 3) | 3);  // osrs_t, osrs_p, mode
    _wire->endTransmission();

    return true;
}

float BME280::readTemperature(){

    // read 20bits raw temp measurements
    _wire->beginTransmission(_addr);
    _wire->write(REG_TEMP_MSB);
    _wire->endTransmission();

    _wire->requestFrom(_addr, (uint8_t)3);
    uint32_t adc_T = (_wire->read() << 12) |
                    (_wire->read() << 4) |
                    (_wire->read() >> 4 & 0x0F);

    // apply compensation formula for temperature (4.2.3 datasheet)
    int32_t var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * (int32_t)dig_T2) >> 11;
    int32_t var2 = (((((adc_T >> 4) - (int32_t)dig_T1) * ((adc_T >> 4) - (int32_t)dig_T1)) >> 12)
                   * (int32_t)dig_T3) >> 14;
    t_fine = var1 + var2;

    // return temp in °C
    return ((t_fine * 5 + 128) >> 8) / 100.0f;
}


float BME280::readPressure(){
    // get t_fine
    readTemperature();

    // read 20bits raw pressure measurements
    _wire->beginTransmission(_addr);
    _wire->write(REG_PRESS_MSB);
    _wire->endTransmission();

    _wire->requestFrom(_addr, (uint8_t)3);
    uint32_t adc_P = (_wire->read() << 12) |
                    (_wire->read() << 4) |
                    (_wire->read() >> 4 & 0x0F);

    //apply compensation formula for pressure (4.2.4)
    int64_t var1 = (int64_t)t_fine - 128000;
    int64_t var2 = var1 * var1 * (int64_t)dig_P6;
    var2 += var1 * (int64_t)dig_P5 << 17;
    var2 += (int64_t)dig_P4 << 35;
    var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;
    if (var1 == 0) return 0;// avoids division by 0
    int64_t p = 1048576-adc_P;
    p = (((p<<31)-var2)*3125)/var1;
    var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
    
    return (float)p / 256.0f;
}

float BME280::readHumidity(){
    // get the t_fine
    readTemperature();

    // read 16bit raw humidity measurements
    _wire->beginTransmission(_addr);
    _wire->write(REG_HUM_MSB);
    _wire->endTransmission();
    _wire->requestFrom(_addr, (uint8_t)2);
    int32_t adc_H1 = _wire->read() << 8;
    int32_t adc_H2 = _wire->read();
    int32_t adc_H = adc_H1 | adc_H2;

    // apply compensation formula
    int32_t v_x1 = t_fine - 76800;
    v_x1 = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) 
        *v_x1)) + ((int32_t)16384)) >> 15) * (((((((v_x1 
        *((int32_t)dig_H6)) >> 10) * (((v_x1 * ((int32_t)dig_H3)) >> 11) 
        +((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) 
        + 8192) >> 14));
    v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) *
    ((int32_t)dig_H1)) >> 4));
    v_x1 = (v_x1 < 0 ? 0 : v_x1);
    v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
    
    return (v_x1>>12) / 1024.0f;
}