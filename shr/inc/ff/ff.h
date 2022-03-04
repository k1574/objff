#ifndef __FF_H
#define __FF_H
#include <inttypes.h>

typedef uint16_t u16;
typedef uint32_t u32;
struct FFPixel { u16 r, g, b, a; } ;
typedef struct FFPixel FFPixel;

int ff_is_little_endian(void);
void ff_swap_endian(void *data, unsigned int size);
void ff_swap_pixel_endian(FFPixel *p);
void ff_swap_pixels_endian(FFPixel *ps, unsigned int n);

int ff_colorname_to_pixel(FFPixel *px, char *cn);

int ff_read_header(int fd, u32 *w, u32 *h);
int ff_read_pixel(int fd, FFPixel *px);
int ff_write_header(int fd, u32 w, u32 h);
int ff_write_pixel(int fd, FFPixel *px);

FFPixel *ff_read_image(int fd, u32 *w, u32 *h);
int ff_write_image(int fd, u32 w, u32 h, FFPixel *buf);

#endif

