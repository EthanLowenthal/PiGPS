#ifndef DATA_H
#define DATA_H

struct Data {
    double pin_lat {};
    double pin_lon {};

    double boat_lat {};
    double boat_lon {};

    double wind_dir {};

    bool timer_started {false};
    double timer_start_time {};

    Data();
};

#endif