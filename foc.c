#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "main.h"
#include "foc.h"
#include "const_def.h"
#include "err_table.h"
#include "motor_ch.h"


int MC_FOC_Init(MC_FOC_T *mc_foc_t,
                int(*fast_loop_handler)(MC_FOC_T *),
                /*float speed_ref*/float id_ref,float iq_ref)
{
    mc_foc_t->mcIdIqRef.id_ref_const=id_ref;
    mc_foc_t->mcIdIqRef.iq_ref_const=iq_ref;
    //mc_foc_t->mcSpeedRef.speed_ref = speed_ref;
    fast_loop_handler(mc_foc_t);
    return NO_ERROR;
}

int fast_loop_control(MC_FOC_T *mc_foc_t)
{
    /*if(MC_ERROR  != 0){

    }else{
        //MC_ERROR = speed_pi_controller(mc_foc_t->mcSpeedRef, &mc_foc_t->mcIdIqRef, 0.5, 0.001, &mc_foc_t->mcSPIPart);
    }*/if(MC_ERROR  != 0){

    }else{
        MC_ERROR = sin_wave(&mc_foc_t->mcTheta, &mc_foc_t->mcABCPhase);
    }if(MC_ERROR  != 0){

    }else{
        MC_ERROR = get_clarke(mc_foc_t->mcABCPhase , &mc_foc_t->mcIAlphaBeta);
    }if(MC_ERROR  != 0){

    }else{
        MC_ERROR = get_park(mc_foc_t->mcIAlphaBeta,mc_foc_t->mcTheta, &mc_foc_t->mcIDQ);
    }if(MC_ERROR  != 0){

    }else{
        MC_ERROR = pi_controller(mc_foc_t->mcIdIqRef, mc_foc_t->mcIDQ, 0.5,0.2, &mc_foc_t->mcVDQ, &mc_foc_t->mcPIPart);
    }if(MC_ERROR  != 0){

    }else{
        MC_ERROR = get_inv_park(mc_foc_t->mcVDQ, mc_foc_t->mcTheta, &mc_foc_t->mcVAlphaBeta);
    }if(MC_ERROR  != 0){

    }else{
        MC_ERROR = get_inv_clarke(mc_foc_t->mcVAlphaBeta, &mc_foc_t->mcVabc);
    }if(MC_ERROR  != 0){

    }else{
        MC_ERROR = get_sector_ident(mc_foc_t->mcTheta, &mc_foc_t->mcSector);
        MC_ERROR = set_duty(mc_foc_t->mcSector, mc_foc_t->mcTheta, mc_foc_t->mcVAlphaBeta, &mc_foc_t->mcTDuty);
    }

    return NO_ERROR;
}


int sin_wave(MC_THETA_T *mcTheta, MC_ABCPHASE_T *mcABCPhase){
    int cycle;
    float inc;
    float angle = 0;
    float sample_a;
    float sample_b;
    float sample_c;
    float amplitude = MAX_i_phase;
    inc = (30.f/360.f)*2.f*PI2; // 30 degree step
    for (cycle = 0; cycle < 30; cycle++)
    {
        if(angle > (360.f/360.f)*2.f*PI2)
        {
            angle = angle - (360.f/360.f)*2.f*PI2;
            sample_a = (amplitude * sin(angle));
            mcABCPhase->a_sin[cycle] = sample_a;
            sample_b = (amplitude * sin(angle+((2*PI2)/3)));
            mcABCPhase->b_sin[cycle] = sample_b;
            sample_c = (amplitude * sin(angle-((2*PI2)/3)));
            mcABCPhase->c_sin[cycle] = sample_c;

        }else {
            sample_a = (amplitude * sin(angle));
            mcABCPhase->a_sin[cycle] = sample_a;
            sample_b = (amplitude * sin(angle+((2*PI2)/3)));
            mcABCPhase->b_sin[cycle] = sample_b;
            sample_c = (amplitude * sin(angle-((2*PI2)/3)));
            mcABCPhase->c_sin[cycle] = sample_c;

        }
        mcTheta->theta_rotor[cycle] = angle;
        angle = angle + inc;
        printf("ERROR=%d a phase=%f b phase=%f c phase=%f\n", MC_ERROR, mcABCPhase->a_sin[cycle], mcABCPhase->b_sin[cycle], mcABCPhase->c_sin[cycle]);
    }

    return NO_ERROR;
}


