
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip



#define cSYS_TIMER0                     (65536 - 160)		// 使用6T双倍速，晶振为11.059MHz，中断时间为100us

#define HAL_IN_PKEY_0()					(P02)
#define HAL_IN_PKEY_1()					(P03)
#define HAL_IN_PKEY_2()					(P04)
#define HAL_IN_PKEY_3()					(P05)
#define HAL_PKEY_0(b)					{P20=b;}
#define HAL_PKEY_1(b)					{P21=b;}
#define HAL_PKEY_2(b)					{P22=b;}
#define HAL_PKEY_3(b)					{}

#ifdef DISPLAY_OLD                                          // 定义就编译 旧版本显示屏(P2。5取反)
#define HAL_DIP_LED(b)                	{P25=b;}
#else
#define HAL_DIP_LED(b)                	{P25=b^1;}
#endif

#define HAL_DIP_CLK(b)					{P10=b;}
#define HAL_DIP_DAT(b)					{P11=b;}
#define HAL_DIP_STB(b)					{P13=b;}

#define HAL_IN_JOP0()					(P07)
#define HAL_IN_JOP1() 					(P06)

#define HAL_LED0(b)                	    {P00=b^1;}
#define HAL_LED1(b)                	    {P01=b^1;}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define cRmNCustom                      0xde
#define cRmCustom                       0x21

#define cPanKey_Standby    			    0x18                // 原输入音源选择
#define cPanKey_JogMenu    				0x28

#define cRmKey_Standby                 	0x26
#define cRmKey_FactorySet           	0x5c
#define cRmKey_VolumeUp		    		0x51
#define cRmKey_VolumeDown	    		0x5b
#define cRmKey_AudioMute                0x50
#define cRmKey_MeunCtrl            		0x5a
#define cRmKey_MeunLeft            		0x20
#define cRmKey_MeunRight           		0x6d
#define cRmKey_PlayPause               	0x5e
#define cRmKey_SkipDown             	0x2a
#define cRmKey_SkipUp             		0x2e
#define cRmKey_Surround	    			0x54
#define cRmKey_InputOnKey    			0x2d                // 所有输入



#define cRmKey_Brightness              	0x58
#define cRmKey_MicCtrl                  0x18

#define cRmKey_TrimUp                	0x4d
#define cRmKey_TrimDown                	0xf1
#define cRmKey_MicUp                	0x0e
#define cRmKey_MicDown                	0x12
#define cRmKey_SwTRimUp		    		0x57
#define cRmKey_SwTRimDown	    		0x60

#define cRmKey_TestTone                 0x45
#define cRmKey_MeunUp              		0x06
#define cRmKey_MeunDown            		0x16

#define cRmKey_TrimCtrl              	0x55
#define cRmKey_Record                  	0x15
#define cRmKey_Random                  	0x52
#define cRmKey_Repeat                  	0xf0
#define cRmKey_Stop                  	0x10

#define cRmKey_FastBack                	0x56
#define cRmKey_FastForward             	0xf4

#define cRmKey_EqSelect    			    0x4e
#define cRmKey_NightMode         		0x4c
#define cRmKey_NoiseSignal         		0x0c
#define cRmKey_MediaType                0x0F                // 蓝牙输入

#define cRmKey_Stereo	    			0x4a
#define cRmKey_VideoSrc    				0x08                // SD输入
#define cRmKey_InputNet	    			0x0b                // UDisk输入

#define cRmKey_InputHdmi1	    		0x46
#define cRmKey_InputHdmi2	    		0x44
#define cRmKey_InputHdmi3	    		0x04
#define cRmKey_InputHdmiArc	    		0x07                // HDMI_ARC或外置7.1输入

#define cRmKey_InputOptica	    		0x42
#define cRmKey_InputCoaxal1	    		0x40
#define cRmKey_InputCoaxal2	    		0x00
#define cRmKey_InputAux	    			0x03

#define cPanKey_InputSource	    		0x18				// 输入音源选择
#define cPanKey_Stereo    				0x14				// 立体声
#define cPanKey_SurroundMode    		0x12
#define cPanKey_EqSelect    			0x11
#define cPanKey_SkipDown   				0x2a
#define cPanKey_SkipUp    				0x2e
#define cPanKey_PlayPause    			0x5e
#define cPanKey_AudioMute    			0x08


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 显示部分的定义 //////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define cSURTestTone_FLch                0
#define cSURTestTone_CNch                1
#define cSURTestTone_FRch                2
#define cSURTestTone_SRch                3
#define cSURTestTone_SLch                4   
#define cSURTestTone_SWch                5


