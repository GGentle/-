#include <reg52.h>

#define uchar unsigned char
#define uint  unsigned int

#define shudu 30
#define dengji 1

sbit B1=P1^5;
sbit G1=P1^4;
sbit R1=P1^3;

uchar Rout,Gout,Bout;

void delay_led(uint z)
{
 uint x,y;
 for(x=0;x<5;x++)
 for(y=0;y<z;y++);
}


void RGBpwm(uchar Rou,uchar Gou,uchar Bou)
{

 if(Rou!=0)
 {
  R1=0;
  delay_led(Rou);
  R1=1;
 }
 if(Gou!=0)
 {
  G1=0;
  delay_led(Gou);
  G1=1;
 }
 if(Bou!=0)
 {
  B1=0;
  delay_led(Bou);
  B1=1;
 } 
}
  

void main()
{
 TMOD=0x01;
 EA=1;
 ET0=1;
 TR0=1;
 
 R1=1;
 G1=1;
 B1=1;
 
 TH0=(65536-256)/256;
 TL0=(65536-256)%256;
 while(1)
 {
  RGBpwm(Rout,Gout,Bout);
 }
}



void zhong() interrupt 1
{
 uint num;
 uint ji;
 TH0=(65536-256)/256;
 TL0=(65536-256)%256;
 num++;
 if(num==shudu)
 {
  num=0;
  ji++;
  if((ji>0)&&(ji<100))
  {
   Rout=240;
   Gout=0;
   Bout=0;
  }
  if((ji>100)&&(ji<340))
  {
   Rout-=dengji;
   Gout+=dengji;
   Bout=0;
  }
  if((ji>340)&&(ji<440))
  {
   Rout=0;
   Gout=240;
   Bout=0;
  }
  if((ji>440)&&(ji<680))
  {
   Rout=0;
   Gout-=dengji;
   Bout+=dengji;
  }
  if((ji>680)&&(ji<780))
  {
   Rout=0;
   Gout=0;
   Bout=240;
  }
  if((ji>780)&&(ji<1020))
  {
   Rout+=dengji;
   Gout+=dengji;
   Bout=240;
  }
  if((ji>1020)&&(ji<1120))
  {
   Rout=240;
   Gout=240;
   Bout=240;
  }
  if((ji>1120)&&(ji<1360))
  {
   Rout=240;
   Gout-=dengji;
   Bout-=dengji;
  }
  if(ji>1360)
  ji=0;
 }
}
