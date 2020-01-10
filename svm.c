#include "svm.h"
#include "error_table.h"
#include "limitation.h"
#include <math.h>


int get_identf_theta(float v_d, float v_q, float *iden_theta)
{
    float tht;
    tht = atan2f( v_q,  v_d);

    if(tht >= 0)
    {
        *iden_theta = tht;
    }else{
        *iden_theta = 2*PI2 + tht;
    }
    *iden_theta = ((*iden_theta)*PI)/(PI2);
    return NO_ERROR;
}

int get_sector_identf(float iden_theta, int *sector){

    float limit_theta = 2*PI/3;

    if(iden_theta >= 0 && iden_theta < (limit_theta/2))
    {
        *sector = 1;
    } else if(iden_theta >= (limit_theta/2) && iden_theta < limit_theta)
        {
            *sector = 2;
        }else if(iden_theta >= limit_theta && iden_theta < ((3*limit_theta)/2))
            {
                *sector = 3;
            }else if(iden_theta >= ((3*limit_theta)/2) && iden_theta < (2*limit_theta))
                {
                    *sector = 4;
                }else if(iden_theta >= (2*limit_theta) && iden_theta < ((5*limit_theta)/2))
                    {
                        *sector = 5;
                    }else{
                            *sector = 6;
                         }
    return NO_ERROR;

}

