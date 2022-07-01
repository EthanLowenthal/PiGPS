#ifndef DATA_H
#define DATA_H

class Data {
    public:
        double pin_lat {};
        double pin_lon {};

        double boat_lat {};
        double boat_lon {};

        double wind_dir {};

        double time_to_start {};

        Data();
};

#endif