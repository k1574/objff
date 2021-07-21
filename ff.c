#include <ff/ff.h>
#include <string.h>

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
