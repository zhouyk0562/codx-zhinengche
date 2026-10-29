#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "DianJiKongZhi.h"
#include "WifiChuanShu.h"

static uint8 fa_song_huan_chong[128];
static uint32 fa_song_ci_shu;

void WifiChuanShu_ChuShiHua(void)
{
    uint8 zhuang_tai;

    do
    {
        zhuang_tai = wifi_spi_init(NULL, NULL);
        printf("\r\nWiFi SPI ZhuangTai=%u BanBen=[%s]\r\n",
               zhuang_tai,
               wifi_spi_version);
    }while(zhuang_tai);

    do
    {
        zhuang_tai = wifi_spi_wifi_connect(WIFI_MINGCHENG, WIFI_MIMA);
        printf("\r\nWiFi ZhuangTai=%u IP=[%s]\r\n",
               zhuang_tai,
               wifi_spi_ip_addr_port);
    }while(zhuang_tai);

    while(wifi_spi_socket_connect("TCP",
                                  DIANNAO_IP,
                                  DIANNAO_DUANKOU,
                                  BENJI_DUANKOU))
    {
        printf("\r\nDianNao TCP LianJie ShiBai\r\n");
    }

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
             "ZiTai:%.2f,%ld,%ld,%ld,%ld,%d\n",
             shu_ju.kong_zhi_jiao,
             (long)shu_ju.mubiao_lun_su_rpm,
             (long)shu_ju.shiji_lun_su_rpm,
             (long)shu_ju.zhankongbi_baifen,
             (long)shu_ju.bianmaqi_rpm,
             shu_ju.duzhuan_zhuangtai);

    chang_du = strlen((char *)fa_song_huan_chong);
    fa_song_ci_shu++;
    if(0 == wifi_spi_send_buffer(fa_song_huan_chong, chang_du))
    {
    }
    else
    {
        printf("\r\nWiFi FaSong ShiBai\r\n");
    }

    if(0 == (fa_song_ci_shu % 20U))
    {
        printf("\r\nJiaoDu=%.2f MuBiao=%ld ShiJi=%ld Duty=%ld Enc=%ld Stall=%d\r\n",
               shu_ju.kong_zhi_jiao,
               (long)shu_ju.mubiao_lun_su_rpm,
               (long)shu_ju.shiji_lun_su_rpm,
               (long)shu_ju.zhankongbi_baifen,
               (long)shu_ju.bianmaqi_rpm,
               shu_ju.duzhuan_zhuangtai);
    }
}
