#include "zf_common_headfile.h"
#include "DianJiKongZhi.h"
#include "ZhuXunHuan.h"

void ZhuXunHuan(void)
{
    DianJiKongZhiShuJu shu_ju;

    while(true)
    {
        if(DianJiKongZhi_QuZuiXinShuJu(&shu_ju))
        {
            printf("\r\nKAIHUAN state=%u dir=%d duty=%u%% pulse=%ld enc=%ld wheel=%ld\r\n",
                   shu_ju.zhuang_tai,
                   shu_ju.fang_xiang,
                   shu_ju.zhan_kong_bi,
                   (long)shu_ju.yuan_shi_maichong,
                   (long)shu_ju.bian_ma_qi_rpm,
                   (long)shu_ju.lun_su_rpm);
        }
    }
}
