#pragma once
#include "main.h"

class Pin
{
private:
    GPIO_TypeDef *port_;
    uint16_t pin_;

public:
    Pin(GPIO_TypeDef *port, uint16_t pin) 
        : port_{port}, pin_{pin} {}

    virtual void init();
    virtual void write(bool state);
};


