/*������ƣ�����һ�����----���̹��ȣ��̼�1�����1ѭ��240�Σ�------
�̵�����(����һ���)----�������ȣ�����1���̼�1ѭ��240�Σ�
-----�������𣨱��֣�-----���׹��ȣ��̼�1�����1ѭ��240�Ρ������䣩*/
#ifndef LED_PWM_H
#define LED_PWM_H



#define uchar unsigned char
#define uint  unsigned int

#define shudu 30//LED�����ٶȵ���
#define dengji 1//LED�仯�ȼ�����

sbit B1=P1^5;//��ɫ�ƿ���IO��
sbit G1=P1^4;//��ɫ�ƿ���IO��
sbit R1=P1^3;//��ɫ�ƿ���IO��




/****************��������***********************/

void RGBpwm(uchar Rou,uchar Gou,uchar Bou);
void delay_led(uint z);
#endif