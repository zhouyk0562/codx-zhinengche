#ifndef _dianji_kongzhi_h_
#define _dianji_kongzhi_h_

#include "zf_common_typedef.h"

typedef struct
{
    uint8 zhuang_tai;
    int8 fang_xiang;
    uint8 zhan_kong_bi;
    int32 yuan_shi_maichong;
    int32 bian_ma_qi_rpm;
    int32 lun_su_rpm;
} DianJiKongZhiShuJu;

void  DianJiKongZhi_ChuShiHua(void);
void  DianJiKongZhi_ZhouQiZhongDuan(void);
uint8 DianJiKongZhi_QuZuiXinShuJu(DianJiKongZhiShuJu *shu_ju);

#endif
