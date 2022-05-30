from fcntl import ioctl
import ctypes
import os

FBIOGET_VSCREENINFO = 0x4600
FBIOGET_FSCREENINFO = 0x4602

class FbFid(int):

    """The framebuffer file descriptor.
    The name attribute gives the
    opened framebuffer file.
    """

    # __slots__ = ('name',)
    pass


class FixScreenInfo(ctypes.Structure):

    """The fb_fix_screeninfo from fb.h."""

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

    """The fb_bitfield struct from fb.h."""

    _fields_ = [
        ('offset', ctypes.c_uint32),
        ('length', ctypes.c_uint32),
        ('msb_right', ctypes.c_uint32),
    ]


class VarScreenInfo(ctypes.Structure):

    """The fb_var_screeninfo struct from fb.h."""

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


class FrameBuffer:
    def __init__(self):
        self.virtual = False

        try:
            self.fd = FbFid(os.open('/dev/fb0', os.O_WRONLY))
        except FileNotFoundError:
            self.virtual = True

        self.var_info = VarScreenInfo()
        self.fix_info = FixScreenInfo()

        if self.virtual:
            self.var_info.bits_per_pixel = 16
            self.var_info.xres_virtual = 1920
            self.var_info.yres_virtual = 1080

        else:
            self.file = os.fdopen(self.fd, "wb")

            ioctl(self.fd, FBIOGET_VSCREENINFO, self.var_info)

            ioctl(self.fd, FBIOGET_FSCREENINFO, self.fix_info)

    def flip(self, surface):
        if self.virtual:
            surface.write_to_png("surface.png")
        else:
            self.file.seek(0)
            self.file.write(surface.get_data())
    

