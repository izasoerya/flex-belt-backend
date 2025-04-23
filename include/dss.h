#pragma once

#include <Arduino.h>
#include "payload.h"

class DSS
{
private:
    Payload payload;

public:
    DSS(Payload payload) : payload(payload) {}

    String generateDescription()
    {
        // TODO: Implement the logic to generate a description based on the payload data
        return "fill this later!";
    }

    String generateStatus()
    {
        // TODO: Implement the logic to generate a status based on the payload data
        return "safe / caution / danger";
    }
};