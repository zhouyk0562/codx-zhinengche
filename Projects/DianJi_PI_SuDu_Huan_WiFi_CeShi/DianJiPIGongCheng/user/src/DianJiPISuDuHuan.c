#include "zf_common_headfile.h"

// 电机接口 2 + 编码器接口 1 + 速度 PI + 堵转保护 + WiFi
//
// 学习板配套灰排线：
//   电机 2：PWM=B12，DIR=B13（当前右侧电机）
//   编码器 1：A26=脉冲，B27=方向，1024 线，SDI 模式
//
// WiFi SPI：
//   MISO=A13  SCK=A12  MOSI=A9
//   CS=A7     INT=A15  RST=A8
//
// 参数沿用上一版实际调过的工程，未擅自改动。

#define WIFI_MINGCHENG          "YOUR_WIFI_SSID"
#define WIFI_MIMA               "YOUR_WIFI_PASSWORD"
#define DIANNAO_IP              "YOUR_PC_IP"
#define DIANNAO_DUANKOU         "8086"
#define BENJI_DUANKOU           "6666"

#define BIANMAQI_TIMER          (TIM_G7)
#define BIANMAQI_LSB            (TIMG7_ENCODER1_CH1_A26)
#define BIANMAQI_DIR            (B27)
#define BIANMAQI_XIAN_SHU       (1024.0f)

#define DIANJI_DIR              (B13)
#define DIANJI_PWM              (PWM_TIM_A0_CH2_B12)
static uint8 wifi_send_buffer[128];

// 目标轮速 250 RPM。
#define MUBIAO_LUN_SU_RPM       (250.0f)
#define PI_KP                   (20.0f)
#define PI_KI                   (20.0f)
#define DUTY_MAX_PERCENT        (35.0f)
#define INTEGRAL_MAX            (20.0f)
#define SAMPLE_SECOND           (0.05f)

// 堵转：占空比达到 13%，轮速仍低于 30 RPM，连续 5 次就保护。
// 保护后停止 60 次采样（约 3 秒），然后清积分并重新软启动。
#define DUZHUAN_DUTY_PERCENT    (13.0f)
#define DUZHUAN_WHEEL_RPM       (30.0f)
#define DUZHUAN_TRIP_TICKS      (5)
#define DUZHUAN_RECOVER_TICKS   (60)

