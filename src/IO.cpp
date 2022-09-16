#include "IO.h"

IO::IO()
{
    #ifdef RPI
    wiringPiSetup();
    pinMode(TIMER_START_PIN, INPUT);
    #endif
}
bool IO::btn_released(int btn) {
    #ifdef RPI
    return digitalRead(btn) == LOW && btn_presses[btn];
    #else
    return false;
    #endif
}
bool IO::btn_pressed(int btn) {
    #ifdef RPI
    return digitalRead(btn) == HIGH && !btn_presses[btn];
    #else
    return false;
    #endif
}
void IO::update(Data& data)
{
    if (btn_pressed(TIMER_START_PIN))
    {
        // if (!data.timer_started) {
        data.timer_started = true;
        data.timer_start_time = Util::get_current_ms();
        // }
        // else {
        //     data.timer_start_time -= 60 * 1000;
        // }

    }
    if (btn_pressed(TIMER_RESET_PIN)) data.timer_started = false;

    if (btn_pressed(TIMER_RECALL_PIN))
    {
        double current_time = Util::get_current_ms();
        int ms_passed = current_time - data.timer_start_time;
        ms_passed %= 60 * 1000;
        double ms_remaining = 60 * 1000 - ms_passed;
        data.timer_start_time -= ms_remaining;
    }

    if (btn_pressed(BOAT_PIN)) data.boat_pos = data.current_pos;
    if (btn_pressed(PIN_PIN)) data.pin_pos = data.current_pos;      

    #ifdef RPI
    for (int pin:pins) {
         btn_presses.at(pin) = digitalRead(pin) == HIGH;
    }
    #endif
}


