
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
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

#define HAL_DIP_LED(b)                	{P25=b;}
#define HAL_DIP_CLK(b)					{P10=b;}
#define HAL_DIP_DAT(b)					{P11=b;}
#define HAL_DIP_STB(b)					{P13=b;}

#define HAL_IN_JOP0()					(P07)
#define HAL_IN_JOP1() 					(P06)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define cRmNCustom                      0xbf       
#define cRmCustom                       0x00

//	待机		亮度		音调		出厂设置
//	通道+			音调+				音量+
//	通道-			音调-				音量-
//	噪音测试		菜单上				静音
//	菜单左			菜单				菜单右
//	格式显示		菜单下				录音暂停
//	随机		重复		停止		播放暂停
//	<<			>>			|<<			>>|
//	音效		夜间模式 	噪音类型	云类型
//	立体声		多声道		视频		云音乐
//	HDMI1		HDMI2		HDMI3		HDMI-ARC
//	光纤		同轴1		同轴2		模拟

#define cRmKey_Standby                 	0x59
#define cRmKey_Brightness              	0x58
#define cRmKey_MicCtrl                  0x18
#define cRmKey_FactorySet           	0x19

#define cRmKey_TrimUp                	0x4d
#define cRmKey_TrimDown                	0x51
#define cRmKey_MicUp                	0x0e
#define cRmKey_MicDown                	0x12
#define cRmKey_VolumeUp		    		0x0d
#define cRmKey_VolumeDown	    		0x11

#define cRmKey_TestTone                 0x45
#define cRmKey_AudioMute                0x05
#define cRmKey_MeunCtrl            		0x1a
#define cRmKey_MeunUp              		0x06
#define cRmKey_MeunDown            		0x16
#define cRmKey_MeunLeft            		0x5a
#define cRmKey_MeunRight           		0x1b

#define cRmKey_TrimCtrl              	0x55
#define cRmKey_Record                  	0x15
#define cRmKey_Random                  	0x52
#define cRmKey_Repeat                  	0x50
#define cRmKey_Stop                  	0x10
#define cRmKey_PlayPause               	0x13

#define cRmKey_FastBack                	0x56
#define cRmKey_FastForward             	0x54
#define cRmKey_SkipDown             	0x14
#define cRmKey_SkipUp             		0x17

#define cRmKey_SoundEffect    			0x4e
#define cRmKey_NightMode         		0x4c
#define cRmKey_NoiseSignal         		0x0c
#define cRmKey_MediaType                0x0F

#define cRmKey_Stereo	    			0x4a
#define cRmKey_Surround	    			0x48
#define cRmKey_VideoSrc    				0x08
#define cRmKey_InputNet	    			0x0b

#define cRmKey_InputHdmi1	    		0x46
#define cRmKey_InputHdmi2	    		0x44
#define cRmKey_InputHdmi3	    		0x04
#define cRmKey_InputHdmiArc	    		0x07

#define cRmKey_InputOptica	    		0x42
#define cRmKey_InputCoaxal1	    		0x40
#define cRmKey_InputCoaxal2	    		0x00
#define cRmKey_InputAux	    			0x03

#define cPanKey_InputSource	    		0x18				// 输入音源选择
#define cPanKey_Stereo    				0x14				// 立体声
#define cPanKey_SurroundMode    		0x12
#define cPanKey_SoundEffect    			0x11
#define cPanKey_SkipDown   				0x01
#define cPanKey_SkipUp    				0x02
#define cPanKey_PlayPause    			0x04
#define cPanKey_AudioMute    			0x08
#define cPanKey_JogMenu    				0x28


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 显示部分的定义 //////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define cSURTestTone_FLch                0
#define cSURTestTone_CNch                1
#define cSURTestTone_FRch                2
#define cSURTestTone_SRch                3
#define cSURTestTone_SLch                4   
#define cSURTestTone_SWch                5

#define cMenu_Restore                   0
#define cMenu_InputSource               1
#define cMenu_PowerOn               	2
#define cMenu_Surround2Ch               3
#define cMenu_Surround8Ch               4
#define cMenu_SoundEffect               5
#define cMenu_VideoSrc               	6
#define cMenu_SrcFormat                 7
#define cMenu_Standby                 	8
#define cMenu_Brightness                9
#define cMenu_AudioMute                 10
#define cMenu_NightMode                 11
#define cMenu_NoiseSignal               12
#define cMenu_Fireware                  13                  // 显示固件升级
#define cMenu_SdInsert                  14                  // 显示SD插入
#define cMenu_SdRemove                  15                  // 显示SD插出
#define cMenu_UDiskInsert               16                  // 显示U盘插入
#define cMenu_UDiskRemove               17                  // 显示U盘插出
#define cMenu_PlayTrack                 18                  // 显示多媒体文件信息
#define cMenu_PlayTime                  19                  // 显示多媒体播放时间



#define cMenu_TToneFL                   36                  // 测试噪音前置左声道
#define cMenu_TToneCE                   37                  // 测试噪音中置声道
#define cMenu_TToneSW                   38                  // 测试噪音超低音声道
#define cMenu_TToneFR                   39                  // 测试噪音前置右声道
#define cMenu_TToneSR                   40                  // 测试噪音环绕右声道
#define cMenu_TToneBR                   41                  // 测试噪音后置右声道
#define cMenu_TToneBL                   42                  // 测试噪音后置左声道
#define cMenu_TToneSL                   43                  // 测试噪音环绕左声道

