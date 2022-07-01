#include "Display.h"

const double PI = 3.14159;

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
    ctx->set_identity_matrix();

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

    ctx->set_identity_matrix();
    ctx->move_to(surface->get_width()-100, 100);
    a += 0.005;
    ctx->rotate(a);

    // ctx->rectangle(surface->get_width()-100, 100, 80, 80);


    // cairo_line_to(cr, surface->get_width()-100, 100);
    // cairo_line_to(cr, 350, 160);
    // cairo_close_path(cr);

    // cairo_stroke_preserve(cr);
    // cairo_fill(cr);

    str_out << "->";
    ctx->show_text(str_out.str());  
    str_out.str("");
    str_out.clear();

    draw_compass(a * 10);
    // put_text(surface->get_width()-100, line_height * 4);

    // delta_ticks = clock() - current_ticks; 
    // if(delta_ticks > 0)
    //     fps = CLOCKS_PER_SEC / delta_ticks;

    // ctx->move_to(surface->get_width()-100, 30);
    // ctx->show_text(std::to_string(fps));  
    
    fb.flip();
};

int get_compass_line_length(int deg) {
    if (deg % 45 == 0) return 20;
    if (deg % 10 == 0) return 15;
    if (deg % 5 == 0) return 10;
    return 0;
}

double deg_rad(double deg) {
    return deg / 180 * PI;
}
double set_precision(double val, int precision) {
    int v = pow(10, precision);
    return (double)((int) val + (int) (val * v) % v / v);
}
void Display::draw_compass(double value) {
        ctx->set_identity_matrix();

        ctx->set_source_rgb(1,1,1);

        double compass_arc = 360 / 8;
        double compass_rad = 150;

        ctx->begin_new_path();
        ctx->translate(surface->get_width()/2,surface->get_height()/2); // position
        ctx->arc_negative(0,0,compass_rad,deg_rad(compass_arc-90),deg_rad(-compass_arc-90));
        ctx->stroke();

        ctx->move_to(0,0);

        ctx->set_font_size(30);
        
        std::string label = std::to_string(set_precision(value, 1));
        Cairo::TextExtents extents;
        ctx->get_text_extents(label, extents);
        ctx->rel_move_to(-extents.width/2,-compass_rad+extents.height/2+35);
        ctx->text_path(label);
        ctx->fill_preserve();

        ctx->set_font_size(20);

        double offset = (int)value % 5;
        ctx->rotate(deg_rad(-offset - 45));
        for (double true_deg = -compass_arc - offset; true_deg < compass_arc - offset + 5; true_deg += 5) {
            double deg = true_deg + value;

            ctx->move_to(0,0);
            ctx->rotate(deg_rad(5));
            ctx->rel_move_to(0,-compass_rad-30);

            int line_length = get_compass_line_length((int) deg);
            ctx->rel_line_to(0, line_length);
            ctx->stroke_preserve();

            if ((int)deg % 20 == 0) {
                ctx->rel_move_to(0,-15);

                std::string label = std::to_string((int)deg % 360);

                ctx->get_text_extents(label, extents);
                ctx->rel_move_to(-extents.width/2,-extents.height/2);
                ctx->text_path(label);
            }
                
            ctx->fill();
        }            
}