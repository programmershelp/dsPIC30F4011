/*********************************************************************/
/*�ļ�˵����LCD1602��ʾ*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */
/********************************************************************/
#include "p30f4011.h"
//����λ
_FOSC(0Xc307);//FCKSM = 00;ʱ���л�ʹ�� FOS = 0111������XT����ģʽ�ⲿ7.3728MHz
_FWDT(0X0000);//�ع�
_FBORPOR(0X83A2);//MCLRENʹ�� Ƿѹ2.7V �ϵ���ʱ16MS
_FGS(0X03);
#define uchar unsigned char
#define uint  unsigned int
/************* LCD���Ŷ��� *************/
#define LCD_data  PORTB       //���ݿ�
#define LCD_RS LATDbits.LATD1//�Ĵ���ѡ������
#define LCD_RW  LATDbits.LATD3      //Һ����/д����
#define LCD_EN  LATDbits.LATD2    //Һ��ʹ�ܿ���
#define LCD_RST LATEbits.LATE8     //Һ����λ�˿�
/*********************************************************/
uchar  dis1[] = {"      Welcome    "};
uchar  dis2[] = {"       BOKE      "};

void delayNOP()//8us��ʱ
{
    uchar i;
    for(i = 0;i < 20;i ++)
    {
        asm("NOP");
    }
}
void delay(int ms)//������ʱ
{
    while(ms--)
	{
            uchar i;
            for(i=0;i<20;i++);
	}
}
void delay1(int ms)//������ʱ
{
    while(ms--)
	{
      uchar y;
	  for(y=0;y<20;y++) ;
	}
}
void Delay_1ms(unsigned int t)//t = 1000���1s
{
    unsigned int i,j;
    for(i = 0;i < t;i ++)
        for(j = 0;j < 2000;j ++);
}
uchar lcd_busy()//æ���
 {
    uchar result;
    TRISBbits.TRISB7 = 1;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    delay(5);
    if(PORTBbits.RB7 == 0)
    result = 0;
    LCD_EN = 0;
    TRISBbits.TRISB7 = 0;
    return(result);
 }
void lcd_wcmd(uchar cmd)//дָ������
{
//   while(lcd_busy());
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    PORTB = cmd;
    delay(5);
    LCD_EN = 1;
    delay(5);
    LCD_EN = 0;
}
void lcd_pos(uchar pos)
{                          //�趨��ʾλ��
lcd_wcmd(pos | 0x80);
}
void lcd_wdat(uchar dat)//д��ʾ����
{
 //  while(lcd_busy());
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    PORTB = dat;
    delay(5);
    LCD_EN = 1;
    delay(5);
    LCD_EN = 0;
}
void lcd_init()//��λ
{
    LCD_RST = 0;	 //Һ����λ
    delay(30);
    LCD_RST = 1;
    delay(30);
    lcd_wcmd(0x38);      //16*2��ʾ��5*7����8λ����
    delay(50);
    lcd_wcmd(0x0c);      //��ʾ�����ع��
    delay(50);
    lcd_wcmd(0x06);      //�ƶ����
    delay(50);
    lcd_wcmd(0x01);      //���LCD����ʾ����
    delay(50);
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
    TRISD = 0x0000;
    TRISB = 0x0000;//���
    PORTB = 0x0000;
    PWMCON1 = 0x0000;//ͨ��IO
    TRISE = 0x0000;//�������
}
int main()
{
    unsigned char i;

    System_Colck();
    System_Init();
    lcd_init();               // ��ʼ��LCD
    
    lcd_wcmd(0x06);            //�����ƶ����
        lcd_pos(0);                //������ʾλ��Ϊ��һ�еĵ�1���ַ�
        i = 0;
        while(dis1[ i ] != '\0')
        {                           //��ʾ�ַ�"WLCOME"
            lcd_wdat(dis1[ i ]);
            i++;
            delay(300);                //��������֮����ʾ�ٶ�
        }
        lcd_pos(0x40);             //������ʾλ��Ϊ�ڶ��е�1���ַ�
        i = 0;
        while(dis2[ i ] != '\0')
        {
            lcd_wdat(dis2[ i ]);      //��ʾ�ַ�" BOKE  "
            i++;
            delay(300);                //��������֮����ʾ�ٶ�
        }
    while(1)
    {
        i ++;
        PORTE = i;
        Delay_1ms(1000);
    }
}
