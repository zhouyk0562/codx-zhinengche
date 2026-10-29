#include <math.h>
#include "XitongPeiZhi.h"
#include "ZiTaiJieSuan.h"

#define HU_DU_ZHUAN_JIAO_DU (57.2957795130823f)

static float jiao_du_gui_fan(float jiao_du)
{
    while(jiao_du > 180.0f)
    {
        jiao_du -= 360.0f;
    }
    while(jiao_du < -180.0f)
    {
        jiao_du += 360.0f;
    }
    return jiao_du;
}

static float jiao_du_rong_he(float tuo_luo_jiao,
                             float chuan_gan_qi_jiao,
                             float xiu_zheng_quan)
{
    float pian_cha = jiao_du_gui_fan(chuan_gan_qi_jiao - tuo_luo_jiao);
    return jiao_du_gui_fan(tuo_luo_jiao + pian_cha * xiu_zheng_quan);
}

void ZiTaiJieSuan_ChuShiHua(ZiTaiJieSuanJieGou *ji_suan,
                            const JiuZhouCaiJiShuJu *shu_ju,
                            float jia_su_bili,
                            float tuo_luo_bili)
{
    ji_suan->jia_su_bili = jia_su_bili;
    ji_suan->tuo_luo_bili = tuo_luo_bili;

    ji_suan->jia_su_lvbo_x = shu_ju->jia_su_x;
    ji_suan->jia_su_lvbo_y = shu_ju->jia_su_y;
    ji_suan->jia_su_lvbo_z = shu_ju->jia_su_z;
    ji_suan->tuo_luo_lvbo_x = shu_ju->tuo_luo_x;
    ji_suan->tuo_luo_lvbo_y = shu_ju->tuo_luo_y;
    ji_suan->tuo_luo_lvbo_z = shu_ju->tuo_luo_z;
    ji_suan->ci_lvbo_x = shu_ju->ci_x;
    ji_suan->ci_lvbo_y = shu_ju->ci_y;
    ji_suan->ci_lvbo_z = shu_ju->ci_z;

    ji_suan->tuo_luo_ling_pian_x = shu_ju->tuo_luo_x;
    ji_suan->tuo_luo_ling_pian_y = shu_ju->tuo_luo_y;
    ji_suan->tuo_luo_ling_pian_z = shu_ju->tuo_luo_z;

    ji_suan->heng_gun_jiao = atan2f(-shu_ju->jia_su_y,
                                     -shu_ju->jia_su_z) *
                             HU_DU_ZHUAN_JIAO_DU;
    ji_suan->fu_yang_jiao = atan2f(
        -shu_ju->jia_su_x,
        sqrtf(shu_ju->jia_su_y * shu_ju->jia_su_y +
              shu_ju->jia_su_z * shu_ju->jia_su_z)) *
        HU_DU_ZHUAN_JIAO_DU;
    ji_suan->hang_xiang_jiao = atan2f(-shu_ju->ci_y,
                                       shu_ju->ci_x) *
                               HU_DU_ZHUAN_JIAO_DU;
}

