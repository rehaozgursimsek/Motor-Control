#include "clarke.h"
#include "main.h"
#include "error_table.h"
#include "park.h"
#include "pi_controller.h"
#include "typedef.h"
#include "svm.h"
#include "limitation.h"
#include <stdio.h>
#include <stdint.h>


float a = 1;
float b = -2;
float c = 1;

float theta_rotor = PI/5;
//float alpha = 0, beta = 0;
float dc_bus = 11;
float iden_theta = 0;
uint8_t sector = 0;

float i_d = 0, i_q = 0;
float v_d = 0, v_q = 0;

float v_pi = 0; // PI controller voltage output
float v_int = 0; // PI controller integral part
float v_prop = 0; // PI controller proportional part

float v_alpha = 0, v_beta = 0;

float id_ref = 0;
float iq_ref = 1.5;

float k_p = 4;
float k_i = 2;

float v_a = 0, v_b = 0, v_c = 0;
float va_duty =0, vb_duty = 0, vc_duty = 0;
float v0_duty= 0, v1_duty =0, v2_duty = 0;

MC_ALPHABETA_T mcAlphaBeta;

int8_t MC_ERROR = 0;
int main()
{
    MC_ERROR = get_clarke(a, b, c , &mcAlphaBeta);

    if(MC_ERROR != 0){
        error_fl = 1;
    } else {
        error_fl = 0;
        //MC_ERROR = get_park(alpha,beta,theta_rotor, &i_d, &i_q);

        if(MC_ERROR != 0){
            error_fl = 1;
        }else {
            error_fl = 0;
            MC_ERROR = pi_controller(id_ref, i_d, k_p, k_i, &v_int, &v_prop, &v_pi); // PI controller for d current
            v_d = v_pi;
            if(MC_ERROR != 0){
                 error_fl = 1;
            }else{
                error_fl = 0;
                MC_ERROR = pi_controller(iq_ref, i_q, k_p, k_i, &v_int, &v_prop, &v_pi); // PI controller for q current
                v_q = v_pi;
                if(MC_ERROR != 0){
                    error_fl = 1;
                }else{
                    error_fl = 0;
                    MC_ERROR = get_inv_park(v_d, v_q, theta_rotor, &v_alpha, &v_beta);
                    if(MC_ERROR != 0)
                    {
                        error_fl = 1;
                    }else{
                        error_fl = 0;
                        MC_ERROR = get_inv_clarke(v_alpha, v_beta, &v_a, &v_b, &v_c);
                        if(MC_ERROR != 0)
                        {
                            error_fl = 1;
                        }else{
                            error_fl = 0;
                            MC_ERROR = get_ident_theta(v_alpha, v_beta, &iden_theta);
                            MC_ERROR = get_sector_ident(iden_theta, &sector);
                            MC_ERROR = set_duty(sector, iden_theta, dc_bus, v_a, v_b, v_c, v_alpha, v_beta, &v0_duty, &v1_duty, &v2_duty);
                            //MC_ERROR = set_duty(sector, dc_bus, v_a, v_b, v_c, &va_duty, &vb_duty, &vc_duty);
                        }

                    }
                }
            }
        }
    }

    printf("a=%f b=%f c=%f id_ref=%f iq_ref=%f theta=%f\n", a, b, c, id_ref, iq_ref, theta_rotor);
    printf("alpha=%f beta=%f id=%f iq=%f\n", mcAlphaBeta.alpha, mcAlphaBeta.beta, i_d, i_q);
    printf("v_d=%f v_q=%f\n", v_d, v_q);
    printf("v_alpha=%f v_beta=%f\n", v_alpha, v_beta);
    printf("v_a=%f v_b=%f v_c=%f\n", v_a, v_b, v_c);
    printf("ERROR=%d\n", MC_ERROR);
    printf("identification theta=%f Sector=%d\n", iden_theta, sector);
    printf("v1 duty=%f v2 duty=%f v0 duty=%f\n", v1_duty, v2_duty, v0_duty);

    return 0;

}
