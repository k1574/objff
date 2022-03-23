#include <ff/ff.h>

FFError
ff_lay_from_fd(int fd, FFPixel *buf,
		u32 w, u32 h,
		void (*lay)(FFPixel *out, FFPixel *bg, FFPixel *fg) )
{
	FFError err;
	FFPixel buf1, buf2;
	u32 wl, hl, wm, hm;
	int i, j, n;
	if(n = ff_read_header(fd, &wl, &hl))
		return n ;
	
	for(i=0 ; i<h && i<hl ; ++i){
		for(j=0 ; j<w && j<wl ; ++j){
			if(!ff_read_pixel(fd, &buf1))
				return FFReadErr ;
			n = i*w + j ;
			lay(&buf2, buf + n, &buf1);
			ff_copy_pixel(buf + n, &buf2);
		}
		if(wl > w)
			ff_skip_n_pixels(fd, wl - w);
	}
	if(hl > h)
		ff_skip_n_pixels(fd, (hl - h) * wl);

	return FFNoErr ;
}

