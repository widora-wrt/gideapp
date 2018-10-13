#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "draw.h"
extern char *fbp;
extern struct fb_var_screeninfo vinfo;
extern struct fb_fix_screeninfo finfo;

unsigned short rgb2tft(int c)
{
    unsigned char r,g,b;
    unsigned int t;
    b=(c&0xff)/8;c>>=8;
    g=(c&0xff)/4;c>>=8;
    r=(c&0xff)/8;
    t=r;t<<=6;t|=g;t<<=5;t|=b;
    return t;
}

void draw_dot(unsigned int x,unsigned int y,unsigned short color)
{
    long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +(y+vinfo.yoffset) * finfo.line_length;
    *((unsigned short int*)(fbp + location)) =color;
}

void draw_line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned short color)
{
    int dx,dy,e;
    dx=x2-x1;
    dy=y2-y1;
    if(dx>=0)
    {
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 1/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    draw_dot(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 2/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    draw_dot(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)

            if(dx>=dy) // 8/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    draw_dot(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 7/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    draw_dot(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}
                    y1-=1;
                    e+=dx;
                }
            }
        }
    }
    else //dx<0
    {
        dx=-dx;        //dx=abs(dx)
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 4/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    draw_dot(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 3/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    draw_dot(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)

            if(dx>=dy) // 5/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    draw_dot(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 6/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    draw_dot(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}
                    y1-=1;
                    e+=dx;
                }
            }
        }
    }
}

void draw_rect(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned short Color)
{
	draw_line(x1,y1,x1,y2,Color);	
	draw_line(x1,y1,x2,y1,Color);	
	draw_line(x2,y2,x1,y2,Color);	
	draw_line(x2,y1,x2,y2,Color);	
}
void draw_rectfill(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned short color)
{
     int x = 0, y = 0;
     for(x=x1;x<x2;x++)
     for(y=y1;y<y2;y++)
     {
        draw_dot(x,y,color);
     }
}


