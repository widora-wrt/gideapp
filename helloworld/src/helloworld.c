#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "draw.h"
#include "font.h"
#include "drawfont.h"
#include "drawbmp.h"
char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;

int fb_init()
{
    int fbfd = 0;
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    return fbfd;
}
int main(int argc, char**argv)
{
    int fbfd = 0,a;
    if(argc<2)
    {
        printf("dot,line,rect,rectfill,circle,circlefill,arcrect\n");
    }
    fbfd=fb_init();
    if(strcmp(argv[1],"dot")==0)
    {
        draw_dot(atoi(argv[2]),atoi(argv[3]),rgb2tft((int)strtol(argv[4],0, 16)));
    }else 
    if(strcmp(argv[1],"line")==0)
    {
        draw_line(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),rgb2tft((int)strtol(argv[6],0, 16)));
    }else 
    if(strcmp(argv[1],"rect")==0)
    {
        draw_rect(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),rgb2tft((int)strtol(argv[6],0, 16)));
    }else
    if(strcmp(argv[1],"rectfill")==0)
    {
        draw_rectfill(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),rgb2tft((int)strtol(argv[6],0, 16)));
    }else
    if(strcmp(argv[1],"circle")==0)
    {
        draw_circle(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),rgb2tft((int)strtol(argv[5],0, 16)));
    }else
    if(strcmp(argv[1],"circlefill")==0)
    {
        draw_circlefill(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),rgb2tft((int)strtol(argv[5],0, 16)));
    }else
    if(strcmp(argv[1],"arcrect")==0)
    {
        draw_arcrect(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),atoi(argv[6]),rgb2tft((int)strtol(argv[7],0, 16)));
    }else
    if(strcmp(argv[1],"clear")==0)//./helloworld clear 000000
    {
        unsigned short c=rgb2tft((int)strtol(argv[2],0, 16));
        draw_clear(c);
    }else
    if(strcmp(argv[1],"x8bit")==0)
    {
        unsigned short c=rgb2tft((int)strtol(argv[3],0, 16));
        Draw_X_8Bit(atoi(argv[2]),0,0,c,0x0000,0x0002);
    }else
    if(strcmp(argv[1],"char")==0)//./helloworld char D 3232 140 10 ffffff ff0000 000000
    {
        drawfont_ascii(atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[2][0],rgb2tft((int)strtol(argv[6],0, 16)),rgb2tft((int)strtol(argv[7],0, 16)),rgb2tft((int)strtol(argv[8],0, 16)));
    }else
    if(strcmp(argv[1],"string")==0)//./helloworld string ABC3333333333 3232 10 10 ffff00 000000 000001
    {
        drawfont_string(atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[2],rgb2tft((int)strtol(argv[6],0, 16)),rgb2tft((int)strtol(argv[7],0, 16)),rgb2tft((int)strtol(argv[8],0, 16)));
    }else
    if(strcmp(argv[1],"debug")==0)//./helloworld string ABC3333333333 3232 10 10 ffff00 000000 000001
    {
        drawfont_string(atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[2],rgb2tft((int)strtol(argv[6],0, 16)),rgb2tft((int)strtol(argv[7],0, 16)),rgb2tft((int)strtol(argv[8],0, 16)));
    }else
    if(strcmp(argv[1],"word")==0)//./helloworld word ffffff 1616 140 10 ffffff ff0000 000000
    {
        drawfont_word(atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),"你",rgb2tft((int)strtol(argv[6],0, 16)),rgb2tft((int)strtol(argv[7],0, 16)),rgb2tft((int)strtol(argv[8],0, 16)));
    }else
    if(strcmp(argv[1],"jpg")==0)//./helloworld word ffffff 1616 140 10 ffffff ff0000 000000
    {
        drawjpg(atoi(argv[3]),atoi(argv[4]),argv[2]);
    }
    munmap(fbp, screensize);
    close(fbfd);
   return 0;
}

