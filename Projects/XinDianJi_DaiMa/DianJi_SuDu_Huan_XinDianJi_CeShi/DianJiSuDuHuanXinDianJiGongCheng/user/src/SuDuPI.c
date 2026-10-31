#include "SuDuPI.h"

static float xian_fu(float shu_zhi, float zui_xiao, float zui_da)
{
    if(shu_zhi > zui_da)
    {
        return zui_da;
    }
    if(shu_zhi < zui_xiao)
    {
        return zui_xiao;
    }
    return shu_zhi;
}

void SuDuPI_ChuShiHua(SuDuPI_JieGou *pi,
                      float kp,
                      float ki,
                      float caiyang_miao,
                      float jifen_shangxian,
                      float shuchu_shangxian,
                      float chu_shi_jifen)
{
    pi->kp = kp;
    pi->ki = ki;
    pi->caiyang_miao = caiyang_miao;
    pi->jifen_shangxian = jifen_shangxian;
    pi->shuchu_shangxian = shuchu_shangxian;
    pi->jifen = xian_fu(chu_shi_jifen,
                        -jifen_shangxian,
                        jifen_shangxian);
    pi->shuchu = 0.0f;
}

float SuDuPI_JiSuan(SuDuPI_JieGou *pi, float wucha)
{
    float jifen_xiaci;
    float shuchu_xiaci;
    float jifen_xian_zhi;

    jifen_xiaci = pi->jifen + wucha * pi->caiyang_miao;
    jifen_xiaci = xian_fu(jifen_xiaci,
                          -pi->jifen_shangxian,
                          pi->jifen_shangxian);

    shuchu_xiaci = pi->kp * wucha + pi->ki * jifen_xiaci;

    // 输出饱和时，不让同方向误差继续累积积分。
    if(shuchu_xiaci > pi->shuchu_shangxian && wucha > 0.0f)
    {
        jifen_xian_zhi = (pi->shuchu_shangxian -
                          pi->kp * wucha) / pi->ki;
        if(jifen_xiaci > jifen_xian_zhi)
        {
            jifen_xiaci = xian_fu(jifen_xian_zhi,
                                  -pi->jifen_shangxian,
                                  pi->jifen_shangxian);
        }
    }
    else if(shuchu_xiaci < -pi->shuchu_shangxian && wucha < 0.0f)
    {
        jifen_xian_zhi = (-pi->shuchu_shangxian -
                          pi->kp * wucha) / pi->ki;
        if(jifen_xiaci < jifen_xian_zhi)
        {
            jifen_xiaci = xian_fu(jifen_xian_zhi,
                                  -pi->jifen_shangxian,
                                  pi->jifen_shangxian);
        }
    }

    pi->jifen = jifen_xiaci;
    pi->shuchu = pi->kp * wucha + pi->ki * pi->jifen;
    pi->shuchu = xian_fu(pi->shuchu,
                         -pi->shuchu_shangxian,
                         pi->shuchu_shangxian);

    return pi->shuchu;
}

void SuDuPI_QingLing(SuDuPI_JieGou *pi)
{
    pi->jifen = 0.0f;
    pi->shuchu = 0.0f;
}
