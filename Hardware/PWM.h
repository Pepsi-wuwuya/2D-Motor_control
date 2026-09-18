#ifndef __PWM_H
#define __PWM_H

void TIM2_PWM_Pulse_Init(void);
void TIM2_Start_Pulses(uint32_t pulse_count);
uint8_t TIM2_Is_Busy(void);

#endif
