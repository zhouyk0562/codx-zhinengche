#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "DianJiKongZhi.h"
#include "WifiChuanShu.h"

static uint8 fa_song_huan_chong[128];
static uint32 fa_song_ci_shu;
static uint8 wifi_yi_lian_jie;
static uint8 wifi_zhuang_tai;

void WifiChuanShu_ChuShiHua(void)
{
    uint8 zhuang_tai;
    uint8 chang_shi_ci_shu;

    wifi_yi_lian_jie = 0;
    wifi_zhuang_tai = 1;
    zhuang_tai = 1;

    for(chang_shi_ci_shu = 0; chang_shi_ci_shu < 10; chang_shi_ci_shu++)
    {
        zhuang_tai = wifi_spi_init(WIFI_MINGCHENG, WIFI_MIMA);
        printf("\r\nWiFi SPI ZhuangTai=%u BanBen=[%s]\r\n",
               zhuang_tai,
               wifi_spi_version);
        if(0 == zhuang_tai)
        {
            break;
        }
        system_delay_ms(200);
    }
    if(zhuang_tai)
    {
        wifi_zhuang_tai = 1;
        printf("\r\nWiFi MoKuai Wei JianCe Dao\r\n");
        return;
    }

    wifi_zhuang_tai = 2;
    printf("\r\nWiFi ReDian IP=[%s]\r\n", wifi_spi_ip_addr_port);

    wifi_zhuang_tai = 3;
    zhuang_tai = 1;
    for(chang_shi_ci_shu = 0; chang_shi_ci_shu < 3; chang_shi_ci_shu++)
    {
        zhuang_tai = wifi_spi_socket_connect("TCP",
                                             DIANNAO_IP,
                                             DIANNAO_DUANKOU,
                                             BENJI_DUANKOU);
        if(0 == zhuang_tai)
        {
            break;
        }
        printf("\r\nDianNao TCP LianJie ShiBai\r\n");
    }
    if(zhuang_tai)
    {
        wifi_zhuang_tai = 3;
        printf("\r\nWiFi ShuJu LianJie ShiBai\r\n");
        return;
    }

    wifi_yi_lian_jie = 1;
    wifi_zhuang_tai = 4;
    fa_song_ci_shu = 0;
    printf("\r\nWiFi TCP LianJie ChengGong\r\n");
}

void WifiChuanShu_ChuLi(void)
{
    DianJiKongZhiShuJu shu_ju;
    uint32 chang_du;

    if(!DianJiKongZhi_QuZuiXinShuJu(&shu_ju))
    {
        return;
    }

    snprintf((char *)fa_song_huan_chong,
             sizeof(fa_song_huan_chong),
             "DianJi:%ld,%ld,%ld,%ld,%d\n",
             (long)shu_ju.mu_biao_lun_rpm,
             (long)shu_ju.shi_ji_lun_rpm,
             (long)shu_ju.zhan_kong_bi,
             (long)shu_ju.bian_ma_qi_rpm,
             shu_ju.duzhuan_zhuangtai);

    chang_du = strlen((char *)fa_song_huan_chong);
    fa_song_ci_shu++;

    if(wifi_yi_lian_jie &&
       wifi_spi_send_buffer(fa_song_huan_chong, chang_du))
    {
        printf("\r\nWiFi FaSong ShiBai\r\n");
    }

    if(0 == (fa_song_ci_shu % 20U))
    {
        printf("\r\nDIANJI target=%ld actual=%ld duty=%ld enc=%ld stall=%d wifi=%u\r\n",
               (long)shu_ju.mu_biao_lun_rpm,
               (long)shu_ju.shi_ji_lun_rpm,
               (long)shu_ju.zhan_kong_bi,
               (long)shu_ju.bian_ma_qi_rpm,
               shu_ju.duzhuan_zhuangtai,
               wifi_zhuang_tai);
    }
}

uint8 WifiChuanShu_QuZhuangTai(void)
{
    return wifi_zhuang_tai;
}
