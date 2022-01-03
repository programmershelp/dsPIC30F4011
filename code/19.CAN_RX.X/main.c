/*********************************************************************/
/*�ļ�˵����CAN����ʵ��*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */

/*ʹ��˵��:OLED��ʾ*/

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
// FOSC
#pragma config FPR = FRC_PLL16          // Primary Oscillator Mode (FRC w/ PLL 16x)
#pragma config FOS = FRC                // Oscillator Source (Internal Fast RC)
#pragma config FCKSMEN = CSW_FSCM_ON    // Clock Switching and Monitor (Sw Enabled, Mon Enabled)
// FWDT
#pragma config FWPSB = WDTPSB_1         // WDT Prescaler B (1:1)
#pragma config FWPSA = WDTPSA_1         // WDT Prescaler A (1:1)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)
// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV27          // Brown Out Voltage (2.7V)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config LPOL = PWMxL_ACT_HI      // Low-side PWM Output Polarity (Active High)
#pragma config HPOL = PWMxH_ACT_HI      // High-side PWM Output Polarity (Active High)
#pragma config PWMPIN = RST_PWMPIN      // PWM Output Pin Reset (Control with HPOL/LPOL bits)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)
// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)
// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)
//����λ
unsigned int buffer1;                                 //����CAN���ջ��嵥Ԫ
unsigned int buffer2;
unsigned int buffer3;
unsigned int buffer4;
void Delay_1ms(unsigned int t);//t = 1000���1s
void System_Init(void);
void System_Init(void)//ϵͳ��ʼ��
{
    OSCCON = 0x33e0;//������ ������������ 4MHz64��Ƶ PLL������ʹ�ܣ�
    TRISB = 0x0000;
    TRISE |= 0x0100;//RE8��������
    INTCON1=0x0000;
    INTCON2=0x0000;//�ر������ж�
}
void CAN_Init(void)
{
    //can set
    C1CTRLbits.REQOP = 0x4; //�����������ģʽ
    while (C1CTRLbits.OPMODE!=0x4){}  //ȷ�Ͻ�������ģʽ
    C1TX0CON = 0x0003;      //����0��������,�����ȼ�
    C1TX0SID = 0x0020;      //����0��׼��ʶ����11λ
    C1TX0DLC = 0x01c0;      //����0���ݳ��ȣ�8�ֽ�
    C1RX0CON = 0x0000;      //����0��������
    C1RX0DLC = 0x0008;      //����0���ݳ��ȣ�8�ֽ�
    C1RXF0SID = 0x0020;     //�����˲���0��׼��ʶ����11λ
    C1RXM0SID = 0xffff;     //��������0��׼��ʶ��
    C1CFG1bits.SJW=0;	    //SJW=1 x TQ
    C1CFG1bits.BRP = 9;     //125K
    C1CFG2 = 0x03Ac;        //SEG2PH=5��SEG1PH=6��PRSEG=4

    C1CTRLbits.REQOP = 0x0; //�����������ģʽ
    while (C1CTRLbits.OPMODE!=0x0){}  //ȷ�Ͻ�������ģʽ
	return;
}
void Delay_1ms(unsigned int t)//t = 1000���1s
{
    unsigned int i,j;
    for(i = 0;i < t;i ++)
        for(j = 0;j < 100;j ++);
}

int main()
{
    unsigned int i = 0,j = 0;
    System_Init();
    OLED_Init();			//��ʼ��OLED
    OLED_Clear();
    CAN_Init();
    disn(2,0,35);disn(3,0,33);disn(4,0,46); disn(6,0,2);disn(7,0,24);disn(8,0,0); disn(10,0,33);disn(11,0,25);disn(12,0,34);//CAN 2.0 A/B
    disn(4,1,35);disn(5,1,33);disn(6,1,46);disn(8,1,50);disn(9,1,56);
    disn(8,2,0);disn(9,2,0);disn(10,2,0);disn(11,2,0);disn(12,2,0);
    disn(8,3,0);disn(9,3,0);disn(10,3,0);disn(11,3,0);disn(12,3,0);
    while(1)
    {
        if(C1RX0CONbits.RXFUL==1)     //���յ�����
        {
            buffer1 = C1RX0B1;          //��ȡ���յ�������
            buffer2 = C1RX0B2;
            buffer3 = C1RX0B3;
            buffer4 = C1RX0B4;
            C1RX0CONbits.RXFUL = 0;
            i = buffer1;
            j = buffer2;
            if(i == 1)
            {
                disn(0,2,46);disn(1,2,26);disn(2,2,0);disn(3,2,1);
                disn(0,3,j/10000);disn(1,3,j%10000/1000);disn(2,3,j%1000/100);disn(3,3,j%100/10);disn(4,3,j%10);
            }
            if(i == 2)
            {
                disn(8,2,46);disn(9,2,26);disn(10,2,0);disn(11,2,2);
                disn(8,3,j/10000);disn(9,3,j%10000/1000);disn(10,3,j%1000/100);disn(11,3,j%100/10);disn(12,3,j%10);
            }
        }
    }
}
