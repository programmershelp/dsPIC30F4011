/*********************************************************************/
/*�ļ�˵����IC1ʵ��*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */

/*ʹ��˵��:��INT1����ʵ��IC1���벶׽
 ע��DSPIC�󲿷ֵ�����ָ��FOSC/4 = 4MHz/4 = 1MHz  1/1000000 = 0.000001s
           Ҳ����1us����Ƶ1:1 TMR1��0000��ffff�仯0.000001*65535 = 0.065535��*/

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//����λ
_FOSC(0Xc307);//FCKSM = 00;ʱ���л�ʹ�� FOS = 0111������XT����ģʽ�ⲿ7.3728MHz
_FWDT(0X0000);//�ع�
_FBORPOR(0X83A2);//MCLRENʹ�� Ƿѹ2.7V �ϵ���ʱ16MS
_FGS(0X03);
unsigned int CaptureValue[5] = {0,0,0,0,0};//������β�׽ֵ�ݴ�����
unsigned int CaptureDiffValue[4] = {0,0,0,0};//�����������β�׽ֵ��һ�ײ��ֵ
void Delay_1ms(unsigned int t)//t = 1000���1s
{
    unsigned int i,j;
    for(i = 0;i < t;i ++)
        for(j = 0;j < 2000;j ++);
}
//��������
void System_Colck(void)
{
    //����Fosc = 7.3728MHz 117.9648MHz 30MIPS
    while (OSCCONbits.COSC!= 0b011)
    while (OSCCONbits.LOCK!= 1) {};//PLL ��������״̬
}
void System_Init(void)
{
    TRISCbits.TRISC13 = 0;//RC13 LED��
    TRISCbits.TRISC14 = 1;//RC14 ����
    INTCON1 = 0x0000; //���ж�
    INTCON2 = 0x0000;
    IEC0 = 0x0000; //IECx�жϽ�ֹ
    IEC1 = 0x0000;
    IEC2 = 0x0000;
}
void Timer1_Init(void)//��ʱ����ʼ��
{
    T1CONbits.TON = 0;//��ʱ���ر�
    T1CON = 0x0000;//���й��� Ԥ��Ƶ1:1 �����ⲿʱ�� FOSC/4
    T1CONbits.TSYNC = 1;//�ⲿʱ��
     PR1 = 0x4e20; //��ʱ��Timer1���ڼĴ���PR1��0x4e20��10ms��ֵ
    IEC0bits.T1IE = 1;//����Timer1�ж�����
    INTCON1 |= 0X8000;//��Ҫ�ж�Ƕ��
    IPC0bits.T1IP = 0x07;//�ж����ȼ�7
    T1CONbits.TON = 1;//��ʱ��ʹ��
}
void Timer3_Init(void)//��ʱ����ʼ��
{
    TMR3 = 0;//��ʱ��3�����Ĵ���TMR3=0
    T3CON = 0x0000;//ʹ���ڲ�ʱ�ӣ�Ԥ��Ƶ��Ϊ1��1
    T3CONbits.TON = 1;//�򿪶�ʱ��TMR3
}
void IC1_Init(void)
{
    TRISD |= 0x0001;//RD0 IC1��������ģʽ
    PORTDbits.RD0 = 0;
    IC1CON = 0x0002;//��׽Timer3,ÿ1�β�׽�¼��ж�һ�Σ�ÿһ���½��ز�׽һ��
    IFS0bits.IC1IF = 0;//��IC6�жϱ�־
    IEC0bits.IC1IE = 1;//ʹ���ж�
    IPC0bits.IC1IP = 6;//�����ж����ȼ�6
}
int main()
{
    System_Colck();
    System_Init();
    Timer1_Init();
    Timer3_Init();//��ʱ����ʼ��
    IC1_Init();
    OLED_Init();			//��ʼ��OLED
    OLED_Clear();
    while(1)
    {
       disn(4,0,CaptureDiffValue[0]/10000);disn(5,0,CaptureDiffValue[0]%10000/1000);disn(6,0,CaptureDiffValue[0]%1000/100);disn(7,0,CaptureDiffValue[0]%100/10);disn(8,0,CaptureDiffValue[0]%10);
       disn(4,1,CaptureDiffValue[1]/10000);disn(5,1,CaptureDiffValue[1]%10000/1000);disn(6,1,CaptureDiffValue[1]%1000/100);disn(7,1,CaptureDiffValue[1]%100/10);disn(8,1,CaptureDiffValue[1]%10);
       disn(4,2,CaptureDiffValue[2]/10000);disn(5,2,CaptureDiffValue[2]%10000/1000);disn(6,2,CaptureDiffValue[2]%1000/100);disn(7,2,CaptureDiffValue[2]%100/10);disn(8,2,CaptureDiffValue[2]%10);
       disn(4,3,CaptureDiffValue[3]/10000);disn(5,3,CaptureDiffValue[3]%10000/1000);disn(6,3,CaptureDiffValue[3]%1000/100);disn(7,3,CaptureDiffValue[3]%100/10);disn(8,3,CaptureDiffValue[3]%10);
    }
}

void __attribute__((__interrupt__, auto_psv))_T1Interrupt(void)
{
    IFS0bits.T1IF = 0;
    PORTCbits.RC13 = ~PORTCbits.RC13;
}

void __attribute__((__interrupt__, auto_psv))_IC1Interrupt(void)
{
    unsigned int i = 0;
    unsigned long CaptureDiffValueTemp = 0;
    for(i=0; i<4; i++)//��һ�ײ��ֵ
    {
        CaptureDiffValue[i] = (CaptureValue[i+1] - CaptureValue[i]);
        if(CaptureDiffValue[i] == 0)
        {
            CaptureDiffValueTemp = 0xffff + CaptureDiffValue[i+1] - CaptureDiffValue[i];
            CaptureDiffValue[i] = CaptureDiffValueTemp;
        }
        CaptureValue[i] = CaptureValue[i+1];
    }
    CaptureValue[4] = IC1BUF;//��ȡ��׽ֵ
    IFS0bits.IC1IF = 0;
}