void ZiTaiJieSuan_GengXin(ZiTaiJieSuanJieGou *ji_suan,
                          const JiuZhouCaiJiShuJu *shu_ju,
                          float cai_yang_miao)
{
    float jia_su_heng_gun;
    float jia_su_fu_yang;
    float ci_hang_xiang;
    float tuo_luo_heng_gun_ji_fen;
    float tuo_luo_fu_yang_ji_fen;
    float tuo_luo_hang_xiang_ji_fen;
    float tuo_luo_x_dps;
    float tuo_luo_y_dps;
    float tuo_luo_z_dps;
    float tuo_luo_su_du;
    float xiu_zheng_quan;

    ji_suan->jia_su_lvbo_x =
        ji_suan->jia_su_lvbo_x * 0.8f + shu_ju->jia_su_x * 0.2f;
    ji_suan->jia_su_lvbo_y =
        ji_suan->jia_su_lvbo_y * 0.8f + shu_ju->jia_su_y * 0.2f;
    ji_suan->jia_su_lvbo_z =
        ji_suan->jia_su_lvbo_z * 0.8f + shu_ju->jia_su_z * 0.2f;

    ji_suan->tuo_luo_lvbo_x =
        ji_suan->tuo_luo_lvbo_x * 0.7f + shu_ju->tuo_luo_x * 0.3f;
    ji_suan->tuo_luo_lvbo_y =
        ji_suan->tuo_luo_lvbo_y * 0.7f + shu_ju->tuo_luo_y * 0.3f;
    ji_suan->tuo_luo_lvbo_z =
        ji_suan->tuo_luo_lvbo_z * 0.7f + shu_ju->tuo_luo_z * 0.3f;

    ji_suan->ci_lvbo_x =
        ji_suan->ci_lvbo_x * 0.9f + shu_ju->ci_x * 0.1f;
    ji_suan->ci_lvbo_y =
        ji_suan->ci_lvbo_y * 0.9f + shu_ju->ci_y * 0.1f;
    ji_suan->ci_lvbo_z =
        ji_suan->ci_lvbo_z * 0.9f + shu_ju->ci_z * 0.1f;

    tuo_luo_x_dps =
        (ji_suan->tuo_luo_lvbo_x - ji_suan->tuo_luo_ling_pian_x) /
        ji_suan->tuo_luo_bili;
    tuo_luo_y_dps =
        (ji_suan->tuo_luo_lvbo_y - ji_suan->tuo_luo_ling_pian_y) /
        ji_suan->tuo_luo_bili;
    tuo_luo_z_dps =
        (ji_suan->tuo_luo_lvbo_z - ji_suan->tuo_luo_ling_pian_z) /
        ji_suan->tuo_luo_bili;

    tuo_luo_su_du = sqrtf(tuo_luo_x_dps * tuo_luo_x_dps +
                          tuo_luo_y_dps * tuo_luo_y_dps +
                          tuo_luo_z_dps * tuo_luo_z_dps);

    if(tuo_luo_su_du < ZITAI_JING_ZHI_DPS)
    {
        xiu_zheng_quan = ZITAI_JING_ZHI_XIU_ZHENG;
    }
    else if(tuo_luo_su_du > ZITAI_YUN_DONG_DPS)
    {
        xiu_zheng_quan = ZITAI_YUN_DONG_XIU_ZHENG;
    }
    else
    {
        xiu_zheng_quan = ZITAI_ZHONG_JIAN_XIU_ZHENG;
    }

    tuo_luo_heng_gun_ji_fen =
        ji_suan->heng_gun_jiao + tuo_luo_x_dps * cai_yang_miao;

    // 当前安装方向下 GY 与 Pitch 角变化方向相反，因此这里取负号。
    tuo_luo_fu_yang_ji_fen =
        ji_suan->fu_yang_jiao - tuo_luo_y_dps * cai_yang_miao;

    tuo_luo_hang_xiang_ji_fen =
        ji_suan->hang_xiang_jiao + tuo_luo_z_dps * cai_yang_miao;

    jia_su_heng_gun = atan2f(-ji_suan->jia_su_lvbo_y,
                             -ji_suan->jia_su_lvbo_z) *
                      HU_DU_ZHUAN_JIAO_DU;
    jia_su_fu_yang = atan2f(
        -ji_suan->jia_su_lvbo_x,
        sqrtf(ji_suan->jia_su_lvbo_y * ji_suan->jia_su_lvbo_y +
              ji_suan->jia_su_lvbo_z * ji_suan->jia_su_lvbo_z)) *
        HU_DU_ZHUAN_JIAO_DU;
    ci_hang_xiang = atan2f(-ji_suan->ci_lvbo_y,
                            ji_suan->ci_lvbo_x) *
                    HU_DU_ZHUAN_JIAO_DU;

    ji_suan->heng_gun_jiao =
        jiao_du_rong_he(tuo_luo_heng_gun_ji_fen,
                        jia_su_heng_gun,
                        xiu_zheng_quan);
    ji_suan->fu_yang_jiao =
        jiao_du_rong_he(tuo_luo_fu_yang_ji_fen,
                        jia_su_fu_yang,
                        xiu_zheng_quan);
    ji_suan->hang_xiang_jiao =
        jiao_du_rong_he(tuo_luo_hang_xiang_ji_fen,
                        ci_hang_xiang,
                        xiu_zheng_quan);
}
