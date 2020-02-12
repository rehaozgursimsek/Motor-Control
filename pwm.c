#include <stdint.h>
#include <stdlib.h>
#include "pwm.h"

uint32_t PWMDR = 0;

void linkPwmData(uint32_t **ptr)
{
    *ptr = &PWMDR;
}
