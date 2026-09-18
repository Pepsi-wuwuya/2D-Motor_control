#ifndef __PWM_Y_H
#define __PWM_Y_H

void TIM4_PWM_Pulse_Init(void);
void TIM4_Start_Pulses(uint32_t pulse_count);
uint8_t TIM4_Is_Busy(void);

#endif