int main(void)
{
    int32  pulses;
    float  target_encoder_rps;
    float  target_now_rps = 0.0f;
    float  actual_encoder_rps;
    float  actual_encoder_rps_guolv = 0.0f;
    float  actual_wheel_rpm;
    float  error;
    float  integral = 0.0f;
    float  duty = 0.0f;
    uint16 stall_ticks = 0;
    uint16 recover_ticks = 0;
    int32  target_wheel_i;
    int32  actual_wheel_i;
    int32  duty_i;
    int32  encoder_rpm_i;
    uint8  duzhuan_zhuangtai = 0;
    uint8  wifi_zhuang_tai;
    uint32 send_length;
    uint32 send_count = 0;

    clock_init(SYSTEM_CLOCK_80M);
    debug_init();

    // 当前编码器使用学习板接口 1 的硬件 SDI 计数。
    encoder_dir_init(BIANMAQI_TIMER, BIANMAQI_LSB, BIANMAQI_DIR);
    encoder_clear_count(BIANMAQI_TIMER);

    // 上电先把当前右侧电机设为零占空比。
    gpio_init(DIANJI_DIR, GPO, GPIO_LOW, GPO_PUSH_PULL);
    pwm_init(DIANJI_PWM, 17000, 0);

    printf("\r\nDianJi PI SuDu Huan + DuZhuan QiDong\r\n");

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

    printf("\r\nTCP YiLianJie, KaiShi PI SuDu Huan CeShi\r\n");

    while(true)
    {
        system_delay_ms(50);

        pulses = encoder_get_count(BIANMAQI_TIMER);
        encoder_clear_count(BIANMAQI_TIMER);

        // 当前训练只做正转。B27 方向线存在偶发毛刺，会让整段
        // 50ms 脉冲数突然变号；速度环使用脉冲幅值作为反馈。
        if(pulses < 0)
        {
            pulses = -pulses;
        }

        // 50ms 脉冲换算为编码器轴角速度。
        actual_encoder_rps = (float)pulses * 20.0f /
                             BIANMAQI_XIAN_SHU;

        // 一阶低通：新速度占 30%，旧结果占 70%。
        actual_encoder_rps_guolv =
            actual_encoder_rps_guolv * 0.7f +
            actual_encoder_rps * 0.3f;
        actual_encoder_rps = actual_encoder_rps_guolv;

        // 轮子 RPM = 编码器 RPM × 78 / 54。
        actual_wheel_rpm = actual_encoder_rps * 60.0f *
                           78.0f / 54.0f;

        // 目标轮速 250 RPM 换算到编码器轴。
        target_encoder_rps =
            (MUBIAO_LUN_SU_RPM * 54.0f / 78.0f) / 60.0f;

        // 目标转速软启动，每次增加 0.2 RPS。
        if(target_now_rps < target_encoder_rps)
        {
            target_now_rps += 0.2f;
            if(target_now_rps > target_encoder_rps)
            {
                target_now_rps = target_encoder_rps;
            }
        }

        if(recover_ticks > 0)
        {
            // 堵转恢复阶段：电机保持停止。
            recover_ticks --;
            duty = 0.0f;
            if(0 == recover_ticks)
            {
                integral = 0.0f;
                stall_ticks = 0;
                target_now_rps = 0.0f;
                duzhuan_zhuangtai = 0;
                printf("\r\nDUZHUAN CHONGSHI\r\n");
            }
        }
        else
        {
            error = target_now_rps - actual_encoder_rps;

            // 抗积分饱和：只有输出仍在 0~35% 内才更新积分。
            {
                float integral_next =
                    integral + error * SAMPLE_SECOND;
                float duty_next =
                    PI_KP * error + PI_KI * integral_next;

                if(integral_next > INTEGRAL_MAX)
                {
                    integral_next = INTEGRAL_MAX;
                }
                else if(integral_next < -INTEGRAL_MAX)
                {
                    integral_next = -INTEGRAL_MAX;
                }

                if(duty_next >= 0.0f &&
                   duty_next <= DUTY_MAX_PERCENT)
                {
                    integral = integral_next;
                }
            }

            duty = PI_KP * error + PI_KI * integral;
            if(duty > DUTY_MAX_PERCENT)
            {
                duty = DUTY_MAX_PERCENT;
            }
            else if(duty < 0.0f)
            {
                duty = 0.0f;
            }

            // 原地堵转判定。
            if(duty >= DUZHUAN_DUTY_PERCENT &&
               actual_wheel_rpm < DUZHUAN_WHEEL_RPM)
            {
                if(stall_ticks < DUZHUAN_TRIP_TICKS)
                {
                    stall_ticks ++;
                }

                if(stall_ticks >= DUZHUAN_TRIP_TICKS)
                {
                    duty = 0.0f;
                    integral = 0.0f;
                    target_now_rps = 0.0f;
                    stall_ticks = 0;
                    recover_ticks = DUZHUAN_RECOVER_TICKS;
                    duzhuan_zhuangtai = 1;
                    printf("\r\nDUZHUAN BAOHU\r\n");
                }
            }
            else
            {
                stall_ticks = 0;
            }
        }

        if(duty > 0.0f)
        {
            gpio_set_level(DIANJI_DIR, GPIO_LOW);
            pwm_set_duty(DIANJI_PWM,
                         (uint16)(duty *
                                  (PWM_DUTY_MAX / 100.0f)));
        }
        else
        {
            pwm_set_duty(DIANJI_PWM, 0);
        }

        target_wheel_i = (int32)(MUBIAO_LUN_SU_RPM + 0.5f);
        actual_wheel_i = (int32)(actual_wheel_rpm +
                                 (actual_wheel_rpm >= 0.0f ?
                                  0.5f : -0.5f));
        duty_i = (int32)(duty + 0.5f);
        encoder_rpm_i = (int32)(actual_encoder_rps * 60.0f +
                                (actual_encoder_rps >= 0.0f ?
                                 0.5f : -0.5f));

        // 通道0目标轮速，通道1实际轮速，通道2占空比，
        // 通道3编码器 RPM，通道4堵转状态。
        snprintf((char *)wifi_send_buffer, sizeof(wifi_send_buffer),
                 "DianJi:%ld,%ld,%ld,%ld,%d\n",
                 (long)target_wheel_i,
                 (long)actual_wheel_i,
                 (long)duty_i,
                 (long)encoder_rpm_i,
                 duzhuan_zhuangtai);

        send_length = strlen((char *)wifi_send_buffer);
        if(0 == wifi_spi_send_buffer(wifi_send_buffer, send_length))
        {
            send_count ++;
            if(0 == (send_count % 20))
            {
                printf("\r\ntarget=%ld actual=%ld duty=%ld enc=%ld MaiChong=%ld duzhuan=%d\r\n",
                       (long)target_wheel_i,
                       (long)actual_wheel_i,
                       (long)duty_i,
                       (long)encoder_rpm_i,
                       (long)pulses,
                       duzhuan_zhuangtai);
            }
        }
        else
        {
            printf("\r\nWiFi FaSong ShiBai\r\n");
        }
    }
}
