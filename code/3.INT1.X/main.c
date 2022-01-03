/*********************************************************************/
/*�ļ�˵����INT1�ⲿ�ж�ʵ��INT1�������½����ж�RE LED��˸*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */
/********************************************************************/
#include "p30f4011.h"
//����λ
_FOSC(0Xc307);//FCKSM = 00;ʱ���л�ʹ�� FOS = 0111������XT����ģʽ�ⲿ7.3728MHz
_FWDT(0X0000);//�ع�
_FBORPOR(0X83A2);//MCLRENʹ�� Ƿѹ2.7V �ϵ���ʱ16MS
_FGS(0X03);
unsigned char INT1_Flag = 0;//�ⲿ�жϱ�־
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
    PORTE = 0xffff;//�ø�
    TRISDbits.TRISD0 = 1;//RD0��������
    INTCON1 |= 0X8000;//��Ҫ�ж�Ƕ��
    IEC1bits.INT1IE = 1;;//�����ⲿ�ж�
    IPC4bits.INT1IP = 7;//������ȼ�7
    IFS1 = 0x0000;//���ⲿ�жϱ�־
    INTCON2bits.INT1EP = 1;//INT1�������ж�
}
int main()
{
    System_Colck();
    System_Init();
    while(1)
    {
        if(INT1_Flag == 1)
        {
            INT1_Flag = 0;
            PORTE =~ PORTE;
        }
    }
}
void __attribute__((__interrupt__, auto_psv)) _INT1Interrupt(void)//�жϴ�����
{
    Delay_1ms(20);//��������
    IFS1bits.INT1IF = 0;//�����ƽ�仯�ж�
     INT1_Flag = 1;
}

