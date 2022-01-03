/*********************************************************************/
/*文件说明：IC1实验*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */

/*使用说明:按INT1按键实现IC1输入捕捉
 注意DSPIC大部分单周期指令FOSC/4 = 4MHz/4 = 1MHz  1/1000000 = 0.000001s
           也就是1us，分频1:1 TMR1从0000到ffff变化0.000001*65535 = 0.065535秒*/

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//配置位
_FOSC(0Xc307);//FCKSM = 00;时钟切换使能 FOS = 0111主振荡器XT晶振模式外部7.3728MHz
_FWDT(0X0000);//关狗
_FBORPOR(0X83A2);//MCLREN使能 欠压2.7V 上电延时16MS
_FGS(0X03);
unsigned int CaptureValue[5] = {0,0,0,0,0};//连续五次捕捉值暂存数组
unsigned int CaptureDiffValue[4] = {0,0,0,0};//保存相邻两次捕捉值的一阶差分值
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
    TRISCbits.TRISC13 = 0;//RC13 LED灯
    TRISCbits.TRISC14 = 1;//RC14 按键
    INTCON1 = 0x0000; //关中断
    INTCON2 = 0x0000;
    IEC0 = 0x0000; //IECx中断禁止
    IEC1 = 0x0000;
    IEC2 = 0x0000;
}
void Timer1_Init(void)//定时器初始化
{
    T1CONbits.TON = 0;//定时器关闭
    T1CON = 0x0000;//空闲工作 预分频1:1 不用外部时钟 FOSC/4
    T1CONbits.TSYNC = 1;//外部时钟
     PR1 = 0x4e20; //定时器Timer1周期寄存器PR1＝0x4e20，10ms初值
    IEC0bits.T1IE = 1;//允许Timer1中断请求
    INTCON1 |= 0X8000;//不要中断嵌套
    IPC0bits.T1IP = 0x07;//中断优先级7
    T1CONbits.TON = 1;//定时器使能
}
void Timer3_Init(void)//定时器初始化
{
    TMR3 = 0;//定时器3计数寄存器TMR3=0
    T3CON = 0x0000;//使用内部时钟，预分频比为1：1
    T3CONbits.TON = 1;//打开定时器TMR3
}
void IC1_Init(void)
{
    TRISD |= 0x0001;//RD0 IC1设置输入模式
    PORTDbits.RD0 = 0;
    IC1CON = 0x0002;//捕捉Timer3,每1次捕捉事件中断一次，每一个下降沿捕捉一次
    IFS0bits.IC1IF = 0;//清IC6中断标志
    IEC0bits.IC1IE = 1;//使能中断
    IPC0bits.IC1IP = 6;//设置中断优先级6
}
int main()
{
    System_Colck();
    System_Init();
    Timer1_Init();
    Timer3_Init();//定时器初始化
    IC1_Init();
    OLED_Init();			//初始化OLED
    OLED_Clear();
    while(1)
    {
       disn(4,0,CaptureDiffValue[0]/10000);disn(5,0,CaptureDiffValue[0]%10000/1000);disn(6,0,CaptureDiffValue[0]%1000/100);disn(7,0,CaptureDiffValue[0]%100/10);disn(8,0,CaptureDiffValue[0]%10);
       disn(4,1,CaptureDiffValue[1]/10000);disn(5,1,CaptureDiffValue[1]%10000/1000);disn(6,1,CaptureDiffValue[1]%1000/100);disn(7,1,CaptureDiffValue[1]%100/10);disn(8,1,CaptureDiffValue[1]%10);
       disn(4,2,CaptureDiffValue[2]/10000);disn(5,2,CaptureDiffValue[2]%10000/1000);disn(6,2,CaptureDiffValue[2]%1000/100);disn(7,2,CaptureDiffValue[2]%100/10);disn(8,2,CaptureDiffValue[2]%10);
       disn(4,3,CaptureDiffValue[3]/10000);disn(5,3,CaptureDiffValue[3]%10000/1000);disn(6,3,CaptureDiffValue[3]%1000/100);disn(7,3,CaptureDiffValue[3]%100/10);disn(8,3,CaptureDiffValue[3]%10);
    }
}

void __attribute__((__interrupt__, auto_psv))_T1Interrupt(void)
{
    IFS0bits.T1IF = 0;
    PORTCbits.RC13 = ~PORTCbits.RC13;
}

void __attribute__((__interrupt__, auto_psv))_IC1Interrupt(void)
{
    unsigned int i = 0;
    unsigned long CaptureDiffValueTemp = 0;
    for(i=0; i<4; i++)//求一阶差分值
    {
        CaptureDiffValue[i] = (CaptureValue[i+1] - CaptureValue[i]);
        if(CaptureDiffValue[i] == 0)
        {
            CaptureDiffValueTemp = 0xffff + CaptureDiffValue[i+1] - CaptureDiffValue[i];
            CaptureDiffValue[i] = CaptureDiffValueTemp;
        }
        CaptureValue[i] = CaptureValue[i+1];
    }
    CaptureValue[4] = IC1BUF;//获取捕捉值
    IFS0bits.IC1IF = 0;
}

