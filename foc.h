#ifndef FOC_H
#define FOC_H

#include <stdint.h>
#include "err_table.h"

int8_t MC_ERROR;

typedef struct{
    float pole_pair;

    float res;
    float ld;
    float lq;

    float flux;
}MC_MOTORPAR_T;

typedef struct{
    float id_ref[30];
    float iq_ref[30];
    float id_ref_const;
    float iq_ref_const;
}MC_IDIQREF_T;

typedef struct{
    float speed_ref;
}MC_SPEEDREF_T;

typedef struct{
    float kp_cur;
    float ki_cur;
    float kp_speed;
    float ki_speed;
}MC_CONTPAR_T;

typedef struct {
    float a_sin[30];
    float b_sin[30];
    float c_sin[30];
}MC_ABCPHASE_T;

typedef struct {
    float alpha[30];
    float beta[30];
}MC_IALPHABETA_T;

typedef struct {
    float i_d[30];
    float i_q[30];
}MC_IDQ_T;

typedef struct {
    float theta_rotor[30];
}MC_THETA_T;

typedef struct {
    float v_d[30];
    float v_q[30];
}MC_VDQ_T;

typedef struct {
    float v_pi_d[30]; // PI controller voltage output
    float v_int_d[30]; // PI controller integral part
    float v_prop_d[30]; // PI controller proportional part
    float v_pi_q[30]; // PI controller voltage output
    float v_int_q[30]; // PI controller integral part
    float v_prop_q[30]; // PI controller proportional part
}MC_PIPART_T;

typedef struct {
    float i_pi_q[30];
    float i_int_q[30];
    float i_prop_q[30];

}MC_SPIPART_T;

typedef struct {
    float v_alpha[30];
    float v_beta[30];
}MC_VALPHABETA_T;

typedef struct {
    float v_a[30];
    float v_b[30];
    float v_c[30];
}MC_VABC_T;

typedef struct {
    uint8_t sector[30];
}MC_SECTOR_T;


typedef struct {
    float t0_duty[30];
    float t1_duty[30];
    float t2_duty[30];
}MC_TDUTY_T;

typedef struct {
    MC_MOTORPAR_T mcMotorPar;
    MC_IDIQREF_T mcIdIqRef;
    MC_SPEEDREF_T mcSpeedRef;
    MC_CONTPAR_T mcContPar;
    MC_ABCPHASE_T mcABCPhase;
    MC_IALPHABETA_T mcIAlphaBeta;
    MC_IDQ_T mcIDQ;
    MC_THETA_T mcTheta;
    MC_PIPART_T mcPIPart;
    MC_SPIPART_T mcSPIPart;
    MC_VDQ_T mcVDQ;
    MC_VALPHABETA_T mcVAlphaBeta;
    MC_VABC_T mcVabc;
    MC_SECTOR_T mcSector;
    MC_TDUTY_T mcTDuty;
}MC_FOC_T;

/*
int foc_init(m_foc_t *m_foc,
    void(**fast_loop_handler_)(m_foc_t *m_foc),
    void(**slow_loop_handler_)(m_foc_t *m_foc),
    void(**main_loop_handler_)(m_foc_t *m_foc),
    void **pvfoc,
    unsigned mref_,
    uint16_t **pwm_channels_);
*/
MC_CONTROL_ABORT_T MC_FOC_Init(MC_FOC_T *m_foc_t,
                int(*fast_loop_handler)(MC_FOC_T *mc_foc_t),
                /*float speed_ref*/ float id_ref, float iq_ref);

int fast_loop_control(MC_FOC_T *mc_foc_t);

int speed_pi_controller(MC_SPEEDREF_T mcSpeedRef, MC_IDIQREF_T *mcIdIqRef, float k_ps, float k_is,  MC_SPIPART_T *mcSPIPart);

int sin_wave(MC_THETA_T *mcTheta, MC_ABCPHASE_T *mcABCPhase);

int get_clarke(MC_ABCPHASE_T mcABCPhase, MC_IALPHABETA_T *mcIAlphaBeta);

int get_park(MC_IALPHABETA_T mcIAlphaBeta, MC_THETA_T mcTheta, MC_IDQ_T *mcIDQ);

int pi_controller(MC_IDIQREF_T mcIdIqRef,  MC_IDQ_T mcIDQ, float k_p, float k_i, MC_VDQ_T *mcVDQ, MC_PIPART_T *mcPIPart);

int get_inv_park(MC_VDQ_T mcVDQ, MC_THETA_T mcTheta, MC_VALPHABETA_T *mcVAlphaBeta);

int get_inv_clarke(MC_VALPHABETA_T mcVAlphaBeta, MC_VABC_T *mcVabc);

int get_sector_ident(MC_THETA_T mcTheta, MC_SECTOR_T *mcSector);

int set_duty(MC_SECTOR_T mcSector, MC_THETA_T mcTheta, MC_VALPHABETA_T mcVAlphaBeta, MC_TDUTY_T *mcTDuty);

#endif // FOC_H
