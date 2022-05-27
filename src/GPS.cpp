#include <libgpsmm.h>
#include <stdlib.h>

#include <ctime>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <time.h>

const double PI = 3.14159265358979323846;

const double MPS_TO_KTS = 1.94384;
const double MPS_MPH = 2.23693;

double toRad(double degree) {
    return degree/180 * PI;
}

double calculateDistance(double lat1, double long1, double lat2, double long2) {
    double dist;
    dist = sin(toRad(lat1)) * sin(toRad(lat2)) + cos(toRad(lat1)) * cos(toRad(lat2)) * cos(toRad(long1 - long2));
    dist = acos(dist);
//        dist = (6371 * pi * dist) / 180;
    //got dist in radian, no need to change back to degree and convert to rad again.
    dist = 6371 * dist;
    return dist;
}

class GPS {
    public:
    gpsmm gps_rec;
    double lat {};
    double lon {};
    double speed {};
    double heading {};
    double last_update {};
    GPS() : gps_rec("localhost", DEFAULT_GPSD_PORT) {
        // system("~/PiGPS/start_gpsd.sh");

        if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == nullptr) {
            std::cerr << "No GPSD running.\n";
        }
    }
    void update() {
        struct gps_data_t* gpsd_data;

        if (((gpsd_data = gps_rec.read()) == nullptr) || (gpsd_data->fix.mode < MODE_2D)) {
            return;
        }

        
        speed = gpsd_data->fix.speed * MPS_MPH;
        heading = gpsd_data->fix.track;
        double eps = gpsd_data->fix.eps;
        timespec time = gpsd_data->fix.time;

        double new_lat = gpsd_data->fix.latitude;
        double new_lon = gpsd_data->fix.longitude;

        double current_time = gpsd_data->fix.time.tv_nsec;
        double dt = last_update - current_time;

        double dist_traveled = calculateDistance(lat, lon, new_lat, new_lon);
        double estimated_speed = dist_traveled / dt * 2.237e+12;

        last_update = current_time;
        lat = new_lat;
        lon = new_lon;

        // const auto hdop{gpsd_data->dop.hdop};
        // const auto vdop{gpsd_data->dop.vdop};
        // const auto pdop{gpsd_data->dop.pdop};
        // const auto s_vis{gpsd_data->satellites_visible};
        // const auto s_used{gpsd_data->satellites_used};
        
        std::cout << std::setprecision(8) << std::fixed;  // set output to fixed floating point, 8 decimal precision
        std::cout << lat << ", " << lon << ", " << speed << " +- " << eps << ", " << estimated_speed << std::endl;
    }
};