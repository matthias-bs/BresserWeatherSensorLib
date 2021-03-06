///////////////////////////////////////////////////////////////////////////////////////////////////
// BresserWeatherSensorCallback.ino
//
// Bresser 5-in-1/6-in-1 868 MHz Weather Sensor Radio Receiver 
// based on CC1101 or SX1276/RFM95W and ESP32/ESP8266
//
// https://github.com/matthias-bs/Bresser_Weather_Sensor_Receiver
//
// Based on:
// ---------
// Bresser5in1-CC1101 by Sean Siford (https://github.com/seaniefs/Bresser5in1-CC1101)
// RadioLib by Jan Gromeš (https://github.com/jgromes/RadioLib)
// rtl433 by Benjamin Larsson (https://github.com/merbanan/rtl_433) 
//     - https://github.com/merbanan/rtl_433/blob/master/src/devices/bresser_5in1.c
//     - https://github.com/merbanan/rtl_433/blob/master/src/devices/bresser_6in1.c
//
// created: 05/2022
//
//
// MIT License
//
// Copyright (c) 2022 Matthias Prinke
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// History:
//
// 20220523 Created from https://github.com/matthias-bs/Bresser5in1-CC1101
// 20220524 Moved code to class WeatherSensor
//
// ToDo: 
// - 
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include "WeatherSensor.h"


WeatherSensor weatherSensor;

// Example for callback function which is executed while waiting for radio messages
void loopCallback(void)
{
    // Normally something really important would be done here
    Serial.print(".");
}

void setup() {    
    Serial.begin(115200);

    weatherSensor.begin();
}


void loop() 
{
    // Attempt to receive entire data set with timeout of 48s and callback function
    bool decode_ok = weatherSensor.getData(48000, true, &loopCallback);
    Serial.println();
    
    if (decode_ok) {
        const float METERS_SEC_TO_MPH = 2.237;
        Serial.printf("Id: [%8X] Battery: [%s] ",
            weatherSensor.sensor_id,
            weatherSensor.battery_ok ? "OK " : "Low");
        #ifdef BRESSER_6_IN_1
            Serial.printf("Ch: [%d] ", weatherSensor.chan);
        #endif
        Serial.printf("Temp: [%5.1fC] Hum: [%3d%%] ",
            weatherSensor.temp_c,
            weatherSensor.humidity);
        Serial.printf("Wind max: [%4.1fm/s] Wind avg: [%4.1fm/s] Wind dir: [%5.1fdeg] ",
            weatherSensor.wind_gust_meter_sec,
            weatherSensor.wind_avg_meter_sec,
            weatherSensor.wind_direction_deg);
        Serial.printf("Rain: [%7.1fmm] ",  
            weatherSensor.rain_mm);
        #ifdef HAS_MOISTURE
            Serial.printf("Moisture: [%2d%%] ",
                weatherSensor.moisture);
        #endif
        Serial.printf("RSSI: [%4.1fdBm]\n", weatherSensor.rssi);
    } else {
        Serial.printf("Sensor timeout\n");
    }
    delay(100);
} // loop()
