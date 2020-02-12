#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "foc.h"

typedef struct {
    uint32_t dcbus;
    uint32_t pin;
    uint32_t speed;
    uint32_t channel;
    uint32_t * data;
} ADC_T;

//void adc_control(ADC_T *mADC);

//void ADC1_init(ADC_T *mADC ,uint32_t pin, uint32_t channel, uint32_t speed);

//void ADC2_init(ADC_T *mADC ,uint32_t pin, uint32_t channel, uint32_t speed);

void linkAdcData(uint32_t **ptr);

#endif // ADC_H
