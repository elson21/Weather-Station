// uv_sensor.cpp

/**
+---------------------------------------------------+
|  Voltage  | UV index | Risk of harm | Colour      |
+---------------------------------------------------+
|  <0.050   |    0     | low          | Green       |
|   0.227   |    1     | low          | Green       |
|   0.318   |    2     | low          | Green       |
|   0.408   |    3     | moderate     | Yellow      |
|   0.503   |    4     | moderate     | Yellow      |
|   0.606   |    5     | moderate     | Yellow      |
|   0.696   |    6     | high         | Orange      |
|   0.795   |    7     | high         | Orange      |
|   0.881   |    8     | very high    | Red         |
|   0.976   |    9     | very high    | Red         |
|   1.079   |   10     | very high    | Red         |
|  >1.170   |   11     | extreme      | Purple      |
+---------------------------------------------------+
 */

#include <Arduino.h>

float getUVVoltage(int uvPin){
    int uvRaw = analogRead(uvPin);

    return uvRaw * (3.3 / 4095.0); // ESP32 uses a 12-bit ADC
}

float voltageToIndex(float voltage){
    if(voltage < 0.050) return 0;
    if(voltage < 0.227) return 1;
    if(voltage < 0.318) return 2;
    if(voltage < 0.408) return 3;
    if(voltage < 0.503) return 4;
    if(voltage < 0.606) return 5;
    if(voltage < 0.696) return 6;
    if(voltage < 0.795) return 7;
    if(voltage < 0.881) return 8;
    if(voltage < 0.976) return 9;
    if(voltage < 1.079) return 10;
    if(voltage < 1.170) return 11;
    return 11; // >= 1.170
}