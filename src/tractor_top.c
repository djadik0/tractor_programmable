#include "tractor_top.h"
#include <stddef.h>

static const TractorDrivers *io;
static TractorState state;
static TractorOutputs outputs;
static volatile bool pending_command;
static volatile TractorCommand command;
static volatile uint8_t pending_sensors;

static void apply_outputs(void)
{
    if (io == NULL) return;
    if (io->set_drive) io->set_drive(outputs.drive);
    if (io->set_boom) io->set_boom(outputs.boom);
    if (io->set_bucket) io->set_bucket(outputs.bucket);
    if (io->set_steering) io->set_steering(outputs.steering);
}

static bool sensor_is_active(TractorSensor id)
{
    return io && io->sensor_active && io->sensor_active(id);
}

static void enforce_limits(void)
{
    if (outputs.boom == TRACTOR_FORWARD && sensor_is_active(TRACTOR_SENSOR_BOOM_UPPER))
        outputs.boom = TRACTOR_STOPPED;
    if (outputs.boom == TRACTOR_BACKWARD && sensor_is_active(TRACTOR_SENSOR_BOOM_LOWER))
        outputs.boom = TRACTOR_STOPPED;
    if (outputs.bucket == TRACTOR_FORWARD && sensor_is_active(TRACTOR_SENSOR_BUCKET_UPPER))
        outputs.bucket = TRACTOR_STOPPED;
    if (outputs.bucket == TRACTOR_BACKWARD && sensor_is_active(TRACTOR_SENSOR_BUCKET_LOWER))
        outputs.bucket = TRACTOR_STOPPED;
}

void Tractor_Init(const TractorDrivers *drivers)
{
    io = drivers;
    state = TRACTOR_IDLE;
    outputs = (TractorOutputs){TRACTOR_STOPPED, TRACTOR_STOPPED,
                               TRACTOR_STOPPED, TRACTOR_STEER_CENTER};
    pending_command = false;
    command = TRACTOR_CMD_NONE;
    pending_sensors = 0U;
    apply_outputs();
}

void Tractor_OnUartCommand(TractorCommand cmd)
{
    if (cmd <= TRACTOR_CMD_NONE || cmd > TRACTOR_CMD_BUCKET_STOP) return;
    command = cmd;
    pending_command = true;
}

void Tractor_OnGpioEvent(TractorSensor sensor)
{
    if ((unsigned)sensor < TRACTOR_SENSOR_COUNT)
        pending_sensors |= (uint8_t)(1U << sensor);
}

static void execute(TractorCommand cmd)
{
    switch (cmd) {
        case TRACTOR_CMD_STOP_ALL:
            outputs.drive = TRACTOR_STOPPED;
            outputs.boom = TRACTOR_STOPPED;
            outputs.bucket = TRACTOR_STOPPED;
            outputs.steering = TRACTOR_STEER_CENTER;
            break;
        case TRACTOR_CMD_DRIVE_FORWARD: outputs.drive = TRACTOR_FORWARD;
            break;
        case TRACTOR_CMD_DRIVE_REVERSE: outputs.drive = TRACTOR_BACKWARD;
            break;
        case TRACTOR_CMD_DRIVE_STOP: outputs.drive = TRACTOR_STOPPED;
            break;
        case TRACTOR_CMD_STEER_LEFT: outputs.steering = TRACTOR_STEER_LEFT;
             break;
        case TRACTOR_CMD_STEER_CENTER: outputs.steering = TRACTOR_STEER_CENTER;
             break;
        case TRACTOR_CMD_STEER_RIGHT: outputs.steering = TRACTOR_STEER_RIGHT;
             break;
        case TRACTOR_CMD_BOOM_UP: outputs.boom = TRACTOR_FORWARD;
             break;
        case TRACTOR_CMD_BOOM_DOWN: outputs.boom = TRACTOR_BACKWARD;
             break;
        case TRACTOR_CMD_BOOM_STOP: outputs.boom = TRACTOR_STOPPED;
             break;
        case TRACTOR_CMD_BUCKET_UP: outputs.bucket = TRACTOR_FORWARD;
             break;
        case TRACTOR_CMD_BUCKET_DOWN: outputs.bucket = TRACTOR_BACKWARD;
             break;
        case TRACTOR_CMD_BUCKET_STOP: outputs.bucket = TRACTOR_STOPPED;
             break;
        default:
            break;
    }
}

void Tractor_Process(void)
{
    switch (state) {
        case TRACTOR_IDLE:
            if (pending_sensors) {
                state = TRACTOR_SET_ZERO;
            } else if (pending_command) {
                state = TRACTOR_SET_CMD;
            }
            break;
        case TRACTOR_SET_CMD: {
            if (pending_sensors) {
                state = TRACTOR_SET_ZERO;
                break;
            }
            TractorCommand cmd = command;
            pending_command = false;
            execute(cmd);
            enforce_limits();
            apply_outputs();
            state = TRACTOR_IDLE;
            break;
        }
        case TRACTOR_SET_ZERO:
            pending_sensors = 0U;
            enforce_limits();
            apply_outputs();
            state = TRACTOR_IDLE;
            break;
        default:
            state = TRACTOR_IDLE;
            break;
    }
}

TractorState Tractor_GetState(void) {
    return state;
}
TractorOutputs Tractor_GetOutputs(void) {
    return outputs;
}
