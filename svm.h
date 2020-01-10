#ifndef SVM_H
#define SVM_H

/**
 * @brief get_identf_theta
 * @param v_d
 * @param v_q
 * @param iden_theta
 * @return
 */
int get_identf_theta(float v_d, float v_q, float *iden_theta);

/**
 * @brief get_sector_identf
 * @param iden_theta
 * @param sector
 * @return
 */
int get_sector_identf(float iden_theta, int *sector);

#endif // SVM_H
