/*********************************************************************/
/*文件说明：EEPROM存储器实验*/
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
    PORTE = 0x0000;//置低
    TRISCbits.TRISC14 = 1;//RC14口设置为输入
}
void WriteConfig(int addr, int value) // 先擦除再写入
 {
	 TBLPAG=0x7f;
	 WREG1=addr+0xFC00; // Init Pointer
	 NVMCON=0x4044;
	 NVMKEY=0x55;
	 NVMKEY=0xAA;
	 _WR=1;
	 Nop();
	 Nop();
	 while(_WR);
	 TBLPAG=0x7f;
	 WREG1=addr+0xFC00; // Init Pointer
	 WREG0=value;
	 __asm__ volatile("TBLWTL W0,[W1]");
	 NVMCON=0x4004;
	 NVMKEY=0x55;
	 NVMKEY=0xAA;
	 _WR=1;
	 Nop();
	 Nop();
	 while(_WR);
 }

unsigned int ReadConfig(int addr) // 读取
{
    TBLPAG=0x7F;
    WREG1=addr+0xFC00; // Init Pointer
    __asm__ volatile("TBLRDL [W1],W4");  //read data EEPROM
    Nop();
    Nop();
    return WREG4;
}
int main()
{
    unsigned int OLED_Data = 0,OLED_Data1 = 0,i = 0;
    System_Colck();
    System_Init();
    OLED_Init();			//初始化OLED
    OLED_Clear();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome

    while(1)
    {
       if(PORTCbits.RC14 == 0)
        {
            delay_ms(20);
            if(PORTCbits.RC14 == 0)
            {
               while(!PORTCbits.RC14);
               OLED_Data ++;
               PORTE = OLED_Data;
               WriteConfig(4,OLED_Data);
            }

        }
        OLED_Data1 = ReadConfig(4);
        disn(5,1,OLED_Data1/1000);disn(6,1,OLED_Data1%1000/100);disn(7,1,OLED_Data1%100/10);disn(8,1,OLED_Data1%10);  
    }
}
