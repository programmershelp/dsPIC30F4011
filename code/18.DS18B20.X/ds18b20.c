#include"ds18b20.h"

void delay(unsigned int t)
{
unsigned int i,j;
for(i = 0;i <t;i ++)
    for(j = 0;j <100;j ++);

}

void Init_DS18B20(void)//Initialize ds1820
{
    unsigned char x=0;
    TRISFbits.TRISF6 = 0;
    DQ = 1; //DQ reset
    delay(8); //Slightly delay
    DQ = 0; //MCU pulls DQ low
    delay(80); //Precise delay is greater than 480us
    DQ = 1; //Pull the bus high
    delay(14);
    TRISFbits.TRISF6 = 1;
    x=DQ; //After a slight delay, if x=0, the initialization is successful x=1, the initialization fails
    delay(20);
    TRISFbits.TRISF6 = 0;
}

/************************************************* *****************************/
unsigned char ReadOneChar(void)//Read a byte
{
    unsigned char i=0;
    unsigned char dat = 0;
    TRISFbits.TRISF6 = 0;
    for (i=8;i>0;i--)
    {
        DQ = 0; // Give pulse signal
        dat>>=1;
        DQ = 1; // Give pulse signal
        TRISFbits.TRISF6 = 1;
        if(DQ)
        dat|=0x80;
        delay(4);
        TRISFbits.TRISF6 = 0;
    }
    return(dat);
}

/************************************************* *****************************/
void WriteOneChar(unsigned char dat) //write a byte
{
    unsigned char i=0;
    TRISFbits.TRISF6 = 0;
    for (i=8; i>0; i--)
    {
        DQ = 0;
        DQ = dat&0x01;
        delay(5);
        DQ = 1;
        dat>>=1;
    }
}

/************************************************* *****************************/
unsigned int ReadTemperature(void)//Read temperature
{
    unsigned char a=0;
    unsigned char b=0;
    unsigned int t=0;
    float tt=0;
    Init_DS18B20(); //First reset, perform temperature conversion
    WriteOneChar(0xCC); // Skip the operation of reading serial number and column number
    WriteOneChar(0x44); // Start temperature conversion
    Init_DS18B20(); // Second reset, read data
    WriteOneChar(0xCC); //Skip the operation of reading serial number column number
    WriteOneChar(0xBE); //Read temperature register
    a=ReadOneChar(); //Read the lower 8 bits
    b=ReadOneChar(); //Read the upper 8 bits
    tt = b * 16 + a / 16;
    t = (unsigned int)tt;
    return(t);
}
