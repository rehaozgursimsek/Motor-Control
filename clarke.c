#include <math.h>
#include "clarke.h"
#include "error_table.h"
#include "limitation.h"


int get_clarke(int a_phase, int b_phase, int c_phase, float *alpha, float *beta)
{

    if (a_phase > MAX_i_phase || b_phase > MAX_i_phase || c_phase > MAX_i_phase)
    {
        return CURRENT_LIMIT_MISMATCH;
    }

    if (a_phase < MIN_i_phase || b_phase < MIN_i_phase || c_phase < MIN_i_phase)
    {
        return CURRENT_LIMIT_MISMATCH;
    }

    *alpha = a_phase;
    *beta = -((a_phase * (1 / (sqrt(3)))) + (2 * b_phase * (1 / (sqrt(3)))));

    return NO_ERROR;
}

int get_inv_clarke(float v_alpha, float v_beta, float* v_a, float* v_b, float* v_c)
{
    *v_a = v_alpha;
    *v_b = -(v_alpha/2) + ((sqrt(3)/2)*(v_beta));
    *v_c = -(v_alpha/2) - ((sqrt(3)/2)*(v_beta));

    return NO_ERROR;
}
