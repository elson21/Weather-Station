// sensors.h

#ifndef SENSORS_H
#define SENSORS_H

#include "bme280.h"
#include "bh1750.h"
#include "uv_sensor.h"

struct SensorData {
    float temperature;  // °C
    float pressure;     // Pa
    float humidity;     // %RH
    float lighLevel;    // lux
    float uvIndex;      // 0-11
};


class Sensors {
    public:
        bool begin();
        SensorData read();

    private:
        BME280 bme;
        BH1750 bh;
        const int UV_PIN = 34;
};


#endif