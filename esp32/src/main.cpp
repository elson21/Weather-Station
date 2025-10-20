
#include <Arduino.h>
#include <WiFi.h>
#include "sensors.h"


// WiFi credentials
// CHANGE IT TO SOMETHING SAFER!!
const char *ssid = "ssid";
const char *password = "password";


// instantiate classes
Sensors sensors;


void setup(){
    Serial.begin(115200);
    Serial.println("Booting...");

    btStop();   // disable bluetooth

    // setup wiifi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
        delay(100);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");

    // initialize sensors
    if (!sensors.begin()){
        Serial.println("Sensor initialization failed.");
        while(1);
    }
}


void loop(){
    
    SensorData data = sensors.read();

    Serial.println("Temperature: ");
    Serial.print(data.temperature);
    Serial.println("°C");

    Serial.println("Pressure: ");
    Serial.print(data.pressure / 100.0);
    Serial.println("hPa");

    Serial.println("Humidity: ");
    Serial.print(data.humidity);
    Serial.println("%");

    Serial.println("Light: ");
    Serial.print(data.lighLevel);
    Serial.println("lux");

    Serial.println("UV Index: ");
    Serial.print(data.uvIndex);

    delay(5000);
}
