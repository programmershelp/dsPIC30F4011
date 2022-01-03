/*********************************************************************/
/*�ļ�˵����10λADCʵ��*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */

/*ʹ��˵��:����VR1����ADCʵ��*/

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//����λ
_FOSC(0Xc307);//FCKSM = 00;ʱ���л�ʹ�� FOS = 0111������XT����ģʽ�ⲿ7.3728MHz
_FWDT(0X0000);//�ع�
_FBORPOR(0X83A2);//MCLRENʹ�� Ƿѹ2.7V �ϵ���ʱ16MS
_FGS(0X03);
unsigned int ADC_Data;
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
void ADC_Init(void)
{
    TRISBbits.TRISB0 = 1;//����
    ADCON1 = 0x0000;//�޷���������ʽ
    ADCON1bits.SSRC = 7;//�ڲ�������������������ʼת�����Զ�ת����
    ADCON1bits.ASAM = 1;//�����Զ� �ϴ�ת��������ʼ
    ADCON2 = 0x0000;//�ο���ѹ����AVDD,AVSS;һ������/ת����ɺ�����ж�
    ADCON3 = 0x1f1f; //ʹ��ϵͳʱ�ӣ��������β���֮���ʱ����Ϊ31Tad��Tad=32Tcy
    ADCHS = 0x0000;////ͨ��0��������Vref-,ͨ��0��������AN2
    ADPCFGbits.PCFG2 = 0;//AN2ģ��ģʽ����ֹ�˿ڶ�ȡ����,A/D�������ŵ�ѹ
    ADCSSL = 0x0000;//������������ɨ��
    IPC2bits.ADIP = 7; //����ADת���ж����ȼ�Ϊ7
    IEC0bits.ADIE = 1; //ʹ��ADת������ж�
    ADCON1bits.ADON = 1; //����ADC
}
int main()
{
    System_Colck();
    System_Init();
    OLED_Init();			//��ʼ��OLED
    OLED_Clear();
    ADC_Init();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
  //  disn(1,1,36);disn(2,1,51);disn(3,1,48);disn(4,1,16);disn(5,1,45);disn(6,1,35);disn(7,1,53);

    while(1)
    {
       disn(4,1,ADC_Data/10000);disn(5,1,ADC_Data%10000/1000);disn(6,1,ADC_Data%1000/100);disn(7,1,ADC_Data%100/10);disn(8,1,ADC_Data%10);
       delay_ms(1000);
    }
}
void __attribute__((interrupt, no_auto_psv)) _ADCInterrupt (void)
{
    IFS0bits.ADIF=0; //��ADת���жϱ�־λ
    ADC_Data = ADCBUF0;
}

