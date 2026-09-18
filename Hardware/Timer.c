#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Global.h"

/**
  * @brief  TIM6 初始化函数 (周期 5s)
  */
void TIM6_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 使能 TIM6 APB1 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    // 2. TIM6 时基配置 (周期 1ms)
    // 72MHz / 72 = 1MHz (计数频率 1us)
    // 1us * 1000 = 1ms
    TIM_TimeBaseStructure.TIM_Period = 30000 - 1;           // 自动重装载值 ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;          // 预分频值 PSC
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    // 3. 【关键】清除因初始化自动产生的更新标志位
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

    // 5. 配置 NVIC 中断通道
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;                       // TIM6 中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;              // 抢占优先级 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                     // 子优先级 1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  TIM6 中断服务函数
  * @note   必须确保函数名与 startup_stm32f10x_hd.s 中的向量表完全一致
  */
void TIM6_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        
				
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

    }
}


