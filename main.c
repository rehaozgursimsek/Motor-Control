#include <stdio.h>
#include "adc.h"
#include "main.h"
#include "motor_ch.h"
#include "foc.h"
#include "pwm.h"

static MC_FOC_T mc_foc_t[TOTAL_MOTOR_NUMBERS];
static MOTOR_T motor_t[TOTAL_MOTOR_NUMBERS];
static ADC_T adc_t;
void MC_ADC_Init(MOTOR_T *motor_t, uint32_t pin, uint32_t channel, uint32_t speed/*void(*adc_handler)(ADC_T *mADC)*/);
void MC_PWM_Init(MOTOR_T *motor_t, uint32_t pin, uint32_t channel, uint32_t speed);
MOTOR_T *motor;
int main()
{
    //motor = &motor_t;
    //motor->mADC = adc_t;
    //MC_ADC_Init(&motor_t[MOTOR0].mADC, adc_control);

    MC_ADC_Init(&motor_t[MOTOR0], 1 , 2 , 3);
    printf("Motor=%d pin=%d channel=%d speed=%d \n", MOTOR0, motor_t[MOTOR0].mADC.pin, motor_t[MOTOR0].mADC.channel,motor_t[MOTOR0].mADC.speed);
    MC_ADC_Init(&motor_t[MOTOR1], 3 , 2 , 5);
    printf("Motor=%d pin=%d channel=%d speed=%d \n", MOTOR1,motor_t[MOTOR1].mADC.pin,motor_t[MOTOR1].mADC.channel,motor_t[MOTOR1].mADC.speed);
    printf("Motor=%d pin=%d channel=%d speed=%d \n", MOTOR0,motor_t[MOTOR0].mADC.pin,motor_t[MOTOR0].mADC.channel,motor_t[MOTOR0].mADC.speed);

    MC_PWM_Init(&motor_t[MOTOR0], 2, 1 , 1);

    printf("Motor=%d pin=%d channel=%d speed=%d \n", MOTOR0, motor_t[MOTOR0].mPWM.pin, motor_t[MOTOR0].mPWM.channel,motor_t[MOTOR0].mPWM.speed);

    MC_FOC_Init(&motor_t[MOTOR0].mFOC, fast_loop_control, 0, -0.5);
}

void MC_ADC_Init(MOTOR_T *motor_t, uint32_t pin, uint32_t channel, uint32_t speed
                 /*void(*adc_handler)(ADC_T *mADC)*/){


   motor_t->mADC.dcbus=DCBUS;
   motor_t->mADC.pin = pin;
   motor_t->mADC.speed = speed;
   motor_t->mADC.channel = channel;

   //adc_handler(&adc_t);

   linkAdcData(&(motor_t->mADC.data));

   printf("ADC Data= %u \n", *(motor_t->mADC.data));

}

void MC_PWM_Init(MOTOR_T *motor_t, uint32_t pin, uint32_t channel, uint32_t speed)
{
     motor_t->mPWM.pin = pin;
     motor_t->mPWM.channel = channel;
     motor_t->mPWM.speed = speed;

     linkPwmData(&(motor_t->mPWM.data));

     printf("PWM Data= %u \n", *(motor_t->mPWM.data));

}

