import cairo
import math

def deg_rad(deg):
    return deg / 180 * math.pi

class FrameBuffer:
    def __init__(self):
        self.file = open('/dev/fb0', 'wb')
    
    def flip(self, surface):
        self.file.seek(0)
        self.file.write(surface.get_data())

class Display:
    def __init__(self):
        self.surface = cairo.ImageSurface(cairo.Format.RGB16_565, 1920, 1080)

        # print(self.surface.get_data().tobytes())
        self.ctx = cairo.Context(self.surface)
        

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

        self.ctx.new_sub_path()
        self.ctx.translate(compass_pos[0],compass_pos[1])
        self.ctx.rotate(deg_rad(-90))
        self.ctx.arc_negative(0,0,compass_rad,deg_rad(compass_arc),deg_rad(-compass_arc))
        self.ctx.stroke()

        self.ctx.identity_matrix()
        self.ctx.translate(compass_pos[0],compass_pos[1])
        self.ctx.move_to(0,0)

        current_heading = 11

        self.ctx.set_font_size(30)
        label = f'{current_heading}˚'
        extents = self.ctx.text_extents(label)
        self.ctx.rel_move_to(-extents.width/2,-compass_rad+extents.height/2+35)
        self.ctx.show_text(label)

        self.ctx.set_font_size(20)

        for true_deg in range(360):
            deg = true_deg

            if true_deg > compass_arc and compass_arc < 360 - true_deg:
                continue
            
            deg += current_heading

            
            self.ctx.identity_matrix()
            self.ctx.translate(compass_pos[0],compass_pos[1])
            self.ctx.rotate(deg_rad(-90))
            self.ctx.move_to(0,0)
            self.ctx.rotate(deg_rad(true_deg))
            self.ctx.rel_move_to(compass_rad+30,0)
            self.ctx.save()

            self.ctx.rotate(math.pi/2)
            line_length = self.get_compass_line_length(deg)
            self.ctx.rel_line_to(0, line_length)
            self.ctx.stroke_preserve()

            if deg % 20 == 0:
                self.ctx.restore()
                self.ctx.rel_move_to(15,0)
                self.ctx.rotate(math.pi/2)

                label = str(deg % 360) 

                extents = self.ctx.text_extents(label)
                self.ctx.rel_move_to(-extents.width/2,-extents.height/2)
                self.ctx.show_text(label)


display = Display()

framebuffer = FrameBuffer()
display.clear()
display.draw_compass()

while True:
    framebuffer.flip(display.surface)

# display.surface.write_to_png("surface.png")