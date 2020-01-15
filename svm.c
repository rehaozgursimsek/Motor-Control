#include "svm.h"
#include "error_table.h"
#include "limitation.h"
#include "main.h"
#include <math.h>
#include <stdlib.h>


int get_ident_theta(float v_alpha, float v_beta, float *iden_theta)
{
    float tht;
    tht = atan2f(v_beta,  v_alpha);

    if(tht >= 0)
    {
        *iden_theta = tht;
    }else{
        *iden_theta = 2*PI2 + tht;
    }
    *iden_theta = ((*iden_theta)*PI)/(PI2);
    return NO_ERROR;
}

int get_sector_ident(float iden_theta, unsigned char *sector){

    float limit_theta = 2*PI/3;

    if(iden_theta >= 0 && iden_theta < (limit_theta/2))
    {
        *sector = 1;
    } else if(iden_theta >= (limit_theta/2) && iden_theta < limit_theta)
    {
        *sector = 2;
    }else if(iden_theta >= limit_theta && iden_theta < ((3*limit_theta)/2))
    {
         *sector = 3;
    }else if(iden_theta >= ((3*limit_theta)/2) && iden_theta < (2*limit_theta))
    {
         *sector = 4;
    }else if(iden_theta >= (2*limit_theta) && iden_theta < ((5*limit_theta)/2))
    {
         *sector = 5;
    }else{
         *sector = 6;
    }

    return NO_ERROR;
}

int set_duty(unsigned char sector, float iden_theta, float dc_bus, float v_a,float v_b, float v_c, float v_alpha, float v_beta, float *v0_duty, float *v1_duty, float *v2_duty){
        float v1_temp, v2_temp, v0_temp;

        if(sector == 1)
            {
                if(v_b < 0)
                {
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
                    //v1_temp = sqrt(2)* sqrt(pow(v_alpha,2)+pow(v_beta,2))*(cos((iden_theta+(PI/6))*(PI2/180)));
                    //v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(sin((iden_theta)*(PI2/180)));

                }else{
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
                    //v1_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(cos((iden_theta+(PI/6))*(PI2/180)));
                    //v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(sin((iden_theta)*(PI2/180)));
                }
            }
            if(sector == 2)
            {
                if(v_a < 0)
                {
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
                    //v1_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(cos((iden_theta+(PI/6))*(PI2/180)));
                    //v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(sin((iden_theta)*(PI2/180)));
                }else{
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
                    //v1_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(cos((iden_theta+(PI/6))*(PI2/180)));
                    //v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(sin((iden_theta)*(PI2/180)));
                }
            }
            if(sector == 3)
            {
                if(v_c < 0)
                {
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
//                    v1_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(cos((iden_theta+(PI/6))*(PI2/180)));
//                    v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(sin((iden_theta)*(PI2/180)));
                }else{
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = 100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
//                    v1_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(cos((iden_theta+(PI/6))*(PI2/180)));
//                    v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(sin((iden_theta)*(PI2/180)));
                }
            }
            if(sector == 4)
            {
                if(v_b < 0)
                {
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta-PI/6)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
                    //v1_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(-cos((iden_theta+(PI/6))*(PI2/180)));
                    //v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(-sin((iden_theta)*(PI2/180)));
                }else{
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = 100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta-PI/6)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
                    //v1_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(-cos((iden_theta+(PI/6))*(PI2/180)));
                    //v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(-sin((iden_theta)*(PI2/180)));
                }
            }
            if(sector == 5)
            {
                if(v_a < 0)
                {
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
//                    v1_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(cos((iden_theta+(PI/6))*(PI2/180)));
//                    v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(sin((iden_theta)*(PI2/180)));

                }else{
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
//                    v1_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(cos((iden_theta+(PI/6))*(PI2/180)));
//                    v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(sin((iden_theta)*(PI2/180)));
                }

            }
            if(sector == 6)
            {
                if(v_c < 0)
                {
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
//                    v1_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(cos((iden_theta+(PI/6))*(PI2/180)));
//                    v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(sin((iden_theta)*(PI2/180)));
                }else{
                    v1_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin(((PI/3)-(iden_theta))*(PI2/180)));
                    v2_temp = -100*(2/sqrt(3))*(sqrt(pow(v_alpha,2)+pow(v_beta,2))/dc_bus)*(sin((iden_theta)*(PI2/180)));
                    v0_temp = 100-v1_temp-v2_temp;
//                    v1_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(cos((iden_theta+(PI/6))*(PI2/180)));
//                    v2_temp = sqrt(2)*sqrt(pow(v_alpha,2)+pow(v_beta,2))*(sin((iden_theta)*(PI2/180)));
                }
            }
        *v0_duty = v0_temp;
        *v1_duty = v1_temp;
        *v2_duty = v2_temp;


        return NO_ERROR;
}

