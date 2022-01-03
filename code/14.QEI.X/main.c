/*********************************************************************/
/*文件说明：QEI实验*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */

/*使用说明:QEI实验*/
/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//配置位
_FOSC(0Xc307);//FCKSM = 00;时钟切换使能 FOS = 0111主振荡器XT晶振模式外部7.3728MHz
_FWDT(0X0000);//关狗
_FBORPOR(0X83A2);//MCLREN使能 欠压2.7V 上电延时16MS
_FGS(0X03);
int QEI_Position = 0;//获取位置
int AngPos[2]={0,0};//用速度计算的两个变量
unsigned int AngPos_QEI = 0;
int POSCNTcopy = 0;
unsigned long Speed;//角速度
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
void QEI_Init(void)
{
    ADPCFG |= 0x0038;//设置为1 即QEI引脚为数字输入
    QEICONbits.QEIM = 0;//禁止QEI模块
    QEICONbits.CNTERR = 0;//清计数错误
    QEICONbits.QEISIDL = 0;//睡眠器件连续工作
    QEICONbits.SWPAB = 0;//QEA和QEB未交换
    QEICONbits.PCDOUT = 0;//位置计数器方向状态输出禁止 标准I/O引脚工作 正常IO引脚工作
    QEICONbits.POSRES = 1;//索引脉冲可使位置计数器复位

    DFLTCONbits.CEID = 1;//禁止计数错误中断
    DFLTCONbits.QEOUT = 1;//数字滤波器输出使能
    DFLTCONbits.QECK = 5;//1:64时钟分频 101
    //DFLTCONbits.INDOUT = 1;//索引通道数字滤波器输出使能位dsPIC20F6011
    //DFLTCONbits.INDCK = 5;//索引通道数字滤波器时钟分频选择位1:64分频//101  dsPIC20F6011
    POSCNT = 0;//复位位置计数器
    MAXCNT = 4000;//最大计数寄存器
    QEICONbits.QEIM=6;//正交编码器接口使能 （x4模式），索引脉冲复位位置计数器
    //启动
}
//计算角位置
void QEI_Angle(void)
{
    POSCNTcopy = POSCNT;//位置计数器
    AngPos[1] = AngPos[0];
    AngPos[0]= AngPos_QEI;

    AngPos_QEI = POSCNTcopy * 9;//放大100倍  POSCNTcopy*(360/4000)*100;
    
}
/*使用说明:注意DSPIC大部分单周期指令FOSC/4 = 7.3728MHz/4 = 1.8432MHz*16 = 29.4912MHz
 *  1/29491200 = 0.00000003391s 也就是33.91ns，分频1:1 TMR1从0000到ffff变化
 * 33.91ns*65535 = 2222291.85ns= 2.2ms = 0.00222229185s*/
void Timer1_Init(void)//定时器初始化
{
    T1CONbits.TON = 0;//定时器关闭
    T1CON = 0x0000;//空闲工作 预分频1:1 不用外部时钟 FOSC/4
    T1CONbits.TSYNC = 1;//外部时钟
    IEC0bits.T1IE = 1;//允许Timer1中断请求
    INTCON1 |= 0X8000;//不要中断嵌套
    IPC0bits.T1IP = 0x07;//中断优先级7
    T1CONbits.TON = 1;//定时器使能
}

void Delay_1ms(unsigned int t)//t = 1000大概1s
{
    unsigned int i,j;
    for(i = 0;i < t;i ++)
        for(j = 0;j < 100;j ++);
}

int main()
{
    System_Colck();
    System_Init();
    OLED_Init();			//初始化OLED
    OLED_Clear();
    QEI_Init();
    Timer1_Init();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
    disn(1,1,48);disn(2,1,47);disn(3,1,51);disn(4,1,35);disn(5,1,46);disn(6,1,52);disn(7,1,26);//POSCNT:
    disn(1,2,33);disn(2,2,78);disn(3,2,71);disn(4,2,48);disn(5,2,79);disn(6,2,83);disn(7,2,26);//AngPos:
    disn(0,3,51);disn(1,3,80);disn(2,3,69);disn(3,3,69);disn(4,3,68);disn(5,3,26);disn(11,3,50);disn(12,3,65);disn(13,3,68);disn(14,3,25);disn(15,3,83);//Speed:00000RPM

    while(1)
    {
        QEI_Position = POSCNT;
        QEI_Angle();
        disn(8,1,QEI_Position/10000);disn(9,1,QEI_Position%10000/1000);disn(10,1,QEI_Position%1000/100);disn(11,1,QEI_Position%100/10);disn(12,1,QEI_Position%10);
        
        disn(8,2,AngPos_QEI/10000);disn(9,2,AngPos_QEI%10000/1000);disn(10,2,AngPos_QEI%1000/100);disn(11,2,24);disn(12,2,AngPos_QEI%100/10);disn(13,2,AngPos_QEI%10);
        
        disn(6,3,Speed/10000);disn(7,3,Speed%10000/1000);disn(8,3,Speed%1000/100);
        disn(9,3,Speed%100/10);disn(10,3,Speed%10);
    }
}

void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)//定时器1中断
{
	IFS0bits.T1IF = 0;//清中断
    if(QEICONbits.UPDN == 1)//位置计数器方向为正 （ +）
        Speed = AngPos[0] - AngPos[1];
    if(QEICONbits.UPDN == 0)//位置计数器方向为负 （ -）
        Speed = AngPos[1] - AngPos[0];
    Speed /= 22;
    
}