#define cMenu_ChTrimFL                  46                  // 声道微调前置左声道
#define cMenu_ChTrimCE                  47                  // 声道微调中置声道
#define cMenu_ChTrimSW                  48                  // 声道微调超低音声道
#define cMenu_ChTrimFR                  49                  // 声道微调前置右声道
#define cMenu_ChTrimSR                  50                  // 声道微调环绕右声道
#define cMenu_ChTrimBR                  51                  // 声道微调后置右声道
#define cMenu_ChTrimBL                  52                  // 声道微调后置左声道
#define cMenu_ChTrimSL                  53                  // 声道微调环绕左声道

#define cMenu_MasterVolume              56
#define cMenu_MicVolume1                57                  // 话筒1音量
#define cMenu_MicVolume2                58                  // 话筒2音量
#define cMenu_MicEcho                   59                  // 话筒回声比例
#define cMenu_MicRepeat                 60                  // 话筒重复比例
#define cMenu_MicDelay                  61                  // 话筒延迟时间
#define cMenu_MicReverb                 62                  // 话筒混响比例


#define cMenu_LipSync           		70
#define cMenu_DelayFront           		71
#define cMenu_DelayCenter          		72					// 菜单中置声道延迟时间调整
#define cMenu_DelaySurround        		73					// 菜单环绕声道延迟时间调整
#define cMenu_DelayBack           		74					// 菜单后置声道延迟时间调整
#define cMenu_SpeakFront           		75
#define cMenu_SpeakCenter          		76
#define cMenu_SpeakWoofer          		77
#define cMenu_SpeakSurround        		78
#define cMenu_SpeakBack           		79
#define cMenu_SpeakFilter          		80

typedef enum {
    MENU_NORMAL = 0,                                        // 正常的显示模式
    MENU_TWINKLE_ON = 1,                                    // 闪烁点亮
    MENU_TWINKLE_OFF = 2,                                   // 闪烁熄灭
    MENU_ADJ_DOWN = 3,                                      // 调整-
    MENU_ADJ_UP = 4                                         // 调整+
} MENU_MODE;


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
    MEM_SURROUND_2CH = 0,                                   // 环绕声模式立体声
    MEM_SURROUND_8CH = 1,                                   // 环绕声模式多声道
    MEM_SELECT_2CH = 2,                                     // 选择为立体声
    MEM_BRIGHTNESS = 3                                      // 显示屏亮度
} MEMORY_USER;                                              // 用户记忆


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 音频部分的定义 //////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
    INPUT_SWITCH_AUX = 0,                                   // 0=模拟输入
    INPUT_SWITCH_OPTIC = 1,                                 // 1=数码1   
    INPUT_SWITCH_COA1 = 2,                                  // 2=数码2   
    INPUT_SWITCH_COA2 = 3,                                  // 3=数码3   
    INPUT_SWITCH_SD = 4,                                    // 4=SD      
    INPUT_SWITCH_UDISK = 5,                                 // 5=UDISK   
    INPUT_SWITCH_HDMI1 = 6,                                 // 6=HDMI1   
    INPUT_SWITCH_HDMI2 = 7,                                 // 7=HDMI2   
    INPUT_SWITCH_HDMI3 = 8,                                 // 8=HDMI3   
    INPUT_SWITCH_H_ARC = 9,                                 // 9=HDMI-ARC
    INPUT_SWITCH_NONE = 10                                  // 循环方式或还没有选择    
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

EXTR BOOL FKCM_I2C_Error;

void MKCM_WriteRegister(					  				// 写DA32Ux的I2C寄存器, 成功返回1
					BYTE address, 							// 地址索引值
					BYTE vaule); 							// 数据
BYTE MKCM_ReadRegister(										// Read byte from DA32C. 读取8位的寄存器
					BYTE address);      					// Index. 寄存器索引值
void MKCM_Write2Byte(										// Write 2 bytes to DA32C. 写入2个字节
					BYTE address,      						// Index. 寄存器索引值
					WORD vaule);      						// 16 bit Data. 16位数据
void MKCM_WriteXByte(										// Write buffer to DA32C. 写入多个字节
					BYTE address,      						// Index. 寄存器索引值
					WORD length,      						// Length. 长度
					BYTE* in_data);   						// Buffer address. 数据缓冲
WORD MKCM_Read2Byte(										// Read word from DA32C. 读取16位的寄存器
					BYTE address);      					// Index. 寄存器索引值
void MKCM_ReadXByte(										// Read buffer from DA32C. 读取多个字节
					BYTE address,      						// Index. 寄存器索引值
					WORD length,    						// Length. 长度
					BYTE* out_data);      					// Buffer 数据缓冲




void MI2C_Bus_Write(BYTE gLocal_1);      					// 往I2C总线写入一个字节，gLocal为待写数据，返回为0表示成功
BYTE MI2C_Bus_Read(BOOL FLocal_NAK);						// 从I2C总线读一个字节，在最后一次读时，FLocal_NAK必须为1
void MI2C_Bus_Start();  									// I2C总线开始
void MI2C_Bus_Stop();										// I2C总线停止
void MI2C_100K_DELAY();										// 按照100Kbps的标准延时，以确保状态稳定
void MI2C_400K_DELAY();										// 按照400Kbps的标准延时，可以有效地提高通讯的速度

#define HAL_KCM_I2C_INT()				(P14)
#define HAL_KCM_I2C_SCL(b)				{P16=b;}
#define HAL_KCM_I2C_SDA(b)				{P15=b;}
#define HAL_KCM_I2C_IN_SDA()			(P15)

