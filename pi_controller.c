#include "pi_controller.h"
#include "limitation.h"
#include "error_table.h"
#include <math.h>


float pi_controller(float i_ref, float i, float k_p, float k_i, float *v_int, float *v_prop, float *v_pi)
{
    float v_integral, v_proportional;

    v_integral = *v_int + (k_i)*(i_ref-i);
    v_proportional = (i_ref-i)*(k_p);
    *v_pi = v_integral + v_proportional;
    *v_int = v_integral;
    *v_prop = v_proportional;

    if (*v_pi < MIN_vd_vq)
    {
        return VOLTAGE_LIMIT_MISMATCH;
    }
    if (*v_pi > MAX_vd_vq)
    {
        return VOLTAGE_LIMIT_MISMATCH;
    }

    return NO_ERROR;
}
