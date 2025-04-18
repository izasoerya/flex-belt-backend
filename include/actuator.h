#pragma once

#include <Arduino_JSON.h>

class Actuator
{
private:
    uint16_t heater;
    bool isCold;

public:
    Actuator(uint16_t v = 0, bool c = false) : heater(v), isCold(c) {}

    JSONVar toJSON()
    {
        JSONVar actuatorData;
        actuatorData["heater"] = heater;
        actuatorData["isCold"] = isCold;

         return actuatorData;
    }

    Actuator fromJSON(const JSONVar &json)
{
    if (json.hasOwnProperty("heater"))
    {
        Serial.print("[DEBUG] JSON heater raw: ");
        Serial.println(JSON.stringify(json["heater"]));
        Serial.print("[DEBUG] As double: ");
        Serial.println((double)json["heater"]);

        heater = (uint16_t)(double)json["heater"];
    }

    if (json.hasOwnProperty("isCold"))
    {
        Serial.print("[DEBUG] JSON isCold raw: ");
        Serial.println(JSON.stringify(json["isCold"]));
        
        isCold = JSON.stringify(json["isCold"]) == "true";
    }

    return *this;
}


    void getData(uint16_t &v, bool &c)
    {
        v = heater;
        c = isCold;
    }
};