# 深圳市酷唱科技有限公司

#### SDK介绍

快速开发音频产品的入口级开发套件，开发套件面板单片机使用最常用的8051单片机STC90C58+，使用串口烧录/下载软件。开发面板上有断电开关，无需关电，可直接拨动断电开关，就能实现STC单片机需要的断电烧录，只需要一个串口就可以使用调试面板单片机的程序。
开发包包含使用windows电脑全部开发工具，包括C51精简版编译器，可以运行批处理直接生成所需要的*.hex文件，用于下载程序的STC工具，可以下载及通过同一个串口直接打印调试的信息。还有一个国产非常实用的语言编辑工具EverEdit，专为国人设计的文本编辑器。


#### WINDOSW使用的实用工具

1.  编译器，精简过的C51v6.14，只保留必要的a51、c51、bl51、oh51、库及c51tiny.h.c文件，其中MLOG为我们开源的微型打印函数，辅助快速显示需要的信息到电脑。
2.  编辑器，阅读、编辑c文件时语法高亮，主题可以选择我们加入的Grey比较愉眼，推荐使用[EverEdit，专为国人设计的文本编辑器](http://www.everedit.cn/)
3.  下载器，STC-ISP下载编程烧录软件，官方当前版本为[stc-isp-15xx-v6.87C.exe](http://www.stcmcudata.com/STCISP/stc-isp-15xx-v6.87C.zip)

#### src文件简介

1.  面板按键及遥控功能函数key.c
2.  3米字+3数字+频谱LED显示屏功能函数display.c
3.  音频功能相关函数audio.c
4.  Kc35h或Kc32C等酷唱科技的KCM产品寄存器类型定义Kc3xType.h
5.  标准的I2C基本函数及KCM寄存器读取写入函数KcmI2c.h及KcmI2c.c
6.  常量、枚举及类型定义const.h
7.  主函数main.h及main.c
8.  C51编译链接脚本文件main.lin

#### 下载地址

1.  [开源软件 - 码云 Gitee.com](https://gitee.com/hsav20/kc3xm51.git)
2.  [GitHub全球化面向开源及私有软件项目的托管平台](https://github.com/hsav20/kc3xm51.git)
3.  [深圳市酷唱科技有限公司官网](http://www.hsav.com/download/kc3xm51.zip)

