#ifndef _xitong_peizhi_h_
#define _xitong_peizhi_h_

#include "zf_common_headfile.h"

// 50 ms 开环控制周期
#define KONGZHI_DINGSHI             (PIT_TIM_G0)
#define KONGZHI_ZHOUQI_MS           (50U)

// 学习板电机接口 2
#define DIANJI_KONG_ZHI_DAI         (PWM_TIM_A0_CH2_B12)
#define DIANJI_FANG_XIANG_PIN       (B13)
#define DIANJI_PWM_PINZHOU          (17000U)
#define DIANJI_ZHENG_DIAN_PING      (GPIO_LOW)
#define DIANJI_FAN_DIAN_PING        (GPIO_HIGH)

// 编码器接口 1
#define BIANMAQI_TIMER              (TIM_G7)
#define BIANMAQI_LSB                (TIMG7_ENCODER1_CH1_A26)
#define BIANMAQI_DIR                (B27)
#define BIANMAQI_XIAN_SHU           (1024.0f)
#define BIANMAQI_CHI_LUN            (78.0f)
#define LUNZI_CHI_LUN               (54.0f)

// 开环测试参数
#define KAIHUAN_YUNXING_SHI_NENG    (0U)    // 0：只停车；确认安全后改成 1 启动自动测试
#define KAIHUAN_ZUI_DA_ZHANKONGBI   (15U)
#define KAIHUAN_SHENGYA_BUCHANG     (1U)
#define KAIHUAN_SHENGYA_TICKS       (15U)
#define KAIHUAN_BAOCHI_TICKS        (40U)
#define KAIHUAN_TINGZHI_TICKS       (20U)
#define KAIHUAN_LOG_TICKS           (1U)

#endif
