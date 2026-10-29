#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "JiuZhouCaiJi.h"
#include "WifiChuanShu.h"
#include "DingShiZhongDuan.h"
#include "ZhuXunHuan.h"

int main(void)
{
    clock_init(SYSTEM_CLOCK_80M);
    debug_init();

    JiuZhouCaiJi_ChuShiHua();
    WifiChuanShu_ChuShiHua();
    DingShiZhongDuan_ChuShiHua();

    ZhuXunHuan();

    return 0;
}
