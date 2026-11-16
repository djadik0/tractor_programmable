#include "tractor_pwm.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
TIM_TypeDef fake_tim1;
static unsigned configured, started, iteration;
int firmware_main(void);
int main(void) { return firmware_main(); }
void test_error(void) { fputs("FAIL: Error_Handler\n", stderr); exit(1); }
void HAL_Init(void) {}
int HAL_RCC_OscConfig(RCC_OscInitTypeDef *p) { (void)p; return HAL_OK; }
int HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *p, uint32_t l) { (void)p; (void)l; return HAL_OK; }
int HAL_TIM_PWM_Init(TIM_HandleTypeDef *p) { p->Instance->arr = p->Init.Period; return HAL_OK; }
int HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *p, TIM_MasterConfigTypeDef *c) { (void)p; (void)c; return HAL_OK; }
int HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *p, TIM_OC_InitTypeDef *c, uint32_t ch) { (void)p; (void)c; configured |= 1U << ch; return HAL_OK; }
int HAL_TIMEx_ConfigBreakDeadTime(TIM_HandleTypeDef *p, TIM_BreakDeadTimeConfigTypeDef *c) { (void)p; (void)c; return HAL_OK; }
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *p) { (void)p; }
int HAL_TIM_PWM_Start(TIM_HandleTypeDef *p, uint32_t ch) { (void)p; started |= 1U << ch; return HAL_OK; }
int HAL_TIM_PWM_Stop(TIM_HandleTypeDef *p, uint32_t ch) { (void)p; (void)ch; return HAL_OK; }

/* main.c is compiled with Tractor_Process renamed to this hook. */
void Test_Process(void) {
    if (iteration == 0) {
        assert(configured == 7 && started == 7 && fake_tim1.arr == 999);
        assert(fake_tim1.compare[0] == 0 && fake_tim1.compare[1] == 0 && fake_tim1.compare[2] == 0);
        Tractor_OnUartCommand(TRACTOR_CMD_DRIVE_FORWARD);
    }
    if (iteration == 2) { assert(fake_tim1.compare[0] == 700); Tractor_OnUartCommand(TRACTOR_CMD_BOOM_UP); }
    if (iteration == 4) { assert(fake_tim1.compare[1] == 700); Tractor_OnUartCommand(TRACTOR_CMD_BUCKET_DOWN); }
    if (iteration == 6) { assert(fake_tim1.compare[2] == 700); Tractor_OnUartCommand(TRACTOR_CMD_STOP_ALL); }
    if (iteration == 8) {
        assert(fake_tim1.compare[0] == 0 && fake_tim1.compare[1] == 0 && fake_tim1.compare[2] == 0);
        puts("PASS: main initialization, PWM channels, commands and stop");
        exit(0);
    }
    ++iteration;
    Tractor_Process();
}
