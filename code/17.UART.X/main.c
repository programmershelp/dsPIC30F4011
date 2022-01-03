/*********************************************************************/
/*文件说明：通用异步收发器UART2 串口通讯实验*/
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
unsigned char Data;
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
void UART2_Init(void)
{
    U2MODE = 0x0000;//8位数据 无奇偶校验 1停止位
    U2MODEbits.UARTEN = 1;//使能UART
    U2STA = 0x0000;//状态和控制寄存器
    U2BRG = ((117964800/4/16)/9600)-1;//117.9648MHz波特率9600（按照计算公式）
    IEC1bits.U2TXIE = 1;//发送中断允许
    IPC6bits.U2TXIP = 7;//发送中断优先级7
    U2STAbits.UTXEN = 1;// 使能发送
    IEC1bits.U2RXIE = 1;//接收中断允许
    IPC6bits.U2RXIP = 6;//接收中断优先级6
}
void UART2_TX(unsigned char TXData)//发送数据
{
    while (U1STAbits.UTXBF);
        U2TXREG = TXData;
}
unsigned char UART2_RX(void)//接收数据
{
    unsigned char RXData;
    while (!U2STAbits.URXDA);
        RXData = U2RXREG;
    return RXData;
}
int main()
{
    System_Colck();
    System_Init();
    OLED_Init();			//初始化OLED
    OLED_Clear();
    UART2_Init();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
  //  disn(1,1,36);disn(2,1,51);disn(3,1,48);disn(4,1,16);disn(5,1,45);disn(6,1,35);disn(7,1,53);
    while(1)
    {
       UART2_TX(UART2_RX());
        Data = UART2_RX();
        disn(4,1,Data/10000);disn(5,1,Data%10000/1000);disn(6,1,Data%1000/100);disn(7,1,Data%100/10);disn(8,1,Data%10);
    }
}
void __attribute__((__interrupt__, auto_psv)) _U2TXInterrupt(void)//发送中断
{
    IFS1bits.U2TXIF = 0;	//清中断标志
}
void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void)//接收中断
{
    IFS1bits.U2RXIF = 0; //清中断标志
}
