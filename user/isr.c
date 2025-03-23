///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,��ɿƼ�
// * All rights reserved.
// * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
// *
// * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
// * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
// *
// * @file       		isr
// * @company	   		�ɶ���ɿƼ����޹�˾
// * @author     		��ɿƼ�(QQ790875685)
// * @version    		�鿴doc��version�ļ� �汾˵��
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC32G12K128
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/
#include "main.h"
#include "encoder.h"
#include "filter.h"
#include "motor.h"
#include "pid.h"


//UART1�ж�
void UART1_Isr() interrupt 4
{
//    uint8 res;
//	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
//        res = SBUF;
//        //�����Զ�����
//        if(res == 0x7F)
//        {
//            if(dwon_count++ > 20)
//                IAP_CONTR = 0x60;
//        }
//        else
//        {
//            dwon_count = 0;
//        }
    }
}



//UART2�ж�
void UART2_Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
		busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//�������ݼĴ���Ϊ��S2BUF


	}
}


//UART3�ж�
void UART3_Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
		busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//�������ݼĴ���Ϊ��S3BUF

	}
}



//UART4�ж�
void UART4_Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
		busy[4] = 0;
	}
    if(UART4_GET_RX_FLAG)
	{
        UART4_CLEAR_RX_FLAG;
		//�������ݼĴ���Ϊ��S4BUF;

		g_RxDat = S4BUF;
		g_RxData[g_RxPointer++] = g_RxDat;
	}
}



//�ⲿ�ж�0
void INT0_Isr() interrupt 0
{

}



//�ⲿ�ж�1
void INT1_Isr() interrupt 2
{

}



//�ⲿ�ж�2
void INT2_Isr() interrupt 10
{
	INT2_CLEAR_FLAG;  //����жϱ�־
	
}



//�ⲿ�ж�3
void INT3_Isr() interrupt 11
{
	INT3_CLEAR_FLAG;  //����жϱ�־
	
}



//�ⲿ�ж�4
void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //����жϱ�־
	
}



//��ʱ��0�ж�
void TM0_Isr() interrupt 1
{
	
}



//��ʱ��1�ж�
void TM1_Isr() interrupt 3
{

	
}


float left_pid = 0, right_pid = 0;               //�ٶȻ�pid������������ת���ٸ������
float pidtopwm_left = 0, pidtopwm_right = 0;     //�ٶȻ�pid��ת��ֵ
int g_DutyLeft = 0, g_DutyRight = 0;             //�������Ҫ�������PWMֵ

float imu693_pid = 0;                            //������pid��ֵ
float Gyro_Z = 0, filtered_GyroZ = 0;            //�����ǽ��ٶȵ�ԭʼֵ�Ϳ������˲�֮���ֵ

int g_LeftPoint = 50;                            //����Ŀ���ٶ�                  
int g_RightPoint = 50;                           //����Ŀ���ٶ�       
float g_IMU693Point = 0.0;                       //������Ŀ����ٶ�       

int count = 0, flag = 0;


//��ʱ��2�ж�
void TM2_Isr() interrupt 12
{
	TIM2_CLEAR_FLAG;  //����жϱ�־
	
	count++;
	if (count >= 300)
	{
		flag = 1;
		count = 0;
	}
	
	//��ȡ�������������ֵ
	g_EncoderLeft = get_left_encoder();
	g_EncoderRight = get_right_encoder();
	
	/*
		��ȡ���ٶȲ�ת��Ϊʵ����������
		��ͻȻ��ת��Gyro_ZΪ��ֵ��ͻȻ��ת��Gyro_ZΪ��ֵ
	*/
	imu963ra_get_gyro();
	Gyro_Z = imu963ra_gyro_transition(imu963ra_gyro_z);
	
	//��Gyro_Z���п������˲�
	filtered_GyroZ = Kalman_Update(&imu693_kf, Gyro_Z);
	
	//���������ǽ��ٶ�pid
	imu693_pid = pid_poisitional(&IMU693PID, filtered_GyroZ, g_IMU693Point);
	
	//���¿������˲���ֵ
	Kalman_Predict(&imu693_kf, imu693_pid);
	
	//�����ٶȻ�pid
	left_pid = pid_increment(&LeftPID, g_EncoderLeft, g_LeftPoint);
	right_pid = pid_increment(&RightPID, g_EncoderRight, g_RightPoint);
	
	/*
		���ٶȻ�pid��ֵת����PWM����ֵ
		��Ϊleft_pid��right_pid��ֵ��С���������㼸���ң������ҾͰ����Ŵ���һ��
		����Ҳ�����ϸ��ת����pwm��������
	*/
	pidtopwm_left = 40.0 * left_pid;
	pidtopwm_right = 40.0 * right_pid;
	
	//����pid�ۼ�
	g_DutyLeft += pidtopwm_left - imu693_pid;
	g_DutyRight += pidtopwm_right + imu693_pid;
	
//	set_pwm_left(g_DutyLeft);
//	set_pwm_right(g_DutyRight);
}



//��ʱ��3�ж�
void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //����жϱ�־
	
}



//��ʱ��4�ж�
void TM4_Isr() interrupt 20
{
	TIM4_CLEAR_FLAG; //����жϱ�־


}


//void  INT0_Isr()  interrupt 0;
//void  TM0_Isr()   interrupt 1;
//void  INT1_Isr()  interrupt 2;
//void  TM1_Isr()   interrupt 3;
//void  UART1_Isr() interrupt 4;
//void  ADC_Isr()   interrupt 5;
//void  LVD_Isr()   interrupt 6;
//void  PCA_Isr()   interrupt 7;
//void  UART2_Isr() interrupt 8;
//void  SPI_Isr()   interrupt 9;
//void  INT2_Isr()  interrupt 10;
//void  INT3_Isr()  interrupt 11;
//void  TM2_Isr()   interrupt 12;
//void  INT4_Isr()  interrupt 16;
//void  UART3_Isr() interrupt 17;
//void  UART4_Isr() interrupt 18;
//void  TM3_Isr()   interrupt 19;
//void  TM4_Isr()   interrupt 20;
//void  CMP_Isr()   interrupt 21;
//void  I2C_Isr()   interrupt 24;
//void  USB_Isr()   interrupt 25;
//void  PWM1_Isr()  interrupt 26;
//void  PWM2_Isr()  interrupt 27;