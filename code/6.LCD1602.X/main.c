/*********************************************************************/
/*文件说明：LCD1602显示*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */
/********************************************************************/
#include "p30f4011.h"
//配置位
_FOSC(0Xc307);//FCKSM = 00;时钟切换使能 FOS = 0111主振荡器XT晶振模式外部7.3728MHz
_FWDT(0X0000);//关狗
_FBORPOR(0X83A2);//MCLREN使能 欠压2.7V 上电延时16MS
_FGS(0X03);
#define uchar unsigned char
#define uint  unsigned int
/************* LCD引脚定义 *************/
#define LCD_data  PORTB       //数据口
#define LCD_RS LATDbits.LATD1//寄存器选择输入
#define LCD_RW  LATDbits.LATD3      //液晶读/写控制
#define LCD_EN  LATDbits.LATD2    //液晶使能控制
#define LCD_RST LATEbits.LATE8     //液晶复位端口
/*********************************************************/
uchar  dis1[] = {"      Welcome    "};
uchar  dis2[] = {"       BOKE      "};

void delayNOP()//8us延时
{
    uchar i;
    for(i = 0;i < 20;i ++)
    {
        asm("NOP");
    }
}
void delay(int ms)//毫秒延时
{
    while(ms--)
	{
            uchar i;
            for(i=0;i<20;i++);
	}
}
void delay1(int ms)//毫秒延时
{
    while(ms--)
	{
      uchar y;
	  for(y=0;y<20;y++) ;
	}
}
void Delay_1ms(unsigned int t)//t = 1000大概1s
{
    unsigned int i,j;
    for(i = 0;i < t;i ++)
        for(j = 0;j < 2000;j ++);
}
uchar lcd_busy()//忙检测
 {
    uchar result;
    TRISBbits.TRISB7 = 1;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    delay(5);
    if(PORTBbits.RB7 == 0)
    result = 0;
    LCD_EN = 0;
    TRISBbits.TRISB7 = 0;
    return(result);
 }
void lcd_wcmd(uchar cmd)//写指令数据
{
//   while(lcd_busy());
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    PORTB = cmd;
    delay(5);
    LCD_EN = 1;
    delay(5);
    LCD_EN = 0;
}
void lcd_pos(uchar pos)
{                          //设定显示位置
lcd_wcmd(pos | 0x80);
}
void lcd_wdat(uchar dat)//写显示数据
{
 //  while(lcd_busy());
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    PORTB = dat;
    delay(5);
    LCD_EN = 1;
    delay(5);
    LCD_EN = 0;
}
void lcd_init()//复位
{
    LCD_RST = 0;	 //液晶复位
    delay(30);
    LCD_RST = 1;
    delay(30);
    lcd_wcmd(0x38);      //16*2显示，5*7点阵，8位数据
    delay(50);
    lcd_wcmd(0x0c);      //显示开，关光标
    delay(50);
    lcd_wcmd(0x06);      //移动光标
    delay(50);
    lcd_wcmd(0x01);      //清除LCD的显示内容
    delay(50);
}
//振荡器配置
void System_Colck(void)
{
    //产生Fosc = 7.3728MHz 117.9648MHz 30MIPS
    while (OSCCONbits.COSC!= 0b011)
    while (OSCCONbits.LOCK!= 1) {};//PLL 处于锁定状态
}
void System_Init(void)
{
    TRISD = 0x0000;
    TRISB = 0x0000;//输出
    PORTB = 0x0000;
    PWMCON1 = 0x0000;//通用IO
    TRISE = 0x0000;//设置输出
}
int main()
{
    unsigned char i;

    System_Colck();
    System_Init();
    lcd_init();               // 初始化LCD
    
    lcd_wcmd(0x06);            //向右移动光标
        lcd_pos(0);                //设置显示位置为第一行的第1个字符
        i = 0;
        while(dis1[ i ] != '\0')
        {                           //显示字符"WLCOME"
            lcd_wdat(dis1[ i ]);
            i++;
            delay(300);                //控制两字之间显示速度
        }
        lcd_pos(0x40);             //设置显示位置为第二行第1个字符
        i = 0;
        while(dis2[ i ] != '\0')
        {
            lcd_wdat(dis2[ i ]);      //显示字符" BOKE  "
            i++;
            delay(300);                //控制两字之间显示速度
        }
    while(1)
    {
        i ++;
        PORTE = i;
        Delay_1ms(1000);
    }
}
