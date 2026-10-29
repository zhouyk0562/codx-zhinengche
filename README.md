# codx智能车

基于 `MSPM0G3519` 学习板的智能车培训代码。当前仓库包含编码器、电机驱动、速度 PI、堵转保护、TFT 显示、IMU963RA 姿态解算和 WiFi/VOFA+ FireWater 数据上传等测试工程。

## 工程说明

| 工程 | 用途 |
|---|---|
| `Projects/ZiTai_SuDu_Kong_Zhi_WiFi_CeShi` | IMU 坡度感知、动态目标轮速、速度 PI、堵转保护和 WiFi 上传 |
| `Projects/IMU963RA_WiFi_ZiTai_CeShi` | IMU963RA 九轴采集、互补滤波、Roll/Pitch/Yaw 姿态解算和 WiFi 上传 |
| `Projects/DianJi_PI_SuDu_Huan_WiFi_CeShi` | 最终速度环工程：电机接口 2、编码器接口 1、PI 速度控制、堵转保护、WiFi 上传 |
| `Projects/DianJi_JieKou1_WiFi_CeShi` | 电机低占空比、编码器和 WiFi 联调测试 |
| `Projects/BianMaQi_JieKou1_WiFi_CeShi` | 编码器接口 1 和 WiFi 测试 |
| `Projects/BianMaQi_JieKou1_PingMu_CeShi` | 编码器接口 1 和 1.8 寸 TFT 显示测试 |

调参过程记录位于 `Backups/DianJi_PI_TiaoCanJiLu_20261026.txt`。

## 当前硬件连接

### 电机驱动

```text
学习板电机接口 2
PWM = B12
DIR = B13
```

DRV8701E 的电机电源必须接电池，不能只依靠 DAP 下载器供电。

### 编码器接口 1

```text
编码器 A/脉冲 = A26
编码器 DIR     = B27
编码器线数     = 1024
```

当前单方向速度测试使用脉冲幅值作为反馈，避免方向线偶发毛刺导致速度符号翻转。

### WiFi SPI

```text
MISO = A13
SCK  = A12
MOSI = A9
CS   = A7
INT  = A15
RST  = A8
VCC  = 5V
GND  = GND
```

### IMU963RA 姿态传感器

IMU963RA 直接连接学习板姿态传感器接口 `P14`：

```text
3V3  = 3V3
GND  = GND
SCK  = B23
MOSI = B22
MISO = B21
CS   = B19
```

姿态工程使用 20 ms 定时中断采集九轴数据，通过低通滤波和互补滤波输出 `Roll/Pitch/Yaw`。VOFA+ FireWater 数据格式为：

```text
JiuZhou:AX,AY,AZ,GX,GY,GZ,MX,MY,MZ,Roll,Pitch,Yaw
```

其中 Ch9、Ch10、Ch11 分别是 `Roll`、`Pitch`、`Yaw`。

## 编译与下载

1. 使用 Keil MDK 打开对应工程目录下的 `.uvprojx` 文件。
2. 使用 CMSIS-DAP 下载器连接核心板。
3. 编译并下载。
4. 调试串口默认使用 `COM6`、波特率 `115200`。

## WiFi 和 VOFA+

仓库中的 WiFi 配置已经替换为占位符：

```c
#define WIFI_MINGCHENG  "YOUR_WIFI_SSID"
#define WIFI_MIMA       "YOUR_WIFI_PASSWORD"
#define DIANNAO_IP      "YOUR_PC_IP"
```

重新使用 WiFi 前，请填写自己的 2.4 GHz 热点名称、密码和当前电脑 WLAN IPv4 地址。

VOFA+ 设置：

```text
协议：FireWater
连接：TCP Server
端口：8086
```

最终工程发送五路数据：

```text
DianJi:目标轮速,实际轮速,占空比,编码器RPM,堵转状态
```

电脑 IP 改变后，必须同步修改源码中的 `DIANNAO_IP` 和 WiFi 库头文件中的 `WIFI_SPI_TARGET_IP`，然后重新编译下载。

## 安全提示

- 首次测试电机时必须让车轮悬空。
- 调试台架应固定，电池开关应能随手关闭。
- 修改电机、编码器或驱动板接线前先关闭总电源。
- 不要把 WiFi 密码、访问令牌或其他账号密钥提交到公开仓库。

## 第三方代码

工程包含逐飞科技 MSPM0G3519 开源库及芯片厂商 SDK。相关版权和许可证以各目录中的原始声明为准。
