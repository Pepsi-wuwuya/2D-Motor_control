#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "PWM.h"
#include "IO.h"
#include "Timer.h"
#include "Global.h"
#include "PWM_Y.h"
#include "Motor.h"
#include "Serial.h"

//初始坐标为（1，2）

uint32_t num_x=0, num_y=0;			//num_x, num_y 分别表示横坐标和纵坐标  初始值为（1，2）
uint8_t flag;
int i=1,j=2;


int main(void)
{
	IO_Init();
	TIM6_Init();
	TIM2_PWM_Pulse_Init();
	TIM4_PWM_Pulse_Init();
	Serial_Init();
	
//	GPIO_SetBits(GPIOB, GPIO_Pin_8);		//上移
//	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
//	TIM4_Start_Pulses(1120);
	
//	GPIO_SetBits(GPIOA, GPIO_Pin_3);		//右移
//	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
//	TIM2_Start_Pulses(1120);
	
	
	
	while (1)
	{

		if((!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)))		////硬件重复执行
		{
			Camera_Cont();
			Delay_s(2);
			Level_Motor_Right(1);
	
			for(i=1; i<4; i=i+2)
			{
				
				Vertical_Motor_Up(i);
				Level_Motor_Left(i);
				Vertical_Motor_Up(1);


				Level_Motor_Right(j);
				Vertical_Motor_Down(j);
				Level_Motor_Right(1);
	
				Serial_SendNumber(i,2);
				Serial_SendNumber(j,2);
				j=j+2;	

			}
			
			TIM_Cmd(TIM2, DISABLE);
			TIM_Cmd(TIM4, DISABLE);
			
		}
		

		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
		{
			if(num_x>1)
			{
				TIM_Cmd(TIM2, ENABLE);
				
				GPIO_SetBits(GPIOA, GPIO_Pin_3);		//右移
				GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				TIM2_Start_Pulses((num_x+1)*1120);
				
				while(TIM2_Is_Busy());
				TIM_Cmd(TIM2, DISABLE);
			}
				
			if(num_y>1)
			{
				TIM_Cmd(TIM4, ENABLE);
				
				GPIO_SetBits(GPIOB, GPIO_Pin_8);		//下移
				GPIO_ResetBits(GPIOB, GPIO_Pin_9);
				TIM4_Start_Pulses((num_y-2)*1120);
				
				while(TIM4_Is_Busy());
				TIM_Cmd(TIM4, DISABLE);
			}
		
		}
		
			
	}
}

