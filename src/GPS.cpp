#include "GPS.h"

const double PI = 3.14159265358979323846;

const double MPS_TO_KTS = 1.94384;
const double MPS_MPH = 2.23693;

double toRad(double degree)
{
    return degree / 180 * PI;
}

double latLonDist(double lat1, double long1, double lat2, double long2)
{
    double dist;
    dist = sin(toRad(lat1)) * sin(toRad(lat2)) + cos(toRad(lat1)) * cos(toRad(lat2)) * cos(toRad(long1 - long2));
    dist = acos(dist);
    //        dist = (6371 * pi * dist) / 180;
    // got dist in radian, no need to change back to degree and convert to rad again.
    dist = 6371 * dist;
    return dist;
}

void GPS::update()
{
    if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == nullptr)
    {
        return;
    }

    struct gps_data_t *gpsd_data;

    if (((gpsd_data = gps_rec.read()) == nullptr) || (gpsd_data->fix.mode < MODE_2D))
    {
        return;
    }

    speed = gpsd_data->fix.speed * MPS_TO_KTS;
    heading = gpsd_data->fix.magnetic_track; // track is cmg

    accuracy = sqrt(gpsd_data->fix.epy * gpsd_data->fix.epy + gpsd_data->fix.epx * gpsd_data->fix.epx);

    satellites_used = gpsd_data->satellites_used;
    satellites_visible = gpsd_data->satellites_visible;

    double eps = gpsd_data->fix.eps;
    timespec time = gpsd_data->fix.time;

    double new_lat = gpsd_data->fix.latitude;
    double new_lon = gpsd_data->fix.longitude;

    double current_time = gpsd_data->fix.time.tv_sec;
    double dt = current_time - last_update;

    double dist_traveled = latLonDist(lat, lon, new_lat, new_lon);
    double estimated_speed = dist_traveled / dt * 2.237e+12;

    last_update = current_time;
    lat = new_lat;
    lon = new_lon;

    // const auto hdop{gpsd_data->dop.hdop};
    // const auto vdop{gpsd_data->dop.vdop};
    // const auto pdop{gpsd_data->dop.pdop};
    // const auto s_vis{gpsd_data->satellites_visible};
    // const auto s_used{gpsd_data->satellites_used};

    // std::cout << std::setprecision(8) << std::fixed;  // set output to fixed floating point, 8 decimal precision
    // std::cout << lat << ", " << lon << ", " << speed << " +- " << eps << ", " << dist_traveled << "," << gpsd_data->fix.time.tv_nsec << std::endl;
}

GPS::GPS() : gps_rec("localhost", DEFAULT_GPSD_PORT)
{
    // system("~/PiGPS/start_gpsd.sh");

    if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == nullptr)
    {
        std::cerr << "No GPSD running.\n";
    }
}