# IMU963RA 姿态解算工程

基于 MSPM0G3519 学习板的 IMU963RA 九轴姿态测试工程。程序通过 20 ms 定时中断采集数据，使用低通滤波和互补滤波计算 `Roll`、`Pitch`、`Yaw`，再通过 WiFi 发送到 VOFA+。

## 工程入口

使用 Keil MDK 打开：

```text
JiuZhouZiTaiGongCheng/mdk/JiuZhou_WiFi_ZiTai_CeShi.uvprojx
```

## 代码结构

| 文件 | 作用 |
|---|---|
| `user/src/ZhuHanshu.c` | 主函数，只负责初始化 |
| `user/src/JiuZhouCaiJi.c` | 定时采集中断与最新数据缓存 |
| `user/src/ZiTaiJieSuan.c` | 低通滤波和姿态解算 |
| `user/src/WifiChuanShu.c` | VOFA+ FireWater 数据发送 |
| `user/src/DingShiZhongDuan.c` | 20 ms 定时中断初始化 |
| `user/inc/XitongPeiZhi.h` | WiFi 连接和姿态滤波参数 |

## 运行前配置

仓库不保存个人网络信息。运行前请把以下占位符改成自己的配置：

```text
user/inc/XitongPeiZhi.h
libraries/zf_device/zf_device_wifi_spi.h
```

需要填写：

```c
#define WIFI_MINGCHENG  "YOUR_WIFI_SSID"
#define WIFI_MIMA       "YOUR_WIFI_PASSWORD"
#define DIANNAO_IP      "YOUR_PC_IP"
```

热点必须为 2.4 GHz。电脑 IP 改变后，上述两个头文件中的 IP 都要同步修改。

## VOFA+ 设置

```text
协议：FireWater
连接：TCP Server
端口：8086
```

发送格式：

```text
JiuZhou:AX,AY,AZ,GX,GY,GZ,MX,MY,MZ,Roll,Pitch,Yaw
```

## 姿态参数

参数位于 `user/inc/XitongPeiZhi.h`：

```c
#define ZITAI_YUN_DONG_XIU_ZHENG    (0.05f)
#define ZITAI_ZHONG_JIAN_XIU_ZHENG  (0.10f)
#define ZITAI_JING_ZHI_XIU_ZHENG    (0.30f)
#define ZITAI_JING_ZHI_DPS          (12.0f)
#define ZITAI_YUN_DONG_DPS          (30.0f)
```

当前算法适用于智能车接近水平行驶时的姿态测量。`Yaw` 尚未进行倾斜补偿，后续需要任意大倾角航向时可升级为四元数 Mahony 姿态解算。
