#ifndef _xitong_peizhi_h_
#define _xitong_peizhi_h_

#include "zf_common_headfile.h"

// WiFi 和电脑接收端
#define WIFI_MINGCHENG                  "YOUR_WIFI_SSID"
#define WIFI_MIMA                       "YOUR_WIFI_PASSWORD"
#define DIANNAO_IP                      "YOUR_PC_IP"
#define DIANNAO_DUANKOU                 "8086"
#define BENJI_DUANKOU                   "6666"

// 新电机低速闭环测试
#define SU_DU_HUAN_YUNXING_SHI_NENG    (1U)    // 0：安全停车；1：启动闭环

// 50 ms 控制周期
#define KONGZHI_DINGSHI                 (PIT_TIM_G0)
#define KONGZHI_ZHOUQI_MS               (50U)
#define KONGZHI_CAIYANG_MIAO            (0.05f)

// 学习板电机接口 2
#define DIANJI_KONG_ZHI_DAI             (PWM_TIM_A0_CH2_B12)
#define DIANJI_FANG_XIANG_PIN           (B13)
#define DIANJI_PWM_PINZHOU              (17000U)
#define DIANJI_ZHENG_DIAN_PING          (GPIO_LOW)

// 编码器接口 1
#define BIANMAQI_TIMER                  (TIM_G7)
#define BIANMAQI_LSB                    (TIMG7_ENCODER1_CH1_A26)
#define BIANMAQI_DIR                    (B27)
#define BIANMAQI_XIAN_SHU               (1024.0f)
#define BIANMAQI_CHI_LUN                (78.0f)
#define LUNZI_CHI_LUN                   (54.0f)

// 闭环目标
#define MUBIAO_LUN_SU_RPM               (250.0f)

// 新电机低速闭环初版参数
#define PI_KP                           (1.0f)
#define PI_KI                           (0.5f)
#define PI_QI_DONG_JIFEN                (0.0f)
#define DUTY_QIAN_KUI_PERCENT           (7.5f)
#define INTEGRAL_MAX                    (5.0f)
#define DUTY_MAX_PERCENT                (15.0f)

// 新电机静摩擦较大，闭环前先短时给启动 PWM
#define QI_DONG_PWM_PERCENT             (11.0f)
#define QI_DONG_ZUI_DA_TICKS            (6U)
#define QI_DONG_ZUI_SHAO_TICKS          (2U)
#define QI_DONG_QI_DONG_RPM             (20.0f)

// 速度滤波和软启动
#define SU_DU_LV_BO_JIU                 (0.8f)
#define SU_DU_LV_BO_XIN                 (0.2f)
#define RUAN_QI_DONG_BU_CHANG           (0.2f)

// 堵转保护
#define DUZHUAN_DUTY_PERCENT            (8.0f)
#define DUZHUAN_WHEEL_RPM               (20.0f)
#define DUZHUAN_TRIP_TICKS              (5U)
#define DUZHUAN_RECOVER_TICKS           (60U)
#define DUZHUAN_QI_DONG_PINGBI_TICKS    (20U)

#endif
