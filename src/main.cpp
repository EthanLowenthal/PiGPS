// g++ $(pkg-config --cflags --libs cairomm-1.0 libgps) -std=c++17 GPS.cpp FrameBuffer.cpp main.cpp -o main -lcairomm-1.0 -lgps
// sudo apt-get update
// sudo apt-get install -y libcairomm-1.0-dev libgps-dev gpsd lsof wiringpi
// https://github.com/WiringPi/WiringPi
// https://techoverflow.net/2021/10/19/how-to-hide-all-boot-text-blinking-cursor-on-raspberry-pi/

// #include <cairomm/context.h>
// #include <cairomm/surface.h>


#include "Display.h"
#include "GPS.h"
#include "Data.h"
#include "IO.h"

#define DEV_MODE 0

int main()
{
  // FrameBuffer fb {0};

  // FORMAT_ARGB32
  // auto surface = Cairo::ImageSurface::create( (unsigned char*) fb.buff, Cairo::Format::FORMAT_RGB16_565,
  // 	fb.fb_info.var.xres, fb.fb_info.var.yres, fb.fb_info.fix.line_length);
  // auto cr = Cairo::Context::create(surface);

  // clock_t current_ticks, delta_ticks;
  // clock_t fps = 0;

  GPS gps{};
  Display display{};
  Data data{};
  IO io{};
  while (true)
  {
    // current_ticks = clock();
    io.update(data);
    gps.update(data);
    display.update(gps, data);
  }
}