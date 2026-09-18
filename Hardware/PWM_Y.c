#include "stm32f10x.h"                  // Device header

static volatile uint32_t TIM4_Pulse_Remaining = 0;
static volatile uint32_t TIM5_Pulse_Remaining = 0;

/* ====================================================================
 * TIM4 PWM & 脉冲计数配置 (Channel 1 -> PB6)
 * ==================================================================== */
void TIM4_PWM_Pulse_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // 2. 配置 GPIO (PB6 为复用推挽输出)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 3. 配置定时器基础参数 (72MHz / 72 / 1000 = 1000Hz = 1ms周期)
    TIM_TimeBaseStructure.TIM_Period = 999;               // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 71;             // PSC
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 4. 配置 PWM 模式 (PWM 模式 1，50% 占空比)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 500;                  // CCR (50% 占空比)
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    // 5. 配置 NVIC 中断
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 清除标志并使能更新中断
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    // 初始状态下不启动计数器
    TIM_Cmd(TIM4, DISABLE);
}

/* 启动 TIM4 发送指定数量脉冲 */
void TIM4_Start_Pulses(uint32_t pulse_count)
{
    if (pulse_count == 0) return;

    TIM4_Pulse_Remaining = pulse_count;
    
    // 清空计数器并重新使能 PWM 输出
    TIM_SetCounter(TIM4, 0);
    TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Enable);
    TIM_Cmd(TIM4, ENABLE);
}

/* 检查 TIM4 是否正在输出脉冲 */
uint8_t TIM4_Is_Busy(void)
{
    return (TIM4_Pulse_Remaining > 0) ? 1 : 0;
}

/* TIM4 中断服务函数 */
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

        if (TIM4_Pulse_Remaining > 0)
        {
            TIM4_Pulse_Remaining--;
            if (TIM4_Pulse_Remaining == 0)
            {
                // 脉冲输出完成，关闭 PWM 通道和定时器
                TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Disable);
                TIM_Cmd(TIM4, DISABLE);
            }
        }
    }
}

