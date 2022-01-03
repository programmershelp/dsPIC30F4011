/*********************************************************************/
/*�ļ�˵����I2Cʵ��*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */

/*ʹ��˵��:OLED��ʾ*/

/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//����λ
_FOSC(0Xc307);//FCKSM = 00;ʱ���л�ʹ�� FOS = 0111������XT����ģʽ�ⲿ7.3728MHz
_FWDT(0X0000);//�ع�
_FBORPOR(0X83A2);//MCLRENʹ�� Ƿѹ2.7V �ϵ���ʱ16MS
_FGS(0X03);
#define	SlaveAddress  0x3C //����IIC�����еĴӵ�ַ
unsigned int BUF[8];
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
void I2C_Write(unsigned char REG_Address,unsigned char REG_data)//I2Cд�ֽں���
{
    I2CCONbits.SEN = 1;           //��������λ
    while(I2CCONbits.SEN==1){}    //�ȴ������������
    I2CTRN = SlaveAddress;           //�����豸��ַ+д�ź�
    while(I2CSTATbits.TRSTAT==1){}   //�ȴ��������
    I2CTRN = REG_Address;//�ڲ��Ĵ�����ַ����ο�����
    while(I2CSTATbits.TRSTAT==1){}   //�ȴ��������
    I2CTRN = REG_data;//�ڲ��Ĵ������ݣ���ο�����
    while(I2CSTATbits.TRSTAT==1){}   //�ȴ��������
    I2CCONbits.PEN = 1;          //����ֹͣλ
    while(I2CCONbits.PEN==1){}   //�ȴ�ֹͣ�������
}
void I2C_Read(void)//I2C���ֽں���
{
    unsigned char i;
    I2CCONbits.SEN = 1;            //��������λ
    while(I2CCONbits.SEN == 1){}   //�ȴ������������
    I2CTRN = SlaveAddress;           //���Ϳ����֣�д׼��
    while(I2CSTATbits.TRSTAT==1){}   //�ȴ��������
    I2CTRN = 0x03;
    while(I2CSTATbits.TRSTAT==1){}   //�ȴ��������
    I2CTRN = 0x03;
    while(I2CSTATbits.TRSTAT==1){}   //�ȴ��������
    I2CCONbits.RSEN = 1;             //����������λ
    while(I2CCONbits.RSEN==1){}      //�ȴ��������������
    I2CTRN = (SlaveAddress+1);           //���Ϳ����֣���׼��
    while(I2CSTATbits.TRSTAT==1){}   //�ȴ��������
    for (i=0;i<7;i++)
	{
            I2CCONbits.RCEN = 1;         //��������ʹ��
            while(I2CSTATbits.RBF==0){}
            BUF[i] = I2CRCV;//�������ݷ�BUF
            I2CCONbits.ACKDT = 0;
            if (i==6)
                I2CCONbits.ACKDT = 1;	 //���һ�ֽ��򲻷�ACK
            I2CCONbits.ACKEN = 1;
            while(I2CCONbits.ACKEN==1){}
    }
    I2CCONbits.PEN = 1;          //����ֹͣλ
    while(I2CCONbits.PEN==1){}   //�ȴ�ֹͣ�������
    delay_ms(500);
}

int main()
{
    unsigned char i;
    System_Init();
    OLED_Init();			//��ʼ��OLED
    OLED_Clear();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
  //  disn(1,1,36);disn(2,1,51);disn(3,1,48);disn(4,1,16);disn(5,1,45);disn(6,1,35);disn(7,1,53);
    while(1)
    {
        i ++;
        I2C_Write(0xaa,0xaa);//��ַ ����
        disn(4,1,i/10000);disn(5,1,i%10000/1000);disn(6,1,i%1000/100);disn(7,1,i%100/10);disn(8,1,i%10);
        delay_ms(500);
    }
}

