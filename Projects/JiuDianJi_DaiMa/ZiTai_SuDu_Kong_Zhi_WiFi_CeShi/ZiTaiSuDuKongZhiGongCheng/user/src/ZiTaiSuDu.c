#include <math.h>
#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "ZiTaiSuDu.h"

static ZiTaiSuDuShuJu zui_xin_shu_ju;
static float jiao_du_lei_ji;

void ZiTaiSuDu_ChuShiHua(void)
{
    zui_xin_shu_ju.yuan_shi_jiao = 0.0f;
    zui_xin_shu_ju.kong_zhi_jiao = 0.0f;
    zui_xin_shu_ju.mu_biao_lun_rpm = 0.0f;
    zui_xin_shu_ju.ling_dian_wan_cheng = 0;
    jiao_du_lei_ji = 0.0f;
}

void ZiTaiSuDu_GengXin(const JiuZhouCaiJiShuJu *shu_ju)
{
    float yuan_shi_jiao;
    float pian_cha;
    float you_xiao_pian_cha;
    float mu_biao_rpm;
    float po_du_xiu_zheng_rpm;

#if ZI_TAI_SHIYONG_HENG_GUN_JIAO
    yuan_shi_jiao = shu_ju->heng_gun_jiao;
#else
    yuan_shi_jiao = shu_ju->fu_yang_jiao;
#endif

    zui_xin_shu_ju.yuan_shi_jiao = yuan_shi_jiao;

    if(0 == zui_xin_shu_ju.ling_dian_wan_cheng)
    {
        jiao_du_lei_ji += yuan_shi_jiao;
        zui_xin_shu_ju.kong_zhi_jiao = 0.0f;
        zui_xin_shu_ju.mu_biao_lun_rpm = 0.0f;

        if(zui_xin_shu_ju.ling_dian_wan_cheng < ZI_TAI_LING_DIAN_CAI_YANG_SHU)
        {
            zui_xin_shu_ju.ling_dian_wan_cheng++;
        }
        if(zui_xin_shu_ju.ling_dian_wan_cheng >= ZI_TAI_LING_DIAN_CAI_YANG_SHU)
        {
            jiao_du_lei_ji /= (float)ZI_TAI_LING_DIAN_CAI_YANG_SHU;
            zui_xin_shu_ju.ling_dian_wan_cheng = 1;
        }
        return;
    }

    pian_cha = yuan_shi_jiao - jiao_du_lei_ji;
    zui_xin_shu_ju.kong_zhi_jiao = pian_cha;

    if(fabsf(pian_cha) <= ZI_TAI_PO_DU_SI_QU)
    {
        po_du_xiu_zheng_rpm = 0.0f;
    }
    else
    {
        if(pian_cha > 0.0f)
        {
            you_xiao_pian_cha = pian_cha - ZI_TAI_PO_DU_SI_QU;
        }
        else
        {
            you_xiao_pian_cha = pian_cha + ZI_TAI_PO_DU_SI_QU;
        }

        po_du_xiu_zheng_rpm = you_xiao_pian_cha *
                              ZI_TAI_PO_DU_RPM_PER_DU *
                              ZI_TAI_PO_DU_FANGXIANG;
    }

    mu_biao_rpm = ZI_TAI_JI_CHU_RPM + po_du_xiu_zheng_rpm;

    if(mu_biao_rpm > ZI_TAI_MU_BIAO_RPM_MAX)
    {
        mu_biao_rpm = ZI_TAI_MU_BIAO_RPM_MAX;
    }
    else if(mu_biao_rpm < ZI_TAI_MU_BIAO_RPM_MIN)
    {
        mu_biao_rpm = ZI_TAI_MU_BIAO_RPM_MIN;
    }

    zui_xin_shu_ju.mu_biao_lun_rpm = mu_biao_rpm;
}

uint8 ZiTaiSuDu_QuZuiXinShuJu(ZiTaiSuDuShuJu *shu_ju)
{
    uint32 zhuang_tai;

    zhuang_tai = interrupt_global_disable();
    *shu_ju = zui_xin_shu_ju;
    interrupt_global_enable(zhuang_tai);

    return shu_ju->ling_dian_wan_cheng;
}
