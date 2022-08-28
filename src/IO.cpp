#include "IO.h"

IO::IO()
{
    wiringPiSetup();
    pinMode(TIMER_START_PIN, INPUT);
}

void IO::update(Data& data)
{
    if (digitalRead(TIMER_START_PIN) && !data.timer_started)
    {
        data.timer_started = true;
        data.timer_start_time = Util::get_current_ms();
    }
}