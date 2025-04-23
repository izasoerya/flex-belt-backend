#pragma once

#include <Arduino_JSON.h>

class Payload
{
private:
    float angleX;
    float angleY;
    float angleZ;
    uint16_t flex;
    String description;
    String status;

public:
    Payload(float x, float y, float z, uint16_t f, String d = "", String s = "")
        : angleX(x), angleY(y), angleZ(z), flex(f), description(d), status(s) {}

    JSONVar toJSON()
    {
        JSONVar payloadData;
        payloadData["angleX"] = angleX;
        payloadData["angleY"] = angleY;
        payloadData["angleZ"] = angleZ;
        payloadData["flex"] = flex;
        payloadData["description"] = description;
        payloadData["status"] = status;

        return payloadData;
    }

    Payload copyWith(
        float x = -1, float y = -1, float z = -1, uint16_t f = 0,
        String d = "", String s = "")
    {
        return Payload(
            x != -1 ? x : angleX,
            y != -1 ? y : angleY,
            z != -1 ? z : angleZ,
            f != 0 ? f : flex,
            d != "" ? d : description,
            s != "" ? s : status);
    }
};