/*********************************************************************/
/*文件说明：I2C实验*/
/*硬件配置:	MCU:dsPIC30F4011开发板, OSC: 使用片外7.3728MHzX16PLL=117.9648MHz振荡器.*/
/*软件配置:开发环境MPLAB X IDE v2.35  编译器XC16 v1.21              */

/*使用说明:OLED显示*/

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//配置位
_FOSC(0Xc307);//FCKSM = 00;时钟切换使能 FOS = 0111主振荡器XT晶振模式外部7.3728MHz
_FWDT(0X0000);//关狗
_FBORPOR(0X83A2);//MCLREN使能 欠压2.7V 上电延时16MS
_FGS(0X03);
#define	SlaveAddress  0x3C //定义IIC总线中的从地址
unsigned int BUF[8];
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
void I2C_Write(unsigned char REG_Address,unsigned char REG_data)//I2C写字节函数
{
    I2CCONbits.SEN = 1;           //发送启动位
    while(I2CCONbits.SEN==1){}    //等待启动操作完成
    I2CTRN = SlaveAddress;           //发送设备地址+写信号
    while(I2CSTATbits.TRSTAT==1){}   //等待发送完成
    I2CTRN = REG_Address;//内部寄存器地址，请参考中文
    while(I2CSTATbits.TRSTAT==1){}   //等待发送完成
    I2CTRN = REG_data;//内部寄存器数据，请参考中文
    while(I2CSTATbits.TRSTAT==1){}   //等待发送完成
    I2CCONbits.PEN = 1;          //发送停止位
    while(I2CCONbits.PEN==1){}   //等待停止操作完成
}
void I2C_Read(void)//I2C读字节函数
{
    unsigned char i;
    I2CCONbits.SEN = 1;            //发送启动位
    while(I2CCONbits.SEN == 1){}   //等待启动操作完成
    I2CTRN = SlaveAddress;           //发送控制字：写准备
    while(I2CSTATbits.TRSTAT==1){}   //等待发送完成
    I2CTRN = 0x03;
    while(I2CSTATbits.TRSTAT==1){}   //等待发送完成
    I2CTRN = 0x03;
    while(I2CSTATbits.TRSTAT==1){}   //等待发送完成
    I2CCONbits.RSEN = 1;             //发送重启动位
    while(I2CCONbits.RSEN==1){}      //等待重启动操作完成
    I2CTRN = (SlaveAddress+1);           //发送控制字：读准备
    while(I2CSTATbits.TRSTAT==1){}   //等待发送完成
    for (i=0;i<7;i++)
	{
            I2CCONbits.RCEN = 1;         //接收数据使能
            while(I2CSTATbits.RBF==0){}
            BUF[i] = I2CRCV;//读出数据放BUF
            I2CCONbits.ACKDT = 0;
            if (i==6)
                I2CCONbits.ACKDT = 1;	 //最后一字节则不发ACK
            I2CCONbits.ACKEN = 1;
            while(I2CCONbits.ACKEN==1){}
    }
    I2CCONbits.PEN = 1;          //发送停止位
    while(I2CCONbits.PEN==1){}   //等待停止操作完成
    delay_ms(500);
}

int main()
{
    unsigned char i;
    System_Init();
    OLED_Init();			//初始化OLED
    OLED_Clear();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
  //  disn(1,1,36);disn(2,1,51);disn(3,1,48);disn(4,1,16);disn(5,1,45);disn(6,1,35);disn(7,1,53);
    while(1)
    {
        i ++;
        I2C_Write(0xaa,0xaa);//地址 数据
        disn(4,1,i/10000);disn(5,1,i%10000/1000);disn(6,1,i%1000/100);disn(7,1,i%100/10);disn(8,1,i%10);
        delay_ms(500);
    }
}

