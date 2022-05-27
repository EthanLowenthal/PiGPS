#include <libgpsmm.h>
#include <stdlib.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

const double MPS_TO_KTS = 1.94384;
const double MPS_MPH = 2.23693;

class GPS {
    public:
    gpsmm gps_rec;
    double lat {};
    double lon {};
    double speed {};
    double heading {};
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

        lat = gpsd_data->fix.latitude;
        lon = gpsd_data->fix.longitude;
        speed = gpsd_data->fix.speed * MPS_MPH;
        heading = gpsd_data->fix.track;

        // const auto hdop{gpsd_data->dop.hdop};
        // const auto vdop{gpsd_data->dop.vdop};
        // const auto pdop{gpsd_data->dop.pdop};
        // const auto s_vis{gpsd_data->satellites_visible};
        // const auto s_used{gpsd_data->satellites_used};
        
        std::cout << std::setprecision(8) << std::fixed;  // set output to fixed floating point, 8 decimal precision
        std::cout << lat << ", " << lon << ", " << speed << " +- " << gpsd_data->fix.eps << ", " << gpsd_data->fix.time << std::endl;
    }
};