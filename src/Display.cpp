#include <iostream>
#include <string>
#include <ctime>
#include <sstream>

#include <cairomm/context.h>
#include <cairomm/surface.h>

#include "FrameBuffer.cpp"
#include "GPS"

class Display {
    FrameBuffer fb {0};

    Cairo::RefPtr<Cairo::Context> ctx;
    Cairo::RefPtr<Cairo::ImageSurface> surface;

    std::ostringstream str_out {};

    void display_text() {
        
    }

    public:
        Display() {
            surface = Cairo::ImageSurface::create( (unsigned char*) fb.buff, Cairo::Format::FORMAT_RGB16_565, 
                fb.fb_info.var.xres, fb.fb_info.var.yres, fb.fb_info.fix.line_length); 

            ctx = Cairo::Context::create(surface);

            str_out << std::setprecision(2);
        };

        template<typename T>
        Display& operator<<(Display& d, const T& obj)
        {
            str_out << obj;
            std::cout << obj << std::endl;
            return d;
        }

        void update(GPS gps) {
            ctx->rectangle(0,0,surface->get_width(),surface->get_height());
            ctx->set_source_rgb(0, 0, 0);
            ctx->fill();

            ctx->select_font_face("serif",
                Cairo::FontSlant::FONT_SLANT_NORMAL,
                Cairo::FontWeight::FONT_WEIGHT_BOLD);

            ctx->set_font_size(40);

            ctx->set_source_rgb(1, 1, 1);

            ctx->move_to(20, 40);
            

            this << "Latitude: " << gps.lat << End;
            // ctx->show_text(str_out.str());  
            // str_out.str("");
            // str_out.clear();

            ctx->move_to(20, 100);
            str_out << "Longitude: " << gps.lon;
            ctx->show_text(str_out.str());  
            ctx->move_to(20, 170);
            ctx->show_text("Speed: "+std::to_string(gps.speed)+" kts");  
            ctx->move_to(20, 240);
            ctx->show_text("CMG: "+std::to_string(gps.heading));

            // delta_ticks = clock() - current_ticks; 
            // if(delta_ticks > 0)
            //     fps = CLOCKS_PER_SEC / delta_ticks;

            // ctx->move_to(surface->get_width()-100, 30);
            // ctx->show_text(std::to_string(fps));  
            
            fb.flip();
        }
};