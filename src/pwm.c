#include<REG52.H>	
#include<math.h>       //Keil library  
#include<stdio.h>      //Keil library	
#include<INTRINS.H>
#include "pwm.h"
#include "led_pwm.h"



//*********LCD1602��ʼ��**********************
void InitLcd()				
{			
	WriteCommandLCM(0x38,1);	
	WriteCommandLCM(0x08,1);	
	WriteCommandLCM(0x01,1);
	WriteCommandLCM(0x06,1);	
	WriteCommandLCM(0x0c,1);
}

//**********���æ�ź�************************
void WaitForEnable(void)	
{					
	DataPort=0xff;		
	LCM_RS=0;LCM_RW=1;_nop_();
	LCM_EN=1;_nop_();_nop_();
	while(DataPort&0x80);	
	LCM_EN=0;				
}
					
//**********д������LCD***********************
void WriteCommandLCM(uchar CMD,uchar Attribc)
{					
	if(Attribc)WaitForEnable();	
	LCM_RS=0;LCM_RW=0;_nop_();
	DataPort=CMD;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}	
				
//**********д������LCD************************
void WriteDataLCM(uchar dataW)
{					
	WaitForEnable();		
	LCM_RS=1;LCM_RW=0;_nop_();
	DataPort=dataW;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}
					
//*********дһ���ַ����ݵ�ָ����Ŀ��***********
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{						
	Y&=1;						
	X&=15;						
	if(Y)X|=0x40;					
	X|=0x80;			
	WriteCommandLCM(X,0);		
	WriteDataLCM(DData);		
}

//**********��ʱ����***************
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
* ��������: t0_init()
* ��������: ��ʱ��0��ʼ��
* ��ڲ���: ��
* ���ڲ���: ��
/********************************************/
void t0_init()
{
     TMOD=0x51;        //T1���� T0��ʱ ������ʽ1
     
     TH0=(65536-40000)/256;
     TL0=(65536-40000)%256;

     TH0=0xE0;
     TL0=0x00;        //11��0592M ����10ms
     EA=1;            //���ж�

     ET0=1;        
     TR0=1;           //����
     TR1=1;
}

//*********************************************
//��ֵת������ʮ��ǧ��ASCII��
//*********************************************
void conversion(uint temp_data)  
{  
    bai=temp_data/100+0x30 ;
    temp_data=temp_data%100;   //ȡ������
    shi=temp_data/10+0x30 ;
    ge=temp_data%10+0x30;      //ȡ������
}

 /*******************************************
* ��������: c10ms_out() 
* ��������: ��ʱ�ж�0�������
            �޸���ɫ��־disp_tc��0:�� 1:�� 2:����
            ����S0 S1 S2 ѡ���˲���
            �������壬��ȡɫֵ
/********************************************/
void c10ms_out() interrupt 1
{       uint temp;
        test_pin=!test_pin; //���Զ�ʱ���ж�Ƶ�����ţ�������ʾ�����۲�
        TR0=0;              //�رն�ʱ
        TR1=0;              //�رռ���
 //   count+1ʵ���ȼ����ɫ,�ټ����ɫ,Ȼ�����ɫ,ѭ�����       
        if(count==0)
        {
        count++;    
        s2=1;s3=1;             //ѡ���˲���Ϊ��ɫ     
	  					 
		temp=(8<<TH1)+TL1;    //�������ʱ���� TCS230 �����������	
		temp/=rp;			
        conversion(temp);
		disp_R[2]=ge;         //��Ϊ��ε��жϣ����ϴ�ѡ���˲�������ֵ
		disp_R[1]=shi;
		disp_R[0]=bai;
        } 

        else if(count==1)
        {            
        count++;
        s2=0;s3=1;            //ѡ���˲���Ϊ��ɫ

		temp=(8<<TH1)+TL1;    //�������ʱ���� TCS230 �����������	
		temp/=gp;			
        conversion(temp);
		disp_G[2]=ge;         //��Ϊ��ε��жϣ����ϴ�ѡ���˲�������ֵ
		disp_G[1]=shi;
		disp_G[0]=bai;
        }

        else if(count==2)
        {            
        count=0;
        s2=0;s3=0;            //ѡ���˲���Ϊ��ɫ
      
		temp=(8<<TH1)+TL1;    //�������ʱ���� TCS230 �����������	
		temp/=bp;		
        conversion(temp);
  	    disp_B[2]=ge;         //��Ϊ��ε��жϣ����ϴ�ѡ���˲�������ֵ
		disp_B[1]=shi;
		disp_B[0]=bai;

        }

     //��ʱ���������ظ���ֵ
     TH0=(65536-40000)/256;
     TL0=(65536-40000)%256; //11��0592M ����Ϊ10ms
     TL1=0x00;//����������
     TH1=0x00;//����������
     TR0=1;   //�򿪶�ʱ��
     TR1=1;   //�򿪼�����
}


/*******************************************
* ��������: main()
/********************************************/
void main()
{

      delay(10); 
      InitLcd();      //lcd��ʼ��
	  s2=0;           //��ʼ�趨S2����
	  s3=0;           //��ʼ�趨S3����
      t0_init();      //��ʱ������ʹ��

     while(1)
     {
     
        DisplayOneChar(0,0,'G'); 
		DisplayOneChar(1,0,':'); 
		DisplayOneChar(2,0,disp_G[0]); 
		DisplayOneChar(3,0,disp_G[1]); 
		DisplayOneChar(4,0,disp_G[2]); 		//��
		RGBpwm(led_pwm(disp_R[0],disp_R[1],disp_R[2]),led_pwm(disp_G[0],disp_G[1],disp_G[2]),led_pwm(disp_B[0],disp_B[1],disp_B[2])) ;

        DisplayOneChar(6,0,'B'); 
		DisplayOneChar(7,0,':'); 
		DisplayOneChar(8,0,disp_B[0]); 
		DisplayOneChar(9,0,disp_B[1]); 
		DisplayOneChar(10,0,disp_B[2]);     //��
		RGBpwm(led_pwm(disp_R[0],disp_R[1],disp_R[2]),led_pwm(disp_G[0],disp_G[1],disp_G[2]),led_pwm(disp_B[0],disp_B[1],disp_B[2])) ;

		DisplayOneChar(0,1,'R'); 
		DisplayOneChar(1,1,':'); 
		DisplayOneChar(2,1,disp_R[0]); 
		DisplayOneChar(3,1,disp_R[1]); 
		DisplayOneChar(4,1,disp_R[2]); 	    //��				
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

