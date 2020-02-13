#include <stdio.h>
#include "err_table.h"
#include "adc.h"
#include "main.h"
#include "motor_ch.h"
#include "foc.h"
#include "pwm.h"

static MOTOR_T motor_t[TOTAL_MOTOR_NUMBERS];
static MC_FOC_T mc_foc_t[TOTAL_MOTOR_NUMBERS];
static ADC_T adc_t;
static PWM_T pwm_t;
MC_CONTROL_ABORT_T MC_ADC_Init(MOTOR_T *motor_t, uint32_t pin, uint32_t channel, uint32_t speed/*void(*adc_handler)(ADC_T *mADC)*/);
MC_CONTROL_ABORT_T MC_PWM_Init(MOTOR_T *motor_t, uint32_t pin, uint32_t channel, uint32_t speed);
MOTOR_T *motor;
int main()
{
    motor = &motor_t;
    motor[MOTOR0].mADC = &adc_t;
    motor[MOTOR1].mADC = &adc_t;
    motor[MOTOR0].mPWM = &pwm_t;
    motor[MOTOR1].mPWM = &pwm_t;
    //MC_ADC_Init(&motor_t[MOTOR0].mADC, adc_control);

    MC_ADC_Init(&motor[MOTOR0], 1 , 2 , 3);
    printf("Motor=%d pin=%d channel=%d speed=%d \n", MOTOR0,adc_t.pin, adc_t.channel,adc_t.speed);

    MC_ADC_Init(&motor[MOTOR1], 3 , 2 , 5);
    printf("Motor=%d pin=%d channel=%d speed=%d \n", MOTOR1,adc_t.pin, adc_t.channel,adc_t.speed);
    MC_ADC_Init(&motor[MOTOR0], 1 , 2 , 3);
    printf("Motor=%d pin=%d channel=%d speed=%d \n", MOTOR0,adc_t.pin, adc_t.channel,adc_t.speed);

    MC_PWM_Init(&motor[MOTOR0], 2, 1 , 1);

    printf("Motor=%d pin=%d channel=%d speed=%d \n", MOTOR0, pwm_t.pin, pwm_t.channel,pwm_t.speed);

    MC_FOC_Init(&motor_t[MOTOR0].mFOC, fast_loop_control, 0, -0.5);
}

MC_CONTROL_ABORT_T MC_ADC_Init(MOTOR_T *motor_t, uint32_t pin, uint32_t channel, uint32_t speed
                 /*void(*adc_handler)(ADC_T *mADC)*/){

   if(MC_ERROR != NO_ERROR)
   {
        printf("Error:%d", MC_ERROR);
   }else{
       motor_t->mADC->dcbus=DCBUS;
       motor_t->mADC->pin = pin;
       motor_t->mADC->speed = speed;
       motor_t->mADC->channel = channel;

       //adc_handler(&adc_t);

       linkAdcData(&(motor_t->mADC->data));

       printf("ADC Data= %u \n", *(motor_t->mADC->data));
   }
}

MC_CONTROL_ABORT_T MC_PWM_Init(MOTOR_T *motor_t, uint32_t pin, uint32_t channel, uint32_t speed)
{
     if(MC_ERROR != NO_ERROR)
     {
          printf("Error:%d", MC_ERROR);
     }else{
         motor_t->mPWM->pin = pin;
         motor_t->mPWM->channel = channel;
         motor_t->mPWM->speed = speed;

         linkPwmData(&(motor_t->mPWM->data));

         printf("PWM Data= %u \n", *(motor_t->mPWM->data));
    }
}

