#include "tractor_pwm.h"
#include <assert.h>
#include <stdio.h>
static TIM_TypeDef registers = {999, {0,0,0}};
static TIM_HandleTypeDef timer = { .Instance = &registers };
static TractorMotion dirs[3];
static bool limits[TRACTOR_SENSOR_COUNT];
static unsigned starts;
int HAL_TIM_PWM_Start(TIM_HandleTypeDef *t, uint32_t c) { (void)t; (void)c; ++starts; return HAL_OK; }
int HAL_TIM_PWM_Stop(TIM_HandleTypeDef *t, uint32_t c) { (void)t; (void)c; return HAL_OK; }
static void direction(TractorMotor id, TractorMotion m) { dirs[id] = m; }
static void steering(TractorSteering s) { (void)s; }
static bool sensor(TractorSensor s) { return limits[s]; }
static void command(TractorCommand c) { Tractor_OnUartCommand(c); Tractor_Process(); Tractor_Process(); }
int main(void) {
    static const TractorPwmConfig config = {
        {{&timer, TIM_CHANNEL_1, 700}, {&timer, TIM_CHANNEL_2, 700}, {&timer, TIM_CHANNEL_3, 700}},
        direction, steering, sensor
    };
    assert(TractorPwm_Init(&config));
    assert(starts == 3 && registers.compare[0] == 0 && registers.compare[1] == 0 && registers.compare[2] == 0);
    command(TRACTOR_CMD_DRIVE_FORWARD);
    assert(registers.compare[0] == 700 && dirs[0] == TRACTOR_FORWARD);
    command(TRACTOR_CMD_DRIVE_REVERSE);
    assert(registers.compare[0] == 700 && dirs[0] == TRACTOR_BACKWARD);
    command(TRACTOR_CMD_BOOM_UP);
    command(TRACTOR_CMD_BUCKET_DOWN);
    assert(registers.compare[1] == 700 && registers.compare[2] == 700);
    limits[TRACTOR_SENSOR_BOOM_UPPER] = true;
    Tractor_OnGpioEvent(TRACTOR_SENSOR_BOOM_UPPER);
    Tractor_Process(); Tractor_Process();
    assert(registers.compare[1] == 0 && registers.compare[2] == 700);
    command(TRACTOR_CMD_BOOM_DOWN);
    assert(registers.compare[1] == 700 && dirs[1] == TRACTOR_BACKWARD);
    command(TRACTOR_CMD_STOP_ALL);
    assert(registers.compare[0] == 0 && registers.compare[1] == 0 && registers.compare[2] == 0);
    puts("PASS: PWM start, commands, limit, reverse, stop");
    return 0;
}
