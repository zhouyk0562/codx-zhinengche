#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "JiuZhouCaiJi.h"
#include "ZiTaiJieSuan.h"

static JiuZhouCaiJiShuJu zui_xin_shu_ju;
static volatile uint8 you_xin_shu_ju;
static ZiTaiJieSuanJieGou zi_tai_ji_suan;
static uint8 zi_tai_yi_chu_shi_hua;

void JiuZhouCaiJi_ChuShiHua(void)
{
    uint8 zhuang_tai;

    do
    {
        zhuang_tai = imu963ra_init();
        printf("\r\nIMU963RA ChuShiHua ZhuangTai=%u\r\n", zhuang_tai);
    }while(zhuang_tai);

    zi_tai_yi_chu_shi_hua = 0;
    you_xin_shu_ju = 0;
    printf("\r\nIMU963RA ZiTai ShuJu CaiJi QiDong\r\n");
}

void JiuZhouCaiJi_ZhouQiZhongDuan(void)
{
    imu963ra_get_acc();
    imu963ra_get_gyro();
    imu963ra_get_mag();

    zui_xin_shu_ju.jia_su_x = imu963ra_acc_x;
    zui_xin_shu_ju.jia_su_y = imu963ra_acc_y;
    zui_xin_shu_ju.jia_su_z = imu963ra_acc_z;
    zui_xin_shu_ju.tuo_luo_x = imu963ra_gyro_x;
    zui_xin_shu_ju.tuo_luo_y = imu963ra_gyro_y;
    zui_xin_shu_ju.tuo_luo_z = imu963ra_gyro_z;
    zui_xin_shu_ju.ci_x = imu963ra_mag_x;
    zui_xin_shu_ju.ci_y = imu963ra_mag_y;
    zui_xin_shu_ju.ci_z = imu963ra_mag_z;

    if(0 == zi_tai_yi_chu_shi_hua)
    {
        if(0 == zui_xin_shu_ju.ci_x &&
           0 == zui_xin_shu_ju.ci_y &&
           0 == zui_xin_shu_ju.ci_z)
        {
            return;
        }

        ZiTaiJieSuan_ChuShiHua(&zi_tai_ji_suan,
                               &zui_xin_shu_ju,
                               imu963ra_transition_factor[0],
                               imu963ra_transition_factor[1]);
        zi_tai_yi_chu_shi_hua = 1;
    }
    else
    {
        ZiTaiJieSuan_GengXin(&zi_tai_ji_suan,
                             &zui_xin_shu_ju,
                             (float)CAIJI_ZHOUQI_MS / 1000.0f);
    }

    zui_xin_shu_ju.heng_gun_jiao = zi_tai_ji_suan.heng_gun_jiao;
    zui_xin_shu_ju.fu_yang_jiao = zi_tai_ji_suan.fu_yang_jiao;
    zui_xin_shu_ju.hang_xiang_jiao = zi_tai_ji_suan.hang_xiang_jiao;
    you_xin_shu_ju = 1;
}

uint8 JiuZhouCaiJi_QuZuiXinShuJu(JiuZhouCaiJiShuJu *shu_ju)
{
    uint32 zhuang_tai;
    uint8 you_shu_ju;

    if(0 == you_xin_shu_ju)
    {
        return 0;
    }

    zhuang_tai = interrupt_global_disable();
    you_shu_ju = you_xin_shu_ju;
    if(you_shu_ju)
    {
        *shu_ju = zui_xin_shu_ju;
        you_xin_shu_ju = 0;
    }
    interrupt_global_enable(zhuang_tai);

    return you_shu_ju;
}

uint8 JiuZhouCaiJi_QuZuiXinShuJuBaoLiu(JiuZhouCaiJiShuJu *shu_ju)
{
    uint32 zhuang_tai;
    uint8 you_shu_ju;

    zhuang_tai = interrupt_global_disable();
    you_shu_ju = you_xin_shu_ju;
    if(you_shu_ju)
    {
        *shu_ju = zui_xin_shu_ju;
    }
    interrupt_global_enable(zhuang_tai);

    return you_shu_ju;
}
