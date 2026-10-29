#include "zf_common_headfile.h"
#include "XitongPeiZhi.h"
#include "JiuZhouCaiJi.h"
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
    JiuZhouCaiJiShuJu shu_ju;
    uint32 chang_du;

    if(!JiuZhouCaiJi_QuZuiXinShuJu(&shu_ju))
    {
        return;
    }

    snprintf((char *)fa_song_huan_chong,
             sizeof(fa_song_huan_chong),
             "JiuZhou:%d,%d,%d,%d,%d,%d,%d,%d,%d,%.2f,%.2f,%.2f\n",
             shu_ju.jia_su_x,
             shu_ju.jia_su_y,
             shu_ju.jia_su_z,
             shu_ju.tuo_luo_x,
             shu_ju.tuo_luo_y,
             shu_ju.tuo_luo_z,
             shu_ju.ci_x,
             shu_ju.ci_y,
             shu_ju.ci_z,
             shu_ju.heng_gun_jiao,
             shu_ju.fu_yang_jiao,
             shu_ju.hang_xiang_jiao);

    chang_du = strlen((char *)fa_song_huan_chong);
    if(0 == wifi_spi_send_buffer(fa_song_huan_chong, chang_du))
    {
        fa_song_ci_shu++;
        if(0 == (fa_song_ci_shu % 50U))
        {
            printf("\r\nRoll=%.2f Pitch=%.2f Yaw=%.2f\r\n",
                   shu_ju.heng_gun_jiao,
                   shu_ju.fu_yang_jiao,
                   shu_ju.hang_xiang_jiao);
        }
    }
    else
    {
        printf("\r\nWiFi FaSong ShiBai\r\n");
    }
}
