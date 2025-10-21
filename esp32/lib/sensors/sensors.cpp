// sensor.cpp

#include <Arduino.h>
#include <Wire.h>
#include "sensors.h"
#include "i2c_manager.h"

bool Sensors::begin(){

    I2CManager::begin();
    Serial.println("I2C initialization completed.");

    I2CManager::scanBus();

    // Initialize BME280 sensor
    if(!bme.begin(I2CManager::getWire(), 0x76)){
        Serial.println("Sensor BME280 not found");
        while(1);
    }
    Serial.println("BME280 initialized succesfully");

    // Initialize BH1750 sensonr
    if(!bh.begin(I2CManager::getWire(), 0x23)){
        Serial.println("Sensor BH1750 not found.");
        while(1);
    }
    Serial.println("BH1750 Initialized succesfully");

    Serial.println("All sensors initialized.");

    return true;
}


SensorData Sensors::read(){
    
    SensorData data;

    // bme readings
    data.temperature = bme.readTemperature();
    data.pressure = bme.readPressure() / 101325.0;
    data.humidity = bme.readHumidity();

    // bh readings
    data.lightLevel = bh.readAmbientLight();

    // uv readings
    float uvVoltage = getUVVoltage(UV_PIN);
    data.uvIndex = voltageToIndex(uvVoltage);

    return data;
}