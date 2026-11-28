#pragma once

#include <esp_log.h>
#include "esp_err.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"

#define FLTMODE_DISARMED 0
#define FLTMODE_IDLE 1
#define FLTMODE_FLIGHT 2

typedef struct {
    float outer_p_k;                        // outer controller proportional gain
    float outer_max;                        // absolute maximum of the produced rate [°/s]
    float inner_p_k;                        // inner controller proportional gain
    float inner_i_k;                        // inner controller integral gain
    float inner_d_k;                        // inner controller derivative gain
    float inner_i_max;                      // absolute maximum of the integral
    float inner_d_fc;                       // FC for the derivative LPF [Hz]
} axis_config;

typedef struct {
    float angle;                            // angle
    float set;                              // setpoint
    float dot;                              // rate
    float dot_set;                          // rate setpoint
    float dot_prev;                         // previous rate
    float dot_i;                            // rate integral
    int8_t sat;                             // saturation flag
    float dot_d;                            // rate derivative
    float com;                              // command
    float alpha;                            // alpha term of the derivative LPF
} axis_state;

typedef struct {
    int16_t flight_mode;
    float mixer_max;                        // maximum of the produced duty cycle [0, 1]
    float thr_com;                          // throttle command [0, 1]
    axis_config axis [3];
} config_state;

typedef struct {
    float thr_app;                          // applied throttle after desaturation [0, 1]
    float motor_dc [4];                     // motor duty cycle [0, 1]
    axis_state axis [3];
} runtime_state;

typedef struct {
    runtime_state runtime;
    config_state config;
} state;


esp_err_t takeConfigLock();

esp_err_t releaseConfigLock();

esp_err_t initState();

state const * getState();

esp_err_t setConfigState(config_state * newConfigState);


void handleFltModeUpdate(state * currentState, config_state * newConfigState);
void handleFcUpdate(state * currentState, config_state * newConfigState, size_t axis);