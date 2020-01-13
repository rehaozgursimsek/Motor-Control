#include <math.h>
#include "main.h"
#include "clarke.h"
#include "error_table.h"
#include "limitation.h"

int get_clarke(int a_phase, int b_phase, int c_phase, float *alpha, float *beta)
{

    if (a_phase > MAX_i_phase || b_phase > MAX_i_phase || c_phase > MAX_i_phase)
    {
        error_fl = 1;
        return CURRENT_LIMIT_MISMATCH;
    }

    if (a_phase < MIN_i_phase || b_phase < MIN_i_phase || c_phase < MIN_i_phase)
    {
        error_fl = 1;
        return CURRENT_LIMIT_MISMATCH;
    }

    *alpha = a_phase;
    *beta = -((a_phase * (1 / (sqrt(3)))) + (2 * b_phase * (1 / (sqrt(3)))));

    return NO_ERROR;
}

int get_inv_clarke(float v_alpha, float v_beta, float* v_a, float* v_b, float* v_c)
{
    float va_temp, vb_temp, vc_temp;
    va_temp = v_alpha;
    vb_temp = -(v_alpha/2) + ((sqrt(3)/2)*(v_beta));
    vc_temp = -(v_alpha/2) - ((sqrt(3)/2)*(v_beta));

    if(va_temp < MIN_v_phase || vb_temp < MIN_v_phase || vc_temp < MIN_v_phase)
    {
        error_fl = 1;
        return VOLTAGE_LIMIT_MISMATCH;
    }
    if(va_temp > MAX_v_phase || vb_temp > MAX_v_phase || vc_temp > MAX_v_phase)
    {
        error_fl = 1;
        return VOLTAGE_LIMIT_MISMATCH;
    }

    *v_a = va_temp;
    *v_b = vb_temp;
    *v_c = vc_temp;

    return NO_ERROR;
}