int get_clarke(MC_ABCPHASE_T mcABCPhase, MC_IALPHABETA_T *mcIAlphaBeta)
{
    float sample_alpha;
    float sample_beta;
    for (int cycle = 0; cycle < 30; cycle++)
    {

        if (mcABCPhase.a_sin[cycle] > MAX_i_phase || mcABCPhase.b_sin[cycle] > MAX_i_phase || mcABCPhase.c_sin[cycle] > MAX_i_phase||mcABCPhase.a_sin[cycle] < MIN_i_phase || mcABCPhase.b_sin[cycle] < MIN_i_phase || mcABCPhase.c_sin[cycle] < MIN_i_phase)
            {
                return CURRENT_LIMIT_MISMATCH;
            }


        sample_alpha = mcABCPhase.a_sin[cycle];
        sample_beta = -((mcABCPhase.a_sin[cycle] * (1 / (SQRT_3))) + (2 * mcABCPhase.b_sin[cycle] * (1 / (SQRT_3))));

        mcIAlphaBeta->alpha[cycle]=sample_alpha;
        mcIAlphaBeta->beta[cycle]=sample_beta;
        printf("ERROR=%d Ialpha =%f Ibeta=%f\n", MC_ERROR, mcIAlphaBeta->alpha[cycle],mcIAlphaBeta->beta[cycle]);

    }
    return NO_ERROR;
}

int get_park(MC_IALPHABETA_T mcIAlphaBeta, MC_THETA_T mcTheta, MC_IDQ_T *mcIDQ)
{
    float sample_i_d;
    float sample_i_q;
    float theta_r[30] = {0};
    for (int cycle = 0; cycle < 30; cycle++)
    {
        theta_r[cycle] = mcTheta.theta_rotor[cycle];
        sample_i_d = (mcIAlphaBeta.alpha[cycle] * cos(theta_r[cycle])) + (mcIAlphaBeta.beta[cycle] * sin(theta_r[cycle])) ;
        sample_i_q = -(mcIAlphaBeta.alpha[cycle] * sin(theta_r[cycle])) + (mcIAlphaBeta.beta[cycle] * cos(theta_r[cycle])) ;
//    mcIDQ->i_d = (mcIAlphaBeta.alpha * cos(theta_r)) + (mcIAlphaBeta.beta * sin(theta_r)) ;
//    mcIDQ->i_q = -(mcIAlphaBeta.alpha * sin(theta_r)) + (mcIAlphaBeta.beta * cos(theta_r));
        mcIDQ->i_d[cycle] =sample_i_d;
        mcIDQ->i_q[cycle] =sample_i_q;
        printf("ERROR=%d Id =%f Iq =%f\n", MC_ERROR, mcIDQ->i_d[cycle],mcIDQ->i_q[cycle]);

    }
    return NO_ERROR;
}

