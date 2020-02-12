#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "foc.h"

typedef struct {
    uint32_t pin;
    uint32_t speed;
    uint32_t channel;
    uint32_t duty[3];
    uint32_t * data;
} PWM_T;

void linkPwmData(uint32_t **ptr);

#endif // PWM_H
