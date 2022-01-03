/*********************************************************************/
/*�ļ�˵����SPIʵ��*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */

/*ʹ��˵��:SPIʵ��*/

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
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
    PORTE = 0x0000;//
}
//void SPI_Init_TX(void)
//{
//   IFS0bits.SPI1IF = 0;//����SPI1�ж�״̬��־λ
//   IEC0bits.SPI1IE = 1;//SPI1�ж�����
//   IPC2bits.SPI1IP = 6;//�ж����ȼ� 7Ϊ������ȼ��ж�
//   SPI1CONbits.DISSDO = 0;//SDOx������ģ�����
//   SPI1CONbits.MODE16 = 0;//ͨ��Ϊ�ֽڿ� ��8λ��
//   SPI1CONbits.SMP = 1;//�����������������ʱ��ĩβ����
//   SPI1CONbits.SSEN = 0;//ģ�鲻ʹ��SS���š������ɶ˿ڹ��ܿ��ơ�
//   SPI1CONbits.MSTEN = 1;//����ģʽ
//   SPI1CONbits.SPRE = 7;//��Ԥ��Ƶ��1:1
//   SPI1CONbits.PPRE = 3;//��Ԥ��Ƶ��1:1
//   SPI1STATbits.SPIROV = 0;//�������
//   SPI1STATbits.SPIEN = 1;//ʹ��ģ�鲢��SCKx��SDOx��SDIx��SSx����Ϊ��������
//}
void SPI_Init_RX(void)
{
    SPI1BUF = 0;
    IFS0bits.SPI1IF = 0;//����SPI1�ж�״̬��־λ
    IEC0bits.SPI1IE = 1;//SPI1�ж�����
    IPC2bits.SPI1IP = 6;//�ж����ȼ� 7Ϊ������ȼ��ж�
    SPI1CONbits.DISSDO = 0;//SDOx������ģ�����
    SPI1CONbits.MODE16 = 0;//ͨ��Ϊ�ֽڿ� ��8λ��
    SPI1CONbits.SMP = 0;//��SPI�ڴӶ�ģʽ��ʹ��ʱ�����뽫SMP���㡣
    SPI1CONbits.SSEN = 0;//ģ�鲻ʹ��SS���š������ɶ˿ڹ��ܿ��ơ�
    SPI1CONbits.MSTEN = 0;//�Ӷ�ģʽ
    SPI1CONbits.SPRE = 7;//��Ԥ��Ƶ��1:1
    SPI1CONbits.PPRE = 3;//��Ԥ��Ƶ��1:1
    SPI1STATbits.SPIROV = 0;//�������
    SPI1STATbits.SPIEN = 1;//ʹ��ģ�鲢��SCKx��SDOx��SDIx��SSx����Ϊ��������
}
int main()
{
    unsigned int i = 0;
    System_Init();
    //SPI_Init_TX();
    SPI_Init_RX();
    OLED_Init();			//��ʼ��OLED
    OLED_Clear();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
  //  disn(1,1,36);disn(2,1,51);disn(3,1,48);disn(4,1,16);disn(5,1,45);disn(6,1,35);disn(7,1,53);
    System_Colck();
    while(1)
    {
//        /*TX*/
//        i ++;
//        if(i > 255)
//            i = 0;
//        SPI1BUF = i;
//        disn(4,1,i/10000);disn(5,1,i%10000/1000);disn(6,1,i%1000/100);disn(7,1,i%100/10);disn(8,1,i%10);
       Delay_1ms(5000);
        /*RX*/
        i = SPI1BUF;
        disn(4,1,i/10000);disn(5,1,i%10000/1000);disn(6,1,i%1000/100);disn(7,1,i%100/10);disn(8,1,i%10);
    }
}

void __attribute__((__interrupt__, auto_psv)) _SPI1Interrupt(void)
{
    IFS0bits.SPI1IF = 0;//�жϱ�־����

}