typedef enum {
    MENU_RESTORE = 0,                                       // 菜单恢复状态
    MENU_INPUT_SOURCE = 1,                                  // 菜单状态:输入音源选择
    MENU_POWER_ON = 2,                                      // 菜单状态:电源打开

    MENU_LISTEN_MODE = 0x03,				                // 菜单聆听模式选择
    MENU_EQ_SELECT = 0x05,                                  // 菜单EQ均衡器选择

    MENU_VIDEO_SRC = 6,
    MENU_SRC_FORMAT = 7,                                    // 菜单显示输入码流格式
    MENU_STANDBY = 8,
    MENU_BRIGHTNESS = 9,
    MENU_AUDIO_MUTE = 10,
    MENU_NIGHT_MODE = 11,
    MENU_TEST_TONE = 12,
    
    MENU_FIREWARE_INFO = 0x0d,                              // 显示固件升级

    MENU_SD_INSERT = 14,                                    // 显示SD插入
    MENU_UD_INSERT = 15,                                    // 显示U盘插入
    MENU_SD_REMOVE = 16,                                    // 显示SD拔出
    MENU_UD_REMOVE = 17,                                    // 显示U盘拔出
    MENU_PC_REMOVE = 18,                 	                // 显示USB声卡拔出
    MENU_BT_REMOVE = 19,                 	                // 显示蓝牙音频拔出
    MENU_HDMI_REMOVE = 20,                                  // 显示HDMI拔出 
    MENU_PLAY_TRACK = 21,                                   // 显示多媒体文件信息
    MENU_PLAY_TIME = 22,                                    // 显示多媒体播放时间

    MENU_TESTTONE_FL = 36,                                  // 测试噪音前置左声道
    MENU_TESTTONE_CE = 37,                                  // 测试噪音中置声道
    MENU_TESTTONE_SW = 38,                                  // 测试噪音超低音声道
    MENU_TESTTONE_FR = 39,                                  // 测试噪音前置右声道
    MENU_TESTTONE_SR = 40,                                  // 测试噪音环绕右声道
    MENU_TESTTONE_BR = 41,                                  // 测试噪音后置右声道
    MENU_TESTTONE_BL = 42,                                  // 测试噪音后置左声道
    MENU_TESTTONE_SL = 43,                                  // 测试噪音环绕左声道

    MENU_CH_TRIM_FL = 46,                                   // 声道微调前置左声道
    MENU_CH_TRIM_CE = 47,                                   // 声道微调中置声道
    MENU_CH_TRIM_SW = 48,                                   // 声道微调超低音声道
    MENU_CH_TRIM_FR = 49,                                   // 声道微调前置右声道
    MENU_CH_TRIM_SR = 50,                                   // 声道微调环绕右声道
    MENU_CH_TRIM_BR = 51,                                   // 声道微调后置右声道
    MENU_CH_TRIM_BL = 52,                                   // 声道微调后置左声道
    MENU_CH_TRIM_SL = 53,                                   // 声道微调环绕左声道

    MENU_MASTER_VOL = 56,                                   // 菜单主音量调节
    MENU_MIC_VOL1 = 57,                                     // 菜单话筒1音量
    MENU_MIC_VOL2 = 58,                                     // 菜单话筒2音量
    MENU_MIC_ECHO = 59,                                     // 菜单话筒回声比例
    MENU_MIC_REPEAT = 60,                                   // 菜单话筒重复比例
    MENU_MIC_DELAY = 61,                                    // 菜单话筒延迟时间
    MENU_MIC_BASS = 62,                                     // 菜单话筒音调低音调节
    MENU_MIC_TREBLE = 63,                                   // 菜单话筒音调高音调节

    MENU_BT_1 = 65,                                         // 蓝牙配对
    MENU_BT_2 = 66,                                         // 蓝牙清空配对
    MENU_MODE_DSP = 67,

    MENU_LIP_SYNC = 70,
    MENU_DELAY_FRONT = 71,
    MENU_DELAY_CENTER = 72,					                // 菜单中置声道延迟时间调整
    MENU_DELAY_SURR = 73,					                // 菜单环绕声道延迟时间调整
    MENU_DELAY_BACK = 74,					                // 菜单后置声道延迟时间调整
    MENU_SPEAK_FRONT = 75,
    MENU_SPEAK_CENTER = 76,
    MENU_SPEAK_WOOFER = 77,
    MENU_SPEAK_SURR = 78,
    MENU_SPEAK_BACK = 79,
    MENU_SPEAK_FILTER = 80,


    MENU_TRIM_WOOFER = 86,                                  // 菜单声道微调超低音声道
    MENU_TRIM_CENTER = 87,                                  // 菜单声道微调中置声道
    MENU_TRIM_SURROUND = 88,                                // 菜单声道微调环绕声道
    MENU_TRIM_BACK = 89                                     // 菜单声道微调后置声道

} MENU_STATE;


