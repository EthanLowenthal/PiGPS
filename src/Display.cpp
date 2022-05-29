#include "Display.h"

std::map<int, Cairo::Format> bpp_to_format {
    {32, Cairo::Format::FORMAT_ARGB32},
    {16, Cairo::Format::FORMAT_RGB16_565}
};

Display::Display() {
    surface = Cairo::ImageSurface::create( (unsigned char*) fb.buff, bpp_to_format[fb.fb_info.var.bits_per_pixel], 
        fb.fb_info.var.xres, fb.fb_info.var.yres, fb.fb_info.fix.line_length); 

    ctx = Cairo::Context::create(surface);

    str_out << std::setprecision(2);
};

void Display::put_text(double x, double y) {
    ctx->move_to(x, y);
    ctx->show_text(str_out.str());  
    str_out.str("");
    str_out.clear();
};

void Display::update(GPS gps) {
    ctx->rectangle(0,0,surface->get_width(),surface->get_height());
    ctx->set_source_rgb(0, 0, 0);
    ctx->fill();

    ctx->select_font_face("serif",
        Cairo::FontSlant::FONT_SLANT_NORMAL,
        Cairo::FontWeight::FONT_WEIGHT_BOLD);

    double font_size = 40;
    double line_height = font_size + 10;
    ctx->set_font_size(font_size);

    ctx->set_source_rgb(1, 1, 1);

    str_out << "Latitude: " << gps.lat;
    put_text(20, line_height);

    str_out << "Longitude: " << gps.lon;
    put_text(20, line_height * 2);

    str_out << "Speed: " << gps.speed << " kts";
    put_text(20, line_height * 3);

    str_out << "CMG: " << gps.heading;
    put_text(20, line_height * 4);

    ctx->move_to(surface->get_width()-100, 100);
    ctx->rotate(45);
    str_out << "->";
    ctx->show_text(str_out.str());  
    str_out.str("");
    str_out.clear();
    // put_text(surface->get_width()-100, line_height * 4);

    // delta_ticks = clock() - current_ticks; 
    // if(delta_ticks > 0)
    //     fps = CLOCKS_PER_SEC / delta_ticks;

    // ctx->move_to(surface->get_width()-100, 30);
    // ctx->show_text(std::to_string(fps));  
    
    fb.flip();
};

