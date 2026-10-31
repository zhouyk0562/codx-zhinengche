#ifndef _zitai_jiesuan_h_
#define _zitai_jiesuan_h_

#include "JiuZhouCaiJi.h"

typedef struct
{
    float heng_gun_jiao;
    float fu_yang_jiao;
    float hang_xiang_jiao;

    float jia_su_lvbo_x;
    float jia_su_lvbo_y;
    float jia_su_lvbo_z;
    float tuo_luo_lvbo_x;
    float tuo_luo_lvbo_y;
    float tuo_luo_lvbo_z;
    float ci_lvbo_x;
    float ci_lvbo_y;
    float ci_lvbo_z;

    float tuo_luo_ling_pian_x;
    float tuo_luo_ling_pian_y;
    float tuo_luo_ling_pian_z;

    float jia_su_bili;
    float tuo_luo_bili;
} ZiTaiJieSuanJieGou;

void ZiTaiJieSuan_ChuShiHua(ZiTaiJieSuanJieGou *ji_suan,
                            const JiuZhouCaiJiShuJu *shu_ju,
                            float jia_su_bili,
                            float tuo_luo_bili);
void ZiTaiJieSuan_GengXin(ZiTaiJieSuanJieGou *ji_suan,
                          const JiuZhouCaiJiShuJu *shu_ju,
                          float cai_yang_miao);

#endif
