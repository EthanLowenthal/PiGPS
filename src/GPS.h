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
    // https://github.com/ukyg9e5r6k7gubiekd6/gpsd/blob/master/gps.h
    public:
        gpsmm gps_rec;
        double lat {};
        double lon {};
        double speed {};
        double heading {};
        double last_update {};
        double accuracy {};

        int satellites_used {};
        int satellites_visible {};

        void update();
        GPS();
};


#endif