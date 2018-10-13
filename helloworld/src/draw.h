#ifndef _DRAW_H_
#define _DRAW_H_

#define BIT(x) (1<<x)
unsigned short rgb2tft(int c);
void draw_dot(unsigned int x,unsigned int y,unsigned short color);
void draw_line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned short color);
void draw_rect(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned short Color);
void draw_rectfill(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned short color);
void circle_point(int x,int y,int x0,int y0,int color);
void draw_circle(int x0,int y0,int r,int color);
void circle_line(int x,int y,int x0,int y0,int color);
void draw_circlefill(unsigned int x0,unsigned int y0,unsigned int r,unsigned short color);
void draw_arcrect(unsigned int SX,unsigned int SY,unsigned int EX,unsigned int EY,unsigned int R,unsigned short ColorD);
void draw_clear(unsigned short c);
void Draw_X_8Bit(unsigned char Data,unsigned short X,unsigned short Y,unsigned short F_Color,unsigned short B_Color,unsigned short A_Color);
void Draw_Y_8Bit(unsigned char Data,unsigned short X,unsigned short Y,unsigned short F_Color,unsigned short B_Color,unsigned short A_Color);
void Draw_Full_List(void *Data,unsigned short Data_Length,unsigned short X,unsigned short Y,unsigned short F_Color,unsigned short B_Color,unsigned short A_Color);
void Draw_Full_Lists(void *Data,unsigned short w,unsigned short Data_Length,unsigned short X,unsigned short Y,unsigned short F_Color,unsigned short B_Color,unsigned short A_Color);
#endif