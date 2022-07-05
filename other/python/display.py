from math import pi
from turtle import width
import cairo
from framebuffer import FrameBuffer
import time

def deg_rad(deg):
    return deg / 180 * pi

img_formats = {
    1: cairo.Format.A1,
    8: cairo.Format.A8,
    16: cairo.Format.RGB16_565,
    24: cairo.Format.RGB24,
    30: cairo.Format.RGB30,
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


        self.ctx.select_font_face(
            "serif",
            cairo.FONT_SLANT_NORMAL,
            cairo.FONT_WEIGHT_NORMAL
        )

        self.last_update = time.time_ns()

        self.last_displayed_values = {}
        self.composite_areas = {}
        
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

    def draw_speed(self):
        speed = 6.7

        if speed == self.last_displayed_values.get('speed'):
            return
        
        self.last_displayed_values['speed'] = speed

        self.ctx.identity_matrix()
        self.ctx.set_font_size(30)
        self.ctx.translate(self.surface.get_width() / 2, 100)
        self.ctx.move_to(0,0)
        label = f'{speed} kts'
        extents = self.ctx.text_extents(label)
        self.ctx.rel_move_to(-extents.width/2,extents.height/2)
        self.ctx.show_text(label)

    def composite(label):
        extents_border = 5

        def _composite(func):
            def wrapper(self, val):
                if val == self.last_displayed_values.get(label):
                    return

                self.ctx.identity_matrix()
            
                if self.composite_areas.get(label) is not None:
                    rect = self.composite_areas[label]

                    self.ctx.move_to(rect[0]-extents_border, rect[1]-extents_border)
                    self.ctx.line_to(rect[0]-extents_border, rect[3]+extents_border)
                    self.ctx.line_to(rect[2]+extents_border, rect[3]+extents_border)
                    self.ctx.line_to(rect[2]+extents_border, rect[1]-extents_border)
                    self.ctx.close_path()

                    self.ctx.set_source_rgba(0,0,0,1)
                    self.ctx.fill()

                self.ctx.set_source_rgba(1,1,1,1)

                self.composite_areas[label] = func(self, val)

                self.last_displayed_values[label] = val
            return wrapper
        return _composite

    @composite('compass')
    def draw_compass(self, current_heading):
        strokes = []
        fills = []

        compass_arc = 360 / 8
        compass_rad = 150
        compass_pos = (self.surface.get_width()/2, 400)

        self.ctx.new_path()
        self.ctx.translate(compass_pos[0],compass_pos[1])
        self.ctx.arc_negative(0,0,compass_rad,deg_rad(compass_arc-90),deg_rad(-compass_arc-90))
        strokes.append(self.ctx.copy_path())
        self.ctx.new_path()

        self.ctx.move_to(0,0)

        self.ctx.set_font_size(30)
        label = f'{current_heading}'
        extents = self.ctx.text_extents(label)
        self.ctx.rel_move_to(-extents.width/2,-compass_rad+extents.height/2+35)
        self.ctx.text_path(label + '˚')
        fills.append(self.ctx.copy_path())

        self.ctx.set_font_size(20)

        offset = current_heading % 5
        self.ctx.rotate(deg_rad(-offset - 45))
        for true_deg in range(int(-compass_arc - offset), int(compass_arc - offset + 5), 5):
            deg = true_deg + current_heading

            self.ctx.move_to(0,0)
            self.ctx.rotate(deg_rad(5))
            self.ctx.rel_move_to(0,-compass_rad-30)

            line_length = self.get_compass_line_length(deg)
            self.ctx.rel_line_to(0, line_length)
            strokes.append(self.ctx.copy_path())

            if deg % 20 == 0:
                self.ctx.rel_move_to(0,-15)

                label = str(int(deg % 360))

                extents = self.ctx.text_extents(label)
                self.ctx.rel_move_to(-extents.width/2,-extents.height/2)
                self.ctx.text_path(label)
                fills.append(self.ctx.copy_path())

        self.ctx.identity_matrix()
        for fill in fills:
            self.ctx.append_path(fill)

        self.ctx.identity_matrix()
        fill_extents = self.ctx.fill_extents()
        self.ctx.fill()
        self.ctx.identity_matrix()

        for stroke in strokes:
            self.ctx.append_path(stroke)
        self.ctx.stroke()

        self.ctx.identity_matrix()
        stroke_extents = self.ctx.path_extents()
        self.ctx.stroke()

        extents = [
            min(stroke_extents[0], fill_extents[0]),
            min(stroke_extents[1], fill_extents[1]),
            max(stroke_extents[2], fill_extents[2]),
            max(stroke_extents[3], fill_extents[3])
        ]

        return extents

    @composite('fps')
    def fps(self, fps):
        self.ctx.set_font_size(20)

        label = f'{fps:.2f} fps'
        # self.ctx.translate(20,20)
        self.ctx.move_to(20,20)
        self.ctx.text_path(label)
        extents = self.ctx.fill_extents()
        self.ctx.fill()

        self.ctx.identity_matrix()
        return extents