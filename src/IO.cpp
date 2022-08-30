#include "IO.h"

IO::IO()
{
    wiringPiSetup();
    pinMode(TIMER_START_PIN, INPUT);
}

void IO::update(Data& data)
{
    if (digitalRead(TIMER_START_PIN) == HIGH)
    {
        if (!data.timer_started) {
            data.timer_started = true;
            data.timer_start_time = Util::get_current_ms();
        }
        else {
            data.timer_start_time -= 60 * 1000;
        }

    }
    if (digitalRead(TIMER_RESET_PIN) == HIGH)
    {
        data.timer_started = false;        
    }
    if (digitalRead(TIMER_RECALL_PIN) == HIGH)
    {
        data.timer_started = true;
        data.timer_start_time += 5 * 60 * 1000;        
    }

    if (digitalRead(BOAT_PIN) == HIGH)
    {
        data.boat_pos = data.current_pos;
    }
    if (digitalRead(PIN_PIN) == HIGH)
    {
        data.pin_pos = data.current_pos;       
    }
}