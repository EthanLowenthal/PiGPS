import cairo
import math
from framebuffer import FrameBuffer
import time

def deg_rad(deg):
    return deg / 180 * math.pi


def round_to(num, base):
    return base * round(num/base)

def round_toi(num, base):
    return int(base * round(num/base))

img_formats = {
    16: cairo.Format.RGB16_565,
    32: cairo.Format.ARGB32
}

class Display:
    def __init__(self):

        self.framebuffer = FrameBuffer()

        img_format = img_formats[self.framebuffer.var_info.bits_per_pixel]

        self.surface = cairo.ImageSurface(
            img_format,
            self.framebuffer.var_info.xres_virtual,
            self.framebuffer.var_info.yres_virtual,
        )

        self.ctx = cairo.Context(self.surface)

        self.current_heading = 0

        self.last_update = time.time_ns()
        
    def flip(self):
        self.framebuffer.flip(self.surface)

    def clear(self):
        self.ctx.set_source_rgba(0,0,0,1)
        self.ctx.paint()

        self.ctx.set_source_rgba(1,1,1,1)

    def get_compass_line_length(self, deg):
        if deg % 45 == 0:
            return 20

        if deg % 10 == 0:
            return 15

        if deg % 5 == 0:
            return 10

        return 0

    def draw_compass(self):
        compass_arc = 360 / 8
        compass_rad = 150
        compass_pos = (400, 300)

        self.ctx.identity_matrix()
        self.ctx.new_sub_path()
        self.ctx.translate(compass_pos[0],compass_pos[1])
        self.ctx.rotate(deg_rad(-90))
        self.ctx.arc_negative(0,0,compass_rad,deg_rad(compass_arc),deg_rad(-compass_arc))
        # self.ctx.stroke()

        self.ctx.identity_matrix()
        self.ctx.translate(compass_pos[0],compass_pos[1])
        self.ctx.move_to(0,0)

        current_heading = int(self.current_heading)
        self.current_heading += 0.1
        self.current_heading %= 360

        self.ctx.set_font_size(30)
        label = f'{current_heading}˚'
        extents = self.ctx.text_extents(label)
        self.ctx.rel_move_to(-extents.width/2,-compass_rad+extents.height/2+35)
        self.ctx.show_text(label)

        self.ctx.set_font_size(20)

        offset = current_heading % 5
        for true_deg in range(int(-compass_arc - offset), int(compass_arc - offset), 5):
            deg = true_deg + current_heading

            # print(deg, true_deg, off)
            self.ctx.identity_matrix()
            self.ctx.translate(compass_pos[0],compass_pos[1])
            self.ctx.rotate(deg_rad(-90 + offset))
            self.ctx.move_to(0,0)
            self.ctx.rotate(deg_rad(true_deg))
            self.ctx.rel_move_to(compass_rad+30,0)
            self.ctx.save()

            self.ctx.rotate(math.pi/2)
            line_length = self.get_compass_line_length(deg)
            self.ctx.rel_line_to(0, line_length)

            if deg % 20 == 0:
                self.ctx.restore()
                self.ctx.rel_move_to(15,0)
                self.ctx.rotate(math.pi/2)

                label = str(int(deg % 360))

                extents = self.ctx.text_extents(label)
                self.ctx.rel_move_to(-extents.width/2,-extents.height/2)
                self.ctx.show_text(label)

        self.ctx.stroke()

    def fps(self):
        now = time.time_ns()

        self.ctx.identity_matrix()
        self.ctx.set_font_size(20)

        fps = 1e+9 / (now - self.last_update)
        label = f'{fps:.2f} fps'
        self.ctx.move_to(20,20)
        self.ctx.show_text(label)

        self.last_update = now

display = Display()


while True:
    display.clear()
    display.draw_compass()
    display.fps()
    display.flip()

# display.surface.write_to_png("surface.png")