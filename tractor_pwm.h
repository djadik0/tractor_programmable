#ifndef TRACTOR_PWM_H
#define TRACTOR_PWM_H

#include "main.h"
#include "tractor_top.h"

typedef enum { PWM_DRIVE, PWM_BOOM, PWM_BUCKET, PWM_MOTOR_COUNT } TractorMotor;

typedef struct {
    TIM_HandleTypeDef *timer;
    uint32_t channel;
    uint32_t compare; /* Fixed PWM value, from 1 to ARR. */
} TractorPwmChannel;

typedef struct {
    TractorPwmChannel motors[PWM_MOTOR_COUNT];
    /* Must set the external motor driver direction, including stopped. */
    void (*set_direction)(TractorMotor motor, TractorMotion direction);
    void (*set_steering)(TractorSteering position);
    bool (*sensor_active)(TractorSensor sensor);
} TractorPwmConfig;

/* Call after GPIO and PWM channel initialization. Config must remain valid. */
bool TractorPwm_Init(const TractorPwmConfig *config);

#endif
