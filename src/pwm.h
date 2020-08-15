/********************************************/
//模块S2-----单片机P2.1
//模块S3-----单片机P2.0
//模块OUT----单片机P3.5(计数器1输入)
//模块VCC----单片机VCC
//模块GND----单片机GND
//**********************************************
#ifndef PWM_H
#define PWM_H


#define uchar unsigned char
#define uint  unsigned int	
#define DataPort P0	   //LCD1602 数据端口

	
sbit    LCM_RS=P1^0;   //LCD1602 控制端口		
sbit    LCM_RW=P1^1;   //LCD1602 控制端口	
sbit    LCM_EN=P2^5;   //LCD1602 控制端口

sbit    Led_red=P1^3;
sbit    Led_green=P1^4;
sbit    Led_blue=P1^5;

/**引脚定义**/  
sbit s2=P2^1;        //TCS3200 S2 
sbit s3=P2^0;        //TCS3200 S3
                     //TCS3200 S0 模块内部默认上拉
                     //TCS3200 S1 模块内部默认上拉
                     //TCS3200 OE 模块内部接地
sbit test_pin=P1^2;  //用示波器看这个引脚，可知道定时器中断频率
//变量、常量定义


uchar ge,shi,bai ;
uchar rp=2,gp=2,bp=2; //定义比例因子，具体环境可以修改
uchar count;          //颜色标志位（0:红 1:绿 2:蓝）

//显示数组
uchar disp_R[3];  //红
uchar disp_G[3];  //绿
uchar disp_B[3];  //蓝

//********定义函数*****************************

void    delay(unsigned int k);
void    InitLcd();
void    WriteDataLCM(uchar dataW);
void    WriteCommandLCM(uchar CMD,uchar Attribc);
void    DisplayOneChar(uchar X,uchar Y,uchar DData);
uchar   led_pwm(uchar a,uchar b,uchar c);

#endif

