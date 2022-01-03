/*********************************************************************/
/*文件说明：OLED显示实验*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */
/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//配置位
_FOSC(0Xc307);//FCKSM = 00;时钟切换使能 FOS = 0111主振荡器XT晶振模式外部7.3728MHz
_FWDT(0X0000);//关狗
_FBORPOR(0X83A2);//MCLREN使能 欠压2.7V 上电延时16MS
_FGS(0X03);
unsigned char CN0_Flag = 0;//外部中断标志
void Delay_1ms(unsigned int t)//t = 1000大概1s
{
    unsigned int i,j;
    for(i = 0;i < t;i ++)
        for(j = 0;j < 2000;j ++);
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
    PWMCON1 = 0x0000;//通用IO
    TRISE = 0x0000;//设置输出
    PORTE = 0x0000;//
}
int main()
{
    unsigned char i;
    unsigned char m;
    System_Colck();
    System_Init();
    OLED_Init();			//初始化OLED
    OLED_Clear();
    //disn(unsigned char X,unsigned char Y,unsigned char n);
    
    for(m=33;m<59;m+=1)
    {
		disn(1,1,m); //A to Z
        Delay_1ms(500);
    }
    
    for(m=65;m<91;m+=1)
    {
		disn(3,1,m); //a to z
        Delay_1ms(500);
    }
    
    for(m=1;m<=9;m+=1)
    {
		disn(5,1,m); //1 to 9
        Delay_1ms(500);
    }
    
    disn(2,2,23);// -
    disn(3,2,24);// .
    disn(4,2,25);// /
    disn(5,2,26);// :
    disn(6,2,27);// ;
    disn(7,2,28);// <
    disn(8,2,29);// =
    disn(9,2,30);// >
    disn(10,2,31);// ?
    disn(11,2,32);// @
    
    disn(3,3,11);// !
    disn(4,3,12);// "
    disn(5,3,13);// #
    disn(6,3,14);// $
    disn(7,3,15);// %
    disn(8,3,16);// &
    disn(9,3,17);// '
    disn(10,3,18);// (
    disn(11,3,19);// )
    disn(12,3,20);// *
    disn(13,3,21);// +
    disn(14,3,22);// ,
    
    disn(3,0,60);// !
    disn(3,0,61);// !
    disn(4,0,62);// "
    disn(5,0,63);// #
    disn(6,0,64);// $
    
    while(1)
    {
        i ++;
        PORTE = i;
        Delay_1ms(1000);
        //disn(14,3,i%100/10);disn(15,3,i%10);      
    }
}
