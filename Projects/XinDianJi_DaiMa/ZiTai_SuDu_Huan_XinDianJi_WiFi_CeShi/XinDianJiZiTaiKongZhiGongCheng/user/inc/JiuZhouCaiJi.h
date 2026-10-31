#ifndef _jiuzhou_caiji_h_
#define _jiuzhou_caiji_h_

#include "zf_common_typedef.h"

typedef struct
{
    int16 jia_su_x;
    int16 jia_su_y;
    int16 jia_su_z;
    int16 tuo_luo_x;
    int16 tuo_luo_y;
    int16 tuo_luo_z;
    int16 ci_x;
    int16 ci_y;
    int16 ci_z;
    float heng_gun_jiao;
    float fu_yang_jiao;
    float hang_xiang_jiao;
} JiuZhouCaiJiShuJu;

void  JiuZhouCaiJi_ChuShiHua(void);
void  JiuZhouCaiJi_ZhouQiZhongDuan(void);
uint8 JiuZhouCaiJi_QuZuiXinShuJu(JiuZhouCaiJiShuJu *shuju);
uint8 JiuZhouCaiJi_QuZuiXinShuJuBaoLiu(JiuZhouCaiJiShuJu *shuju);

#endif
