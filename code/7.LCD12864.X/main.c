/*********************************************************************/
/*文件说明：LCD12864显示*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */
//注意事项： 做该实验时请将AD-EN跳线冒取下或将ADC电位器调至中间位置，否则LCD可能会显示乱码
/********************************************************************/
#include "p30f4011.h"
//配置位
_FOSC(0Xc304);//FCKSM = 00;时钟切换使能 FOS = 0111主振荡器XT晶振模式外部7.3728MHz
_FWDT(0X0000);//关狗
_FBORPOR(0X83A2);//MCLREN使能 欠压2.7V 上电延时16MS
_FGS(0X03);
#define uchar unsigned char
#define uint  unsigned int
/************* 12864LCD引脚定义 *************/
#define LCD_data  PORTB       //数据口
#define LCD_RS LATDbits.LATD1//寄存器选择输入
#define LCD_RW  LATDbits.LATD3      //液晶读/写控制
#define LCD_EN  LATDbits.LATD2    //液晶使能控制
#define LCD_PSB LATCbits.LATC13      //串/并方式控制
#define LCD_RST LATEbits.LATE8     //液晶复位端口
/*********************************************************/
uchar  DIS1[] = {"****博科电子*****"};
uchar  DIS2[] = {"****************"};
uchar  DIS3[] = {"****************"};
uchar  DIS4[] = {"***2017/07/06***"};
uchar temp[6]={0};
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
    LCD_PSB = 1;         //并口方式
    LCD_RST = 0;	 //液晶复位
    delay(30);
    LCD_RST = 1;
    delay(30);
    lcd_wcmd(0x34);      //扩充指令操作
    delay(50);
    lcd_wcmd(0x30);      //基本指令操作
    delay(50);
    lcd_wcmd(0x0C);      //显示开，关光标
    delay(50);
    lcd_wcmd(0x01);      //清除LCD的显示内容
    delay(50);
}
void lcd_pos(uchar X,uchar Y)//写显示设置
{
   uchar  pos;
   if (X==1)
     {X=0x80;}
   else if (X==2)
     {X=0x90;}
   else if (X==3)
     {X=0x88;}
   else if (X==4)
     {X=0x98;}
   pos = X+Y ;

   lcd_wcmd(pos);     //显示地址
}
void  clr_screen()//清屏
{
   lcd_wcmd(0x34);      //扩充指令操作
   delay(5);
   lcd_wcmd(0x30);      //基本指令操作
   delay(5);
   lcd_wcmd(0x01);      //清屏
   delay(5);
}
void  bytecode()//字符
{
   uchar  s;
   clr_screen();          //清屏
   lcd_wcmd(0x80);        //设置显示位置为第一行
   for(s=0;s<16;s++)
   {
     lcd_wdat(0x30+s);
   }
   lcd_wcmd(0x90);        //设置显示位置为第二行
   for(s=0;s<16;s++)
   {
     lcd_wdat(0x40+s);
   }
   lcd_wcmd(0x88);        //设置显示位置为第三行
   for(s=0;s<16;s++)
   {
     lcd_wdat(0x50+s);
   }
   lcd_wcmd(0x98);       //设置显示位置为第四行
   for(s=0;s<16;s++)
   {
     lcd_wdat(0x60+s);
   }
}
void Display(unsigned int NUM)
{
//    temp[0]=NUM/100000+0x30;
//    temp[1]=NUM%100000/10000+0x30;
//    temp[2]=NUM%100000%10000/1000+0x30;
//    temp[3]=NUM%100000%10000%1000/100+0x30;
//    temp[4]=NUM%100000%10000%1000%100/10+0x30;
//    temp[5]=NUM%100000%10000%1000%100%10+0x30;
    temp[0]=NUM%100000%10000%1000%100/10+0x30;
    temp[1]=NUM%100000%10000%1000%100%10+0x30;    
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
    TRISC = 0x0000;
    TRISB = 0x0000;//输出
    PORTB = 0x0000;
    PWMCON1 = 0x0000;//通用IO
    TRISE = 0x0000;//设置输出
}
int main()
{
    unsigned char i;
    unsigned char s;
    System_Colck();
    System_Init();
    lcd_init(); //初始化LCD
    bytecode();//字符显示
    clr_screen();//清屏
    delay(1000);//上电，等待稳定
    lcd_wcmd(0x80);//写指令
    lcd_pos(1,0);//设置显示位置为第0行
     for(i=0;i<16;i++)
     {
        lcd_wdat(DIS1[i]);//DIS1[] = {" X "};
        delay(30);
     }
     lcd_pos(2,0);             //设置显示位置为第1行
     for(i=0;i<16;i++)
     {
       lcd_wdat(DIS2[i]);//DIS2[] = {"  Y:           "};
       delay(30);
     }
     lcd_pos(3,0);             //设置显示位置为第2行
     for(i=0;i<16;i++)
     {
       lcd_wdat(DIS3[i]);//DIS3[] = {"  Z:           "};
       delay(30);
     }
     lcd_pos(4,0);             //设置显示位置为第4行
     for(i=0;i<16;i++)
     {
       lcd_wdat(DIS4[i]);//DIS4[] = {" ANGLE:          "};
       delay(30);
     }
    while(1)
    {
        s ++;
        if(s>30)
            s = 0;
        Display(s);
        lcd_wcmd(0x9e);        //设置显示位置为第4行
        for(i=0;i<2;i++)
        {
           lcd_wdat(temp[i]); // 显示字符
        }
        PORTEbits.RE0 = 0;
        delay(6000);
        PORTEbits.RE0 = 1;
        delay(6000);
    }
}
