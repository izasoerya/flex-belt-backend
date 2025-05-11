#pragma once

#include <Arduino_JSON.h>

class Actuator
{
private:
    uint16_t heater;
    bool isCold;
    bool resetEncoder; // Assuming this is not used in the context

public:
    Actuator(uint16_t v = 0, bool c = false, bool r = false)
        : heater(v), isCold(c), resetEncoder(r) {}

    JSONVar toJSON()
    {
        JSONVar actuatorData;
        actuatorData["heater"] = heater;
        actuatorData["isCold"] = isCold;
        actuatorData["resetEncoder"] = resetEncoder; // Assuming this is not used in the context

        return actuatorData;
    }

    // Didint work dunno why, lazy to investigate
    Actuator fromJSON(const JSONVar &json)
    {
        Serial.println(JSONVar().stringify(json)); // <== Add this to see the JSON data
        if (json.hasOwnProperty("heater"))
        {
            heater = static_cast<int>(json["heater"]); // <== force cast
        }
        if (json.hasOwnProperty("isCold"))
        {
            isCold = static_cast<bool>(json["isCold"]);
        }
        if (json.hasOwnProperty("resetEncoder"))
        {
            resetEncoder = static_cast<bool>(json["resetEncoder"]);
        }
        return *this;
    }

    Actuator fromRawJson(const String &jsonStr)
    {
        int heaterStart = jsonStr.indexOf("\"heater\":") + 9;
        int heaterEnd = jsonStr.indexOf(',', heaterStart);
        String heaterValue = jsonStr.substring(heaterStart, heaterEnd);
        heater = heaterValue.toInt();

        int isColdStart = jsonStr.indexOf("\"isCold\":") + 9;
        int isColdEnd = jsonStr.indexOf(',', isColdStart);
        String isColdValue = jsonStr.substring(isColdStart, isColdEnd);
        isColdValue.trim(); // clean up whitespace
        isCold = (isColdValue == "true");

        int resetEncoderStart = jsonStr.indexOf("\"resetEncoder\":") + 15;
        int resetEncoderEnd = jsonStr.indexOf('}', resetEncoderStart);
        String resetEncoderValue = jsonStr.substring(resetEncoderStart, resetEncoderEnd);
        resetEncoderValue.trim(); // clean up whitespace
        Serial.println(resetEncoderValue);
        resetEncoder = (resetEncoderValue == "true");

        return *this;
    }

    void getData(uint16_t &v, bool &c, bool &r)
    {
        v = heater;
        c = isCold;
        r = resetEncoder; // Assuming resetEncoder is not used in this context
    }
};