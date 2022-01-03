/*********************************************************************/
/*文件说明：SPI实验*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */

/*使用说明:SPI实验*/

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
//void SPI_Init_TX(void)
//{
//   IFS0bits.SPI1IF = 0;//清零SPI1中断状态标志位
//   IEC0bits.SPI1IE = 1;//SPI1中断允许
//   IPC2bits.SPI1IP = 6;//中断优先级 7为最高优先级中断
//   SPI1CONbits.DISSDO = 0;//SDOx引脚由模块控制
//   SPI1CONbits.MODE16 = 0;//通信为字节宽 （8位）
//   SPI1CONbits.SMP = 1;//输入数据在数据输出时间末尾采样
//   SPI1CONbits.SSEN = 0;//模块不使用SS引脚。引脚由端口功能控制。
//   SPI1CONbits.MSTEN = 1;//主控模式
//   SPI1CONbits.SPRE = 7;//辅预分频比1:1
//   SPI1CONbits.PPRE = 3;//主预分频比1:1
//   SPI1STATbits.SPIROV = 0;//溢出清零
//   SPI1STATbits.SPIEN = 1;//使能模块并将SCKx、SDOx、SDIx和SSx配置为串口引脚
//}
void SPI_Init_RX(void)
{
    SPI1BUF = 0;
    IFS0bits.SPI1IF = 0;//清零SPI1中断状态标志位
    IEC0bits.SPI1IE = 1;//SPI1中断允许
    IPC2bits.SPI1IP = 6;//中断优先级 7为最高优先级中断
    SPI1CONbits.DISSDO = 0;//SDOx引脚由模块控制
    SPI1CONbits.MODE16 = 0;//通信为字节宽 （8位）
    SPI1CONbits.SMP = 0;//当SPI在从动模式下使用时，必须将SMP清零。
    SPI1CONbits.SSEN = 0;//模块不使用SS引脚。引脚由端口功能控制。
    SPI1CONbits.MSTEN = 0;//从动模式
    SPI1CONbits.SPRE = 7;//辅预分频比1:1
    SPI1CONbits.PPRE = 3;//主预分频比1:1
    SPI1STATbits.SPIROV = 0;//溢出清零
    SPI1STATbits.SPIEN = 1;//使能模块并将SCKx、SDOx、SDIx和SSx配置为串口引脚
}
int main()
{
    unsigned int i = 0;
    System_Init();
    //SPI_Init_TX();
    SPI_Init_RX();
    OLED_Init();			//初始化OLED
    OLED_Clear();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
  //  disn(1,1,36);disn(2,1,51);disn(3,1,48);disn(4,1,16);disn(5,1,45);disn(6,1,35);disn(7,1,53);
    System_Colck();
    while(1)
    {
//        /*TX*/
//        i ++;
//        if(i > 255)
//            i = 0;
//        SPI1BUF = i;
//        disn(4,1,i/10000);disn(5,1,i%10000/1000);disn(6,1,i%1000/100);disn(7,1,i%100/10);disn(8,1,i%10);
       Delay_1ms(5000);
        /*RX*/
        i = SPI1BUF;
        disn(4,1,i/10000);disn(5,1,i%10000/1000);disn(6,1,i%1000/100);disn(7,1,i%100/10);disn(8,1,i%10);
    }
}

void __attribute__((__interrupt__, auto_psv)) _SPI1Interrupt(void)
{
    IFS0bits.SPI1IF = 0;//中断标志清零

}
