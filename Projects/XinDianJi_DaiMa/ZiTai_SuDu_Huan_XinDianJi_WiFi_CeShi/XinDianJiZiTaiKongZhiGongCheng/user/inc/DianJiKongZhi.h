#ifndef _dianji_kongzhi_h_
#define _dianji_kongzhi_h_

#include "zf_common_typedef.h"

typedef struct
{
    float kong_zhi_jiao;
    int32 mu_biao_lun_rpm;
    int32 shi_ji_lun_rpm;
    int32 bian_ma_qi_rpm;
    int32 zhan_kong_bi;
    int32 yuan_shi_maichong;
    uint8 duzhuan_zhuangtai;
} DianJiKongZhiShuJu;

void  DianJiKongZhi_ChuShiHua(void);
void  DianJiKongZhi_ZhouQiZhongDuan(void);
uint8 DianJiKongZhi_QuZuiXinShuJu(DianJiKongZhiShuJu *shu_ju);

#endif
