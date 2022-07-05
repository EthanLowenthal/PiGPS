from fcntl import ioctl
import ctypes
import os

FBIOGET_VSCREENINFO = 0x4600
FBIOGET_FSCREENINFO = 0x4602

class FbFid(int):
    pass


class FixScreenInfo(ctypes.Structure):
    _fields_ = [
        ('id_name', ctypes.c_char * 16),
        ('smem_start', ctypes.c_ulong),
        ('smem_len', ctypes.c_uint32),
        ('type', ctypes.c_uint32),
        ('type_aux', ctypes.c_uint32),
        ('visual', ctypes.c_uint32),
        ('xpanstep', ctypes.c_uint16),
        ('ypanstep', ctypes.c_uint16),
        ('ywrapstep', ctypes.c_uint16),
        ('line_length', ctypes.c_uint32),
        ('mmio_start', ctypes.c_ulong),
        ('mmio_len', ctypes.c_uint32),
        ('accel', ctypes.c_uint32),
        ('reserved', ctypes.c_uint16 * 3),
    ]


class FbBitField(ctypes.Structure):
    _fields_ = [
        ('offset', ctypes.c_uint32),
        ('length', ctypes.c_uint32),
        ('msb_right', ctypes.c_uint32),
    ]


class VarScreenInfo(ctypes.Structure):
    _fields_ = [
        ('xres', ctypes.c_uint32),
        ('yres', ctypes.c_uint32),
        ('xres_virtual', ctypes.c_uint32),
        ('yres_virtual', ctypes.c_uint32),
        ('xoffset', ctypes.c_uint32),
        ('yoffset', ctypes.c_uint32),

        ('bits_per_pixel', ctypes.c_uint32),
        ('grayscale', ctypes.c_uint32),

        ('red', FbBitField),
        ('green', FbBitField),
        ('blue', FbBitField),
        ('transp', FbBitField),
    ]

class VirtualFrameBuffer:
    __instance = None

    def __new__(cls):
        if VirtualFrameBuffer.__instance is None:
            return object.__new__(cls)

        return VirtualFrameBuffer.__instance

    def __init__(self):
        import pygame

        self.pygame = pygame

        self.pygame.init()

        VirtualFrameBuffer.__instance = self

        self.var_info = VarScreenInfo()
        self.fix_info = FixScreenInfo()

        self.var_info.bits_per_pixel = 32
        self.var_info.xres_virtual = 1920
        self.var_info.yres_virtual = 1080

        self.screen = pygame.display.set_mode((
            self.var_info.xres_virtual,
            self.var_info.yres_virtual
        ))

    def flip(self, surface):
        from PIL import Image

        for event in self.pygame.event.get():
            if event.type == self.pygame.QUIT:
                quit(0)

        # img = Image.frombuffer(
        #         'RGBA', (surface.get_width(),
        #                 surface.get_height()),
        #         surface.get_data(), 'raw', 'BGRA', 0, 1)

        # data_string = img.tostring('raw', 'RGBA', 0, 1)

        pygame_surface = self.pygame.image.frombuffer(
            surface.get_data().tobytes(), (surface.get_width(), surface.get_height()), 'RGBA')

        # Show PyGame surface
        self.screen.blit(pygame_surface, (0,0)) 
        self.pygame.display.flip()

        self.pygame.display.flip()


class LinuxFrameBuffer:
    __instance = None
    
    def __new__(cls):
        if LinuxFrameBuffer.__instance is None:
            return object.__new__(cls)

        return LinuxFrameBuffer.__instance
        
    def __init__(self):
        LinuxFrameBuffer.__instance = self

        self.fd = FbFid(os.open('/dev/fb0', os.O_WRONLY))
        self.file = os.fdopen(self.fd, "wb")

        self.var_info = VarScreenInfo()
        self.fix_info = FixScreenInfo()

        ioctl(self.fd, FBIOGET_VSCREENINFO, self.var_info)
        ioctl(self.fd, FBIOGET_FSCREENINFO, self.fix_info)

    def flip(self, surface):
        self.file.seek(0)
        self.file.write(surface.get_data())

FrameBuffer = VirtualFrameBuffer

try:
    os.open('/dev/fb0', os.O_WRONLY)
    FrameBuffer = LinuxFrameBuffer

except FileNotFoundError:
    pass

