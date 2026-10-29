#ifndef _dianji_kongzhi_h_
#define _dianji_kongzhi_h_

#include "zf_common_typedef.h"

typedef struct
{
    float kong_zhi_jiao;
    int32 mubiao_lun_su_rpm;
    int32 shiji_lun_su_rpm;
    int32 zhankongbi_baifen;
    int32 bianmaqi_rpm;
    int32 yuan_shi_maichong;
    uint8 duzhuan_zhuangtai;
} DianJiKongZhiShuJu;

void  DianJiKongZhi_ChuShiHua(void);
void  DianJiKongZhi_ZhouQiZhongDuan(void);
uint8 DianJiKongZhi_QuZuiXinShuJu(DianJiKongZhiShuJu *shu_ju);

#endif
