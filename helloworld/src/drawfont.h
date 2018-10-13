#ifndef _DRAEFONT_H_
#define _DRAEFONT_H_

#include "font.h"

void drawfont_ascii(uint16 f,uint16 X,uint16 Y,uint16 Char,uint16 f_color,uint16 b_color,uint16 a_color);
void drawfont_string(uint16 f,uint16 X,uint16 Y,uint8 *Str,uint16 f_color,uint16 b_color,uint16 a_color);
void drawfont_word(uint16 f,uint16 X,uint16 Y,uint8 *word,uint16 f_color,uint16 b_color,uint16 a_color);
#endif

