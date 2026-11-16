#ifndef TEST_MAIN_H
#define TEST_MAIN_H
#include <stdint.h>
typedef struct { uint32_t arr, compare[3]; } TIM_TypeDef;
typedef struct { uint32_t Prescaler, CounterMode, Period, ClockDivision, RepetitionCounter, AutoReloadPreload; } TIM_InitTypeDef;
typedef struct { TIM_TypeDef *Instance; TIM_InitTypeDef Init; } TIM_HandleTypeDef;
#ifdef TEST_FULL_MAIN
typedef struct { uint32_t PLLState; } PLLType;
typedef struct { uint32_t OscillatorType, HSIState, HSICalibrationValue; PLLType PLL; } RCC_OscInitTypeDef;
typedef struct { uint32_t ClockType, SYSCLKSource, AHBCLKDivider, APB1CLKDivider, APB2CLKDivider; } RCC_ClkInitTypeDef;
typedef struct { uint32_t MasterOutputTrigger, MasterSlaveMode; } TIM_MasterConfigTypeDef;
typedef struct { uint32_t OCMode, Pulse, OCPolarity, OCNPolarity, OCFastMode, OCIdleState, OCNIdleState; } TIM_OC_InitTypeDef;
typedef struct { uint32_t OffStateRunMode, OffStateIDLEMode, LockLevel, DeadTime, BreakState, BreakPolarity, AutomaticOutput; } TIM_BreakDeadTimeConfigTypeDef;
extern TIM_TypeDef fake_tim1;
#define TIM1 (&fake_tim1)
#define RCC_OSCILLATORTYPE_HSI 1
#define RCC_HSI_ON 1
#define RCC_HSICALIBRATION_DEFAULT 0
#define RCC_PLL_NONE 0
#define RCC_CLOCKTYPE_HCLK 1
#define RCC_CLOCKTYPE_SYSCLK 2
#define RCC_CLOCKTYPE_PCLK1 4
#define RCC_CLOCKTYPE_PCLK2 8
#define RCC_SYSCLKSOURCE_HSI 0
#define RCC_SYSCLK_DIV1 0
#define RCC_HCLK_DIV1 0
#define FLASH_LATENCY_0 0
#define TIM_COUNTERMODE_UP 0
#define TIM_CLOCKDIVISION_DIV1 0
#define TIM_AUTORELOAD_PRELOAD_DISABLE 0
#define TIM_TRGO_RESET 0
#define TIM_MASTERSLAVEMODE_DISABLE 0
#define TIM_OCMODE_PWM1 0
#define TIM_OCPOLARITY_HIGH 0
#define TIM_OCNPOLARITY_HIGH 0
#define TIM_OCFAST_DISABLE 0
#define TIM_OCIDLESTATE_RESET 0
#define TIM_OCNIDLESTATE_RESET 0
#define TIM_OSSR_DISABLE 0
#define TIM_OSSI_DISABLE 0
#define TIM_LOCKLEVEL_OFF 0
#define TIM_BREAK_DISABLE 0
#define TIM_BREAKPOLARITY_HIGH 0
#define TIM_AUTOMATICOUTPUT_DISABLE 0
#define __HAL_RCC_GPIOA_CLK_ENABLE() ((void)0)
#define __disable_irq() test_error()
void test_error(void);
void HAL_Init(void);
int HAL_RCC_OscConfig(RCC_OscInitTypeDef *p);
int HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *p, uint32_t latency);
int HAL_TIM_PWM_Init(TIM_HandleTypeDef *p);
int HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *p, TIM_MasterConfigTypeDef *c);
int HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *p, TIM_OC_InitTypeDef *c, uint32_t ch);
int HAL_TIMEx_ConfigBreakDeadTime(TIM_HandleTypeDef *p, TIM_BreakDeadTimeConfigTypeDef *c);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *p);
void Error_Handler(void);
#endif
#define TIM_CHANNEL_1 0U
#define TIM_CHANNEL_2 1U
#define TIM_CHANNEL_3 2U
#define HAL_OK 0
#define __HAL_TIM_SET_COMPARE(t,c,v) ((t)->Instance->compare[(c)] = (v))
#define __HAL_TIM_GET_AUTORELOAD(t) ((t)->Instance->arr)
int HAL_TIM_PWM_Start(TIM_HandleTypeDef *timer, uint32_t channel);
int HAL_TIM_PWM_Stop(TIM_HandleTypeDef *timer, uint32_t channel);
#endif
