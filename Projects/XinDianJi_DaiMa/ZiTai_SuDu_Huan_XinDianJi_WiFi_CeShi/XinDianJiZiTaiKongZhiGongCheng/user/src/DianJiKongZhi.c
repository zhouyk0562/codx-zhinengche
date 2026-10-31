#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "SuDuPI.h"
#include "ZiTaiSuDu.h"
#include "DianJiKongZhi.h"

static SuDuPI_JieGou sudu_pi;
static float shi_ji_rps_lv_bo;
static float mu_biao_dang_qian_rpm;
static uint8 qi_dong_jie_duan;
static uint16 qi_dong_ji_shu;
static uint16 duzhuan_ji_shu;
static uint16 hui_fu_ji_shu;
static uint8 duzhuan_zhuang_tai;
static DianJiKongZhiShuJu zui_xin_shu_ju;
static volatile uint8 you_xin_shu_ju;

static int32 si_she_wu_ru(float shu_zhi)
{
    return (int32)(shu_zhi + (shu_zhi >= 0.0f ? 0.5f : -0.5f));
}

static void pi_chu_shi_hua(void)
{
    SuDuPI_ChuShiHua(&sudu_pi,
                     PI_KP,
                     PI_KI,
                     KONGZHI_CAIYANG_MIAO,
                     INTEGRAL_MAX,
                     DUTY_MAX_PERCENT,
                     PI_QI_DONG_JIFEN);
}

static float mu_biao_ping_hua(float dang_qian, float mu_biao)
{
    if(dang_qian < mu_biao)
    {
        dang_qian += MU_BIAO_RUAN_HUA_RPM_PER_TICK;
        if(dang_qian > mu_biao)
        {
            dang_qian = mu_biao;
        }
    }
    else if(dang_qian > mu_biao)
    {
        dang_qian -= MU_BIAO_RUAN_HUA_RPM_PER_TICK;
        if(dang_qian < mu_biao)
        {
            dang_qian = mu_biao;
        }
    }
    return dang_qian;
}

static void jin_ru_duzhuan_baohu(void)
{
    SuDuPI_QingLing(&sudu_pi);
    duzhuan_zhuang_tai = 1;
    duzhuan_ji_shu = 0;
    hui_fu_ji_shu = DUZHUAN_RECOVER_TICKS;
    qi_dong_jie_duan = 0;
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

    pi_chu_shi_hua();
    shi_ji_rps_lv_bo = 0.0f;
    mu_biao_dang_qian_rpm = 0.0f;
    qi_dong_jie_duan = 1;
    qi_dong_ji_shu = 0;
    duzhuan_ji_shu = 0;
    hui_fu_ji_shu = 0;
    duzhuan_zhuang_tai = 0;
    you_xin_shu_ju = 0;

    zui_xin_shu_ju.kong_zhi_jiao = 0.0f;
    zui_xin_shu_ju.mu_biao_lun_rpm = 0;
    zui_xin_shu_ju.shi_ji_lun_rpm = 0;
    zui_xin_shu_ju.bian_ma_qi_rpm = 0;
    zui_xin_shu_ju.zhan_kong_bi = 0;
    zui_xin_shu_ju.yuan_shi_maichong = 0;
    zui_xin_shu_ju.duzhuan_zhuangtai = 0;
    pwm_set_duty(DIANJI_KONG_ZHI_DAI, 0);
}

