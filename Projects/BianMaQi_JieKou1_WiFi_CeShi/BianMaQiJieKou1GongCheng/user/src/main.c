/*********************************************************************************************************************
* MSPM0G3519 Opensource Library 即（MSPM0G3519 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MSPM0G3519 开源库的一部分
* 
* MSPM0G3519 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          mian
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.37
* 适用平台          MSPM0G3519
* 店铺链接          https://seekfree.taobao.com/
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include <stdio.h>
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// *************************** 例程硬件连接说明 ***************************
// 使用逐飞科技 CMSIS-DAP | ARM 调试下载器连接
//      直接将下载器正确连接在核心板的调试下载接口即可
// 
// QY1503-SPI5E-65536 绝对值编码器接线：
//      编码器 Pin1 GND   -> 核心板 GND
//      编码器 Pin2 VCC   -> 核心板 3V3
//      编码器 Pin3 SO    -> B8
//      编码器 Pin4 SI    -> B26
//      编码器 Pin5 CLK   -> B14
//      编码器 Pin6 CSN   -> B16
//
// 1.8 寸 TFT（VCC 必须接 3V3）：
//      SCL=A12  SDA=A9  RES=A7  DC=A15  CS=A8  BL=A13
//
// 使用 DAP 下载器，串口为 COM6，波特率 115200。
// 下载后用手转动编码器轴，串口会输出原始角度、角度变化量、状态和校验值。

// **************************** 代码区域 ****************************
#define BIANMAQI_SO                 (B8)        // 编码器 SO，主机输入
#define BIANMAQI_SI                 (B26)       // 编码器 SI，主机输出
#define BIANMAQI_CLK                (B14)       // 编码器 CLK
#define BIANMAQI_CSN                (B16)       // 编码器片选，低有效

static uint8 zuijin_zi_jie[5];

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

static void bianmaqi_yinjiao_chushihua(void)
{
    gpio_init(BIANMAQI_SO,  GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(BIANMAQI_SI,  GPO, GPIO_LOW,  GPO_PUSH_PULL);
    gpio_init(BIANMAQI_CLK, GPO, GPIO_LOW,  GPO_PUSH_PULL);
    gpio_init(BIANMAQI_CSN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
}

static uint8 bianmaqi_chuanshu_zijie(uint8 fasong_shuju)
{
    uint8 jieshou_shuju = 0;
    uint8 i;

    for(i = 0; i < 8; i ++)
    {
        if(fasong_shuju & 0x80)
        {
            gpio_set_level(BIANMAQI_SI, GPIO_HIGH);
        }
        else
        {
            gpio_set_level(BIANMAQI_SI, GPIO_LOW);
        }
        fasong_shuju <<= 1;
        system_delay_us(2);

        gpio_set_level(BIANMAQI_CLK, GPIO_HIGH);
        system_delay_us(2);

        jieshou_shuju <<= 1;
        if(gpio_get_level(BIANMAQI_SO))
        {
            jieshou_shuju |= 0x01;
        }

        gpio_set_level(BIANMAQI_CLK, GPIO_LOW);
        system_delay_us(2);
    }

    return jieshou_shuju;
}

static uint8 bianmaqi_chuanshu_zijie_xiajiang(uint8 fasong_shuju)
{
    uint8 jieshou_shuju = 0;
    uint8 i;

    for(i = 0; i < 8; i ++)
    {
        if(fasong_shuju & 0x80)
        {
            gpio_set_level(BIANMAQI_SI, GPIO_HIGH);
        }
        else
        {
            gpio_set_level(BIANMAQI_SI, GPIO_LOW);
        }
        fasong_shuju <<= 1;
        system_delay_us(2);

        gpio_set_level(BIANMAQI_CLK, GPIO_HIGH);
        system_delay_us(2);
        gpio_set_level(BIANMAQI_CLK, GPIO_LOW);
        system_delay_us(2);

        jieshou_shuju <<= 1;
        if(gpio_get_level(BIANMAQI_SO))
        {
            jieshou_shuju |= 0x01;
        }
    }

    return jieshou_shuju;
}

static void bianmaqi_duqu(uint16 *jiao_du, uint8 *zhuang_tai, uint8 *jiao_yan)
{
    uint8 shuju[5];

    gpio_set_level(BIANMAQI_CSN, GPIO_LOW);
    system_delay_us(2);

    // 第 1 个字节发送命令，MISO 此时是无效数据，必须丢弃。
    shuju[0] = bianmaqi_chuanshu_zijie(0x05);

    // 后面 4 个字节依次是：角度高字节、角度低字节、状态、CRC。
    shuju[1] = bianmaqi_chuanshu_zijie(0x00);
    shuju[2] = bianmaqi_chuanshu_zijie(0x00);
    shuju[3] = bianmaqi_chuanshu_zijie(0x00);
    shuju[4] = bianmaqi_chuanshu_zijie(0x00);

    gpio_set_level(BIANMAQI_CSN, GPIO_HIGH);
    system_delay_us(2);

    zuijin_zi_jie[0] = shuju[0];
    zuijin_zi_jie[1] = shuju[1];
    zuijin_zi_jie[2] = shuju[2];
    zuijin_zi_jie[3] = shuju[3];
    zuijin_zi_jie[4] = shuju[4];

    *jiao_du  = ((uint16)shuju[1] << 8) | shuju[2];
    *zhuang_tai = shuju[3];
    *jiao_yan = shuju[4];
}

static void bianmaqi_duqu_xiajiang(uint8 *shuju)
{
    gpio_set_level(BIANMAQI_CSN, GPIO_LOW);
    system_delay_us(2);

    shuju[0] = bianmaqi_chuanshu_zijie_xiajiang(0x05);
    shuju[1] = bianmaqi_chuanshu_zijie_xiajiang(0x00);
    shuju[2] = bianmaqi_chuanshu_zijie_xiajiang(0x00);
    shuju[3] = bianmaqi_chuanshu_zijie_xiajiang(0x00);
    shuju[4] = bianmaqi_chuanshu_zijie_xiajiang(0x00);

    gpio_set_level(BIANMAQI_CSN, GPIO_HIGH);
    system_delay_us(2);
}

int main (void)
{
    uint16 yuan_shi_jiao_du = 0;
    uint16 shang_ci_jiao_du = 0;
    uint8  zhuang_tai = 0;
    uint8  jiao_yan = 0;
    int32  jiao_du_bian_hua;
    uint32 jiao_du_bai_fen;
    char   wenben[32];
    uint8  shuju_xiajiang[5];
    uint8  shuju_youxiao;
    int32  zhuansu_rpm_x10;
    long   zhuansu_juedui;
    char   fuhao;

    clock_init(SYSTEM_CLOCK_80M);
    debug_init();

    bianmaqi_yinjiao_chushihua();

    tft180_set_font(TFT180_8X16_FONT);
    tft180_set_dir(TFT180_PORTAIT);
    tft180_set_color(RGB565_WHITE, RGB565_BLACK);
    tft180_init();
    tft180_clear();

    tft180_set_color(RGB565_YELLOW, RGB565_BLACK);
    tft180_show_string(0, 0, "QY1503");
    xianshi_yihang(16, "ZhuanSu");
    tft180_set_color(RGB565_WHITE, RGB565_BLACK);

    printf("QY1503 SPI BianMaQi QiDong\r\n");

    while(true)
    {
        bianmaqi_duqu(&yuan_shi_jiao_du, &zhuang_tai, &jiao_yan);
        system_delay_us(10);
        bianmaqi_duqu_xiajiang(shuju_xiajiang);

        jiao_du_bian_hua = (int32)yuan_shi_jiao_du - (int32)shang_ci_jiao_du;
        if(jiao_du_bian_hua > 32768)
        {
            jiao_du_bian_hua -= 65536;
        }
        else if(jiao_du_bian_hua < -32768)
        {
            jiao_du_bian_hua += 65536;
        }
        shang_ci_jiao_du = yuan_shi_jiao_du;

        jiao_du_bai_fen = ((uint32)yuan_shi_jiao_du * 36000U) / 65536U;
        zhuansu_rpm_x10 = (jiao_du_bian_hua * 6000L) / 65536L;

        shuju_youxiao = 1;
        if((0xFF == zuijin_zi_jie[1] && 0xFF == zuijin_zi_jie[2] &&
            0xFF == zuijin_zi_jie[3] && 0xFF == zuijin_zi_jie[4]) ||
           (0x00 == zuijin_zi_jie[1] && 0x00 == zuijin_zi_jie[2] &&
            0x00 == zuijin_zi_jie[3] && 0x00 == zuijin_zi_jie[4]))
        {
            shuju_youxiao = 0;
        }

        printf("YuanShi=%u  JiaoDu=%lu.%02lu  BianHua=%ld  ZhuangTai=0x%02X  JiaoYan=0x%02X\r\n",
               yuan_shi_jiao_du,
               (unsigned long)(jiao_du_bai_fen / 100U),
               (unsigned long)(jiao_du_bai_fen % 100U),
               (long)jiao_du_bian_hua,
               zhuang_tai,
               jiao_yan);

        printf("ZiJieS=%02X %02X %02X %02X %02X\r\n",
               zuijin_zi_jie[0],
               zuijin_zi_jie[1],
               zuijin_zi_jie[2],
               zuijin_zi_jie[3],
               zuijin_zi_jie[4]);

        printf("ZiJieX=%02X %02X %02X %02X %02X\r\n",
               shuju_xiajiang[0],
               shuju_xiajiang[1],
               shuju_xiajiang[2],
               shuju_xiajiang[3],
               shuju_xiajiang[4]);

        if(shuju_youxiao)
        {
            zhuansu_juedui = zhuansu_rpm_x10;
            fuhao = '+';
            if(zhuansu_juedui < 0)
            {
                zhuansu_juedui = -zhuansu_juedui;
                fuhao = '-';
            }
            snprintf(wenben, sizeof(wenben), "ZhuanSu:%c%ld.%ld",
                     fuhao,
                     zhuansu_juedui / 10L,
                     zhuansu_juedui % 10L);
        }
        else
        {
            snprintf(wenben, sizeof(wenben), "ZhuanSu:--");
        }
        xianshi_yihang(48, wenben);
        xianshi_yihang(80, "RPM");

        system_delay_ms(100);
    }
}

