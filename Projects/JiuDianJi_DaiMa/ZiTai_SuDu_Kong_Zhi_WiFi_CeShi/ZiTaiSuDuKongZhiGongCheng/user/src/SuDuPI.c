#include "SuDuPI.h"

void SuDuPI_ChuShiHua(SuDuPI_JieGou *pi,
                      float kp,
                      float ki,
                      float caiyang_miao,
                      float jifen_shangxian,
                      float shuchu_shangxian)
{
    pi->kp = kp;
    pi->ki = ki;
    pi->caiyang_miao = caiyang_miao;
    pi->jifen_shangxian = jifen_shangxian;
    pi->shuchu_shangxian = shuchu_shangxian;
    pi->jifen = 0.0f;
    pi->shuchu = 0.0f;
}

float SuDuPI_JiSuan(SuDuPI_JieGou *pi, float wucha)
{
    float jifen_xiaci = pi->jifen + wucha * pi->caiyang_miao;
    float shuchu_xiaci;

    if(jifen_xiaci > pi->jifen_shangxian)
    {
        jifen_xiaci = pi->jifen_shangxian;
    }
    else if(jifen_xiaci < -pi->jifen_shangxian)
    {
        jifen_xiaci = -pi->jifen_shangxian;
    }

    shuchu_xiaci = pi->kp * wucha + pi->ki * jifen_xiaci;

    // 输出饱和时，只允许把输出拉回允许范围的积分继续累加。
    if((shuchu_xiaci <= pi->shuchu_shangxian &&
        shuchu_xiaci >= -pi->shuchu_shangxian) ||
       (shuchu_xiaci > pi->shuchu_shangxian && wucha < 0.0f) ||
       (shuchu_xiaci < -pi->shuchu_shangxian && wucha > 0.0f))
    {
        pi->jifen = jifen_xiaci;
    }

    pi->shuchu = pi->kp * wucha + pi->ki * pi->jifen;

    if(pi->shuchu > pi->shuchu_shangxian)
    {
        pi->shuchu = pi->shuchu_shangxian;
    }
    else if(pi->shuchu < -pi->shuchu_shangxian)
    {
        pi->shuchu = -pi->shuchu_shangxian;
    }

    return pi->shuchu;
}

void SuDuPI_QingLing(SuDuPI_JieGou *pi)
{
    pi->jifen = 0.0f;
    pi->shuchu = 0.0f;
}
