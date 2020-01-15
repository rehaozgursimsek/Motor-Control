#ifndef CLARKE_H
#define CLARKE_H

#include <main.h>

/**
 * @brief get_clarke
 * @param a_phase
 * @param b_phase
 * @param c_phase
 * @param alpha
 * @param beta
 * @return
 */
int get_clarke(int a_phase, int b_phase, int c_phase, MC_ALPHABETA_T *mcAlphaBeta);

/**
 * @brief get_inv_clarke
 * @param v_alpha
 * @param v_beta
 * @param v_a
 * @param v_b
 * @param v_c
 * @return
 */
int get_inv_clarke(float v_alpha, float v_beta, float* v_a, float* v_b, float* v_c);


#endif // !CLARKE_H

