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
    int encoder;
    float battery;

public:
    Payload(float x, float y, float z, uint16_t f, String d = "", String s = "", int e = 0, float b = 0)
        : angleX(x), angleY(y), angleZ(z), flex(f), description(d), status(s), encoder(e), battery(b) {}

    JSONVar toJSON()
    {
        JSONVar payloadData;
        payloadData["angleX"] = angleX;
        payloadData["angleY"] = angleY;
        payloadData["angleZ"] = angleZ;
        payloadData["flex"] = flex;
        payloadData["description"] = description;
        payloadData["status"] = status;
        payloadData["encoder"] = encoder;
        payloadData["battery"] = battery;

        return payloadData;
    }

    Payload copyWith(
        float x = -1, float y = -1, float z = -1, uint16_t f = 0,
        String d = "", String s = "", int e = 0, float b = 0)
    {
        return Payload(
            x != -1 ? x : angleX,
            y != -1 ? y : angleY,
            z != -1 ? z : angleZ,
            f != 0 ? f : flex,
            d != "" ? d : description,
            s != "" ? s : status,
            e != 0 ? e : encoder,
            b != 0 ? b : battery);
    }
};