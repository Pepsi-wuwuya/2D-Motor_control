#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "PWM_Y.h"
#include "Global.h"
#include "Delay.h"
#include "io.h"

void Level_Motor_Right(uint32_t num)
{
	int i=0;
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);		//右移
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	
	for(i=0; i<num; i++)					//电机右移一个单位， 拍照一次
	{
		TIM2_Start_Pulses(1120);
		while(TIM2_Is_Busy());
		Camera_Cont();
		Delay_s(2);
		
	}
	
	num_x=num_x+num;
}

void Level_Motor_Left(uint32_t num)
{
	int i=0;
	GPIO_SetBits(GPIOA, GPIO_Pin_3);		//左移
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	
	for(i=0; i<num; i++)					//电机左移一个单位， 拍照一次
	{
		TIM2_Start_Pulses(1120);
		while(TIM2_Is_Busy());
		Camera_Cont();
		Delay_s(2);
		
	}
	
	num_x=num_x-num;
}

void Vertical_Motor_Up(uint32_t num)
{
	int i=0;
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);		//上移
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	for(i=0; i<num; i++)					//电机右移一个单位， 拍照一次
	{
		TIM4_Start_Pulses(1120);
		while(TIM4_Is_Busy());
		Camera_Cont();
		Delay_s(2);
		
	}
	
	num_y=num_y+num;
}

void Vertical_Motor_Down(uint32_t num)
{
	int i=0;
	GPIO_SetBits(GPIOB, GPIO_Pin_8);		//下移
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	for(i=0; i<num; i++)					//电机右移一个单位， 拍照一次
	{
		TIM4_Start_Pulses(1120);
		while(TIM4_Is_Busy());
		Camera_Cont();
		Delay_s(2);
		
	}
	
	num_y=num_y-num;
}

