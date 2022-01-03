/************************************************* ********************/
/*File description: key input experiment INT1 (RD0) key press RE LED flashes*/
/*Hardware configuration: MCU: dsPIC30F4011 development board, OSC: use off-chip 7.3728MHz X 16 PLL=117.9648MHz oscillator.*/
/*Software configuration: development environment MPLAB X IDE v5.10 compiler XC16 v1.50 */
/************************************************* *******************/
#include "p30f4011.h"

//Configuration bits
_FOSC(0Xc307); //FCKSM = 00; clock switch enable FOS = 0111 main oscillator XT crystal mode external 7.3728MHz
_FWDT(0X0000);
_FBORPOR(0X83A2);//MCLREN enable Undervoltage 2.7V Power-up delay 16MS
_FGS(0X03);

void Delay_1ms(unsigned int t)//t = 1000 about 1s
{
    unsigned int i,j;
    for(i = 0;i <t;i ++)
        for(j = 0;j <2000;j ++);
}

//Oscillator configuration
void System_Clock(void)
{
    //Generate Fosc = 7.3728MHz 117.9648MHz 30MIPS
    while (OSCCONbits.COSC!= 0b011)
    while (OSCCONbits.LOCK!= 1) {};//PLL is locked
}

void System_Init(void)
{
    PWMCON1 = 0x0000; //General IO
    TRISE = 0x0000; //Set output
    PORTE = 0xffff;//Set high
    TRISCbits.TRISC14 = 1; //RC13 port is set as input
    TRISDbits.TRISD0 = 1; //RC13 port is set as input
}

int main()
{
    System_Clock();
    System_Init();
    while(1)
    {
        if((PORTCbits.RC14 == 0)||(PORTDbits.RD0 == 0))
        {
            Delay_1ms(20);
            if(PORTDbits.RD0 == 1)
            {
                //while(!PORTCbits.RC14);//Wait for the key to be released
                //while(!PORTDbits.RD0);//Wait for the key to be released
                PORTE = 0xAA;
                Delay_1ms(500);
            }
            if(PORTCbits.RC14 == 1)
            {
                //while(!PORTCbits.RC14);//Wait for the key to be released
                //while(!PORTDbits.RD0);//Wait for the key to be released
                PORTE = 0x55;
                Delay_1ms(500);
            }
        }
        PORTE = 0x0000;
    }
}
