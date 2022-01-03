/*********************************************************************/
/*�ļ�˵����QEIʵ��*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */

/*ʹ��˵��:QEIʵ��*/
/********************************************************************/
#include "p30f4011.h"
#include "oled.h"
//����λ
_FOSC(0Xc307);//FCKSM = 00;ʱ���л�ʹ�� FOS = 0111������XT����ģʽ�ⲿ7.3728MHz
_FWDT(0X0000);//�ع�
_FBORPOR(0X83A2);//MCLRENʹ�� Ƿѹ2.7V �ϵ���ʱ16MS
_FGS(0X03);
int QEI_Position = 0;//��ȡλ��
int AngPos[2]={0,0};//���ٶȼ������������
unsigned int AngPos_QEI = 0;
int POSCNTcopy = 0;
unsigned long Speed;//���ٶ�
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
void QEI_Init(void)
{
    ADPCFG |= 0x0038;//����Ϊ1 ��QEI����Ϊ��������
    QEICONbits.QEIM = 0;//��ֹQEIģ��
    QEICONbits.CNTERR = 0;//���������
    QEICONbits.QEISIDL = 0;//˯��������������
    QEICONbits.SWPAB = 0;//QEA��QEBδ����
    QEICONbits.PCDOUT = 0;//λ�ü���������״̬�����ֹ ��׼I/O���Ź��� ����IO���Ź���
    QEICONbits.POSRES = 1;//���������ʹλ�ü�������λ

    DFLTCONbits.CEID = 1;//��ֹ���������ж�
    DFLTCONbits.QEOUT = 1;//�����˲������ʹ��
    DFLTCONbits.QECK = 5;//1:64ʱ�ӷ�Ƶ 101
    //DFLTCONbits.INDOUT = 1;//����ͨ�������˲������ʹ��λdsPIC20F6011
    //DFLTCONbits.INDCK = 5;//����ͨ�������˲���ʱ�ӷ�Ƶѡ��λ1:64��Ƶ//101  dsPIC20F6011
    POSCNT = 0;//��λλ�ü�����
    MAXCNT = 4000;//�������Ĵ���
    QEICONbits.QEIM=6;//�����������ӿ�ʹ�� ��x4ģʽ�����������帴λλ�ü�����
    //����
}
//�����λ��
void QEI_Angle(void)
{
    POSCNTcopy = POSCNT;//λ�ü�����
    AngPos[1] = AngPos[0];
    AngPos[0]= AngPos_QEI;

    AngPos_QEI = POSCNTcopy * 9;//�Ŵ�100��  POSCNTcopy*(360/4000)*100;
    
}
/*ʹ��˵��:ע��DSPIC�󲿷ֵ�����ָ��FOSC/4 = 7.3728MHz/4 = 1.8432MHz*16 = 29.4912MHz
 *  1/29491200 = 0.00000003391s Ҳ����33.91ns����Ƶ1:1 TMR1��0000��ffff�仯
 * 33.91ns*65535 = 2222291.85ns= 2.2ms = 0.00222229185s*/
void Timer1_Init(void)//��ʱ����ʼ��
{
    T1CONbits.TON = 0;//��ʱ���ر�
    T1CON = 0x0000;//���й��� Ԥ��Ƶ1:1 �����ⲿʱ�� FOSC/4
    T1CONbits.TSYNC = 1;//�ⲿʱ��
    IEC0bits.T1IE = 1;//����Timer1�ж�����
    INTCON1 |= 0X8000;//��Ҫ�ж�Ƕ��
    IPC0bits.T1IP = 0x07;//�ж����ȼ�7
    T1CONbits.TON = 1;//��ʱ��ʹ��
}

void Delay_1ms(unsigned int t)//t = 1000���1s
{
    unsigned int i,j;
    for(i = 0;i < t;i ++)
        for(j = 0;j < 100;j ++);
}

int main()
{
    System_Colck();
    System_Init();
    OLED_Init();			//��ʼ��OLED
    OLED_Clear();
    QEI_Init();
    Timer1_Init();
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
    disn(1,1,48);disn(2,1,47);disn(3,1,51);disn(4,1,35);disn(5,1,46);disn(6,1,52);disn(7,1,26);//POSCNT:
    disn(1,2,33);disn(2,2,78);disn(3,2,71);disn(4,2,48);disn(5,2,79);disn(6,2,83);disn(7,2,26);//AngPos:
    disn(0,3,51);disn(1,3,80);disn(2,3,69);disn(3,3,69);disn(4,3,68);disn(5,3,26);disn(11,3,50);disn(12,3,65);disn(13,3,68);disn(14,3,25);disn(15,3,83);//Speed:00000RPM

    while(1)
    {
        QEI_Position = POSCNT;
        QEI_Angle();
        disn(8,1,QEI_Position/10000);disn(9,1,QEI_Position%10000/1000);disn(10,1,QEI_Position%1000/100);disn(11,1,QEI_Position%100/10);disn(12,1,QEI_Position%10);
        
        disn(8,2,AngPos_QEI/10000);disn(9,2,AngPos_QEI%10000/1000);disn(10,2,AngPos_QEI%1000/100);disn(11,2,24);disn(12,2,AngPos_QEI%100/10);disn(13,2,AngPos_QEI%10);
        
        disn(6,3,Speed/10000);disn(7,3,Speed%10000/1000);disn(8,3,Speed%1000/100);
        disn(9,3,Speed%100/10);disn(10,3,Speed%10);
    }
}

void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)//��ʱ��1�ж�
{
	IFS0bits.T1IF = 0;//���ж�
    if(QEICONbits.UPDN == 1)//λ�ü���������Ϊ�� �� +��
        Speed = AngPos[0] - AngPos[1];
    if(QEICONbits.UPDN == 0)//λ�ü���������Ϊ�� �� -��
        Speed = AngPos[1] - AngPos[0];
    Speed /= 22;
    
}
