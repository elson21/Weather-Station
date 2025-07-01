
#include <Arduino.h>
#include <Wire.h>
#include "bme280.h"

// instantiate classes
BME280 bme; // temperature, humidity & pressure

void setup(){
    Serial.begin(115200);
    Serial.println("Booting...");

    Wire.begin(21, 22);
    Serial.println("Wire.begin completed");

    if(!bme.begin(Wire, 0x76)){
        Serial.println("Sensor BME280 not found");
        while(1);
    }

    Serial.println("BME280 initialized succesfully");
}

void loop(){
    Serial.println("Loop running...");
    
    // read sensors
    float temperature = bme.readTemperature();
    float pressure = bme.readPressure();
    float humidity = bme.readHumidity();

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    
    Serial.print("Pressure: ");
    Serial.print(pressure / 100000.0f);
    Serial.println("bar");
    
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    delay(1000);
}
