#include "zf_common_headfile.h"

// 学习板编码器接口 1 测试
// 编码器 QY1503-CDZ/SDI5E：
// 默认 ABZ 模式，硬件 QEI 使用 TIM_G8、A26/A27 四倍频计数。
// 如果编码器配置为 SDI（步/方向）模式，把 BIANMAQI_MOSHI 改成 1。
// 接口 1 的配套线已接好，VCC/GND 由学习板提供。

#define BIANMAQI_MOSHI          (1)         // 0=ABZ 正交，1=SDI 步/方向
#define BIANMAQI_XIAN_SHU       (1024)      // 编码器线数，实物 1024 线
#if BIANMAQI_MOSHI == 0
#define BIANMAQI_JISHU_MEI_QUAN (4)         // 硬件 QEI 四倍频
#else
#define BIANMAQI_JISHU_MEI_QUAN (1)         // SDI 单脉冲计数
#endif

#if BIANMAQI_MOSHI == 0
#define BIANMAQI_TIMER          (TIM_G8)
#define BIANMAQI_A              (TIMG8_ENCODER1_CH1_A26)
#define BIANMAQI_B              (TIMG8_ENCODER1_CH2_A27)
#else
#define BIANMAQI_TIMER          (TIM_G7)
#define BIANMAQI_LSB            (TIMG7_ENCODER1_CH1_A26)
#define BIANMAQI_DIR            (B27)
#endif

#define BIANMAQI_CHI_LUN        (78.0f)
#define LUNZI_CHI_LUN           (54.0f)

static void xianshi_yihang(uint16 y, const char *wenben)
{
    char hang[17];
    uint8 i;

    for(i = 0; i < 16; i ++)
    {
        hang[i] = ' ';
    }
    hang[16] = '\0';

    for(i = 0; i < 16 && '\0' != wenben[i]; i ++)
    {
        hang[i] = wenben[i];
    }
    tft180_show_string(0, y, hang);
}

static void geshihua_yiwei(char *wenben, int32 shuzhi_x10)
{
    int32 juedui = shuzhi_x10;
    char fuhao = '+';

    if(juedui < 0)
    {
        juedui = -juedui;
        fuhao = '-';
    }

    snprintf(wenben, 24, "%c%ld.%ld",
             fuhao,
             (long)(juedui / 10L),
             (long)(juedui % 10L));
}

int main(void)
{
    int32  pulses;
    float  encoder_rps;
    float  encoder_rpm;
    float  wheel_rpm;
    int32  encoder_rpm_x10;
    int32  wheel_rpm_x10;
    char   wenben[32];
    char   enc_wenben[24];
    char   rpm_wenben[24];
    uint8  display_div = 0;

    clock_init(SYSTEM_CLOCK_80M);
    debug_init();

#if BIANMAQI_MOSHI == 0
    encoder_quad_init(BIANMAQI_TIMER, BIANMAQI_A, BIANMAQI_B);
#else
    encoder_dir_init(BIANMAQI_TIMER, BIANMAQI_LSB, BIANMAQI_DIR);
#endif

    tft180_set_font(TFT180_8X16_FONT);
    tft180_set_dir(TFT180_PORTAIT);
    tft180_set_color(RGB565_WHITE, RGB565_BLACK);
    tft180_init();
    tft180_clear();

    tft180_set_color(RGB565_YELLOW, RGB565_BLACK);
    xianshi_yihang(0, "BianMaQi 1");
    tft180_set_color(RGB565_WHITE, RGB565_BLACK);

#if BIANMAQI_MOSHI == 0
    printf("BianMaQi JieKou1 ABZ QiDong\r\n");
#else
    printf("BianMaQi JieKou1 SDI QiDong\r\n");
#endif

    while(true)
    {
        system_delay_ms(50);

        pulses = encoder_get_count(BIANMAQI_TIMER);
        encoder_clear_count(BIANMAQI_TIMER);

        encoder_rps = (float)pulses * 20.0f /
                      (float)(BIANMAQI_XIAN_SHU * BIANMAQI_JISHU_MEI_QUAN);
        encoder_rpm = encoder_rps * 60.0f;
        wheel_rpm = encoder_rpm * BIANMAQI_CHI_LUN / LUNZI_CHI_LUN;

        display_div ++;
        if(display_div >= 2)
        {
            display_div = 0;

#if BIANMAQI_MOSHI == 0
            xianshi_yihang(16, "MoShi:ABZ");
#else
            xianshi_yihang(16, "MoShi:SDI");
#endif

            snprintf(wenben, sizeof(wenben), "MaiChong:%ld", (long)pulses);
            xianshi_yihang(32, wenben);

            encoder_rpm_x10 = (int32)(encoder_rpm * 10.0f +
                                      (encoder_rpm >= 0.0f ? 0.5f : -0.5f));
            geshihua_yiwei(enc_wenben, encoder_rpm_x10);
            snprintf(wenben, sizeof(wenben), "BianMaQi:%s",
                     enc_wenben);
            xianshi_yihang(48, wenben);

            wheel_rpm_x10 = (int32)(wheel_rpm * 10.0f +
                                    (wheel_rpm >= 0.0f ? 0.5f : -0.5f));
            geshihua_yiwei(rpm_wenben, wheel_rpm_x10);
            snprintf(wenben, sizeof(wenben), "LunZi:%s", rpm_wenben);
            xianshi_yihang(64, wenben);

#if BIANMAQI_MOSHI == 0
            printf("ABZ MaiChong=%ld Enc=%s Wheel=%s\r\n",
                   (long)pulses, enc_wenben, rpm_wenben);
#else
            printf("SDI MaiChong=%ld Enc=%s Wheel=%s\r\n",
                   (long)pulses, enc_wenben, rpm_wenben);
#endif

#if BIANMAQI_MOSHI == 0
            xianshi_yihang(80, "RPM / PPR4096");
#else
            xianshi_yihang(80, "RPM / PPR1024");
#endif
        }
    }
}
