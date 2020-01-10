#ifndef PARK_H
#define PARK_H

/**
 * @brief get_park
 * @param alpha_phase
 * @param beta_phase
 * @param theta_r
 * @param i_d
 * @param i_q
 * @return
 */
int get_park(float alpha_phase, float beta_phase, float theta_r, float* i_d, float* i_q);

/**
 * @brief get_inv_park
 * @param v_d
 * @param v_q
 * @param theta_r
 * @param v_alpha
 * @param v_beta
 * @return
 */
int get_inv_park(float v_d, float v_q, float theta_r, float* v_alpha, float* v_beta);

#endif // PARK_H
