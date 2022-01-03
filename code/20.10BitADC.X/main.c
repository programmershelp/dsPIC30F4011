/*********************************************************************/
/*文件说明：10位ADC实验*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */

/*使用说明:调节VR1进行ADC实验*/

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//配置位
_FOSC(0Xc307);//FCKSM = 00;时钟切换使能 FOS = 0111主振荡器XT晶振模式外部7.3728MHz
_FWDT(0X0000);//关狗
_FBORPOR(0X83A2);//MCLREN使能 欠压2.7V 上电延时16MS
_FGS(0X03);
unsigned int ADC_Data;
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
void ADC_Init(void)
{
    TRISBbits.TRISB0 = 1;//输入
    ADCON1 = 0x0000;//无符号整数格式
    ADCON1bits.SSRC = 7;//内部计数器结束采样并开始转换（自动转换）
    ADCON1bits.ASAM = 1;//采样自动 上次转换结束后开始
    ADCON2 = 0x0000;//参考电压配置AVDD,AVSS;一个采样/转换完成后产生中断
    ADCON3 = 0x1f1f; //使用系统时钟，相邻两次采样之间的时间间隔为31Tad，Tad=32Tcy
    ADCHS = 0x0000;////通道0负输入是Vref-,通道0正输入是AN2
    ADPCFGbits.PCFG2 = 0;//AN2模拟模式，禁止端口读取输入,A/D采样引脚电压
    ADCSSL = 0x0000;//不对输入引脚扫描
    IPC2bits.ADIP = 7; //设置AD转换中断优先级为7
    IEC0bits.ADIE = 1; //使能AD转换完成中断
    ADCON1bits.ADON = 1; //启动ADC
}
int main()
{
    System_Colck();
    System_Init();
    OLED_Init();			//初始化OLED
    OLED_Clear();
    ADC_Init();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
  //  disn(1,1,36);disn(2,1,51);disn(3,1,48);disn(4,1,16);disn(5,1,45);disn(6,1,35);disn(7,1,53);

    while(1)
    {
       disn(4,1,ADC_Data/10000);disn(5,1,ADC_Data%10000/1000);disn(6,1,ADC_Data%1000/100);disn(7,1,ADC_Data%100/10);disn(8,1,ADC_Data%10);
       delay_ms(1000);
    }
}
void __attribute__((interrupt, no_auto_psv)) _ADCInterrupt (void)
{
    IFS0bits.ADIF=0; //清AD转换中断标志位
    ADC_Data = ADCBUF0;
}