void circle_point(int x,int y,int x0,int y0,int color)
{
	draw_dot(x+x0,y+y0,color);
	draw_dot(y+x0,x+y0,color);
	draw_dot(y+x0,-x+y0,color);
	draw_dot(x+x0,-y+y0,color);
	draw_dot(-x+x0,-y+y0,color);
	draw_dot(-y+x0,-x+y0,color);
	draw_dot(-y+x0,x+y0,color);
	draw_dot(-x+x0,y+y0,color);
}
void draw_circle(int x0,int y0,int r,int color)
{
	int x,y,d;
	x=0;
	y=r;
	d=1-r;
	circle_point(x,y,x0,y0,color);
	while(x<=y)
	{
		if(d<0){d+=2*x+3;x++;}
		else{d+=2*(x-y)+5;x++;y--;}
		circle_point(x,y,x0,y0,color);
	}
}
void circle_line(int x,int y,int x0,int y0,int color)
{
    draw_line(x+x0,y+y0,-x+x0,y+y0,color);
    draw_line(y+x0,-x+y0,-y+x0,-x+y0,color);
    draw_line(-x+x0,-y+y0,x+x0,-y+y0,color);
    draw_line(-y+x0,x+y0,y+x0,x+y0,color);
}
void draw_circlefill(unsigned int x0,unsigned int y0,unsigned int r,unsigned short color)
{
	int x,y,d;
	x=0;
	y=r;
	d=1-r;
	circle_point(x,y,x0,y0,color);
	while(x<=y)
	{
		if(d<0){d+=2*x+3;x++;}
		else{d+=2*(x-y)+5;x++;y--;}
		circle_line(x,y,x0,y0,color);
	}
    draw_line(x0-r,y0,x0+r,y0,color);
}
void draw_arcrect(unsigned int SX,unsigned int SY,unsigned int EX,unsigned int EY,unsigned int R,unsigned short ColorD)
{
	unsigned int C,X,Y,XA,YA,XB,YB;
	short Fe;
	draw_line(SX+R,SY,EX-R,SY,ColorD);						//上边线
	draw_line(SX+R,EY,EX-R,EY,ColorD);						//下边线
	draw_line(SX,SY+R,SX,EY-R,ColorD);						//左边线
	draw_line(EX,SY+R,EX,EY-R,ColorD);						//右边线
	XA=EX-R;YA=SY+R;											//第一象限原点
	SX+=R;	SY+=R;												//第二象限原点
	XB=SX; 	YB=EY-R;											//第三象限原点
	EX-=R;	EY-=R;												//第四象限原点
	C=R<<1;Fe=0;X=R,Y=0;
	while(C--){
		if(Fe>0)Fe=Fe-2*X+1;
		else Fe=Fe+2*Y+1;
		if((Fe>0)&&(X>0))X--;
		else Y++;
		draw_dot(EX+X,EY+Y,ColorD);						//绘制第四象限
		draw_dot(XB-X,YB+Y,ColorD); 						//绘制第三象限
		draw_dot(SX-X,SY-Y,ColorD);						//绘制第二象限
		draw_dot(XA+X,YA-Y,ColorD);						//绘制第一象限
	}	
}
void draw_clear(unsigned short c)
{
    draw_rectfill(0,0,vinfo.xres,vinfo.yres,c);
}
/****************************************************************************
Date:2013/8/14
Vision:V1.0
Func:水平打印一字节显示像素
Note:(Data) 一字节控制像素亮灭 (X,Y) 引用全局地址但不改变全局地址
****************************************************************************/
void Draw_X_8Bit(unsigned char Data,unsigned short X,unsigned short Y,unsigned short F_Color,unsigned short B_Color,unsigned short A_Color)
{
	char i;
	for(i=0;i<8;i++)
	{
		if(BIT(i)&Data)draw_dot(X,Y,F_Color);
		else if(B_Color!=A_Color)draw_dot(X,Y,B_Color);
		X++;
	}
}
void Draw_X_8Bit_(unsigned char Data,unsigned short X,unsigned short Y,unsigned short F_Color,unsigned short B_Color,unsigned short A_Color)
{
	char i;
	for(i=7;i>-1;i--)
	{
		if(BIT(i)&Data)draw_dot(X,Y,F_Color);
		else if(B_Color!=A_Color)draw_dot(X,Y,B_Color);
		X++;
	}
}
/****************************************************************************
Date:2013/8/14
Vision:V1.0
Func:垂直打印一字节显示像素
Note:(Data) 一字节控制像素亮灭 (X,Y) 引用全局地址但不改变全局地址
****************************************************************************/
void Draw_Y_8Bit(unsigned char Data,unsigned short X,unsigned short Y,unsigned short F_Color,unsigned short B_Color,unsigned short A_Color)
{
	char i;
	for(i=0;i<8;i++)
	{
		if(BIT(i)&Data)draw_dot(X,Y,F_Color);
		else if(B_Color!=A_Color)draw_dot(X,Y,B_Color);
		Y++;
	}
}
/****************************************************************************
Date:2013/8/14
Vision:V1.0
Func:垂直平铺一字节宽度
Note:(Data) 平铺一系列参数 (X,Y) 引用全局地址但不改变全局地址
****************************************************************************/

void Draw_Full_List(void *Data,unsigned short Data_Length,unsigned short X,unsigned short Y,unsigned short F_Color,unsigned short B_Color,unsigned short A_Color)
{
#ifdef FAST_Draw_Full_List
	Fast_Draw_Fill_List(Data,Data_Length,X,Y,F_Color,B_Color,A_Color);
#else 
	unsigned short i;
	unsigned char *p;
	p=(unsigned char *)Data;
	for(i=0;i<Data_Length;i++)
	{
		Draw_X_8Bit(*p++,X,Y,F_Color,B_Color,A_Color);
		Y++;
	}
#endif
}
void Draw_Full_Lists(void *Data,unsigned short w,unsigned short Data_Length,unsigned short X,unsigned short Y,unsigned short F_Color,unsigned short B_Color,unsigned short A_Color)
{
	unsigned short i,n;
	unsigned char *p;
	p=(unsigned char *)Data;
    w/=8;
	for(i=0;i<Data_Length;i++)
	{
        for(n=0;n<w;n++)
        {
		    Draw_X_8Bit_(*p++,X+n*8,Y,F_Color,B_Color,A_Color);
        }
		Y++;
	}
}