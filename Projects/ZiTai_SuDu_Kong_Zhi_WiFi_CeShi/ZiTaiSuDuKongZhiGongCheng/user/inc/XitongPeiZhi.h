#ifndef _xitong_peizhi_h_
#define _xitong_peizhi_h_

#include "zf_common_headfile.h"

// WiFi 和电脑接收端
#define WIFI_MINGCHENG          "YOUR_WIFI_SSID"
#define WIFI_MIMA               "YOUR_WIFI_PASSWORD"
#define DIANNAO_IP              "YOUR_PC_IP"
#define DIANNAO_DUANKOU         "8086"
#define BENJI_DUANKOU           "6666"

// 原始九轴采集周期
#define CAIJI_DINGSHI          (PIT_TIM_G0)
#define CAIJI_ZHOUQI_MS        (20U)

// 姿态互补滤波修正权重
#define ZITAI_YUN_DONG_XIU_ZHENG    (0.05f)
#define ZITAI_ZHONG_JIAN_XIU_ZHENG  (0.10f)
#define ZITAI_JING_ZHI_XIU_ZHENG    (0.30f)
#define ZITAI_JING_ZHI_DPS          (12.0f)
#define ZITAI_YUN_DONG_DPS          (30.0f)

// 姿态控制目标
#define ZI_TAI_SHIYONG_HENG_GUN_JIAO    (1)       // 1：Roll 控制，0：Pitch 控制
#define ZI_TAI_LING_DIAN_CAI_YANG_SHU   (20U)     // 上电后取 20 个姿态样本确定零点
#define ZI_TAI_JI_CHU_RPM               (150.0f)  // 水平路面的基础目标轮速
#define ZI_TAI_PO_DU_SI_QU              (1.0f)    // 坡度死区，避免静止抖动改变转速
#define ZI_TAI_PO_DU_RPM_PER_DU         (5.0f)    // 上坡每度增加、下坡每度减少的轮速
#define ZI_TAI_MU_BIAO_RPM_MIN          (0.0f)    // 下坡最低降到停车
#define ZI_TAI_MU_BIAO_RPM_MAX          (300.0f)  // 目标轮速上限
#define ZI_TAI_PO_DU_FANGXIANG          (1.0f)    // 上坡减小、下坡增加时改成 -1.0f

// 电机速度控制周期
#define KONGZHI_DINGSHI             (PIT_TIM_G6)
#define KONGZHI_ZHOUQI_MS           (50U)
#define KONGZHI_CAIYANG_MIAO        (0.05f)

// 编码器接口 1
#define BIANMAQI_TIMER              (TIM_G7)
#define BIANMAQI_LSB                (TIMG7_ENCODER1_CH1_A26)
#define BIANMAQI_DIR                (B27)
#define BIANMAQI_XIAN_SHU           (1024.0f)

// 电机接口 2
#define DIANJI_DIR                  (B13)
#define DIANJI_PWM                  (PWM_TIM_A0_CH2_B12)
#define DIANJI_PWM_PINZHOU          (17000U)
#define DIANJI_ZHENG_ZHUAN_DIAN_PING    (GPIO_LOW)
#define DIANJI_FAN_ZHUAN_DIAN_PING      (GPIO_HIGH)

// 齿轮传动比
#define BIANMAQI_CHI_LUN            (78.0f)
#define LUNZI_CHI_LUN               (54.0f)

// 速度 PI，沿用已经调好的参数
#define PI_KP                       (20.0f)
#define PI_KI                       (20.0f)
#define DUTY_MAX_PERCENT            (35.0f)
#define INTEGRAL_MAX                (20.0f)

// 速度滤波和软启动
#define SU_DU_LV_BO_JIU             (0.7f)
#define SU_DU_LV_BO_XIN             (0.3f)
#define RUAN_QI_DONG_BU_CHANG       (0.2f)

// 堵转保护，沿用已经调好的参数
#define DUZHUAN_DUTY_PERCENT        (15.0f)
#define DUZHUAN_WHEEL_RPM           (30.0f)
#define DUZHUAN_TRIP_TICKS          (5U)
#define DUZHUAN_RECOVER_TICKS       (60U)

#endif
