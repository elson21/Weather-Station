
#include <Arduino.h>
#include <WiFi.h>
#include "sensors.h"
#include "http_client.h"
#include "config.h"


// instantiate classes
Sensors sensors;
HTTPWebClient http;


void setup(){
    Serial.begin(115200);
    Serial.println("Booting...");

    btStop();   // disable bluetooth

    // setup wiifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED){
        delay(100);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");

    http.init(SERVER_URL);

    // initialize sensors
    if (!sensors.begin()){
        Serial.println("Sensor initialization failed.");
        while(1);
    }
}


void loop(){
    
    SensorData data = sensors.read();

    // Serial.println("Temperature: ");
    // Serial.print(data.temperature);
    // Serial.println("°C");

    // Serial.println("Pressure: ");
    // Serial.print(data.pressure / 100.0);
    // Serial.println("hPa");

    // Serial.println("Humidity: ");
    // Serial.print(data.humidity);
    // Serial.println("%");

    // Serial.println("Light: ");
    // Serial.print(data.lightLevel);
    // Serial.println("lux");

    // Serial.println("UV Index: ");
    // Serial.print(data.uvIndex);
    // Serial.println();

    http.sendReadings(data);

    delay(5000);
}
