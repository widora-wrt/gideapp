#include "font.h"
#include "draw.h"
#include "drawfont.h"
#include "gb2312_utf8_code.h"
#include <linux/fb.h>
#include <stdio.h>
extern struct fb_var_screeninfo vinfo;
extern struct fb_fix_screeninfo finfo;


void drawfont_ascii(uint16 f,uint16 X,uint16 Y,uint16 Char,uint16 f_color,uint16 b_color,uint16 a_color)
{            
    FontTypeStruct Font;  
    uint32 Addr;
	uint8 i,w;
    Font=FontGetType(f);
    Addr=(uint32)Font.ELib;
	w=(Font.Witdh+4)/8;//+7是为了照顾宽度为6/12的字符
    Addr+=(Char-' ')*w*Font.High; 
	for(i=0;i<w;i++)
	{
		Draw_Full_List((uint8 *)(Addr+i*Font.High),Font.High,X,Y,f_color,b_color,a_color);
		X+=8;
	}
}
void drawfont_word(uint16 f,uint16 X,uint16 Y,uint8 *word,uint16 f_color,uint16 b_color,uint16 a_color)
{            
    FILE* fphzk = NULL;
    int  k=0, offset,utf8word;
    FontTypeStruct Font; 
    unsigned char buffer[32];
    unsigned char buf[3] = "啊";
    Font=FontGetType(f);
    fphzk = fopen("/www/opt/bin/hzk16", "rb");
    if(fphzk == NULL){
        fprintf(stderr, "error hzk16\n");
        return;
    }
    if((word[0]&0xf0)==0xe0)
    {
        utf8word=word[0];utf8word<<=8;utf8word|=word[1];utf8word<<=8;utf8word|=word[2];
    }else{
        utf8word=word[0];utf8word<<=8;utf8word|=word[1];
    }
    buf[0]=0xB0;
    buf[1]=0xA1;
    while(gb2312_utf8_code[k][1])
    {
        if(gb2312_utf8_code[k][1]==utf8word)
        {
            buf[0]=gb2312_utf8_code[k][0]>>8;
            buf[1]=gb2312_utf8_code[k][0]&0xff;
            break;
        }
        k++;
    }
    offset = (94*(unsigned int)(buf[0]-0xa0-1)+(buf[1]-0xa0-1))*32;
    fseek(fphzk, offset, SEEK_SET);
    fread(buffer, 1, 32, fphzk);
    Draw_Full_Lists((uint8 *)(&buffer[0]),16,Font.High,X,Y,f_color,b_color,a_color);
    fclose(fphzk);
    fphzk = NULL;
}
void drawfont_string(uint16 f,uint16 X,uint16 Y,uint8 *Str,uint16 f_color,uint16 b_color,uint16 a_color)
{            
    uint16 XX,i;
    FontTypeStruct Font;  
    int offset=0;
	uint8 *P=Str,w;
    Font=FontGetType(f);
    XX=X;
	w=(Font.Witdh+4)/8;//+7是为了照顾宽度为6/12的字符
    int l=strlen(P);
    for(i=0;i<l;i)
    {
        if(P[i]<0x80)
        {
            if((vinfo.xres-X)<Font.Witdh)
            {
                X=XX;Y+=Font.High;
            } 
            drawfont_ascii(f,X,Y,P[i],f_color,b_color,a_color);
            i++;
            X+=Font.Witdh;
        }
        else 
        {
            if((vinfo.xres-X)<(8*2))
            {
                X=XX;Y+=Font.High;
            } 
            offset=0;
            if(Font.High>16)offset=(((int)Font.High-16)/2+2);
            drawfont_word(1616,X,(int)Y+offset,&P[i],f_color,b_color,a_color);
            if((P[i]&0xf0)==0xe0)i+=3;
             else i+=2;
            X+=8*2;
        }
        
    }
}