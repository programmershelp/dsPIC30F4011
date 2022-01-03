/*********************************************************************/
/*�ļ�˵����Timer3ʵ��*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */

/*ʹ��˵��:ע��DSPIC�󲿷ֵ�����ָ��FOSC/4 = 7.3728MHz/4 = 1.8432MHz*16 = 29.4912MHz
 *  1/29491200 = 0.00000003391s Ҳ����33.91ns����Ƶ1:1 TMR1��0000��ffff�仯
 * 33.91ns*65535 = 2222291.85ns= 2.2ms = 0.00222229185s */

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//����λ
_FOSC(0Xc307);//FCKSM = 00;ʱ���л�ʹ�� FOS = 0111������XT����ģʽ�ⲿ7.3728MHz
_FWDT(0X0000);//�ع�
_FBORPOR(0X83A2);//MCLRENʹ�� Ƿѹ2.7V �ϵ���ʱ16MS
_FGS(0X03);
unsigned int T,Time;//����ʱ�����
unsigned char Flag;
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
    PORTE = 0x0000;//�ø�
}
void Timer3_Init(void)//��ʱ����ʼ��
{
    T3CONbits.TON = 0;//��ʱ���ر�
    T3CON = 0x0000;//���й��� Ԥ��Ƶ1:1 �����ⲿʱ�� FOSC/4
    //T3CONbits.TSYNC = 1;//�ⲿʱ��
    IEC0bits.T3IE = 1;//����Timer2�ж�����
    INTCON1 |= 0X8000;//��Ҫ�ж�Ƕ��
    IPC1bits.T3IP = 0x07;//�ж����ȼ�7
    T3CONbits.TON = 1;//��ʱ��ʹ��
}
int main()
{
    unsigned int i;
    System_Colck();
    System_Init();
    OLED_Init();			//��ʼ��OLED
    OLED_Clear();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
    //disn(1,1,36);disn(2,1,51);disn(3,1,48);disn(4,1,16);disn(5,1,45);disn(6,1,35);disn(7,1,53);
    Timer3_Init();
    while(1)
    {
        disn(5,1,T/1000);disn(6,1,T%1000/100);disn(7,1,T%100/10);disn(8,1,T%10);
        if(Flag == 1)
        {
            Flag = 0;
            i ++;
            if(i > 0x3f)
            i = 0;
            PORTE = i;
        } 
    }
}
void __attribute__((__interrupt__, auto_psv)) _T3Interrupt(void)//��ʱ��1�ж�
{

	IFS0bits.T3IF = 0;//���ж�
	//��ʱ���жϴ�����
	Time ++;
	if(Time == 450)
	{
		Time = 0;
        Flag = 1;
                T ++;
	}
}

