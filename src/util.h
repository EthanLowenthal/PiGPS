#include <chrono>
#include "Vec2.h"
#ifndef UTIL_H
#define UTIL_H

using Position = vec2d;

struct Util
{

    static const double DEG_RAD = M_PI / 180;

    static double get_current_ms()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
    }
    static double lat_lon_bearing(Position p1, Position p2)
    {
        p1 *= DEG_RAD;
        p2 *= DEG_RAD;

        double x = cos(p2.latitude()) * sin(p2.longitude() - p1.longitude());
        double y = cos(p1.latitude()) * sin(p2.latitude()) - sin(p1.latitude()) * cos(p2.latitude()) * cos(p2.longitude() - p1.longitude());
        return atan2(x, y) / DEG_RAD;
    }

    static double lat_lon_dist(Position p1, Position p2)
    {
        const int RADIUS = 6371000;
        
        p1 *= DEG_RAD;
        p2 *= DEG_RAD;

        Position dp = p2 - p1;

        double a = sin(dp.latitude() / 2) * sin(dp.latitude() / 2) + cos(p1.latitude()) * cos(p1.latitude()) * sin(dp.longitude() / 2) * sin(dp.longitude() / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));

        return c * RADIUS;
    }
};

#endif