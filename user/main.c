#include "main.h"
#include "isr.h"
#include "motor.h"
#include "encoder.h"
#include "filter.h"

void main(void)
{
	board_init();// 初始化寄存器,不能删
	
	uart_init(UART_4, UART4_RX_P02, UART4_TX_P03, 115200, TIM_4);
	
	pit_timer_ms(TIM_2, 10);
	
	motor_init();
	encoder_init();
	
	imu963ra_init();
	
	Kalman_Init(&imu693_kf, 0.98, 0.02, imu693kf_Q, imu693kf_R, 0.0);
	
	
	
    while(1)
	{
		//串口接收
		if(g_RxPointer != 0)
		{
			int temp = g_RxPointer;
			delay_ms(4);
			
			if(temp == g_RxPointer)
			{
				uart4_interrupt_callback();
			}
		}
		
		
		
		if (flag == 1)
		{
			
			flag = 0;
		}

		
//		sprintf(g_TxData, "%d,%d,%d\n", g_EncoderLeft, g_RightPoint, g_EncoderRight);
//		uart_putstr(UART_4, g_TxData);
		
		sprintf(g_TxData, "%f,%f,%f,%d,%d\n",Gyro_Z, filtered_GyroZ, g_IMU693Point, g_EncoderLeft, g_EncoderRight);
		uart_putstr(UART_4, g_TxData);
		
		
    }
}