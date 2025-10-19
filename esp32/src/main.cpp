
#include <Arduino.h>
#include <Wire.h>
#include "bme280.h"
#include "bh1750.h"

// instantiate classes
BME280 bme; // temperature, humidity & pressure
BH1750 bh;  // ambient light

void setup(){
    Serial.begin(115200);
    Serial.println("Booting...");

    Wire.begin(21, 22); // initialize I2C
    Serial.println("Wire.begin completed");

    if(!bme.begin(Wire, 0x76)){
        Serial.println("Sensor BME280 not found");
        while(1);
    }
    Serial.println("BME280 initialized succesfully");

    if(!bh.begin(Wire, 0x23)){
        Serial.println("Sensor BH1750 not found.");
        while(1);
    }
}

void loop(){
    Serial.println("Loop running...");
    
    // read BME sensors
    float temperature = bme.readTemperature();
    float pressure = bme.readPressure();
    float humidity = bme.readHumidity();

    // read BH sensor
    float ambientLight = bh.readAmbientLight();

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
    Serial.print(" lux");

    delay(1000);
}
