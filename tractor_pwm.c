#include "tractor_pwm.h"
#include <stddef.h>

static const TractorPwmConfig *hw;

static void motor_set(TractorMotor motor, TractorMotion direction)
{
    const TractorPwmChannel *p = &hw->motors[motor];
    /* Disable PWM before changing direction. */
    __HAL_TIM_SET_COMPARE(p->timer, p->channel, 0);
    hw->set_direction(motor, direction);
    if (direction != TRACTOR_STOPPED)
        __HAL_TIM_SET_COMPARE(p->timer, p->channel, p->compare);
}

static void drive(TractorMotion m) { motor_set(PWM_DRIVE, m); }
static void boom(TractorMotion m) { motor_set(PWM_BOOM, m); }
static void bucket(TractorMotion m) { motor_set(PWM_BUCKET, m); }
static void steering(TractorSteering s) { hw->set_steering(s); }
static bool sensor(TractorSensor s) { return hw->sensor_active(s); }

static const TractorDrivers drivers = { drive, boom, bucket, steering, sensor };

bool TractorPwm_Init(const TractorPwmConfig *config)
{
    unsigned i, j;
    if (!config || !config->set_direction || !config->set_steering ||
        !config->sensor_active) return false;
    for (i = 0; i < PWM_MOTOR_COUNT; ++i) {
        const TractorPwmChannel *p = &config->motors[i];
        if (!p->timer || !p->timer->Instance || p->compare == 0 ||
            p->compare > __HAL_TIM_GET_AUTORELOAD(p->timer)) return false;
        for (j = 0; j < i; ++j)
            if (p->timer->Instance == config->motors[j].timer->Instance &&
                p->channel == config->motors[j].channel) return false;
    }
    hw = config;
    for (i = 0; i < PWM_MOTOR_COUNT; ++i) {
        const TractorPwmChannel *p = &hw->motors[i];
        __HAL_TIM_SET_COMPARE(p->timer, p->channel, 0);
        hw->set_direction((TractorMotor)i, TRACTOR_STOPPED);
    }
    for (i = 0; i < PWM_MOTOR_COUNT; ++i) {
        const TractorPwmChannel *p = &hw->motors[i];
        if (HAL_TIM_PWM_Start(p->timer, p->channel) != HAL_OK) {
            while (i > 0) {
                --i;
                HAL_TIM_PWM_Stop(hw->motors[i].timer, hw->motors[i].channel);
            }
            hw = NULL;
            return false;
        }
    }
    Tractor_Init(&drivers);
    return true;
}
