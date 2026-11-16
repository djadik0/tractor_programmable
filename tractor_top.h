#ifndef TRACTOR_TOP_H
#define TRACTOR_TOP_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    TRACTOR_IDLE = 0,
    TRACTOR_SET_CMD,
    TRACTOR_SET_ZERO
} TractorState;

typedef enum {
    TRACTOR_CMD_NONE = 0,
    TRACTOR_CMD_STOP_ALL,
    TRACTOR_CMD_DRIVE_FORWARD,
    TRACTOR_CMD_DRIVE_REVERSE,
    TRACTOR_CMD_DRIVE_STOP,
    TRACTOR_CMD_STEER_LEFT,
    TRACTOR_CMD_STEER_CENTER,
    TRACTOR_CMD_STEER_RIGHT,
    TRACTOR_CMD_BOOM_UP,
    TRACTOR_CMD_BOOM_DOWN,
    TRACTOR_CMD_BOOM_STOP,
    TRACTOR_CMD_BUCKET_UP,
    TRACTOR_CMD_BUCKET_DOWN,
    TRACTOR_CMD_BUCKET_STOP
} TractorCommand;

typedef enum {
    TRACTOR_SENSOR_BOOM_UPPER = 0,
    TRACTOR_SENSOR_BOOM_LOWER,
    TRACTOR_SENSOR_BUCKET_UPPER,
    TRACTOR_SENSOR_BUCKET_LOWER,
    TRACTOR_SENSOR_COUNT
} TractorSensor;

typedef enum {
    TRACTOR_STOPPED = 0,
    TRACTOR_FORWARD,
    TRACTOR_BACKWARD
} TractorMotion;

typedef enum {
    TRACTOR_STEER_LEFT = 0,
    TRACTOR_STEER_CENTER,
    TRACTOR_STEER_RIGHT
} TractorSteering;

typedef struct {
    TractorMotion drive;
    TractorMotion boom;
    TractorMotion bucket;
    TractorSteering steering;
} TractorOutputs;

typedef struct {
    void (*set_drive)(TractorMotion mode);
    void (*set_boom)(TractorMotion mode);
    void (*set_bucket)(TractorMotion mode);
    void (*set_steering)(TractorSteering mode);
    bool (*sensor_active)(TractorSensor id);
} TractorDrivers;

void Tractor_Init(const TractorDrivers *drivers);
void Tractor_OnUartCommand(TractorCommand cmd);
void Tractor_OnGpioEvent(TractorSensor sensor);
void Tractor_Process(void);
TractorState Tractor_GetState(void);
TractorOutputs Tractor_GetOutputs(void);

#endif
