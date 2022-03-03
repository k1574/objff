#include <ff/ff.h>
#include <ff/ff_raw.h>
#include <string.h>
#include <unistd.h>

void
ff_swap_pixel_endian(FFPixel *p)
{
	ff_swap_endian(&p->r, sizeof(p->r));
	ff_swap_endian(&p->g, sizeof(p->g));
	ff_swap_endian(&p->b, sizeof(p->b));
	ff_swap_endian(&p->a, sizeof(p->a));
}

void
ff_swap_pixels_endian(FFPixel *ps, unsigned int n)
{
	int i;
	for(i=0 ; i<n ; ++i)
		ff_swap_pixel_endian(&ps[i]);
}

int
ff_is_little_endian(void)
{
	static int n = 1;
	return *((char *)&n) ;
}

void
ff_swap_endian(void *data, unsigned int size)
{
	unsigned int i, j;
	char buf;
	for( i=0, j=size-1 ; i<j; ++i, --j ){
		buf = ((char*)data)[i] ;
		((char*)data)[i] = ((char*)data)[j] ;
		((char*)data)[j] = buf ;
	}
}

static int
hexchrval(char c)
{
	if('0'<=c && c<='9')
		return c - '0' ;
	else if('A' <= c && c <= 'F')
		return 10 + c - 'A' ;
	else if('a' <= c && c <= 'f')
		return 10 + c - 'a' ;

	return 0 ;
}

int
ff_hexstr_to_int(char *s, const unsigned int n)
{
	char c;
	char *p = s+n-1 ;
	int i, mul = 1, ret = 0 ;
	for(i=0 ; i<n ; ++i, --p){
		ret += mul * hexchrval(*p) ;
		mul *= 16 ;
	}

	return ret ;
}

int
ff_colorname_to_pixel(FFPixel *px, char *s)
{
	int len = strlen(s) ;
	switch(*s++){
	case '#' : /* Default RGB[A] format. */
		px->a = ~0 ;
		switch(len){
		case 9 : /* With alpha. */
			px->a = ff_hexstr_to_int(s+6, 2) * 256 ;
		/***** Fallthrough. *****/
		case 7 : /* Without aplha. */
			px->r = ff_hexstr_to_int(s, 2) * 256 ;
			px->g = ff_hexstr_to_int(s+2, 2) * 256 ;
			px->b = ff_hexstr_to_int(s+4, 2) * 256 ;
		break;
		default:
			return 1 ;
		}
	break;
	default:
		return 1 ;
	}

	return 0 ;
}

int
ff_read_header(int fd, u32 *w, u32 *h)
{

	int n;
	char buf[sizeof(FFHeaderMagicValue)];

	n = read(fd, buf, sizeof(FFHeaderMagicValue)-1) ;
	if(n != sizeof(FFHeaderMagicValue))
		return 1 ;

	n = read(fd, w, sizeof(*w)) ;
	if(n <= 0)
		return 2 ;

	n = read(fd, h, sizeof(*h)) ;
	if(n <= 0)
		return 3 ;
		
	if(ff_is_little_endian()){
		ff_swap_endian(w, sizeof(*w));
		ff_swap_endian(h, sizeof(*h));
	}

	return 0 ;
}

int
ff_read_pixel(int fd, FFPixel *px)
{
	int n;
	n = read(fd, px, sizeof(*px)) ;

	if((n == sizeof(*px)) && ff_is_little_endian()){
		ff_swap_pixel_endian(px);
	}

	return n ;
}

int
ff_write_header(int fd, u32 w, u32 h)
{
	u32 wl, hl;

	wl = w ;
	hl = h ;
	
	if(ff_is_little_endian()){
		ff_swap_endian(&wl, sizeof(wl));
		ff_swap_endian(&hl, sizeof(hl));
	}

	write(fd, FFHeaderMagicValue, sizeof(FFHeaderMagicValue)-1);
	int n = write(fd, &wl, sizeof(wl)) ;
	if(n <= 0)
		return 1 ;

	n = write(fd, &hl, sizeof(hl)) ;
	if(n <= 0)
		return 2 ;

	return 0 ;
}

int
ff_write_pixel(int fd, FFPixel *px)
{
	int n;
	FFPixel bpx;
	bpx.r = px->r ;
	bpx.g = px->g ;
	bpx.b = px->b ;
	bpx.a = px->a ;
	if(ff_is_little_endian()){
		ff_swap_pixel_endian(&bpx);
	}

	n = write(fd, &bpx, sizeof(bpx));
	return n ;
}
