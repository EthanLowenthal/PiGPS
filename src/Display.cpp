#include "Display.h"

const double PI = 3.14159;

std::map<int, Cairo::Format> bpp_to_format {
    {32, Cairo::Format::FORMAT_ARGB32},
    {16, Cairo::Format::FORMAT_RGB16_565}
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
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

Display::Display() {
    surface = Cairo::ImageSurface::create( (unsigned char*) fb.buff, bpp_to_format[fb.fb_info.var.bits_per_pixel], 
        fb.fb_info.var.xres, fb.fb_info.var.yres, fb.fb_info.fix.line_length); 

    ctx = Cairo::Context::create(surface);

    width = surface->get_width();
    height = surface->get_height();

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

    start_screen(gps);

    // str_out << "Latitude: " << gps.lat;
    // put_text(20, line_height);

    // str_out << "Longitude: " << gps.lon;
    // put_text(20, line_height * 2);

    // str_out << "Speed: " << gps.speed << " kts";
    // put_text(20, line_height * 3);

    // str_out << "CMG: " << gps.heading;
    // put_text(20, line_height * 4);

    // ctx->set_identity_matrix();
    // a += 0.005;
    // draw_compass(a * 10);
    
    fb.flip();
};

void Display::draw_compass(double value) {
        ctx->set_identity_matrix();

        ctx->set_source_rgb(1,1,1);

        int compass_arc = 360 / 8;
        int compass_rad = 150;

        ctx->begin_new_path();
        ctx->translate(width/2,height/2); // position
        ctx->arc_negative(0,0,compass_rad,deg_rad(compass_arc-90),deg_rad(-compass_arc-90));
        ctx->stroke();

        ctx->move_to(0,0);

        ctx->set_font_size(30);
        
        std::string label = to_string_with_precision(value, 1);
        Cairo::TextExtents extents;
        ctx->get_text_extents(label, extents);
        ctx->rel_move_to(-extents.width/2,-compass_rad+extents.height/2+35);
        ctx->text_path(label);
        ctx->fill_preserve();

        ctx->set_font_size(20);

        int offset = (int) value % 5;
        ctx->rotate(deg_rad(-offset - 45));
        for (int true_deg = -compass_arc - offset; true_deg <= compass_arc - offset; true_deg += 5) {
            int deg = true_deg + (int) value + 5;

            ctx->move_to(0,0);
            ctx->rotate(deg_rad(5));
            ctx->rel_move_to(0,-compass_rad-30);

            int line_length = get_compass_line_length(deg);
            ctx->rel_line_to(0, line_length);
            ctx->stroke_preserve();

            if (deg % 20 == 0) {
                ctx->rel_move_to(0,-15);

                std::string label = std::to_string(deg % 360);

                ctx->get_text_extents(label, extents);
                ctx->rel_move_to(-extents.width/2,-extents.height/2);
                ctx->text_path(label);
            }
                
            ctx->fill();
        }            
}

void Display::start_screen(GPS gps) {
    double divider_height = height * 0.75;
    ctx->move_to(0,divider_height);
    ctx->line_to(width,divider_height);

    int divisions = 3;
    double cell_width = 1.0/divisions;

    for (int i=1;i<divisions;i++) {
        ctx->move_to(i * cell_width,divider_height);
        ctx->line_to(i * cell_width,0);
    }

    ctx->stroke();

    ctx->move_to(cell_width * 0.5, divider_height * 0.5);

    std::string label = to_string_with_precision(gps.speed, 1) + " kts";
    Cairo::TextExtents extents;
    ctx->get_text_extents(label, extents);
    ctx->rel_move_to(-extents.width/2,extents.height/2);
    ctx->text_path(label);
    ctx->fill();


    ctx->move_to(cell_width * 1.5, divider_height * 0.5);

    std::string label = "time here";
    Cairo::TextExtents extents;
    ctx->get_text_extents(label, extents);
    ctx->rel_move_to(-extents.width/2,extents.height/2);
    ctx->text_path(label);
    ctx->fill();



}