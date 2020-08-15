#include<REG52.H>	
#include<math.h>       //Keil library  
#include<stdio.h>      //Keil library	
#include<INTRINS.H>
#include "pwm.h"
#include "led_pwm.h"



//*********LCD1602初始化**********************
void InitLcd()				
{			
	WriteCommandLCM(0x38,1);	
	WriteCommandLCM(0x08,1);	
	WriteCommandLCM(0x01,1);
	WriteCommandLCM(0x06,1);	
	WriteCommandLCM(0x0c,1);
}

//**********检测忙信号************************
void WaitForEnable(void)	
{					
	DataPort=0xff;		
	LCM_RS=0;LCM_RW=1;_nop_();
	LCM_EN=1;_nop_();_nop_();
	while(DataPort&0x80);	
	LCM_EN=0;				
}
					
//**********写命令至LCD***********************
void WriteCommandLCM(uchar CMD,uchar Attribc)
{					
	if(Attribc)WaitForEnable();	
	LCM_RS=0;LCM_RW=0;_nop_();
	DataPort=CMD;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}	
				
//**********写数据至LCD************************
void WriteDataLCM(uchar dataW)
{					
	WaitForEnable();		
	LCM_RS=1;LCM_RW=0;_nop_();
	DataPort=dataW;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}
					
//*********写一个字符数据到指定的目标***********
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{						
	Y&=1;						
	X&=15;						
	if(Y)X|=0x40;					
	X|=0x80;			
	WriteCommandLCM(X,0);		
	WriteDataLCM(DData);		
}

//**********延时函数***************
void delay(unsigned int k)	
{						
	unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
	for(j=0;j<121;j++)			
	{;}
	}						
}						            


/*******************************************
* 函数名称: t0_init()
* 函数功能: 定时器0初始化
* 入口参数: 无
* 出口参数: 无
/********************************************/
void t0_init()
{
     TMOD=0x51;        //T1计数 T0定时 工作方式1
     
     TH0=(65536-40000)/256;
     TL0=(65536-40000)%256;

     TH0=0xE0;
     TL0=0x00;        //11。0592M 晶振10ms
     EA=1;            //开中断

     ET0=1;        
     TR0=1;           //启动
     TR1=1;
}

//*********************************************
//数值转换出个十百千的ASCII码
//*********************************************
void conversion(uint temp_data)  
{  
    bai=temp_data/100+0x30 ;
    temp_data=temp_data%100;   //取余运算
    shi=temp_data/10+0x30 ;
    ge=temp_data%10+0x30;      //取余运算
}

 /*******************************************
* 函数名称: c10ms_out() 
* 函数功能: 定时中断0服务程序
            修改颜色标志disp_tc（0:红 1:绿 2:蓝）
            设置S0 S1 S2 选择滤波器
            计算脉冲，读取色值
/********************************************/
void c10ms_out() interrupt 1
{       uint temp;
        test_pin=!test_pin; //测试定时器中断频率引脚，可以用示波器观察
        TR0=0;              //关闭定时
        TR1=0;              //关闭计数
 //   count+1实现先检测绿色,再检测蓝色,然后检测红色,循环检测       
        if(count==0)
        {
        count++;    
        s2=1;s3=1;             //选择滤波器为绿色     
	  					 
		temp=(8<<TH1)+TL1;    //计算这段时间内 TCS230 的输出脉冲数	
		temp/=rp;			
        conversion(temp);
		disp_R[2]=ge;         //因为这次的中断，是上次选择滤波器的数值
		disp_R[1]=shi;
		disp_R[0]=bai;
        } 

        else if(count==1)
        {            
        count++;
        s2=0;s3=1;            //选择滤波器为蓝色

		temp=(8<<TH1)+TL1;    //计算这段时间内 TCS230 的输出脉冲数	
		temp/=gp;			
        conversion(temp);
		disp_G[2]=ge;         //因为这次的中断，是上次选择滤波器的数值
		disp_G[1]=shi;
		disp_G[0]=bai;
        }

        else if(count==2)
        {            
        count=0;
        s2=0;s3=0;            //选择滤波器为红色
      
		temp=(8<<TH1)+TL1;    //计算这段时间内 TCS230 的输出脉冲数	
		temp/=bp;		
        conversion(temp);
  	    disp_B[2]=ge;         //因为这次的中断，是上次选择滤波器的数值
		disp_B[1]=shi;
		disp_B[0]=bai;

        }

     //定时器计数器重赋初值
     TH0=(65536-40000)/256;
     TL0=(65536-40000)%256; //11。0592M 晶振，为10ms
     TL1=0x00;//计数器清零
     TH1=0x00;//计数器清零
     TR0=1;   //打开定时器
     TR1=1;   //打开计数器
}


/*******************************************
* 函数名称: main()
/********************************************/
void main()
{

      delay(10); 
      InitLcd();      //lcd初始化
	  s2=0;           //初始设定S2引脚
	  s3=0;           //初始设定S3引脚
      t0_init();      //定时计数初使化

     while(1)
     {
     
        DisplayOneChar(0,0,'G'); 
		DisplayOneChar(1,0,':'); 
		DisplayOneChar(2,0,disp_G[0]); 
		DisplayOneChar(3,0,disp_G[1]); 
		DisplayOneChar(4,0,disp_G[2]); 		//绿
		RGBpwm(led_pwm(disp_R[0],disp_R[1],disp_R[2]),led_pwm(disp_G[0],disp_G[1],disp_G[2]),led_pwm(disp_B[0],disp_B[1],disp_B[2])) ;

        DisplayOneChar(6,0,'B'); 
		DisplayOneChar(7,0,':'); 
		DisplayOneChar(8,0,disp_B[0]); 
		DisplayOneChar(9,0,disp_B[1]); 
		DisplayOneChar(10,0,disp_B[2]);     //蓝
		RGBpwm(led_pwm(disp_R[0],disp_R[1],disp_R[2]),led_pwm(disp_G[0],disp_G[1],disp_G[2]),led_pwm(disp_B[0],disp_B[1],disp_B[2])) ;

		DisplayOneChar(0,1,'R'); 
		DisplayOneChar(1,1,':'); 
		DisplayOneChar(2,1,disp_R[0]); 
		DisplayOneChar(3,1,disp_R[1]); 
		DisplayOneChar(4,1,disp_R[2]); 	    //红				
		RGBpwm(led_pwm(disp_R[0],disp_R[1],disp_R[2]),led_pwm(disp_G[0],disp_G[1],disp_G[2]),led_pwm(disp_B[0],disp_B[1],disp_B[2])) ;

        delay(100) ; 


     }
}


uchar led_pwm(uchar a,uchar b,uchar c)
{
   uchar sum;

   sum=(a-0x30)*100+(b-0x30)*10+(c-0x30);

   return sum;
}

