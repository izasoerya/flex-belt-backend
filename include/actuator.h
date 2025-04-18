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

    static Actuator fromJSON(const JSONVar &json)
    {
        uint16_t heater = json.hasOwnProperty("heater") ? (int)json["heater"] : 0;
        bool isCold = json.hasOwnProperty("isCold") ? (bool)json["isCold"] : false;
        return Actuator(heater, isCold);
    }

    void getData(uint16_t &v, bool &c)
    {
        v = heater;
        c = isCold;
    }
};