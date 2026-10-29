#include <math.h>
#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "SuDuPI.h"
#include "ZiTaiSuDu.h"
#include "DianJiKongZhi.h"

static SuDuPI_JieGou sudu_pi;

static float mubiao_dangqian_rps;
static float shiji_rps_lvbo;
static uint16 duzhuan_jishu;
static uint16 huifu_jishu;
static uint8 duzhuan_zhuangtai;

static DianJiKongZhiShuJu zui_xin_shu_ju;
static volatile uint8 you_xin_shu_ju;

static int32 si_she_wu_ru(float shu_zhi)
{
    return (int32)(shu_zhi + (shu_zhi >= 0.0f ? 0.5f : -0.5f));
}

static float ruan_qi_dong(float dang_qian, float mu_biao)
{
    if(dang_qian < mu_biao)
    {
        dang_qian += RUAN_QI_DONG_BU_CHANG;
        if(dang_qian > mu_biao)
        {
            dang_qian = mu_biao;
        }
    }
    else if(dang_qian > mu_biao)
    {
        dang_qian -= RUAN_QI_DONG_BU_CHANG;
        if(dang_qian < mu_biao)
        {
            dang_qian = mu_biao;
        }
    }

    return dang_qian;
}

void DianJiKongZhi_ChuShiHua(void)
{
    encoder_dir_init(BIANMAQI_TIMER, BIANMAQI_LSB, BIANMAQI_DIR);
    encoder_clear_count(BIANMAQI_TIMER);

    gpio_init(DIANJI_DIR, GPO, DIANJI_ZHENG_ZHUAN_DIAN_PING, GPO_PUSH_PULL);
    pwm_init(DIANJI_PWM, DIANJI_PWM_PINZHOU, 0);

    SuDuPI_ChuShiHua(&sudu_pi,
                     PI_KP,
                     PI_KI,
                     KONGZHI_CAIYANG_MIAO,
                     INTEGRAL_MAX,
                     DUTY_MAX_PERCENT);

    mubiao_dangqian_rps = 0.0f;
    shiji_rps_lvbo = 0.0f;
    duzhuan_jishu = 0;
    huifu_jishu = 0;
    duzhuan_zhuangtai = 0;
    you_xin_shu_ju = 0;

    zui_xin_shu_ju.kong_zhi_jiao = 0.0f;
    zui_xin_shu_ju.mubiao_lun_su_rpm = 0;
    zui_xin_shu_ju.shiji_lun_su_rpm = 0;
    zui_xin_shu_ju.zhankongbi_baifen = 0;
    zui_xin_shu_ju.bianmaqi_rpm = 0;
    zui_xin_shu_ju.yuan_shi_maichong = 0;
    zui_xin_shu_ju.duzhuan_zhuangtai = 0;
}

