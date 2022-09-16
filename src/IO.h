#ifdef RPI
#include <wiringPi.h>
#endif


#include "Data.h"
#include "util.h"
#include <vector>

#ifndef IO_H
#define IO_H

class IO
{
public:
    IO();
    void update(Data& data);
    bool btn_pressed(int btn);
    bool btn_released(int btn);

private:
    int TIMER_START_PIN = 0;
    int TIMER_RESET_PIN = 1;
    int TIMER_RECALL_PIN = 2;
    int MODE_PIN = 3;
    int BOAT_PIN = 4;
    int PIN_PIN = 5;

    std::vector<int> pins {
        TIMER_START_PIN,
        TIMER_RESET_PIN,
        TIMER_RECALL_PIN,
        MODE_PIN,
        BOAT_PIN,
        PIN_PIN
    };
    
    std::vector<bool> btn_presses {false, false, false, false, false, false};


};

#endif