typedef enum {
    MENU_SET_NORMAL = 0,                                        // 正常的显示模式
    MENU_SET_TWINKLE_ON = 1,                                    // 闪烁点亮
    MENU_SET_TWINKLE_OFF = 2,                                   // 闪烁熄灭
    MENU_SET_ADJ_DOWN = 3,                                      // 调整-
    MENU_SET_ADJ_UP = 4                                         // 调整+
} MENU_SET;


typedef enum {
    LISTEN_MODE_HIFI = 0,                                   // 聆听模式：立体声HIFI
    LISTEN_MODE_2_1CH = 1,                                  // 聆听模式：立体声2.1CH
    LISTEN_MODE_SURROUND1 = 2,				                // 聆听模式：多声道源码模式，没有任何多声道算法
    LISTEN_MODE_SURROUND2 = 3,				                // 聆听模式：多声道模式算法0
    LISTEN_MODE_SURROUND3 = 4,				                // 聆听模式：多声道模式算法1
    LISTEN_MODE_SURROUND4 = 5				                // 聆听模式：多声道音效
} LISTEN_MODE_STATE;



typedef enum {
	CHANNEL_FL,												// 前置左声道
	CHANNEL_FR,												// 前置右声道
	CHANNEL_CE,												// 中置声道
	CHANNEL_SW,												// 超低音声道
	CHANNEL_SL,												// 环绕左声道
	CHANNEL_SR,												// 环绕右声道
	CHANNEL_BL,												// 后置左声道
	CHANNEL_BR												// 后置右声道
} SPKER_CHANNEL;

typedef enum {
	SPKER_NONE,												// 对应声道的喇叭没有安装
	SPKER_SMALL,											// 对应声道为小喇叭
	SPKER_LARGE											    // 对应声道为大喇叭
} SPKER_SETUP;

typedef enum {
    MEM_BRIGHTNESS = 0,                                     // 记忆显示屏亮度
    MEM_SOURCE_AUTO = 1,                                   	// 记忆自动输入的恢复 
    MEM_MIC_BASS = 2,                                       // 记忆话筒音调低音调节
    MEM_MIC_TREBLE = 3                                      // 记忆话筒音调高音调节
} MEMORY_USER;                                              // 用户记忆


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 音频部分的定义 //////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
    INPUT_SWITCH_AUX = 0,                                   // 模拟输入
    INPUT_SWITCH_OPTIC = 1,                                 // 数码1   
    INPUT_SWITCH_COA1 = 2,                                  // 数码2   
    INPUT_SWITCH_COA2 = 3,                                  // 数码3   
    INPUT_SWITCH_SD = 4,                                    // SD      
    INPUT_SWITCH_UDISK = 5,                                 // UDISK   
    INPUT_SWITCH_USBA = 6,                                 	// USB声卡
    INPUT_SWITCH_E8CH = 7,                                  // 外置7.1声道
    INPUT_SWITCH_BT = 8,                                 	// 蓝牙音频
    INPUT_SWITCH_HDMI1 = 9,                                 // HDMI1   
    INPUT_SWITCH_HDMI2 = 10,                                // HDMI2   
    INPUT_SWITCH_HDMI3 = 11,                                // HDMI3   
    INPUT_SWITCH_H_ARC = 12,                                // HDMI-ARC
    INPUT_SWITCH_NONE = 13                                  // 循环方式或还没有选择    
} INPUT_SWITCH;

#define cDIP_SURR_STEREO               	0
#define cDIP_SURR_ST_SW               	1
#define cDIP_SURR_MODE1                	2
#define cDIP_SURR_MODE2                 3
#define cDIP_SURR_MODE3                 4
#define cDIP_SURR_MODE4                 5


#define cI2C_ACK						0
#define cI2C_NAK						1

#define cI2C_ADDRESS					0xcc				// 设置I2C的从机地址

