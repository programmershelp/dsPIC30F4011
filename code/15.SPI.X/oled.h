#ifndef OLED_H
#define	OLED_H
#include "p30f4011.h"
#include "font.h"
#include "bmp.h"
#include "delay.h"
#define  u8 unsigned char
#define  u32 unsigned int
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0
#define OLED_CS LATBbits.LATB1 //片选
#define OLED_RST LATBbits.LATB6//复位
#define OLED_DC LATBbits.LATB2//数据/命令控制
#define OLED_SCL LATBbits.LATB8//时钟 D0（SCLK?
#define OLED_SDIN LATBbits.LATB7//D1（MOSI） 数据
#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1;
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64

void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void disn(unsigned char X,unsigned char Y,unsigned char n);
void picture(void);
void OLED_Init(void);


#endif	/* OLED_H */

