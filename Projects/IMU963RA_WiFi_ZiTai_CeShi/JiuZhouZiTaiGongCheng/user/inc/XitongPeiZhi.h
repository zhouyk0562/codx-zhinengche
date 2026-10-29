#ifndef _xitong_peizhi_h_
#define _xitong_peizhi_h_

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

#endif
