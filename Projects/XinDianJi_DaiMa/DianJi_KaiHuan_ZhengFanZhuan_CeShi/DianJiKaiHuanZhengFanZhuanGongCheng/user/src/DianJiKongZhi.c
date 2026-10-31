#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "DianJiKongZhi.h"

typedef enum
{
    ZHUANGTAI_TINGZHI_ZHENG = 0,
    ZHUANGTAI_ZHENG_SHENGSU,
    ZHUANGTAI_ZHENG_BAOCHI,
    ZHUANGTAI_TINGZHI_FAN,
    ZHUANGTAI_FAN_SHENGSU,
    ZHUANGTAI_FAN_BAOCHI,
    ZHUANGTAI_TINGZHI_HUIZHENG
} KaiHuanZhuangTai;

static KaiHuanZhuangTai dang_qian_zhuang_tai;
static uint16 zhuang_tai_ji_shu;
static uint8 zhan_kong_bi;
static uint8 you_xin_shu_ju;
static DianJiKongZhiShuJu zui_xin_shu_ju;

static int32 si_she_wu_ru(float shu_zhi)
{
    return (int32)(shu_zhi + (shu_zhi >= 0.0f ? 0.5f : -0.5f));
}

static void dian_ji_shu_chu(uint8 zhan_kong, uint8 fan_xiang)
{
    if(fan_xiang)
    {
        gpio_set_level(DIANJI_FANG_XIANG_PIN, DIANJI_FAN_DIAN_PING);
    }
    else
    {
        gpio_set_level(DIANJI_FANG_XIANG_PIN, DIANJI_ZHENG_DIAN_PING);
    }

    pwm_set_duty(DIANJI_KONG_ZHI_DAI,
                 (uint16)(zhan_kong * (PWM_DUTY_MAX / 100.0f)));
}

static void zhuang_tai_qie_huan(KaiHuanZhuangTai xin_zhuang_tai)
{
    dang_qian_zhuang_tai = xin_zhuang_tai;
    zhuang_tai_ji_shu = 0;
    you_xin_shu_ju = 1;
}

void DianJiKongZhi_ChuShiHua(void)
{
    encoder_dir_init(BIANMAQI_TIMER, BIANMAQI_LSB, BIANMAQI_DIR);
    encoder_clear_count(BIANMAQI_TIMER);

    gpio_init(DIANJI_FANG_XIANG_PIN,
              GPO,
              DIANJI_ZHENG_DIAN_PING,
              GPO_PUSH_PULL);
    pwm_init(DIANJI_KONG_ZHI_DAI, DIANJI_PWM_PINZHOU, 0);

    dang_qian_zhuang_tai = ZHUANGTAI_TINGZHI_ZHENG;
    zhuang_tai_ji_shu = 0;
    zhan_kong_bi = 0;
    you_xin_shu_ju = 0;

    zui_xin_shu_ju.zhuang_tai = dang_qian_zhuang_tai;
    zui_xin_shu_ju.fang_xiang = 0;
    zui_xin_shu_ju.zhan_kong_bi = 0;
    zui_xin_shu_ju.yuan_shi_maichong = 0;
    zui_xin_shu_ju.bian_ma_qi_rpm = 0;
    zui_xin_shu_ju.lun_su_rpm = 0;
    dian_ji_shu_chu(0, 0);
}

