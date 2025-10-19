
#include <Arduino.h>
#include <Wire.h>
#include "bme280.h"
#include "bh1750.h"
#include "uv_sensor.h"
#include "i2c_manager.h"


// Pins
const int UV_PIN = 34;


// instantiate classes
BME280 bme; // temperature, humidity & pressure
BH1750 bh;  // ambient light


void setup(){
    Serial.begin(115200);
    Serial.println("Booting...");

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
}


void loop(){
    Serial.println("Loop running...");
    
    // read BME sensors
    float temperature = bme.readTemperature();
    float pressure = bme.readPressure();
    float humidity = bme.readHumidity();

    // read BH sensor
    float ambientLight = bh.readAmbientLight();

    // read UV sensor
    float uvVoltage = getUVVoltage(UV_PIN);
    float uvIndex = voltageToIndex(uvVoltage);

    // print values
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    
    Serial.print("Pressure: ");
    Serial.print(pressure / 100000.0f);
    Serial.println("bar");
    
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    Serial.print("Light intensity: ");
    Serial.print(ambientLight);
    Serial.println(" lux");

    Serial.print("UV index: ");
    Serial.print(uvIndex);
    Serial.println();

    Serial.print("UV voltage: ");
    Serial.print(analogRead(UV_PIN));
    Serial.println(" V");

    delay(5000);
}
