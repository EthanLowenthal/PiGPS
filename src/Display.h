#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <map>

#include <cairomm/context.h>
#include <cairomm/surface.h>

#include "FrameBuffer.cpp"
#include "GPS.h"

#ifndef DISPLAY_H
#define DISPLAY_H

std::map<int, Cairo::Format> bpp_to_format {
    {32, Cairo::Format::FORMAT_ARGB32},
    {16, Cairo::Format::FORMAT_RGB16_565}
};

class Display {
    FrameBuffer fb {0};

    Cairo::RefPtr<Cairo::Context> ctx;
    Cairo::RefPtr<Cairo::ImageSurface> surface;

    std::ostringstream str_out {};
    public:
        Display();
        void put_text(double x, double y);
        void update(GPS gps);
};

#endif