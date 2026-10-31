#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "DianJiKongZhi.h"
#include "DingShiZhongDuan.h"
#include "WifiChuanShu.h"
#include "ZhuXunHuan.h"

// 程序入口：只负责初始化，周期任务放在中断和主循环模块中。
int main(void)
{
    clock_init(SYSTEM_CLOCK_80M);
    debug_init();

    DianJiKongZhi_ChuShiHua();
    WifiChuanShu_ChuShiHua();
    DingShiZhongDuan_ChuShiHua();

    ZhuXunHuan();

    return 0;
}