void DianJiKongZhi_ZhouQiZhongDuan(void)
{
    ZiTaiSuDuShuJu zi_tai_shu_ju;
    int32 pulses;
    float shiji_rps;
    float shiji_lun_rpm;
    float mu_biao_lun_rpm;
    float mu_biao_rps;
    float wucha;
    float kong_zhi_liang;
    float duty;
    float duty_jue_dui;

    ZiTaiSuDu_QuZuiXinShuJu(&zi_tai_shu_ju);
    mu_biao_lun_rpm = zi_tai_shu_ju.mu_biao_lun_rpm;

    pulses = encoder_get_count(BIANMAQI_TIMER);
    encoder_clear_count(BIANMAQI_TIMER);

    // 速度目标始终非负，方向线毛刺会让负脉冲污染闭环，因此这里取绝对脉冲数。
    if(pulses < 0)
    {
        pulses = -pulses;
    }

    shiji_rps = (float)pulses /
                (BIANMAQI_XIAN_SHU * KONGZHI_CAIYANG_MIAO);

    shiji_rps_lvbo = shiji_rps_lvbo * SU_DU_LV_BO_JIU +
                     shiji_rps * SU_DU_LV_BO_XIN;
    shiji_rps = shiji_rps_lvbo;

    shiji_lun_rpm = shiji_rps * 60.0f *
                    BIANMAQI_CHI_LUN / LUNZI_CHI_LUN;

    mu_biao_rps = (mu_biao_lun_rpm * LUNZI_CHI_LUN /
                   BIANMAQI_CHI_LUN) / 60.0f;
    mubiao_dangqian_rps = ruan_qi_dong(mubiao_dangqian_rps, mu_biao_rps);

    if(huifu_jishu > 0)
    {
        huifu_jishu--;
        duty = 0.0f;

        if(0 == huifu_jishu)
        {
            SuDuPI_QingLing(&sudu_pi);
            duzhuan_jishu = 0;
            mubiao_dangqian_rps = 0.0f;
            duzhuan_zhuangtai = 0;
        }
    }
    else
    {
        wucha = mubiao_dangqian_rps - shiji_rps;
        kong_zhi_liang = SuDuPI_JiSuan(&sudu_pi, wucha);

        // 轮速超过目标时只关闭 PWM，不让电机反转。
        if(kong_zhi_liang > 0.0f)
        {
            duty = kong_zhi_liang;
        }
        else
        {
            duty = 0.0f;
        }
        duty_jue_dui = duty;

        if(fabsf(mu_biao_lun_rpm) > DUZHUAN_WHEEL_RPM &&
           duty_jue_dui >= DUZHUAN_DUTY_PERCENT &&
           fabsf(shiji_lun_rpm) < DUZHUAN_WHEEL_RPM)
        {
            if(duzhuan_jishu < DUZHUAN_TRIP_TICKS)
            {
                duzhuan_jishu++;
            }

            if(duzhuan_jishu >= DUZHUAN_TRIP_TICKS)
            {
                duty = 0.0f;
                SuDuPI_QingLing(&sudu_pi);
                mubiao_dangqian_rps = 0.0f;
                duzhuan_jishu = 0;
                huifu_jishu = DUZHUAN_RECOVER_TICKS;
                duzhuan_zhuangtai = 1;
            }
        }
        else
        {
            duzhuan_jishu = 0;
        }
    }

    duty_jue_dui = fabsf(duty);
    if(duty_jue_dui > DUTY_MAX_PERCENT)
    {
        duty_jue_dui = DUTY_MAX_PERCENT;
    }

    if(duty_jue_dui > 0.0f)
    {
        if(duty >= 0.0f)
        {
            gpio_set_level(DIANJI_DIR, DIANJI_ZHENG_ZHUAN_DIAN_PING);
        }
        else
        {
            gpio_set_level(DIANJI_DIR, DIANJI_FAN_ZHUAN_DIAN_PING);
        }
        pwm_set_duty(DIANJI_PWM,
                     (uint16)(duty_jue_dui *
                              (PWM_DUTY_MAX / 100.0f)));
    }
    else
    {
        gpio_set_level(DIANJI_DIR, DIANJI_ZHENG_ZHUAN_DIAN_PING);
        pwm_set_duty(DIANJI_PWM, 0);
    }

    zui_xin_shu_ju.kong_zhi_jiao = zi_tai_shu_ju.kong_zhi_jiao;
    zui_xin_shu_ju.mubiao_lun_su_rpm =
        si_she_wu_ru(mubiao_dangqian_rps * 60.0f *
                     BIANMAQI_CHI_LUN / LUNZI_CHI_LUN);
    zui_xin_shu_ju.shiji_lun_su_rpm = si_she_wu_ru(shiji_lun_rpm);
    zui_xin_shu_ju.zhankongbi_baifen =
        si_she_wu_ru(duty >= 0.0f ? duty_jue_dui : -duty_jue_dui);
    zui_xin_shu_ju.bianmaqi_rpm = si_she_wu_ru(shiji_rps * 60.0f);
    zui_xin_shu_ju.yuan_shi_maichong = pulses;
    zui_xin_shu_ju.duzhuan_zhuangtai = duzhuan_zhuangtai;
    you_xin_shu_ju = 1;
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
