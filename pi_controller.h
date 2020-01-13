#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

//struct CONT_PAR{
//    float k_p = 4;
//    float k_i = 2;
//};

//struct CONT_PAR* cpar;

/**
 * @brief pi_controller
 * @param i_ref
 * @param i
 * @param k_p
 * @param k_i
 * @param v_int
 * @param v_prop
 * @param v_pi
 * @return
 */
float pi_controller(float i_ref, float i, float k_p, float k_i, float *v_int, float *v_prop, float *v_pi);


#endif // PI_CONTROLLER_H
