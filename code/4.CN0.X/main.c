/*********************************************************************/
/*文件说明：电平变化通知输入实验*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */
/*使用说明:按下CN0(即RC14 RUN按键)电平变化通知中断RE LED闪烁*/
/********************************************************************/
#include "p30f4011.h"
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
    PORTE = 0xffff;//置高
    TRISCbits.TRISC14 = 1;//设置为数字输入
    CNEN1bits.CN0IE = 1;//允许CN0引脚中断
    CNPU1bits.CN0PUE = 1;//打开弱上拉
    IFS0bits.CNIF = 0;//清中断标志
    IPC3bits.CNIP = 3;//优先级中断
    IEC0bits.CNIE = 1;//中断允许
    PORTC = PORTC;//读C口值，以锁存旧值
}
int main()
{
    System_Colck();
    System_Init();
    while(1)
    {
        if(CN0_Flag == 1)
        {
            CN0_Flag = 0;
            PORTE =~ PORTE;
        }
    }
}
void __attribute__((__interrupt__, auto_psv)) _CNInterrupt(void)
{
    Delay_1ms(20);
    if(PORTCbits.RC14 == 0)
        CN0_Flag = 1;
    PORTC = PORTC;//读C口值，以锁存旧值
    IFS0bits.CNIF = 0;//清中断标志

}

