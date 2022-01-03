/*********************************************************************/
/*�ļ�˵��������Ƚ�ʵ��*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */

/*ʹ��˵��:RD0(OC1)�öŰ��߽ӵ�LED3���LED
 * ע��DSPIC�󲿷ֵ�����ָ��FOSC/4 = 7.3728MHz/4 = 1.8432MHz*16 = 29.4912MHz
 *  1/29491200 = 0.00000003391s Ҳ����33.91ns����Ƶ1:1 TMR2��0000��ffff�仯
 * 33.91ns*65535 = 2222291.85ns= 2.2ms = 0.00222229185s        */

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//����λ
_FOSC(0Xc307);//FCKSM = 00;ʱ���л�ʹ�� FOS = 0111������XT����ģʽ�ⲿ7.3728MHz
_FWDT(0X0000);//�ع�
_FBORPOR(0X83A2);//MCLRENʹ�� Ƿѹ2.7V �ϵ���ʱ16MS
_FGS(0X03);
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
    PWMCON1 = 0x0000;//ͨ��IO
    TRISE = 0x0000;//�������
    PORTE = 0x0000;//
}
void OC1_Init(void)
{
    OC1CONbits.OCM = 0b000;//��ֹ����Ƚ�
    OC1CONbits.OCSIDL = 0;//����ģʽ����
    OC1CONbits.OCTSEL = 0;//�Ƚ�Timer2��ʱ��Դ
    TMR2  =0x0000; //TMR2��ֵ0x0006
    PR2 = 10; //���ڼĴ�����ֵ0x002f,TMR2��0x002f��λ
    OC1R = 5; //����ƥ��ֵ0x001a����TMR2���ֵƥ��ʱ����������������
    OC1RS = 10;	//����ƥ��ֵ0x002a����TMR2���ֵƥ��ʱ�����������½���

    IPC0bits.OC1IP = 0x02;//�ж����ȼ�1
    IFS0bits.OC1IF = 0;//���ж�
    IEC0bits.OC1IE = 1;//�ж�����

    OC1CONbits.OCM = 0b101;//��OCx���ų�ʼ��Ϊ�͵�ƽ����OCx�����ϲ��������������
}
void Timer2_Timing_Init(void)//��ʱ��ģʽ
{
    T2CONbits.TON = 0;//��ֹ16λTimer2
    T2CONbits.TCS = 0;//�ڲ�ʱ��Fosc/2
    T2CONbits.TGATE = 0;//�ſ�ʱ���ۼ�λ ��ֹ
    T2CONbits.TCKPS = 0x00;//����ʱ��Ԥ��Ƶ��1:1
    T2CONbits.TSIDL = 0;//����ģʽ��������
    T2CONbits.T32 = 0;//TMR2��TMR C�๹�ɶ�����16λ��ʱ��

    IPC1bits.T2IP = 0x01; // ���ȼ�1
    IFS0bits.T2IF = 0; // Timer2�жϱ�־״̬λ ����
    IEC0bits.T2IE = 1; // Timer2�ж�����λ ����
    T2CONbits.TON = 1;//����ʹ��16λTimer2
}
int main()
{
    System_Colck();
    System_Init();
    Timer2_Timing_Init();
    OC1_Init();

    while(1)
    {
    }
}
/* OC1�ж�*/
void __attribute__((__interrupt__, auto_psv)) _OC1Interrupt(void)
{
    IFS0bits.OC1IF = 0;// ���ж�
}
/* ��ʱ��2�ж�*/
void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0;  //���ж�
}
