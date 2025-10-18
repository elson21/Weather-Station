// bh1750.cpp

/**
 * BH1750::begin()
 *      1. Check the I2C bus is up
 *      2. Check the sensor is alive
 *      3. Power on the sensor
 *      4. Start continuous measurement mode
 *
 * BH1750::readAmbientLight()
 */

 #include <Arduino.h>
 #include "bh1750.h"

 namespace {
    constexpr uint8_t POWER_DOWN =       0X00;
    constexpr uint8_t POWER_ON =         0X01;
    constexpr uint8_t RESET =            0X07;  
    constexpr uint8_t CONTINUOUS_H_RES = 0X10; // Start continuous measurement at 1lx | measure every 120ms
    constexpr uint8_t ONE_TIME_H_RES =   0X20; // One time measurement at 1lx and go to POWER_DOWN
 }


 bool BH1750::begin(TwoWire &wire, uint8_t addr){
    _wire = &wire;
    _addr = addr;

    // Check I2c bus is up
    Serial.println("bh.begin: starting");
    _wire->begin();

    // Check the sensor is alive
    Serial.println("bh.begin: checking ID");
    _wire->beginTransmission(_addr);
    if(_wire->endTransmission() != 0){
        Serial.println("bh.begin: endTransmission failed!");
        return false;
    }

    _wire->beginTransmission(_addr);
    _wire->write(POWER_ON);
    _wire->endTransmission();

    _wire->beginTransmission(_addr);
    _wire->write(CONTINUOUS_H_RES);
    _wire->endTransmission();
    
    return true;
 }


float BH1750::readAmbientLight(){
    
    _wire->requestFrom(_addr, (uint8_t)2);
    
    uint8_t highByte = _wire->read();
    uint8_t lowByte = _wire->read();
    uint16_t rawValue = (highByte << 8) | lowByte;

    float lightLevel = rawValue / 1.2;   // Datasheet pg.7

    return lightLevel;
}