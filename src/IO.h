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
    int TIMER_START_PIN = 0;
    int TIMER_RESET_PIN = 1;
    int TIMER_RECALL_PIN = 2;
    int MODE_PIN = 3;
    int BOAT_PIN = 4;
    int PIN_PIN = 5;


};

#endif