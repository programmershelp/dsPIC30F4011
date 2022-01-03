/*********************************************************************/
/*�ļ�˵����LCD12864��ʾ*/
/*Ӳ������:	MCU:dsPIC30F4011������, OSC: ʹ��Ƭ��7.3728MHzX16PLL=117.9648MHz����.*/
/*�������:��������MPLAB X IDE v2.35  ������XC16 v1.21              */
//ע����� ����ʵ��ʱ�뽫AD-EN����ðȡ�»�ADC��λ�������м�λ�ã�����LCD���ܻ���ʾ����
/********************************************************************/
#include "p30f4011.h"
//����λ
_FOSC(0Xc304);//FCKSM = 00;ʱ���л�ʹ�� FOS = 0111������XT����ģʽ�ⲿ7.3728MHz
_FWDT(0X0000);//�ع�
_FBORPOR(0X83A2);//MCLRENʹ�� Ƿѹ2.7V �ϵ���ʱ16MS
_FGS(0X03);
#define uchar unsigned char
#define uint  unsigned int
/************* 12864LCD���Ŷ��� *************/
#define LCD_data  PORTB       //���ݿ�
#define LCD_RS LATDbits.LATD1//�Ĵ���ѡ������
#define LCD_RW  LATDbits.LATD3      //Һ����/д����
#define LCD_EN  LATDbits.LATD2    //Һ��ʹ�ܿ���
#define LCD_PSB LATCbits.LATC13      //��/����ʽ����
#define LCD_RST LATEbits.LATE8     //Һ����λ�˿�
/*********************************************************/
uchar  DIS1[] = {"****���Ƶ���*****"};
uchar  DIS2[] = {"****************"};
uchar  DIS3[] = {"****************"};
uchar  DIS4[] = {"***2017/07/06***"};
uchar temp[6]={0};
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
    LCD_PSB = 1;         //���ڷ�ʽ
    LCD_RST = 0;	 //Һ����λ
    delay(30);
    LCD_RST = 1;
    delay(30);
    lcd_wcmd(0x34);      //����ָ�����
    delay(50);
    lcd_wcmd(0x30);      //����ָ�����
    delay(50);
    lcd_wcmd(0x0C);      //��ʾ�����ع��
    delay(50);
    lcd_wcmd(0x01);      //���LCD����ʾ����
    delay(50);
}
void lcd_pos(uchar X,uchar Y)//д��ʾ����
{
   uchar  pos;
   if (X==1)
     {X=0x80;}
   else if (X==2)
     {X=0x90;}
   else if (X==3)
     {X=0x88;}
   else if (X==4)
     {X=0x98;}
   pos = X+Y ;

   lcd_wcmd(pos);     //��ʾ��ַ
}
void  clr_screen()//����
{
   lcd_wcmd(0x34);      //����ָ�����
   delay(5);
   lcd_wcmd(0x30);      //����ָ�����
   delay(5);
   lcd_wcmd(0x01);      //����
   delay(5);
}
void  bytecode()//�ַ�
{
   uchar  s;
   clr_screen();          //����
   lcd_wcmd(0x80);        //������ʾλ��Ϊ��һ��
   for(s=0;s<16;s++)
   {
     lcd_wdat(0x30+s);
   }
   lcd_wcmd(0x90);        //������ʾλ��Ϊ�ڶ���
   for(s=0;s<16;s++)
   {
     lcd_wdat(0x40+s);
   }
   lcd_wcmd(0x88);        //������ʾλ��Ϊ������
   for(s=0;s<16;s++)
   {
     lcd_wdat(0x50+s);
   }
   lcd_wcmd(0x98);       //������ʾλ��Ϊ������
   for(s=0;s<16;s++)
   {
     lcd_wdat(0x60+s);
   }
}
void Display(unsigned int NUM)
{
//    temp[0]=NUM/100000+0x30;
//    temp[1]=NUM%100000/10000+0x30;
//    temp[2]=NUM%100000%10000/1000+0x30;
//    temp[3]=NUM%100000%10000%1000/100+0x30;
//    temp[4]=NUM%100000%10000%1000%100/10+0x30;
//    temp[5]=NUM%100000%10000%1000%100%10+0x30;
    temp[0]=NUM%100000%10000%1000%100/10+0x30;
    temp[1]=NUM%100000%10000%1000%100%10+0x30;    
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
    TRISC = 0x0000;
    TRISB = 0x0000;//���
    PORTB = 0x0000;
    PWMCON1 = 0x0000;//ͨ��IO
    TRISE = 0x0000;//�������
}
int main()
{
    unsigned char i;
    unsigned char s;
    System_Colck();
    System_Init();
    lcd_init(); //��ʼ��LCD
    bytecode();//�ַ���ʾ
    clr_screen();//����
    delay(1000);//�ϵ磬�ȴ��ȶ�
    lcd_wcmd(0x80);//дָ��
    lcd_pos(1,0);//������ʾλ��Ϊ��0��
     for(i=0;i<16;i++)
     {
        lcd_wdat(DIS1[i]);//DIS1[] = {" X "};
        delay(30);
     }
     lcd_pos(2,0);             //������ʾλ��Ϊ��1��
     for(i=0;i<16;i++)
     {
       lcd_wdat(DIS2[i]);//DIS2[] = {"  Y:           "};
       delay(30);
     }
     lcd_pos(3,0);             //������ʾλ��Ϊ��2��
     for(i=0;i<16;i++)
     {
       lcd_wdat(DIS3[i]);//DIS3[] = {"  Z:           "};
       delay(30);
     }
     lcd_pos(4,0);             //������ʾλ��Ϊ��4��
     for(i=0;i<16;i++)
     {
       lcd_wdat(DIS4[i]);//DIS4[] = {" ANGLE:          "};
       delay(30);
     }
    while(1)
    {
        s ++;
        if(s>30)
            s = 0;
        Display(s);
        lcd_wcmd(0x9e);        //������ʾλ��Ϊ��4��
        for(i=0;i<2;i++)
        {
           lcd_wdat(temp[i]); // ��ʾ�ַ�
        }
        PORTEbits.RE0 = 0;
        delay(6000);
        PORTEbits.RE0 = 1;
        delay(6000);
    }
}
