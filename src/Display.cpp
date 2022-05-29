#include <cairomm/context.h>
#include <cairomm/surface.h>

#include "FrameBuffer.cpp"

class Display {
    FrameBuffer fb {0};
    
    Cairo::RefPtr<Cairo::Context> ctx;
    Cairo::RefPtr<Cairo::ImageSurface> surface;

    public:
        Display() {
            surface = Cairo::ImageSurface::create( (unsigned char*) fb.buff, Cairo::Format::FORMAT_RGB16_565, 
                fb.fb_info.var.xres, fb.fb_info.var.yres, fb.fb_info.fix.line_length); 

            ctx = Cairo::Context::create(surface);
        };

        void update() {
            ctx->rectangle(0,0,surface->get_width(),surface->get_height());
            ctx->set_source_rgb(0, 1, 0);
            ctx->fill();

            fb.flip();
        }
};