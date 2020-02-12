#ifndef MOTOR_CH_H
#define MOTOR_CH_H

#include "adc.h"
#include "foc.h"
#include "pwm.h"

typedef struct {
    ADC_T mADC;
    PWM_T mPWM;
    MC_FOC_T mFOC;

} MOTOR_T;

#endif // MOTOR_CH_H

