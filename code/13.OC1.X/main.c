/*********************************************************************/
/*文件说明：输出比较实验*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */

/*使用说明:RD0(OC1)用杜邦线接到LED3输出LED
 * 注意DSPIC大部分单周期指令FOSC/4 = 7.3728MHz/4 = 1.8432MHz*16 = 29.4912MHz
 *  1/29491200 = 0.00000003391s 也就是33.91ns，分频1:1 TMR2从0000到ffff变化
 * 33.91ns*65535 = 2222291.85ns= 2.2ms = 0.00222229185s        */

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//配置位
_FOSC(0Xc307);//FCKSM = 00;时钟切换使能 FOS = 0111主振荡器XT晶振模式外部7.3728MHz
_FWDT(0X0000);//关狗
_FBORPOR(0X83A2);//MCLREN使能 欠压2.7V 上电延时16MS
_FGS(0X03);
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
void OC1_Init(void)
{
    OC1CONbits.OCM = 0b000;//禁止输出比较
    OC1CONbits.OCSIDL = 0;//空闲模式继续
    OC1CONbits.OCTSEL = 0;//比较Timer2的时钟源
    TMR2  =0x0000; //TMR2初值0x0006
    PR2 = 10; //周期寄存器赋值0x002f,TMR2在0x002f复位
    OC1R = 5; //设置匹配值0x001a，在TMR2与此值匹配时，产生脉冲上升沿
    OC1RS = 10;	//设置匹配值0x002a，在TMR2与此值匹配时，产生脉冲下降沿

    IPC0bits.OC1IP = 0x02;//中断优先级1
    IFS0bits.OC1IF = 0;//清中断
    IEC0bits.OC1IE = 1;//中断允许

    OC1CONbits.OCM = 0b101;//将OCx引脚初始化为低电平，在OCx引脚上产生连续输出脉冲
}
void Timer2_Timing_Init(void)//定时器模式
{
    T2CONbits.TON = 0;//禁止16位Timer2
    T2CONbits.TCS = 0;//内部时钟Fosc/2
    T2CONbits.TGATE = 0;//门控时间累加位 禁止
    T2CONbits.TCKPS = 0x00;//输入时钟预分频比1:1
    T2CONbits.TSIDL = 0;//空闲模式继续工作
    T2CONbits.T32 = 0;//TMR2和TMR C类构成独立的16位定时器

    IPC1bits.T2IP = 0x01; // 优先级1
    IFS0bits.T2IF = 0; // Timer2中断标志状态位 清零
    IEC0bits.T2IE = 1; // Timer2中断允许位 允许
    T2CONbits.TON = 1;//启动使能16位Timer2
}
int main()
{
    System_Colck();
    System_Init();
    Timer2_Timing_Init();
    OC1_Init();

    while(1)
    {
    }
}
/* OC1中断*/
void __attribute__((__interrupt__, auto_psv)) _OC1Interrupt(void)
{
    IFS0bits.OC1IF = 0;// 清中断
}
/* 定时器2中断*/
void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0;  //清中断
}