int speed_pi_controller(MC_SPEEDREF_T mcSpeedRef, MC_IDIQREF_T *mcIdIqRef, float k_ps, float k_is,  MC_SPIPART_T *mcSPIPart)
{
    float i_integral_q[30], i_proportional_q[30] , i_sum_q[30];

    float w_ref;
    float speed=0;
    float speed_r=0;
    float inc;
    float w_ref_r;

    w_ref = mcSpeedRef.speed_ref;
    printf("Speed Ref =%f \n", mcSpeedRef.speed_ref);
    inc = (30.f/360.f)*2.f*PI2;
    w_ref_r = w_ref/9.55;
    for(int cycle = 0; cycle < 30; cycle++)
    {

        if(cycle == 0)
        {
            if(speed < w_ref){
                i_integral_q[cycle] =(k_is)*(w_ref_r-speed_r); //mcPIPart->v_int_q + then will add //4 2
                i_proportional_q[cycle] = (w_ref_r-speed_r)*(k_ps);    //2 1
                i_sum_q[cycle] = i_integral_q[cycle] + i_proportional_q[cycle]; //3

                speed_r = speed_r + inc;
                speed = speed_r*(9.55);
            }else{
                i_integral_q[cycle] =i_integral_q[cycle-1]+(k_is)*(w_ref_r-speed_r); //mcPIPart->v_int_q + then will add //4 2
                i_proportional_q[cycle] = (w_ref_r-speed_r)*(k_ps);    //2 1
                i_sum_q[cycle] = i_integral_q[cycle] + i_proportional_q[cycle]; //3
            }
        }else{
            if(speed < w_ref){
                i_integral_q[cycle] =i_integral_q[cycle-1]+(k_is)*(w_ref_r-speed_r); //mcPIPart->v_int_q + then will add //4 2
                i_proportional_q[cycle] = (w_ref_r-speed_r)*(k_ps);    //2 1
                i_sum_q[cycle] = i_integral_q[cycle] + i_proportional_q[cycle]; //3

                speed_r = speed_r + inc;
                speed = speed_r*(9.55);
            }else{
                i_integral_q[cycle] =i_integral_q[cycle-1]+(k_is)*(w_ref_r-speed_r); //mcPIPart->v_int_q + then will add //4 2
                i_proportional_q[cycle] = (w_ref_r-speed_r)*(k_ps);    //2 1
                i_sum_q[cycle] = i_integral_q[cycle] + i_proportional_q[cycle]; //3
            }
        }


        if((i_sum_q[cycle] < -2)||(i_sum_q[cycle] > 2))
        {
            if(i_sum_q[cycle] > 2)
            {
                i_sum_q[cycle] = 2;
            }else{
                i_sum_q[cycle] = -2;
            }
        }
        //Gecici sureligine yorum satiri yapildi.
        /*if ((v_sum_d[cycle] < MIN_vd_vq || v_sum_d[cycle] > MAX_vd_vq) || (v_sum_q[cycle] < MIN_vd_vq || v_sum_q[cycle] > MAX_vd_vq) )
        {

            return VOLTAGE_LIMIT_MISMATCH;
        }
        if ((id_ref < MIN_i_phase || id_ref > MAX_i_phase) || (iq_ref < MIN_i_phase || iq_ref > MAX_i_phase))
        {

            return CURRENT_LIMIT_MISMATCH;
        }*/


        mcSPIPart->i_pi_q[cycle] = i_sum_q[cycle];
        mcSPIPart->i_int_q[cycle] = i_integral_q[cycle];
        mcSPIPart->i_prop_q[cycle] = i_proportional_q[cycle];
        //mcVDQ->v_d[cycle] = v_sum_d[cycle];
        //mcVDQ->v_q[cycle] = v_sum_q[cycle];
        mcIdIqRef->iq_ref[cycle] = i_sum_q[cycle];
        printf("Idref =%f Iqref =%f\n", mcIdIqRef->id_ref[cycle], mcIdIqRef->iq_ref[cycle]);
    }
    return NO_ERROR;
}


