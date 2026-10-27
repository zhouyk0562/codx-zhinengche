#include "zf_common_headfile.h"

// 编码器接口 1 + WiFi 测试
//
// 编码器接口 1：
//   步/方向（SDI）模式，A26 = 脉冲，B27 = 方向，1024 线。
//
// WiFi SPI：
//   MISO=A13  SCK=A12  MOSI=A9
//   CS=A7     INT=A15  RST=A8
//   VCC 必须接学习板 5V，GND 接学习板 GND。
//
// 屏幕已经取下，本程序不初始化屏幕。
// 串口 COM6 用于查看启动、连接和编码器调试信息。
// VOFA+ 使用 TCP Server + FireWater，端口 8086。

#define WIFI_MINGCHENG          "YOUR_WIFI_SSID"
#define WIFI_MIMA               "YOUR_WIFI_PASSWORD"
#define DIANNAO_IP              "YOUR_PC_IP"
#define DIANNAO_DUANKOU         "8086"
#define BENJI_DUANKOU           "6666"

#define BIANMAQI_TIMER          (TIM_G7)
#define BIANMAQI_LSB            (TIMG7_ENCODER1_CH1_A26)
#define BIANMAQI_DIR            (B27)
#define BIANMAQI_XIAN_SHU       (1024)
#define BIANMAQI_CHI_LUN        (78.0f)
#define LUNZI_CHI_LUN           (54.0f)

static uint8 wifi_send_buffer[128];

static void geshihua_yiwei(char *wenben, int32 shuzhi_x10)
{
    int32 juedui = shuzhi_x10;
    char fuhao = '+';

    if(juedui < 0)
    {
        juedui = -juedui;
        fuhao = '-';
    }

    sprintf(wenben, "%c%ld.%ld",
            fuhao,
            (long)(juedui / 10L),
            (long)(juedui % 10L));
}

int main(void)
{
    int32  pulses;
    int32  encoder_rpm_x10;
    int32  wheel_rpm_x10;
    uint8  wifi_zhuang_tai;
    uint32 send_length;
    uint32 send_count = 0;
    uint8  print_count = 0;
    char   enc_wenben[24];
    char   wheel_wenben[24];

    clock_init(SYSTEM_CLOCK_80M);
    debug_init();

    // 接口 1 使用 SDI 模式；不要改成 ABZ，否则计数会出错。
    encoder_dir_init(BIANMAQI_TIMER, BIANMAQI_LSB, BIANMAQI_DIR);
    encoder_clear_count(BIANMAQI_TIMER);

    printf("\r\nBianMaQi JieKou1 + WiFi QiDong\r\n");

    // 第一步只复位并读取模块版本，不连接热点。
    // 如果这里失败，说明 SPI 接线、供电或模块本身还没有准备好。
    do
    {
        wifi_zhuang_tai = wifi_spi_init(NULL, NULL);
        printf("\r\nSPI ZhuangTai=%u BanBen=[%s]\r\n",
               wifi_zhuang_tai,
               wifi_spi_version);

        if(wifi_zhuang_tai)
        {
            system_delay_ms(500);
        }
    }while(wifi_zhuang_tai);

    // 第二步单独连接 2.4G 热点，方便区分 SPI 故障和热点故障。
    do
    {
        wifi_zhuang_tai = wifi_spi_wifi_connect(WIFI_MINGCHENG, WIFI_MIMA);
        printf("\r\nWiFi ZhuangTai=%u MoKuaiIP=[%s]\r\n",
               wifi_zhuang_tai,
               wifi_spi_ip_addr_port);

        if(wifi_zhuang_tai)
        {
            system_delay_ms(500);
        }
    }while(wifi_zhuang_tai);

    printf("\r\nWiFi MoKuai BanBen:%s", wifi_spi_version);
    printf("\r\nWiFi MoKuai IP:%s\r\n", wifi_spi_ip_addr_port);

    if(0 == WIFI_SPI_AUTO_CONNECT)
    {
        while(wifi_spi_socket_connect("TCP",
                                      DIANNAO_IP,
                                      DIANNAO_DUANKOU,
                                      BENJI_DUANKOU))
        {
            printf("\r\nDianNao TCP LianJie ShiBai, ZhengZai ZhongShi...\r\n");
            system_delay_ms(500);
        }
    }

    printf("\r\nTCP YiLianJie, KaiShi FaSong BianMaQi ShuJu\r\n");

    while(true)
    {
        // 每 50ms 读取并清零一次，得到这段时间内的脉冲数。
        system_delay_ms(50);

        pulses = encoder_get_count(BIANMAQI_TIMER);
        encoder_clear_count(BIANMAQI_TIMER);

        // Enc RPM x10 = 脉冲数 × 20 × 60 × 10 / 1024。
        if(pulses >= 0)
        {
            encoder_rpm_x10 = (pulses * 12000L + 512L) / 1024L;
        }
        else
        {
            encoder_rpm_x10 = (pulses * 12000L - 512L) / 1024L;
        }

        // 轮子 RPM = 编码器 RPM × 78 / 54。
        if(encoder_rpm_x10 >= 0)
        {
            wheel_rpm_x10 =
                (int32)(((int64)encoder_rpm_x10 * 78L +
                         (int64)(LUNZI_CHI_LUN / 2.0f)) /
                        (int64)LUNZI_CHI_LUN);
        }
        else
        {
            wheel_rpm_x10 =
                (int32)(((int64)encoder_rpm_x10 * 78L -
                         (int64)(LUNZI_CHI_LUN / 2.0f)) /
                        (int64)LUNZI_CHI_LUN);
        }

        geshihua_yiwei(enc_wenben, encoder_rpm_x10);
        geshihua_yiwei(wheel_wenben, wheel_rpm_x10);

        // FireWater：名称:通道0,通道1,通道2\n
        snprintf((char *)wifi_send_buffer, sizeof(wifi_send_buffer),
                 "BianMaQi:%s,%s,%ld\n",
                 enc_wenben,
                 wheel_wenben,
                 (long)pulses);

        send_length = strlen((char *)wifi_send_buffer);
        if(0 == wifi_spi_send_buffer(wifi_send_buffer, send_length))
        {
            send_count ++;
        }
        else
        {
            printf("\r\nWiFi FaSong ShiBai\r\n");
        }

        print_count ++;
        if(0 == (print_count % 10))
        {
            printf("\r\nMaiChong=%ld Enc=%s Wheel=%s Sent=%lu\r\n",
                   (long)pulses,
                   enc_wenben,
                   wheel_wenben,
                   (unsigned long)send_count);
        }
    }
}
