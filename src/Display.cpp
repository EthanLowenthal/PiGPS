#include "Display.h"

const double DEG_RAD = M_PI / 180;

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

double lat_lon_bearing(double la1, double lo1, double la2, double lo2) {
    double lat1 = la1 * DEG_RAD;
    double lon1 = lo1 * DEG_RAD;
    double lat2 = la2 * DEG_RAD;
    double lon2 = lo2 * DEG_RAD;

    double x = cos(lat2) * sin(lon2-lon1);
    double y = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lon2-lon1);
    return atan2(x, y) / DEG_RAD;
}

double lat_lon_dist(double la1, double lo1, double la2, double lo2) {
    const int RADIUS = 6371000;

    double lat1 = la1 * DEG_RAD;
    double lon1 = lo1 * DEG_RAD;
    double lat2 = la2 * DEG_RAD;
    double lon2 = lo2 * DEG_RAD;

    double d_lat = (lat2-lat1);
    double d_lon = (lon2-lon1);

    double a = sin(d_lat / 2) * sin(d_lat / 2) + cos(lat1) * cos(lat1) * sin(d_lon / 2) * sin(d_lon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return c * RADIUS;
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

const std::string currentDateTime() {
    setenv("TZ", "PST8PDT", 1);
    tzset();

    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%a, %b %e, %l:%M:%S %p", &tstruct);

    return buf;
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

void Display::update(GPS& gps) {
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


    // top_bar(gps);

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
    ctx->arc_negative(0,0,compass_rad,DEG_RAD * (compass_arc-90),DEG_RAD * (-compass_arc-90));
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
    ctx->rotate(DEG_RAD * (-offset - 45));
    for (int true_deg = -compass_arc - offset; true_deg <= compass_arc - offset; true_deg += 5) {
        int deg = true_deg + (int) value + 5;

        ctx->move_to(0,0);
        ctx->rotate(DEG_RAD * 5);
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

void Display::label_bar(double divider_y, double divider_height, std::vector<std::string> labels) {


   
    // double divider_height = height * 0.1;
    double cell_width = (double)width/labels.size();
    double text_width = cell_width * 0.75;
    double text_height = divider_height * 0.5;

    ctx->move_to(0,divider_y);
    ctx->line_to(width,divider_y);

    ctx->move_to(0,divider_y + divider_height);
    ctx->line_to(width,divider_y + divider_height);

    Cairo::TextExtents extents;

    double font_size = 100;
    for (int i=0;i<labels.size();i++) {
        auto label = labels.at(i);

        ctx->set_font_size(font_size);
        ctx->get_text_extents(label, extents);
        double new_font_size = font_size / extents.width * text_width;
        if (new_font_size < font_size) font_size = new_font_size;
    }

    for (int i=1;i<labels.size();i++) {
        ctx->move_to(i * cell_width,divider_y + divider_height);
        ctx->line_to(i * cell_width,divider_y);
    }
    ctx->stroke();

    ctx->set_font_size(font_size);

    for (int i=0;i<labels.size();i++) {
        auto label = labels.at(i);

        ctx->get_text_extents(label, extents);
        ctx->move_to(cell_width * i + (cell_width - extents.width) * 0.5, divider_y + text_height);
        ctx->text_path(label);
    }

    ctx->fill();
}

void Display::start_screen(GPS& gps) {

    // TOP INFO

    label_bar(0, height * 0.1, {
        "Sats: " + std::to_string(gps.satellites_used) + "/" + std::to_string(gps.satellites_visible),
        "Accuracy: " + to_string_with_precision(gps.accuracy, 2) + "m",
        currentDateTime(),
    });

    // BOTTOM INFO

    label_bar(height * 0.75, height * 0.25, {
        to_string_with_precision(gps.speed, 1) + " kts",
        "5:00",
        "+0:03 | 1m",
    });


    // START LINE DISPLAY

    double pin_pos = width * 0.15;
    double boat_pos = width * 0.85;
    double line_height = height * 0.225;
    double buoy_size = 40;

    ctx->move_to(pin_pos, line_height);
    ctx->arc(
        pin_pos, 
        line_height,
        buoy_size/2,
        0,
        2 * M_PI
    );

    bool has_pos_data = false;

    double blink = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count() / 250.0;

    // if (data.pin_lat == 0 || data.pin_lon == 0) {
    if (true) {
        double b = (sin(blink) + 1) / 2.0;
        ctx->set_source_rgb(b, b, b);
    } else {
        ctx->set_source_rgb(1, 1, 1);
        has_pos_data = true;
    }
    ctx->fill();
    
    ctx->rectangle(
        boat_pos - buoy_size/2,
        line_height - buoy_size/2,
        buoy_size, 
        buoy_size
    );

    // if (data.boat_lat == 0 || data.boat_lon == 0) {
    if (true) {
        double b = (sin(blink) + 1) / 2.0;
        ctx->set_source_rgb(b, b, b);
    } else {
        ctx->set_source_rgb(1, 1, 1);
        has_pos_data = true;
    }
    ctx->fill();
    
    ctx->set_source_rgb(1, 1, 1);


    ctx->move_to(pin_pos, line_height);
    ctx->line_to(boat_pos, line_height);
    std::vector<double> dashes = {14.0, 6.0};

    ctx->set_dash(dashes, 1);
    ctx->stroke();
    ctx->unset_dash();


    // WIND ARROW
    // if (data.wind_dir == 0) {
    //     double b = (sin(blink) + 1) / 2.0;
    //     ctx->set_source_rgb(b, b, b);
    // } else {
    //     ctx->set_source_rgb(1, 1, 1);
    // }

    // double arrow_size = 20;

    // ctx->move_to(width * 0.5, height * 0.12);
    // ctx->rotate(data.wind_dir); // TODO make relative to line
    // ctx->rel_line_to(0, arrow_size * 2);
    // ctx->stroke_preserve();
    // ctx->rel_line_to(-arrow_size, -arrow_size);
    // ctx->rel_line_to(arrow_size * 2, 0);
    // ctx->rel_line_to(-arrow_size, arrow_size);

    // ctx->fill();
    ctx->set_source_rgb(1, 1, 1);

    // TODO draw player
    // if (has_pos_data) {
    // 
    // }

}