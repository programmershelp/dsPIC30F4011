/*********************************************************************/
/*�ļ�˵����ͨ���첽�շ���UART2 ����ͨѶʵ��*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */
/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//����λ
_FOSC(0Xc307);//FCKSM = 00;ʱ���л�ʹ�� FOS = 0111������XT����ģʽ�ⲿ7.3728MHz
_FWDT(0X0000);//�ع�
_FBORPOR(0X83A2);//MCLRENʹ�� Ƿѹ2.7V �ϵ���ʱ16MS
_FGS(0X03);
unsigned char Data;
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
void UART2_Init(void)
{
    U2MODE = 0x0000;//8λ���� ����żУ�� 1ֹͣλ
    U2MODEbits.UARTEN = 1;//ʹ��UART
    U2STA = 0x0000;//״̬�Ϳ��ƼĴ���
    U2BRG = ((117964800/4/16)/9600)-1;//117.9648MHz������9600�����ռ��㹫ʽ��
    IEC1bits.U2TXIE = 1;//�����ж�����
    IPC6bits.U2TXIP = 7;//�����ж����ȼ�7
    U2STAbits.UTXEN = 1;// ʹ�ܷ���
    IEC1bits.U2RXIE = 1;//�����ж�����
    IPC6bits.U2RXIP = 6;//�����ж����ȼ�6
}
void UART2_TX(unsigned char TXData)//��������
{
    while (U1STAbits.UTXBF);
        U2TXREG = TXData;
}
unsigned char UART2_RX(void)//��������
{
    unsigned char RXData;
    while (!U2STAbits.URXDA);
        RXData = U2RXREG;
    return RXData;
}
int main()
{
    System_Colck();
    System_Init();
    OLED_Init();			//��ʼ��OLED
    OLED_Clear();
    UART2_Init();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
  //  disn(1,1,36);disn(2,1,51);disn(3,1,48);disn(4,1,16);disn(5,1,45);disn(6,1,35);disn(7,1,53);
    while(1)
    {
       UART2_TX(UART2_RX());
        Data = UART2_RX();
        disn(4,1,Data/10000);disn(5,1,Data%10000/1000);disn(6,1,Data%1000/100);disn(7,1,Data%100/10);disn(8,1,Data%10);
    }
}
void __attribute__((__interrupt__, auto_psv)) _U2TXInterrupt(void)//�����ж�
{
    IFS1bits.U2TXIF = 0;	//���жϱ�־
}
void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void)//�����ж�
{
    IFS1bits.U2RXIF = 0; //���жϱ�־
}
