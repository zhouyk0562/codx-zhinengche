#ifndef _sudu_pi_h_
#define _sudu_pi_h_

#include "zf_common_typedef.h"

typedef struct
{
    float kp;
    float ki;
    float caiyang_miao;
    float jifen_shangxian;
    float shuchu_shangxian;
    float jifen;
    float shuchu;
} SuDuPI_JieGou;

void  SuDuPI_ChuShiHua(SuDuPI_JieGou *pi,
                       float kp,
                       float ki,
                       float caiyang_miao,
                       float jifen_shangxian,
                       float shuchu_shangxian);
float SuDuPI_JiSuan(SuDuPI_JieGou *pi, float wucha);
void  SuDuPI_QingLing(SuDuPI_JieGou *pi);

#endif