void DianJiKongZhi_ZhouQiZhongDuan(void)
{
    int32 pulses;
    float bian_ma_qi_rps;
    float lun_su_rpm;

    pulses = encoder_get_count(BIANMAQI_TIMER);
    encoder_clear_count(BIANMAQI_TIMER);

    bian_ma_qi_rps = (float)pulses /
                     (BIANMAQI_XIAN_SHU *
                      ((float)KONGZHI_ZHOUQI_MS / 1000.0f));
    lun_su_rpm = bian_ma_qi_rps * 60.0f *
                 BIANMAQI_CHI_LUN / LUNZI_CHI_LUN;

#if !KAIHUAN_YUNXING_SHI_NENG
    zhan_kong_bi = 0;
    dian_ji_shu_chu(0, 0);
    zui_xin_shu_ju.zhuang_tai = ZHUANGTAI_TINGZHI_ZHENG;
    zui_xin_shu_ju.fang_xiang = 0;
    zui_xin_shu_ju.zhan_kong_bi = 0;
    you_xin_shu_ju = 1;
    return;
#endif

    zhuang_tai_ji_shu++;

    switch(dang_qian_zhuang_tai)
    {
        case ZHUANGTAI_TINGZHI_ZHENG:
        {
            zhan_kong_bi = 0;
            dian_ji_shu_chu(0, 0);
            if(zhuang_tai_ji_shu >= KAIHUAN_TINGZHI_TICKS)
            {
                zhuang_tai_qie_huan(ZHUANGTAI_ZHENG_SHENGSU);
            }
        }break;

        case ZHUANGTAI_ZHENG_SHENGSU:
        {
            if(zhan_kong_bi < KAIHUAN_ZUI_DA_ZHANKONGBI)
            {
                zhan_kong_bi += KAIHUAN_SHENGYA_BUCHANG;
                if(zhan_kong_bi > KAIHUAN_ZUI_DA_ZHANKONGBI)
                {
                    zhan_kong_bi = KAIHUAN_ZUI_DA_ZHANKONGBI;
                }
            }

            dian_ji_shu_chu(zhan_kong_bi, 0);
            if(zhuang_tai_ji_shu >= KAIHUAN_SHENGYA_TICKS)
            {
                zhuang_tai_qie_huan(ZHUANGTAI_ZHENG_BAOCHI);
            }
        }break;

        case ZHUANGTAI_ZHENG_BAOCHI:
        {
            zhan_kong_bi = KAIHUAN_ZUI_DA_ZHANKONGBI;
            dian_ji_shu_chu(zhan_kong_bi, 0);
            if(zhuang_tai_ji_shu >= KAIHUAN_BAOCHI_TICKS)
            {
                zhan_kong_bi = 0;
                dian_ji_shu_chu(0, 0);
                zhuang_tai_qie_huan(ZHUANGTAI_TINGZHI_FAN);
            }
        }break;

        case ZHUANGTAI_TINGZHI_FAN:
        {
            zhan_kong_bi = 0;
            dian_ji_shu_chu(0, 0);
            if(zhuang_tai_ji_shu >= KAIHUAN_TINGZHI_TICKS)
            {
                zhuang_tai_qie_huan(ZHUANGTAI_FAN_SHENGSU);
            }
        }break;

        case ZHUANGTAI_FAN_SHENGSU:
        {
            if(zhan_kong_bi < KAIHUAN_ZUI_DA_ZHANKONGBI)
            {
                zhan_kong_bi += KAIHUAN_SHENGYA_BUCHANG;
                if(zhan_kong_bi > KAIHUAN_ZUI_DA_ZHANKONGBI)
                {
                    zhan_kong_bi = KAIHUAN_ZUI_DA_ZHANKONGBI;
                }
            }

            dian_ji_shu_chu(zhan_kong_bi, 1);
            if(zhuang_tai_ji_shu >= KAIHUAN_SHENGYA_TICKS)
            {
                zhuang_tai_qie_huan(ZHUANGTAI_FAN_BAOCHI);
            }
        }break;

        case ZHUANGTAI_FAN_BAOCHI:
        {
            zhan_kong_bi = KAIHUAN_ZUI_DA_ZHANKONGBI;
            dian_ji_shu_chu(zhan_kong_bi, 1);
            if(zhuang_tai_ji_shu >= KAIHUAN_BAOCHI_TICKS)
            {
                zhan_kong_bi = 0;
                dian_ji_shu_chu(0, 0);
                zhuang_tai_qie_huan(ZHUANGTAI_TINGZHI_HUIZHENG);
            }
        }break;

        case ZHUANGTAI_TINGZHI_HUIZHENG:
        default:
        {
            zhan_kong_bi = 0;
            dian_ji_shu_chu(0, 0);
            if(zhuang_tai_ji_shu >= KAIHUAN_TINGZHI_TICKS)
            {
                zhuang_tai_qie_huan(ZHUANGTAI_ZHENG_SHENGSU);
            }
        }break;
    }

    if(0 == (zhuang_tai_ji_shu % KAIHUAN_LOG_TICKS))
    {
        you_xin_shu_ju = 1;
    }

    zui_xin_shu_ju.zhuang_tai = dang_qian_zhuang_tai;
    zui_xin_shu_ju.fang_xiang =
        (ZHUANGTAI_FAN_SHENGSU == dang_qian_zhuang_tai ||
         ZHUANGTAI_FAN_BAOCHI == dang_qian_zhuang_tai) ? 1 : 0;
    zui_xin_shu_ju.zhan_kong_bi = zhan_kong_bi;
    zui_xin_shu_ju.yuan_shi_maichong = pulses;
    zui_xin_shu_ju.bian_ma_qi_rpm =
        si_she_wu_ru(bian_ma_qi_rps * 60.0f);
    zui_xin_shu_ju.lun_su_rpm = si_she_wu_ru(lun_su_rpm);
}

uint8 DianJiKongZhi_QuZuiXinShuJu(DianJiKongZhiShuJu *shu_ju)
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