int pi_controller(MC_IDIQREF_T mcIdIqRef,  MC_IDQ_T mcIDQ, float k_p, float k_i, MC_VDQ_T *mcVDQ, MC_PIPART_T *mcPIPart)
{
    float v_integral_d[30], v_integral_q[30], v_proportional_d[30] ,v_proportional_q[30], v_sum_d[30], v_sum_q[30];

    //float id_ref[30]={0};
    //float iq_ref[30]={0};
    float id_ref_const;
    float iq_ref_const;

    printf("Idref =%f Iqref =%f\n", mcIdIqRef.id_ref_const, mcIdIqRef.iq_ref_const);
    for(int cycle = 0; cycle < 30; cycle++)
    {
        //id_ref[cycle] = mcIdIqRef.id_ref[cycle];
        //iq_ref[cycle] = mcIdIqRef.iq_ref[cycle];
        id_ref_const = mcIdIqRef.id_ref_const;
        iq_ref_const = mcIdIqRef.iq_ref_const;

        if(cycle == 0)
        {
//            v_integral_d[cycle] =(k_i)*(id_ref[cycle]-mcIDQ.i_d[cycle]); //mcPIPart->v_int_d + then will add //4 -8
//            v_proportional_d[cycle] = (id_ref[cycle]-mcIDQ.i_d[cycle])*(k_p);    //2 -4
//            v_integral_q[cycle] =(k_i)*(iq_ref[cycle]-mcIDQ.i_q[cycle]); //mcPIPart->v_int_q + then will add //4 2
//            v_proportional_q[cycle] = (iq_ref[cycle]-mcIDQ.i_q[cycle])*(k_p);    //2 1
//            v_sum_d[cycle] = v_integral_d[cycle] + v_proportional_d[cycle]; //-12
//            v_sum_q[cycle] = v_integral_q[cycle] + v_proportional_q[cycle]; //3
            v_integral_d[cycle] =(k_i)*(id_ref_const-mcIDQ.i_d[cycle]); //mcPIPart->v_int_d + then will add //4 -8
            v_proportional_d[cycle] = (id_ref_const-mcIDQ.i_d[cycle])*(k_p);    //2 -4
            v_integral_q[cycle] =(k_i)*(iq_ref_const-mcIDQ.i_q[cycle]); //mcPIPart->v_int_q + then will add //4 2
            v_proportional_q[cycle] = (iq_ref_const-mcIDQ.i_q[cycle])*(k_p);    //2 1
            v_sum_d[cycle] = v_integral_d[cycle] + v_proportional_d[cycle]; //-12
            v_sum_q[cycle] = 5;//v_integral_q[cycle] + v_proportional_q[cycle]; //3


        }else{
//            v_integral_d[cycle] =v_integral_d[cycle-1] + (k_i)*(id_ref[cycle]-mcIDQ.i_d[cycle]); //mcPIPart->v_int_d + then will add //4 -8
//            v_proportional_d[cycle] = (id_ref[cycle]-mcIDQ.i_d[cycle])*(k_p);    //2 -4
//            v_integral_q[cycle] = v_integral_q[cycle-1] + (k_i)*(iq_ref[cycle]-mcIDQ.i_q[cycle]); //mcPIPart->v_int_q + then will add //4 2
//            v_proportional_q[cycle] = (iq_ref[cycle]-mcIDQ.i_q[cycle])*(k_p);    //2 1
//            v_sum_d[cycle] = v_integral_d[cycle] + v_proportional_d[cycle]; //-12
//            v_sum_q[cycle] = v_integral_q[cycle] + v_proportional_q[cycle]; //3
            v_integral_d[cycle] =v_integral_d[cycle-1] + (k_i)*(id_ref_const-mcIDQ.i_d[cycle]); //mcPIPart->v_int_d + then will add //4 -8
            v_proportional_d[cycle] = (id_ref_const-mcIDQ.i_d[cycle])*(k_p);    //2 -4
            v_integral_q[cycle] = v_integral_q[cycle-1] + (k_i)*(iq_ref_const-mcIDQ.i_q[cycle]); //mcPIPart->v_int_q + then will add //4 2
            v_proportional_q[cycle] = (iq_ref_const-mcIDQ.i_q[cycle])*(k_p);    //2 1
            v_sum_d[cycle] = v_integral_d[cycle] + v_proportional_d[cycle]; //-12
            v_sum_q[cycle] = 5;//v_integral_q[cycle] + v_proportional_q[cycle]; //3

        }

        if((v_sum_d[cycle] > DCBUS) || (v_sum_q[cycle] > DCBUS))
        {
            if(v_sum_d[cycle] > DCBUS)
            {
                v_sum_d[cycle] = DCBUS;
            }else{
                v_sum_q[cycle] = DCBUS;
            }
        }
        //Gecici sureligine yorum satiri yapildi.
        /*
        if ((v_sum_d[cycle] < MIN_vd_vq || v_sum_d[cycle] > MAX_vd_vq) || (v_sum_q[cycle] < MIN_vd_vq || v_sum_q[cycle] > MAX_vd_vq) )
        {

            return VOLTAGE_LIMIT_MISMATCH;
        }
        if ((id_ref[cycle] < MIN_i_phase || id_ref[cycle] > MAX_i_phase) || (iq_ref[cycle] < MIN_i_phase || iq_ref[cycle] > MAX_i_phase))
        {

            return CURRENT_LIMIT_MISMATCH;
        }*/

        mcPIPart->v_pi_d[cycle] = v_sum_d[cycle];
        mcPIPart->v_int_d[cycle] = v_integral_d[cycle];
        mcPIPart->v_prop_d[cycle] = v_proportional_d[cycle];
        mcPIPart->v_pi_q[cycle] = v_sum_q[cycle];
        mcPIPart->v_int_q[cycle] = v_integral_q[cycle];
        mcPIPart->v_prop_q[cycle] = v_proportional_q[cycle];
        mcVDQ->v_d[cycle] = v_sum_d[cycle];
        mcVDQ->v_q[cycle] = v_sum_q[cycle];

        printf("Error= %d Vd =%f Vq =%f\n", MC_ERROR, mcVDQ->v_d[cycle],mcVDQ->v_q[cycle]);

    }
    return NO_ERROR;
}

