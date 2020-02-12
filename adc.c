#include <stdio.h>
#include "main.h"
#include "adc.h"

uint32_t ADCDR = 0;

/*void adc_control(ADC_T *mADC)
{
    ADC1_init(mADC ,2, 3, 1);
    ADC2_init(mADC ,3, 4, 5);
}

void ADC1_init(ADC_T *mADC ,uint32_t pin, uint32_t channel, uint32_t speed)
{
    mADC->dcbus = DCBUS;
    mADC->pin = pin;
    mADC->speed = speed;
    mADC->channel = channel;

    printf("ADC1 Data= %u \n", *(mADC->data));
}

void ADC2_init(ADC_T *mADC ,uint32_t pin, uint32_t channel, uint32_t speed)
{
    mADC->dcbus = DCBUS;
    mADC->pin = pin;
    mADC->speed = speed;
    mADC->channel = channel;

    printf("ADC2 Data= %u \n", *(mADC->data));
}
*/
void linkAdcData(uint32_t **ptr)
{
    *ptr = &ADCDR;
}


