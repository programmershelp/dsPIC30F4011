/*********************************************************************/
/*�ļ�˵������ƽ�仯֪ͨ����ʵ��*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */
/*ʹ��˵��:����CN0(��RC14 RUN����)��ƽ�仯֪ͨ�ж�RE LED��˸*/
/********************************************************************/
#include "p30f4011.h"
//����λ
_FOSC(0Xc307);//FCKSM = 00;ʱ���л�ʹ�� FOS = 0111������XT����ģʽ�ⲿ7.3728MHz
_FWDT(0X0000);//�ع�
_FBORPOR(0X83A2);//MCLRENʹ�� Ƿѹ2.7V �ϵ���ʱ16MS
_FGS(0X03);
unsigned char CN0_Flag = 0;//�ⲿ�жϱ�־
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
    TRISCbits.TRISC14 = 1;//����Ϊ��������
    CNEN1bits.CN0IE = 1;//����CN0�����ж�
    CNPU1bits.CN0PUE = 1;//��������
    IFS0bits.CNIF = 0;//���жϱ�־
    IPC3bits.CNIP = 3;//���ȼ��ж�
    IEC0bits.CNIE = 1;//�ж�����
    PORTC = PORTC;//��C��ֵ���������ֵ
}
int main()
{
    System_Colck();
    System_Init();
    while(1)
    {
        if(CN0_Flag == 1)
        {
            CN0_Flag = 0;
            PORTE =~ PORTE;
        }
    }
}
void __attribute__((__interrupt__, auto_psv)) _CNInterrupt(void)
{
    Delay_1ms(20);
    if(PORTCbits.RC14 == 0)
        CN0_Flag = 1;
    PORTC = PORTC;//��C��ֵ���������ֵ
    IFS0bits.CNIF = 0;//���жϱ�־

}

