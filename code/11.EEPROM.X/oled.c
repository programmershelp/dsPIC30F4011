#include "oled.h"
//spi
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	u8 i;
	if(cmd)
	  OLED_DC_Set();
	else
	  OLED_DC_Clr();
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();
		if(dat&0x80)
			{
		   OLED_SDIN_Set();
			}
else
		   OLED_SDIN_Clr();
				OLED_SCLK_Set();
		dat<<=1;
	}
	OLED_CS_Set();
	OLED_DC_Set();
}
//设置行列地址
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}
//开启OLED显示
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
//清屏函数!
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置?列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置?列高地址
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);
	} 
}
/**************************实现函数********************************************
*函数原型:		disn(unsigned char X,unsigned char Y,unsigned char n);
*功　　能:		数字。
*******************************************************************************/
void disn(unsigned char X,unsigned char Y,unsigned char n)
{
	unsigned char m;
	OLED_WR_Byte(0xb7-(Y<<1),OLED_CMD);
	if(X%2)
		OLED_WR_Byte(0x08,OLED_CMD);
	else
		OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0x10+(X>>1),OLED_CMD);
	for(m=0;m<=15;m+=2)
		OLED_WR_Byte(*(num[n]+m),OLED_DATA);

	OLED_WR_Byte(0xb7-(Y<<1)-1,OLED_CMD);
	if(X%2)
		OLED_WR_Byte(0x08,OLED_CMD);
	else
		OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0x10+(X>>1),OLED_CMD);
	for(m=1;m<=15;m+=2)
		OLED_WR_Byte(*(num[n]+m),OLED_DATA);
}
/**************************实现函数********************************************
*函数原型:		picture(void);
*功　　能:		图像显示。
*******************************************************************************/
void picture(void)
{
    unsigned char i,j;
	for(i=0;i<8;i++)
		{
			OLED_WR_Byte(0xB0+i,OLED_CMD);//设置显示位置?行
			OLED_WR_Byte(0x00,OLED_CMD); //设置显示位置?列低地址
			OLED_WR_Byte(0x10,OLED_CMD); //设置显示位置?列高地址
			for(j=0;j<128;j++)
		    OLED_WR_Byte(nBitmapDot[128*i+j],OLED_DATA);
		}
}
//初始化SSD1306
void OLED_Init(void)
{
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;//OLED SPI OUT
        OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set();
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC0,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
	OLED_Clear();
	OLED_Set_Pos(0,0);
}

