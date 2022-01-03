/*********************************************************************/
/*文件说明：Timer3实验*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */

/*使用说明:注意DSPIC大部分单周期指令FOSC/4 = 7.3728MHz/4 = 1.8432MHz*16 = 29.4912MHz
 *  1/29491200 = 0.00000003391s 也就是33.91ns，分频1:1 TMR1从0000到ffff变化
 * 33.91ns*65535 = 2222291.85ns= 2.2ms = 0.00222229185s */

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//配置位
_FOSC(0Xc307);//FCKSM = 00;时钟切换使能 FOS = 0111主振荡器XT晶振模式外部7.3728MHz
_FWDT(0X0000);//关狗
_FBORPOR(0X83A2);//MCLREN使能 欠压2.7V 上电延时16MS
_FGS(0X03);
unsigned int T,Time;//定义时间变量
unsigned char Flag;
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
    PORTE = 0x0000;//置高
}
void Timer3_Init(void)//定时器初始化
{
    T3CONbits.TON = 0;//定时器关闭
    T3CON = 0x0000;//空闲工作 预分频1:1 不用外部时钟 FOSC/4
    //T3CONbits.TSYNC = 1;//外部时钟
    IEC0bits.T3IE = 1;//允许Timer2中断请求
    INTCON1 |= 0X8000;//不要中断嵌套
    IPC1bits.T3IP = 0x07;//中断优先级7
    T3CONbits.TON = 1;//定时器使能
}
int main()
{
    unsigned int i;
    System_Colck();
    System_Init();
    OLED_Init();			//初始化OLED
    OLED_Clear();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
    //disn(1,1,36);disn(2,1,51);disn(3,1,48);disn(4,1,16);disn(5,1,45);disn(6,1,35);disn(7,1,53);
    Timer3_Init();
    while(1)
    {
        disn(5,1,T/1000);disn(6,1,T%1000/100);disn(7,1,T%100/10);disn(8,1,T%10);
        if(Flag == 1)
        {
            Flag = 0;
            i ++;
            if(i > 0x3f)
            i = 0;
            PORTE = i;
        } 
    }
}
void __attribute__((__interrupt__, auto_psv)) _T3Interrupt(void)//定时器1中断
{

	IFS0bits.T3IF = 0;//清中断
	//定时器中断处理函数
	Time ++;
	if(Time == 450)
	{
		Time = 0;
        Flag = 1;
                T ++;
	}
}

