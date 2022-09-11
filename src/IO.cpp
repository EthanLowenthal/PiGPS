#include "IO.h"

IO::IO()
{
    wiringPiSetup();
    pinMode(TIMER_START_PIN, INPUT);
}
bool IO::btn_released(int btn) {
    return digitalRead(btn) == LOW && btn_presses[btn];
}
void IO::update(Data& data)
{
    if (btn_released(TIMER_START_PIN))
    {
        if (!data.timer_started) {
            data.timer_started = true;
            data.timer_start_time = Util::get_current_ms();
        }
        else {
            data.timer_start_time -= 60 * 1000;
        }

    }
    if (btn_released(TIMER_RESET_PIN))
    {
        data.timer_started = false;        
    }
    if (btn_released(TIMER_RECALL_PIN))
    {
        data.timer_started = true;
        data.timer_start_time += 5 * 60 * 1000;        
    }

    if (btn_released(BOAT_PIN))
    {
        data.boat_pos = data.current_pos;
    }
    if (btn_released(PIN_PIN))
    {
        data.pin_pos = data.current_pos;       
    }

    btn_released.at(TIMER_START_PIN) = digitalRead(TIMER_START_PIN) == HIGH;
    btn_released.at(TIMER_RESET_PIN) = digitalRead(TIMER_RESET_PIN) == HIGH;
    btn_released.at(TIMER_RECALL_PIN) = digitalRead(TIMER_RECALL_PIN) == HIGH;
    btn_released.at(MODE_PIN) = digitalRead(MODE_PIN) == HIGH;
    btn_released.at(BOAT_PIN) = digitalRead(BOAT_PIN) == HIGH;
    btn_released.at(PIN_PIN) = digitalRead(PIN_PIN) == HIGH;

}