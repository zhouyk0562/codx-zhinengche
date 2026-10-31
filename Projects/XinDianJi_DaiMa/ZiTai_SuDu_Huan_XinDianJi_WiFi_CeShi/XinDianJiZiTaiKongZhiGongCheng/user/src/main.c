#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "JiuZhouCaiJi.h"
#include "ZiTaiSuDu.h"
#include "DianJiKongZhi.h"
#include "WifiChuanShu.h"
#include "DingShiZhongDuan.h"
#include "ZhuXunHuan.h"

// 程序入口：只负责初始化，20 ms/50 ms 周期任务在独立模块中。
int main(void)
{
    clock_init(SYSTEM_CLOCK_80M);
    debug_init();

    JiuZhouCaiJi_ChuShiHua();
    ZiTaiSuDu_ChuShiHua();
    DianJiKongZhi_ChuShiHua();
    WifiChuanShu_ChuShiHua();
    DingShiZhongDuan_ChuShiHua();

    ZhuXunHuan();

    return 0;
}
