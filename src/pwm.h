/********************************************/
//ģ��S2-----��Ƭ��P2.1
//ģ��S3-----��Ƭ��P2.0
//ģ��OUT----��Ƭ��P3.5(������1����)
//ģ��VCC----��Ƭ��VCC
//ģ��GND----��Ƭ��GND
//**********************************************
#ifndef PWM_H
#define PWM_H


#define uchar unsigned char
#define uint  unsigned int	
#define DataPort P0	   //LCD1602 ���ݶ˿�

	
sbit    LCM_RS=P1^0;   //LCD1602 ���ƶ˿�		
sbit    LCM_RW=P1^1;   //LCD1602 ���ƶ˿�	
sbit    LCM_EN=P2^5;   //LCD1602 ���ƶ˿�

sbit    Led_red=P1^3;
sbit    Led_green=P1^4;
sbit    Led_blue=P1^5;

/**���Ŷ���**/  
sbit s2=P2^1;        //TCS3200 S2 
sbit s3=P2^0;        //TCS3200 S3
                     //TCS3200 S0 ģ���ڲ�Ĭ������
                     //TCS3200 S1 ģ���ڲ�Ĭ������
                     //TCS3200 OE ģ���ڲ��ӵ�
sbit test_pin=P1^2;  //��ʾ������������ţ���֪����ʱ���ж�Ƶ��
//��������������


uchar ge,shi,bai ;
uchar rp=2,gp=2,bp=2; //����������ӣ����廷�������޸�
uchar count;          //��ɫ��־λ��0:�� 1:�� 2:����

//��ʾ����
uchar disp_R[3];  //��
uchar disp_G[3];  //��
uchar disp_B[3];  //��

//********���庯��*****************************

void    delay(unsigned int k);
void    InitLcd();
void    WriteDataLCM(uchar dataW);
void    WriteCommandLCM(uchar CMD,uchar Attribc);
void    DisplayOneChar(uchar X,uchar Y,uchar DData);
uchar   led_pwm(uchar a,uchar b,uchar c);

#endif

