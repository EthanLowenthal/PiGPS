// g++ $(pkg-config --cflags --libs cairomm-1.0 libgps) -std=c++17 GPS.cpp FrameBuffer.cpp main.cpp -o main -lcairomm-1.0 -lgps
// sudo apt-get install libcairomm-1.0-dev libgps-dev
// https://techoverflow.net/2021/10/19/how-to-hide-all-boot-text-blinking-cursor-on-raspberry-pi/

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>

#include <cairomm/context.h>
#include <cairomm/surface.h>

#include "Display.cpp"
#include "GPS.cpp"

int main() {
  // FrameBuffer fb {0};

 // FORMAT_ARGB32
  auto surface = Cairo::ImageSurface::create( (unsigned char*) fb.buff, Cairo::Format::FORMAT_RGB16_565, 
		fb.fb_info.var.xres, fb.fb_info.var.yres, fb.fb_info.fix.line_length); 
  auto cr = Cairo::Context::create(surface);

  clock_t current_ticks, delta_ticks;
  clock_t fps = 0;

  GPS gps {};
  Display display {};

  while (true) {
    current_ticks = clock();
    
    gps.update();

    // cr->rectangle(0,0,surface->get_width(),surface->get_height());
    // cr->set_source_rgb(0, 0, 0);
    // cr->fill();

    // cr->select_font_face("serif",
    //     Cairo::FontSlant::FONT_SLANT_NORMAL,
    //     Cairo::FontWeight::FONT_WEIGHT_BOLD);

    // cr->set_font_size(40);

    // cr->set_source_rgb(1, 1, 1);

    // cr->move_to(20, 40);

    // std::ostringstream str_out;
    // str_out << std::setprecision(2);

    // str_out << "Latitude: " << gps.lat;
    // cr->show_text(str_out.str());  
    // str_out.str("");
    // str_out.clear();

    // cr->move_to(20, 100);
    // str_out << "Longitude: " << gps.lon;
    // cr->show_text(str_out.str());  
    // cr->move_to(20, 170);
    // cr->show_text("Speed: "+std::to_string(gps.speed)+" kts");  
    // cr->move_to(20, 240);
    // cr->show_text("CMG: "+std::to_string(gps.heading));

    // delta_ticks = clock() - current_ticks; 
    // if(delta_ticks > 0)
    //     fps = CLOCKS_PER_SEC / delta_ticks;

    // cr->move_to(surface->get_width()-100, 30);
    // cr->show_text(std::to_string(fps));  
    // fb.flip();
    display.update();
  }
}