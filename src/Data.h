#include "util.h"

#ifndef DATA_H
#define DATA_H

struct Data {
    Position pin_pos {};
    Position boat_pos {};
    Position current_pos {};
    
    double speed {};
    double heading {};

    bool timer_started {false};
    double timer_start_time {};

    Data();
};

#endif