#include <libgpsmm.h>
#include <stdlib.h>

#include <ctime>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <time.h>

#ifndef GPS_H
#define GPS_H

class GPS {
    public:
        gpsmm gps_rec;
        double lat {};
        double lon {};
        double speed {};
        double heading {};
        double last_update {};

        void update();
        void GPS();
};


#endif