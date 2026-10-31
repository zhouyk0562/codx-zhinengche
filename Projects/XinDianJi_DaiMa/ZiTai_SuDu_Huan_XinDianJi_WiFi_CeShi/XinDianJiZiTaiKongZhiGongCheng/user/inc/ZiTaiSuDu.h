#ifndef _zitai_sudu_h_
#define _zitai_sudu_h_

#include "JiuZhouCaiJi.h"

typedef struct
{
    float yuan_shi_jiao;
    float kong_zhi_jiao;
    float mu_biao_lun_rpm;
    uint8 ling_dian_wan_cheng;
} ZiTaiSuDuShuJu;

void  ZiTaiSuDu_ChuShiHua(void);
void  ZiTaiSuDu_GengXin(const JiuZhouCaiJiShuJu *shu_ju);
uint8 ZiTaiSuDu_QuZuiXinShuJu(ZiTaiSuDuShuJu *shu_ju);

#endif