int get_inv_park(MC_VDQ_T mcVDQ, MC_THETA_T mcTheta, MC_VALPHABETA_T *mcVAlphaBeta)
{
    float sample_v_alpha;
    float sample_v_beta;
    float theta_r[30] = {0};
    for (int cycle = 0; cycle < 30; cycle++)
    {
        theta_r[cycle] = mcTheta.theta_rotor[cycle];
        sample_v_alpha = (mcVDQ.v_d[cycle] * cos(theta_r[cycle])) - (mcVDQ.v_q[cycle] * sin(theta_r[cycle]));
        sample_v_beta = (mcVDQ.v_d[cycle] * sin(theta_r[cycle])) + (mcVDQ.v_q[cycle] * cos(theta_r[cycle]));
        mcVAlphaBeta->v_alpha[cycle] = sample_v_alpha;
        mcVAlphaBeta->v_beta[cycle] = sample_v_beta;
        printf("Valpha =%f Vbeta =%f\n", mcVAlphaBeta->v_alpha[cycle],mcVAlphaBeta->v_beta[cycle]);

    }
    return NO_ERROR;
}

int get_inv_clarke(MC_VALPHABETA_T mcVAlphaBeta, MC_VABC_T *mcVabc)
{
    float va_temp, vb_temp, vc_temp;
    for (int cycle = 0; cycle < 30; cycle++)
    {
        va_temp = mcVAlphaBeta.v_alpha[cycle];  //-12
        vb_temp = -(mcVAlphaBeta.v_alpha[cycle]/2) + ((sqrt(3)/2)*(mcVAlphaBeta.v_beta[cycle]));
        vc_temp = -(mcVAlphaBeta.v_alpha[cycle]/2) - ((sqrt(3)/2)*(mcVAlphaBeta.v_beta[cycle]));

        if(va_temp < MIN_v_phase || vb_temp < MIN_v_phase || vc_temp < MIN_v_phase || va_temp > MAX_v_phase || vb_temp > MAX_v_phase || vc_temp > MAX_v_phase)
        {

            return VOLTAGE_LIMIT_MISMATCH;
        }

        mcVabc->v_a[cycle] = va_temp;
        mcVabc->v_b[cycle] = vb_temp;
        mcVabc->v_c[cycle] = vc_temp;
        printf("ERROR=%d Va phase =%f Vb phase =%f Vc phase =%f\n", MC_ERROR, mcVabc->v_a[cycle],mcVabc->v_b[cycle], mcVabc->v_c[cycle]);

    }
    return NO_ERROR;
}

int get_sector_ident(MC_THETA_T mcTheta, MC_SECTOR_T *mcSector)
{

    float theta_r[30] = {0};
    float limit_theta = 2*PI/3;
    float theta_degree[30] = {0};
    float sector_temp[30] = {0};
    for (int cycle = 0; cycle < 30; cycle++)
    {
        theta_r[cycle] = mcTheta.theta_rotor[cycle];
        theta_degree[cycle]=(theta_r[cycle]*PI)/PI2;
        if(theta_degree[cycle] >= 0 && theta_degree[cycle] < (limit_theta/2))
        {
            sector_temp[cycle] = 1;
        } else if(theta_degree[cycle]  >= (limit_theta/2) && theta_degree[cycle]  < limit_theta)
        {
            sector_temp[cycle] = 2;
        }else if(theta_degree[cycle]  >= limit_theta && theta_degree[cycle]  < ((3*limit_theta)/2))
        {
            sector_temp[cycle] = 3;
        }else if(theta_degree[cycle]  >= ((3*limit_theta)/2) && theta_degree[cycle]  < (2*limit_theta))
        {
            sector_temp[cycle] = 4;
        }else if(theta_degree[cycle] >= (2*limit_theta) && theta_degree[cycle] < ((5*limit_theta)/2))
        {
            sector_temp[cycle] = 5;
        }else{
            sector_temp[cycle] = 6;
        }

        mcSector->sector[cycle] = sector_temp[cycle];
        printf("Sector =%d\n", mcSector->sector[cycle]);
    }
    return NO_ERROR;
}

