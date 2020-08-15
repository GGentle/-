/*先亮红灯（保持一会儿）----红绿过度（绿加1、红减1循环240次）------
绿灯亮起(保持一会儿)----绿兰过度（兰加1、绿减1循环240次）
-----兰灯亮起（保持）-----兰白过度（绿加1、红加1循环240次、兰不变）*/
#ifndef LED_PWM_H
#define LED_PWM_H



#define uchar unsigned char
#define uint  unsigned int

#define shudu 30//LED渐变速度调整
#define dengji 1//LED变化等级调整

sbit B1=P1^5;//蓝色灯控制IO口
sbit G1=P1^4;//绿色灯控制IO口
sbit R1=P1^3;//红色灯控制IO口




/****************函数定义***********************/

void RGBpwm(uchar Rou,uchar Gou,uchar Bou);
void delay_led(uint z);
#endif