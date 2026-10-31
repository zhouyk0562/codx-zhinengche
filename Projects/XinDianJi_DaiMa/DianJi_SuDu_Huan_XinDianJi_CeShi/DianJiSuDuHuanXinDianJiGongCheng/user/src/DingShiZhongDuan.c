#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "DianJiKongZhi.h"
#include "DingShiZhongDuan.h"

static void ding_shi_hui_diao(uint32 zhuangtai, void *zhi_zhen)
{
    (void)zhuangtai;
    (void)zhi_zhen;

    DianJiKongZhi_ZhouQiZhongDuan();
}

void DingShiZhongDuan_ChuShiHua(void)
{
    interrupt_set_priority(TIMG0_INT_IRQn, 1);
    pit_ms_init(KONGZHI_DINGSHI,
                KONGZHI_ZHOUQI_MS,
                ding_shi_hui_diao,
                NULL);
    interrupt_global_enable(0);
}
