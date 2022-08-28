#include <wiringPi.h>
#include "Data.h"
#include "util.h"

#ifndef IO_H
#define IO_H

class IO
{
public:
    IO();
    void update(Data& data);

private:
    int TIMER_START_PIN = 8;
};

#endif