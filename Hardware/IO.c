#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);					//PA5归位，PA6启动   低电平有效

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;      //PA3作为方向控制，  PA4作为使能控制  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;						              //PA3低电平右移 高电平左移     PA4低电平有效
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_14;				//PA8作为方向控制，  PA9作为使能控制  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;					//PA8低电平上移 高电平下移 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_14);
}

void Camera_Cont(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	Delay_ms(20);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
}