void DianJiKongZhi_ZhouQiZhongDuan(void)
{
    ZiTaiSuDuShuJu zi_tai_shu_ju;
    uint8 zi_tai_zhun_bei;
    int32 pulses;
    float shi_ji_rps;
    float shi_ji_lun_rpm;
    float mu_biao_rps;
    float wucha;
    float duty;

    zi_tai_zhun_bei = ZiTaiSuDu_QuZuiXinShuJu(&zi_tai_shu_ju);
    zui_xin_shu_ju.kong_zhi_jiao = zi_tai_shu_ju.kong_zhi_jiao;

#if !SU_DU_HUAN_YUNXING_SHI_NENG
    (void)zi_tai_zhun_bei;
    pwm_set_duty(DIANJI_KONG_ZHI_DAI, 0);
    zui_xin_shu_ju.mu_biao_lun_rpm = 0;
    zui_xin_shu_ju.zhan_kong_bi = 0;
    zui_xin_shu_ju.duzhuan_zhuangtai = 0;
    you_xin_shu_ju = 1;
    return;
#endif

    if(zi_tai_zhun_bei &&
       zi_tai_shu_ju.mu_biao_lun_rpm >= QI_DONG_QI_DONG_RPM)
    {
        mu_biao_dang_qian_rpm = mu_biao_ping_hua(
            mu_biao_dang_qian_rpm,
            zi_tai_shu_ju.mu_biao_lun_rpm);
    }
    else
    {
        mu_biao_dang_qian_rpm = 0.0f;
    }

    pulses = encoder_get_count(BIANMAQI_TIMER);
    encoder_clear_count(BIANMAQI_TIMER);
    if(pulses < 0)
    {
        pulses = -pulses;
    }

    shi_ji_rps = (float)pulses /
                 (BIANMAQI_XIAN_SHU * KONGZHI_CAIYANG_MIAO);
    shi_ji_rps_lv_bo = shi_ji_rps_lv_bo * SU_DU_LV_BO_JIU +
                       shi_ji_rps * SU_DU_LV_BO_XIN;
    shi_ji_rps = shi_ji_rps_lv_bo;
    shi_ji_lun_rpm = shi_ji_rps * 60.0f *
                     BIANMAQI_CHI_LUN / LUNZI_CHI_LUN;

    duty = 0.0f;
    if(mu_biao_dang_qian_rpm < QI_DONG_QI_DONG_RPM)
    {
        pi_chu_shi_hua();
        qi_dong_jie_duan = 1;
        qi_dong_ji_shu = 0;
        duzhuan_ji_shu = 0;
        duzhuan_zhuang_tai = 0;
    }
    else if(hui_fu_ji_shu > 0)
    {
        hui_fu_ji_shu--;
        if(0 == hui_fu_ji_shu)
        {
            pi_chu_shi_hua();
            qi_dong_jie_duan = 1;
            qi_dong_ji_shu = 0;
            duzhuan_ji_shu = 0;
            duzhuan_zhuang_tai = 0;
            mu_biao_dang_qian_rpm = 0.0f;
        }
    }
    else if(qi_dong_jie_duan)
    {
        qi_dong_ji_shu++;
        duty = QI_DONG_PWM_PERCENT;

        if(qi_dong_ji_shu >= QI_DONG_ZUI_SHAO_TICKS &&
           shi_ji_lun_rpm >= QI_DONG_WAN_CHENG_RPM)
        {
            qi_dong_jie_duan = 0;
            qi_dong_ji_shu = 0;
            pi_chu_shi_hua();
        }
        else if(qi_dong_ji_shu >= QI_DONG_ZUI_DA_TICKS)
        {
            if(shi_ji_lun_rpm < DUZHUAN_WHEEL_RPM)
            {
                jin_ru_duzhuan_baohu();
                duty = 0.0f;
            }
            else
            {
                qi_dong_jie_duan = 0;
                qi_dong_ji_shu = 0;
                pi_chu_shi_hua();
            }
        }
    }
    else
    {
        mu_biao_rps = (mu_biao_dang_qian_rpm * LUNZI_CHI_LUN /
                       BIANMAQI_CHI_LUN) / 60.0f;
        wucha = mu_biao_rps - shi_ji_rps;
        duty = DUTY_QIAN_KUI_JI_CHU_PERCENT +
               DUTY_QIAN_KUI_RPM_XISHU * mu_biao_dang_qian_rpm +
               SuDuPI_JiSuan(&sudu_pi, wucha);
        if(duty > DUTY_MAX_PERCENT)
        {
            duty = DUTY_MAX_PERCENT;
        }
        else if(duty < 0.0f)
        {
            duty = 0.0f;
        }

        if(duty >= DUZHUAN_DUTY_PERCENT &&
           shi_ji_lun_rpm < DUZHUAN_WHEEL_RPM)
        {
            if(duzhuan_ji_shu < DUZHUAN_TRIP_TICKS)
            {
                duzhuan_ji_shu++;
            }
            if(duzhuan_ji_shu >= DUZHUAN_TRIP_TICKS)
            {
                jin_ru_duzhuan_baohu();
                duty = 0.0f;
            }
        }
        else
        {
            duzhuan_ji_shu = 0;
        }
    }

    if(duty > 0.0f)
    {
        gpio_set_level(DIANJI_FANG_XIANG_PIN, DIANJI_ZHENG_DIAN_PING);
        pwm_set_duty(DIANJI_KONG_ZHI_DAI,
                     (uint16)(duty * (PWM_DUTY_MAX / 100.0f)));
    }
    else
    {
        duty = 0.0f;
        pwm_set_duty(DIANJI_KONG_ZHI_DAI, 0);
    }

    zui_xin_shu_ju.mu_biao_lun_rpm = si_she_wu_ru(mu_biao_dang_qian_rpm);
    zui_xin_shu_ju.shi_ji_lun_rpm = si_she_wu_ru(shi_ji_lun_rpm);
    zui_xin_shu_ju.bian_ma_qi_rpm = si_she_wu_ru(shi_ji_rps * 60.0f);
    zui_xin_shu_ju.zhan_kong_bi = si_she_wu_ru(duty);
    zui_xin_shu_ju.yuan_shi_maichong = pulses;
    zui_xin_shu_ju.duzhuan_zhuangtai = duzhuan_zhuang_tai;
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
