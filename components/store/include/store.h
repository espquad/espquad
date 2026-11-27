#pragma once

#include <esp_log.h>
#include "esp_err.h"
#include <stdio.h>

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
} control_config;

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
} control_state;

typedef struct {
    float mixer_max;                        // maximum of the produced duty cycle [0, 1]
    control_config roll;
    control_config pitch;
    control_config yaw;
} config_state;

typedef struct {
    int16_t flight_mode;
    float thr_com;                          // throttle command [0, 1]
    float thr_app;                          // applied throttle after desaturation [0, 1]
    float motor_dc [4];                     // motor duty cycle [0, 1]
    control_state roll;
    control_state pitch;
    control_state yaw;
    config_state config;
} state;


state app_state;

void initState();

state getState();

void setState(state newState);
