
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

// ！！！编译版本选择

// 标准版本 使用PT2258音量IC，支持话筒，字节2(B6:4为PT2258)字节3(B4打开MIC输入)
#define CUSTOM_CODE                     0x12123010

// 数码输出 使用PT2258音量IC，支持话筒MIC，字节2(B6:4为PT2258)字节3(B4打开MIC输入)
//#define CUSTOM_CODE                     0x12123010          

// 数码输出 使用PT2258音量IC，I2S数码格式为左对齐，字节2(B6:4为PT2258)字节3(B7:6为1)
//#define CUSTOM_CODE                     0x12123040          

// 数码输出 使用PT2258音量IC，I2S数码格式为右对齐，字节2(B6:4为PT2258)字节3(B7:6为2)
//#define CUSTOM_CODE                     0x12123080          

// 数码输出 使用PT2258音量IC，选择BCK及WCK为输入，字节2(B6:4为PT2258)字节3(B5为1)
//#define CUSTOM_CODE                     0x12123020          

// 数码输出 每步1dB的DSP数码音量，支持话筒MIC，字节2(B6:4为DSP每步1dB音量)字节3(B4打开MIC输入)
//#define CUSTOM_CODE                     0x12122010 

// 数码输出 每步0.5dB的DSP数码音量，支持话筒MIC，字节2(B6:4为DSP每步0.5dB音量)字节3(B4打开MIC输入)
//#define CUSTOM_CODE                     0x12121010 

// 数码输出 使用PT2258音量IC，支持话筒MIC(与模拟输入交换)，字节2(B6:4为PT2258)字节3(B4打开MIC输入 B3话筒MIC与模拟输入交换)
//#define CUSTOM_CODE                     0x12123018          

 // 标准版本旧
//#define CUSTOM_CODE                     0x12120000 

// 旧音频板，中置超低音与环绕声 对调  使用PT2258音量IC
//#define CUSTOM_CODE                     0x44443000          

// ！！！以上设置请选择一个适合自己的编译，而且必须选择一个(只有一个没有//注释)


// #define DISPLAY_OLD                                      // 定义就编译 旧版本显示屏(P2.5取反)


#define TONE_ENABLE                                       	// 定义就使用遥控器的音调、音调+、音调-
#define SPECTRUM_ENABLE                                    	// 定义就显示频谱

#if (CUSTOM_CODE & 0x00000010) != 0							// 支持话筒
#define MICROPHONE                                         	// 定义就将遥控器的音调、音调+、音调-改变为话筒控制
#endif

