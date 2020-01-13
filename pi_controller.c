#include "pi_controller.h"
#include "limitation.h"
#include "error_table.h"
#include "main.h"
#include <math.h>


float pi_controller(float i_ref, float i, float k_p, float k_i, float *v_int, float *v_prop, float *v_pi)
{
    float v_integral, v_proportional, v_sum;

    v_integral = *v_int + (k_i)*(i_ref-i);
    v_proportional = (i_ref-i)*(k_p);
    v_sum = v_integral + v_proportional;

    if (v_sum < MIN_vd_vq || v_sum > MAX_vd_vq)
    {
        error_fl = 1;
        return VOLTAGE_LIMIT_MISMATCH;
    }
    if (i_ref < MIN_i_phase || i_ref > MAX_i_phase)
    {
        error_fl = 1;
        return CURRENT_LIMIT_MISMATCH;
    }

    *v_pi = v_sum;
    *v_int = v_integral;
    *v_prop = v_proportional;

    return NO_ERROR;
}
