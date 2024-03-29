#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <sstream>
#include <map>

#include <stdio.h>
#include <time.h>

#include <cairomm/context.h>
#include <cairomm/surface.h>

#include "FrameBuffer.cpp"
#include "GPS.h"
#include "Data.h"
#include "util.h"

#ifndef DISPLAY_H
#define DISPLAY_H

class Display
{
    FrameBuffer fb{0};

    double a{};

    int width{};
    int height{};

    Cairo::RefPtr<Cairo::Context> ctx;
    Cairo::RefPtr<Cairo::ImageSurface> surface;

    std::ostringstream str_out{};

public:
    Display();
    void put_text(double x, double y);
    void update(GPS &gps, Data& data);
    void draw_compass(double value);
    void start_screen(GPS &gps, Data& data);
    void data_screen(GPS &gps, Data& data);
    void label_bar(double divider_y, double divider_height, std::vector<std::string> labels);
};

#endif