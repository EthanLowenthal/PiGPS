#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/fb.h>
#include <linux/kd.h>

struct FramebufferInfo
{
	int fd;

	void *ptr;

	struct fb_var_screeninfo var;
	struct fb_fix_screeninfo fix;
	unsigned bytespp;
};

class FrameBuffer {
  public:
    FrameBuffer(int id) : fb_id(id) {
      open_fb();
    }
    ~FrameBuffer() {
      free(buff);
      close(fd);
    }

    void fill(unsigned int color) {
      	unsigned x, y;

        for (y = 0; y < fb_info.var.yres; y++) {
          for (x = 0; x <  fb_info.var.xres; x++)
            draw_pixel(x, y, color);
        }
    }


    void draw_pixel(int x, int y, unsigned color)
    {
      unsigned char *p;

      p = (unsigned char *)fb_info.ptr + fb_info.fix.line_length * y + 4 * x;
      *p++ = color;
      *p++ = color >> 8;
      *p++ = color >> 16;
      *p   = 0xFF; //alpha
    }

    void draw_rectangle(int x, int y, unsigned width, unsigned height, unsigned color) {
      unsigned i, j;
      for (i=0;i<height;i++) {
        for (j=0;j<width;j++) {
          draw_pixel(j+x, i+y, color);
        }
      }
    }

    void flip() {
      memcpy(fb_info.ptr, buff, fb_info.var.yres_virtual * fb_info.fix.line_length);
    }

  void *buff;
  FramebufferInfo fb_info {};

  private:
    void open_fb() {

#ifdef DEV_MODE
      int tty = open("/dev/tty1", O_RDWR);

      if(ioctl(tty, KDSETMODE, KD_GRAPHICS) == -1)
        std::cout << "Failed to set graphics mode on tty1" << std::endl;

      close(tty);
#endif

      std::string fb_name = "/dev/fb"+std::to_string(fb_id);
      fd = open(fb_name.c_str(), O_RDWR);

      if (fd < 0) {
        std::cout << "Failed to open framebuffer" << std::endl;
        exit(1);
      }


      fb_info.fd = fd;

      if (ioctl(fd, FBIOGET_VSCREENINFO, &fb_info.var)){
        std::cout << "ioctl failed on var" << std::endl;
        exit(1);
      }
      if (ioctl(fd, FBIOGET_FSCREENINFO, &fb_info.fix)) {
        std::cout << "ioctl failed on fil" << std::endl;
        exit(1);
      }

      std::cout << "fb res " << fb_info.var.xres << "x" << fb_info.var.yres <<\
        ", virtual " << fb_info.var.xres_virtual << "x" << fb_info.var.yres_virtual <<\
        ", line_len "<< fb_info.fix.line_length << ", bpp " << fb_info.var.bits_per_pixel << std::endl;

      void *ptr = mmap(0,
          fb_info.var.yres_virtual * fb_info.fix.line_length,
          PROT_WRITE | PROT_READ,
          MAP_SHARED, fd, 0);

     if (ptr == MAP_FAILED) {
        std::cout << "Framebuffer map failed" << std::endl;
        exit(1);
      }

      fb_info.ptr = ptr;
      buff = malloc(fb_info.var.yres_virtual * fb_info.fix.line_length);
    }

    int fb_id;
    int fd {};
};
