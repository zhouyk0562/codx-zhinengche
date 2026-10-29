#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "JiuZhouCaiJi.h"
#include "DingShiZhongDuan.h"

static void ding_shi_hui_diao(uint32 zhuang_tai, void *zhi_zhen)
{
    (void)zhuang_tai;
    (void)zhi_zhen;

    JiuZhouCaiJi_ZhouQiZhongDuan();
}

void DingShiZhongDuan_ChuShiHua(void)
{
    interrupt_set_priority(TIMG0_INT_IRQn, 1);
    pit_ms_init(CAIJI_DINGSHI,
                CAIJI_ZHOUQI_MS,
                ding_shi_hui_diao,
                NULL);
    interrupt_global_enable(0);
}