//int set_duty(int sector, float dc_bus, float v_a, float v_b, float v_c, float *va_duty, float *vb_duty, float *vc_duty){
//    float va_temp, vb_temp, vc_temp;

//    if(sector == 1)
//        {
//            if(v_b < 0)
//            {
//                va_temp = ((dc_bus-v_a)/(dc_bus))*100.0;
//                vb_temp = (-(v_c)/(dc_bus))*100.0;
//                vc_temp = (-(v_b)/(dc_bus))*100.0;
//            }else{
//                va_temp = (v_b/(dc_bus))*100.0;
//                vb_temp = (v_a/(dc_bus))*100.0;
//                vc_temp = ((dc_bus+v_c)/(dc_bus))*100.0;
//            }
//        }
//        if(sector == 2)
//        {
//            if(v_a < 0)
//            {
//                va_temp = (-(v_c)/(dc_bus))*100.0;
//                vb_temp = ((dc_bus-v_b)/(dc_bus))*100.0;
//                vc_temp = (-(v_a)/(dc_bus))*100.0;
//            }else{
//                va_temp = (v_b/(dc_bus))*100.0;
//                vb_temp = (v_a/(dc_bus))*100.0;
//                vc_temp = ((dc_bus+v_c)/(dc_bus))*100.0;
//            }
//        }
//        if(sector == 3)
//        {
//            if(v_c < 0)
//            {
//                va_temp = (-(v_c)/(dc_bus))*100.0;
//                vb_temp = ((dc_bus-v_b)/(dc_bus))*100.0;
//                vc_temp = (-(v_a)/(dc_bus))*100.0;
//            }else{
//                va_temp = ((dc_bus+v_a)/(dc_bus))*100.0;
//                vb_temp = ((v_c)/(dc_bus))*100.0;
//                vc_temp = ((v_b)/(dc_bus))*100.0;
//            }
//        }
//        if(sector == 4)
//        {
//            if(v_b < 0)
//            {
//                va_temp = (-(v_b)/(dc_bus))*100.0;
//                vb_temp = (-(v_a)/(dc_bus))*100.0;
//                vc_temp = ((dc_bus-v_c)/(dc_bus))*100.0;
//            }else{
//                va_temp = ((dc_bus+v_a)/(dc_bus))*100.0;
//                vb_temp = (v_c/(dc_bus))*100.0;
//                vc_temp = (v_b/(dc_bus))*100.0;
//            }
//        }
//        if(sector == 5)
//        {
//            if(v_a < 0)
//            {
//                va_temp = (-(v_b)/(dc_bus))*100.0;
//                vb_temp = (-(v_a)/(dc_bus))*100.0;
//                vc_temp = ((dc_bus-v_c)/(dc_bus))*100.0;

//            }else{
//                va_temp = (v_a/(dc_bus))*100.0;
//                vb_temp = ((dc_bus-v_b)/(dc_bus))*100.0;
//                vc_temp = (v_c/(dc_bus))*100.0;
//            }

//        }
//        if(sector == 6)
//        {
//            if(v_c < 0)
//            {
//                va_temp = (v_a/(dc_bus))*100.0;
//                vb_temp = ((dc_bus-v_b)/(dc_bus))*100.0;
//                vc_temp = (v_c/(dc_bus))*100.0;
//            }else{
//                va_temp = ((dc_bus-v_a)/(dc_bus))*100.0;
//                vb_temp = (-(v_c)/(dc_bus))*100.0;
//                vc_temp = (-(v_b)/(dc_bus))*100.0;
//            }
//        }

//    *va_duty = va_temp;
//    *vb_duty = vb_temp;
//    *vc_duty = vc_temp;

//    return NO_ERROR;
//}


