/*********************************************************************/
/*文件说明：INT1外部中断实验INT1按键按下进入中断RE LED闪烁*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */
/********************************************************************/
#include "p30f4011.h"
//配置位
_FOSC(0Xc307);//FCKSM = 00;时钟切换使能 FOS = 0111主振荡器XT晶振模式外部7.3728MHz
_FWDT(0X0000);//关狗
_FBORPOR(0X83A2);//MCLREN使能 欠压2.7V 上电延时16MS
_FGS(0X03);
unsigned char INT1_Flag = 0;//外部中断标志
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
    PORTE = 0xffff;//置高
    TRISDbits.TRISD0 = 1;//RD0按键输入
    INTCON1 |= 0X8000;//不要中断嵌套
    IEC1bits.INT1IE = 1;;//允许外部中断
    IPC4bits.INT1IP = 7;//最高优先级7
    IFS1 = 0x0000;//清外部中断标志
    INTCON2bits.INT1EP = 1;//INT1负边沿中断
}
int main()
{
    System_Colck();
    System_Init();
    while(1)
    {
        if(INT1_Flag == 1)
        {
            INT1_Flag = 0;
            PORTE =~ PORTE;
        }
    }
}
void __attribute__((__interrupt__, auto_psv)) _INT1Interrupt(void)//中断处理函数
{
    Delay_1ms(20);//按键消抖
    IFS1bits.INT1IF = 0;//清零电平变化中断
     INT1_Flag = 1;
}

