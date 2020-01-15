#ifndef SVM_H
#define SVM_H

/**
 * @brief get_identf_theta
 * @param v_alpha
 * @param v_beta
 * @param iden_theta
 * @return
 */
int get_ident_theta(float v_alpha, float v_beta, float *iden_theta);

/**
 * @brief get_sector_identf
 * @param iden_theta
 * @param sector
 * @return
 */
int get_sector_ident(float iden_theta, unsigned char *sector);

/**
 * @brief set_duty
 * @param sector
 * @param dc_bus
 * @param v_a
 * @param v_b
 * @param v_c
 * @param va_duty
 * @param vb_duty
 * @param vc_duty
 * @return
 */
//int set_duty(int sector, float dc_bus, float v_a, float v_b, float v_c, float *va_duty, float *vb_duty, float *vc_duty);
int set_duty(unsigned char sector, float iden_theta, float dc_bus, float v_a, float v_b, float v_c, float v_alpha, float v_beta,float *v0_duty, float *v1_duty, float *v2_duty);

#endif // SVM_H
