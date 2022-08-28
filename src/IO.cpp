#include "IO.h"

IO::IO()
{
    wiringPiSetup();
    pinMode(TIMER_START_PIN, INPUT);
}

IO::update(Data& data)
{
    if (!data.timer_started)
    {
        data.timer_started = true;
        data.timer_start_time = get_current_ms();
    }
}