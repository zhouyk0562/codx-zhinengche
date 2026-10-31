#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "JiuZhouCaiJi.h"
#include "ZiTaiSuDu.h"
#include "DianJiKongZhi.h"
#include "DingShiZhongDuan.h"

static void zi_tai_hui_diao(uint32 zhuang_tai, void *zhi_zhen)
{
    JiuZhouCaiJiShuJu shu_ju;

    (void)zhuang_tai;
    (void)zhi_zhen;

    JiuZhouCaiJi_ZhouQiZhongDuan();
    if(JiuZhouCaiJi_QuZuiXinShuJuBaoLiu(&shu_ju))
    {
        ZiTaiSuDu_GengXin(&shu_ju);
    }
}

static void dian_ji_hui_diao(uint32 zhuang_tai, void *zhi_zhen)
{
    (void)zhuang_tai;
    (void)zhi_zhen;

    DianJiKongZhi_ZhouQiZhongDuan();
}

void DingShiZhongDuan_ChuShiHua(void)
{
    interrupt_set_priority(TIMG0_INT_IRQn, 1);
    interrupt_set_priority(TIMG6_INT_IRQn, 1);

    pit_ms_init(CAIJI_DINGSHI,
                CAIJI_ZHOUQI_MS,
                zi_tai_hui_diao,
                NULL);
    pit_ms_init(KONGZHI_DINGSHI,
                KONGZHI_ZHOUQI_MS,
                dian_ji_hui_diao,
                NULL);

    interrupt_global_enable(0);
}
