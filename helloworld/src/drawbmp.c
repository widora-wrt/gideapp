#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <jpeglib.h>
#include <jerror.h> 

#define    FB_DEV  "/dev/fb0"
unsigned char  *fbmem;
unsigned int    screensize;
unsigned int    fb_width;
unsigned int    fb_height;
unsigned int    fb_depth;
unsigned int    x;
unsigned int    y;
 
unsigned char * display_jpeg(char * filename, int *w, int *h)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE           *infile;
	unsigned char  *buffer;
	unsigned char *temp;
	if ((infile = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "open %s failed/n", filename);
		exit(-1);
	}
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	*w = cinfo.output_width;
	*h = cinfo.output_height;
	if ((cinfo.output_width > fb_width) ||(cinfo.output_height > fb_height)) {
		printf("too large JPEG file,cannot display/n");
		return 0;
	}
	buffer = (unsigned char *) malloc(cinfo.output_width *cinfo.output_components * cinfo.output_height);
	temp = buffer;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, &buffer, 1);
		buffer += cinfo.output_width * cinfo.output_components;
	}
    
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	return temp;
	fclose(infile);
}

void drawjpg(unsigned int x,unsigned int y,unsigned char *name)
{
    int w ,h,i,j;
    int color;
    fb_width=320;
    fb_height=240;
    fb_depth=16;
	unsigned char *imgbuf;
	imgbuf = display_jpeg(name,&w,&h);
	for(j = 0; j < h; j++)
	{
		for( i = 0; i < w; i++)
		{
			color= imgbuf[i*3 + j*w*3];
            color<<=8;
			color|=  imgbuf[i*3 + j*w*3 + 1];
            color<<=8;
			color|= imgbuf[i*3 + j*w*3+2];
            draw_dot(x+i,y+j,rgb2tft(color));
		}
	}
    free(imgbuf);
}