#include "clarke.h"
#include "error_table.h"
#include "park.h"
#include "pi_controller.h"
#include "typedef.h"
#include "svm.h"
#include "limitation.h"
#include <stdio.h>

float_t a = 0.5;
float_t b = -1.5;
float_t c = 1;

float_t theta_rotor = PI/6;
float_t alpha = 0, beta = 0;
float_t dc_bus = 11;
float_t iden_theta = 0;
int sector = 0;

float_t i_d = 0, i_q = 0;
float_t v_d = 0, v_q = 0;

float_t v_pi = 0; // PI controller voltage output
float_t v_int = 0; // PI controller integral part
float_t v_prop = 0; // PI controller proportional part

float_t v_alpha = 0, v_beta = 0;

float_t id_ref = 0;
float_t iq_ref = 0.1;

float_t k_p = 4;
float_t k_i = 1;

float_t v_a = 0, v_b = 0, v_c = 0;

_Bool error_fl = 0;

int8_t MC_ERROR = 0;
int main()
{
    MC_ERROR = get_clarke(a, b, c , &alpha, &beta);

    if(MC_ERROR != 0){
        error_fl = 1;
    } else {
        error_fl = 0;
        MC_ERROR = get_park(alpha,beta,theta_rotor, &i_d, &i_q);

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
                        MC_ERROR = get_identf_theta(v_d, v_q, &iden_theta);
                        MC_ERROR = get_sector_identf(iden_theta, &sector);
                    }
                }
            }
        }
    }

    printf("a=%f b=%f c=%f id_ref=%f iq_ref=%f theta=%f\n", a, b, c, id_ref, iq_ref, theta_rotor);
    printf("alpha=%f beta=%f id=%f iq=%f\n", alpha, beta, i_d, i_q);
    printf("v_d=%f v_q=%f\n", v_d, v_q);
    printf("v_alpha=%f v_beta=%f\n", v_alpha, v_beta);
    printf("v_a=%f v_b=%f v_c=%f\n", v_a, v_b, v_c);
    printf("ERROR=%d\n", MC_ERROR);
    printf("identification theta=%f Sector=%d\n", iden_theta, sector);

    return 0;

}
