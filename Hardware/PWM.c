#include "stm32f10x.h"                  // Device header


// 记录剩余输出脉冲数的变量
static volatile uint32_t TIM2_Pulse_Remaining = 0;

/**
  * @brief  初始化 TIM2 PWM 输出 (CH1 -> PA0)
  * @param  无
  * @retval 无
  */
void TIM2_PWM_Pulse_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 使能 TIM2 与 GPIOA、AFIO 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    // 2. 配置 PA0 为复用推挽输出 (TIM2_CH1)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 配置定时器基础时基参数
    // 定时器时钟 = 72MHz
    // 计数频率 = 72MHz / (71 + 1) = 1MHz (1us/tick)
    // 溢出时间 = (999 + 1) * 1us = 1000us = 1ms (1kHz)
    TIM_TimeBaseStructure.TIM_Period = 999;               // ARR (自动重装载值)
    TIM_TimeBaseStructure.TIM_Prescaler = 71;             // PSC (预分频值)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 4. 配置 PWM 模式 1 (50% 占空比)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 500;                  // CCR (占空比 500/1000 = 50%)
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // 5. 配置 TIM2 更新中断 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 清除更新中断标志位并使能中断
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // 初始状态下禁用定时器
    TIM_Cmd(TIM2, DISABLE);
}

/**
  * @brief  启动 TIM2 输出指定数量的 PWM 脉冲
  * @param  pulse_count: 要输出的脉冲个数
  * @retval 无
  */
void TIM2_Start_Pulses(uint32_t pulse_count)
{
    if (pulse_count == 0) return;

    TIM2_Pulse_Remaining = pulse_count;

    // 清零计数器，开启输出通道，开启定时器
    TIM_SetCounter(TIM2, 0);
    TIM_CCxCmd(TIM2, TIM_Channel_1, TIM_CCx_Enable);
    TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief  查询 TIM2 当前是否正在发送脉冲
  * @retval 1: 正在发送，0: 发送完成/空闲
  */
uint8_t TIM2_Is_Busy(void)
{
    return (TIM2_Pulse_Remaining > 0) ? 1 : 0;
}

/**
  * @brief  TIM2 中断服务函数
  * @param  无
  * @retval 无
  */
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        if (TIM2_Pulse_Remaining > 0)
        {
            TIM2_Pulse_Remaining--;
            if (TIM2_Pulse_Remaining == 0)
            {
                // 脉冲数达到目标，关闭 PWM 通道与定时器
                TIM_CCxCmd(TIM2, TIM_Channel_1, TIM_CCx_Disable);
                TIM_Cmd(TIM2, DISABLE);
            }
        }
    }
}