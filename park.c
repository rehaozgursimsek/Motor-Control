#include <math.h>
#include "park.h"
#include "error_table.h"
#include "limitation.h"

int get_park(float alpha_phase, float beta_phase, float theta_r, float* i_d, float* i_q)
{

    *i_d = (alpha_phase * sin(theta_r)) + (beta_phase * cos(theta_r)) ;
    *i_q = (alpha_phase * cos(theta_r)) - (beta_phase * sin(theta_r));

    return NO_ERROR;
}

int get_inv_park(float v_d, float v_q, float theta_r, float* v_alpha, float* v_beta)
{
    *v_alpha = (v_q * cos(theta_r)) + (v_d * sin(theta_r));
    *v_beta = -(v_q * sin(theta_r)) + (v_d * cos(theta_r));

    return NO_ERROR;
}