int set_duty(MC_SECTOR_T mcSector, MC_THETA_T mcTheta, MC_VALPHABETA_T mcVAlphaBeta, MC_TDUTY_T *mcTDuty)
{
    float sp, v_ref, time_0;
    float theta_r[30] = {0};
    float theta_degree[30] = {0};
    float v_alpha[30] = {0};
    float v_beta[30] = {0};

    float t_0[30] = {0};
    float t_1[30] = {0};
    float t_2[30] = {0};
    int sector;

    sp = (150)/DCBUS;
    time_0 = 100;
    for (int cycle = 0; cycle < 30; cycle++)
    {

        theta_r[cycle] = mcTheta.theta_rotor[cycle];
        theta_degree[cycle]=(theta_r[cycle]*PI)/PI2;
        v_alpha[cycle] = mcVAlphaBeta.v_alpha[cycle];
        v_beta[cycle] = mcVAlphaBeta.v_beta[cycle];

        sector = mcSector.sector[cycle];



                if(mcSector.sector[cycle] == 1)
                {

                    t_1[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((sin(((sector*PI)/3)*(PI2/PI))*cos(theta_r[cycle]))-(cos(((sector*PI)/3)*(PI2/PI))*sin(theta_r[cycle])));
                    t_2[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((-cos(theta_r[cycle])*sin((((sector-1)*PI)/3)*(PI2/PI)))+(sin(theta_r[cycle])*cos((((sector-1)*PI)/3)*(PI2/PI))));
                    t_0[cycle] = time_0-t_1[cycle]-t_2[cycle];

                }
                if(mcSector.sector[cycle] == 2)
                {
                    t_1[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((sin(((sector*PI)/3)*(PI2/PI))*cos((theta_r[cycle]-(PI/3))*(PI2/PI)))-(cos(((sector*PI)/3)*(PI2/PI))*sin((theta_r[cycle]-(PI/3))*(PI2/PI))));
                    t_2[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((-cos(theta_r[cycle])*sin((((sector-1)*PI)/3)*(PI2/PI)))+(sin(theta_r[cycle])*cos((((sector-1)*PI)/3)*(PI2/PI))));
                    t_0[cycle] = time_0-t_1[cycle]-t_2[cycle];
                }
                if(mcSector.sector[cycle] == 3)
                {
                    t_1[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((sin(((sector*PI)/3)*(PI2/PI))*cos(theta_r[cycle]))-(cos(((sector*PI)/3)*(PI2/PI))*sin(theta_r[cycle])));
                    t_2[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((-cos(theta_r[cycle])*sin((((sector-1)*PI)/3)*(PI2/PI)))+(sin(theta_r[cycle])*cos((((sector-1)*PI)/3)*(PI2/PI))));
                    t_0[cycle] = time_0-t_1[cycle]-t_2[cycle];
                }
                if(mcSector.sector[cycle] == 4)
                {
                    t_1[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((sin(((sector*PI)/3)*(PI2/PI))*cos(theta_r[cycle]))-(cos(((sector*PI)/3)*(PI2/PI))*sin(theta_r[cycle])));
                    t_2[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((-cos(theta_r[cycle])*sin((((sector-1)*PI)/3)*(PI2/PI)))+(sin(theta_r[cycle])*cos((((sector-1)*PI)/3)*(PI2/PI))));
                    t_0[cycle] = time_0-t_1[cycle]-t_2[cycle];
                }
                if(mcSector.sector[cycle] == 5)
                {
                    t_1[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((sin(((sector*PI)/3)*(PI2/PI))*cos(theta_r[cycle]))-(cos(((sector*PI)/3)*(PI2/PI))*sin(theta_r[cycle])));
                    t_2[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((-cos(theta_r[cycle])*sin((((sector-1)*PI)/3)*(PI2/PI)))+(sin(theta_r[cycle])*cos((((sector-1)*PI)/3)*(PI2/PI))));
                    t_0[cycle] = time_0-t_1[cycle]-t_2[cycle];
                }
                if(mcSector.sector[cycle] == 6)
                {
                    t_1[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((sin(((sector*PI)/3)*(PI2/PI))*cos(theta_r[cycle]))-(cos(((sector*PI)/3)*(PI2/PI))*sin(theta_r[cycle])));
                    t_2[cycle] = (sqrt(3)*time_0*(sqrt((v_alpha[cycle]*v_alpha[cycle])+(v_beta[cycle]*v_beta[cycle]))))/(DCBUS)*((-cos(theta_r[cycle])*sin((((sector-1)*PI)/3)*(PI2/PI)))+(sin(theta_r[cycle])*cos((((sector-1)*PI)/3)*(PI2/PI))));
                    t_0[cycle] = time_0-t_1[cycle]-t_2[cycle];
                }

        mcTDuty->t0_duty[cycle] = t_0[cycle];
        mcTDuty->t1_duty[cycle] = t_1[cycle];
        mcTDuty->t2_duty[cycle] = t_2[cycle];

        printf("t0 duty=%f t1 duty=%f t2 duty=%f\n", mcTDuty->t0_duty[cycle], mcTDuty->t1_duty[cycle], mcTDuty->t2_duty[cycle]);

    }
    return NO_ERROR;

}

