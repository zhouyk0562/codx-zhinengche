#ifndef _xitong_peizhi_h_
#define _xitong_peizhi_h_

#include "zf_common_headfile.h"

// WiFi 和电脑接收端
#define WIFI_MINGCHENG                  "YOUR_WIFI_SSID"
#define WIFI_MIMA                       "YOUR_WIFI_PASSWORD"
#define DIANNAO_IP                      "YOUR_PC_IP"
#define DIANNAO_DUANKOU                 "8086"
#define BENJI_DUANKOU                   "6666"

// 20 ms 姿态采集
#define CAIJI_DINGSHI                  (PIT_TIM_G0)
#define CAIJI_ZHOUQI_MS                (20U)

// 50 ms 电机速度控制
#define KONGZHI_DINGSHI                 (PIT_TIM_G6)
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

// 姿态互补滤波
#define ZITAI_YUN_DONG_XIU_ZHENG        (0.05f)
#define ZITAI_ZHONG_JIAN_XIU_ZHENG      (0.10f)
#define ZITAI_JING_ZHI_XIU_ZHENG        (0.30f)
#define ZITAI_JING_ZHI_DPS              (12.0f)
#define ZITAI_YUN_DONG_DPS              (30.0f)

// 姿态坡度映射
#define SU_DU_HUAN_YUNXING_SHI_NENG    (1U)
#define ZI_TAI_SHIYONG_HENG_GUN_JIAO    (1)
#define ZI_TAI_LING_DIAN_CAI_YANG_SHU   (20U)
#define ZI_TAI_JI_CHU_RPM               (250.0f)
#define ZI_TAI_PO_DU_SI_QU              (1.0f)
#define ZI_TAI_PO_DU_RPM_PER_DU         (6.0f)
#define ZI_TAI_MU_BIAO_RPM_MIN          (100.0f)
#define ZI_TAI_MU_BIAO_RPM_MAX          (400.0f)
#define ZI_TAI_PO_DU_FANGXIANG          (1.0f)

// 新电机速度环
#define PI_KP                           (1.5f)
#define PI_KI                           (0.5f)
#define PI_QI_DONG_JIFEN                (0.0f)
#define DUTY_QIAN_KUI_JI_CHU_PERCENT    (5.0f)
#define DUTY_QIAN_KUI_RPM_XISHU         (0.01f)
#define INTEGRAL_MAX                    (5.0f)
#define DUTY_MAX_PERCENT                (20.0f)

// 目标速度平滑，每 50 ms 最多变化 5 RPM
#define MU_BIAO_RUAN_HUA_RPM_PER_TICK   (20.0f)

// 新电机低速启动
#define QI_DONG_PWM_PERCENT             (12.0f)
#define QI_DONG_ZUI_DA_TICKS            (12U)
#define QI_DONG_ZUI_SHAO_TICKS          (4U)
#define QI_DONG_QI_DONG_RPM             (20.0f)
#define QI_DONG_WAN_CHENG_RPM           (100.0f)

// 测速滤波
#define SU_DU_LV_BO_JIU                 (0.8f)
#define SU_DU_LV_BO_XIN                 (0.2f)

// 堵转保护
#define DUZHUAN_DUTY_PERCENT            (8.0f)
#define DUZHUAN_WHEEL_RPM               (20.0f)
#define DUZHUAN_TRIP_TICKS              (5U)
#define DUZHUAN_RECOVER_TICKS           (60U)
#define DUZHUAN_QI_DONG_PINGBI_TICKS    (20U)

#endif
