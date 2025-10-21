// http_client.cpp

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "http_client.h"

void HTTPWebClient::init(const char *serverURL){
    _serverURL = serverURL;
}


bool HTTPWebClient::sendReadings(const SensorData &data){
    if(WiFi.status() != WL_CONNECTED){
        Serial.println("WiFi not conneceted.");
        return false;
    }

    ::HTTPClient http;  // Arduino class
    http.begin(_serverURL);
    http.addHeader("Content-Type", "application/json");

    String json = "{";
    json += "\"temperature\":" + String(data.temperature, 2) + ", ";
    json += "\"humidity\":" + String(data.humidity, 2) + ", ";
    json += "\"pressure\":" + String(data.pressure, 2) + ", ";
    json += "\"light_level\":" + String(data.lightLevel, 2) + ", ";
    json += "\"uv_index\":" + String(data.uvIndex, 1);
    json += "}";

    Serial.println("Sending: " + json);

    int httpCode = http.POST(json);

    if (httpCode == 200){
        Serial.println("Data sent succesfully!");
        http.end();
        return true;
    }
    else if (httpCode > 0){
        Serial.printf("HTTP Error %d\n", httpCode);
    }
    else{
        Serial.println("Connection failed.");
    }

    http.end();

    return false;
}