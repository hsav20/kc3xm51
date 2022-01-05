
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip



#define cSYS_TIMER0                     (65536 - 160)		// ʹ��6T˫���٣�����Ϊ11.059MHz���ж�ʱ��Ϊ100us

#define HAL_IN_PKEY_0()					(P02)
#define HAL_IN_PKEY_1()					(P03)
#define HAL_IN_PKEY_2()					(P04)
#define HAL_IN_PKEY_3()					(P05)
#define HAL_PKEY_0(b)					{P20=b;}
#define HAL_PKEY_1(b)					{P21=b;}
#define HAL_PKEY_2(b)					{P22=b;}
#define HAL_PKEY_3(b)					{}

#ifdef DISPLAY_OLD                                          // ����ͱ��� �ɰ汾��ʾ��(P2��5ȡ��)
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

#define cPanKey_Standby    			    0x18                // ԭ������Դѡ��
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
#define cRmKey_InputOnKey    			0x2d                // ��������



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
#define cRmKey_MediaType                0x0F                // ��������

#define cRmKey_Stereo	    			0x4a
#define cRmKey_VideoSrc    				0x08                // SD����
#define cRmKey_InputNet	    			0x0b                // UDisk����

#define cRmKey_InputHdmi1	    		0x46
#define cRmKey_InputHdmi2	    		0x44
#define cRmKey_InputHdmi3	    		0x04
#define cRmKey_InputHdmiArc	    		0x07                // HDMI_ARC������7.1����

#define cRmKey_InputOptica	    		0x42
#define cRmKey_InputCoaxal1	    		0x40
#define cRmKey_InputCoaxal2	    		0x00
#define cRmKey_InputAux	    			0x03

#define cPanKey_InputSource	    		0x18				// ������Դѡ��
#define cPanKey_Stereo    				0x14				// ������
#define cPanKey_SurroundMode    		0x12
#define cPanKey_EqSelect    			0x11
#define cPanKey_SkipDown   				0x2a
#define cPanKey_SkipUp    				0x2e
#define cPanKey_PlayPause    			0x5e
#define cPanKey_AudioMute    			0x08


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ʾ���ֵĶ��� //////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define cSURTestTone_FLch                0
#define cSURTestTone_CNch                1
#define cSURTestTone_FRch                2
#define cSURTestTone_SRch                3
#define cSURTestTone_SLch                4   
#define cSURTestTone_SWch                5


typedef enum {
    MENU_RESTORE = 0,                                       // �˵��ָ�״̬
    MENU_INPUT_SOURCE = 1,                                  // �˵�״̬:������Դѡ��
    MENU_POWER_ON = 2,                                      // �˵�״̬:��Դ��

    MENU_LISTEN_MODE = 0x03,				                // �˵�����ģʽѡ��
    MENU_EQ_SELECT = 0x05,                                  // �˵�EQ������ѡ��

    MENU_VIDEO_SRC = 6,
    MENU_SRC_FORMAT = 7,                                    // �˵���ʾ����������ʽ
    MENU_STANDBY = 8,
    MENU_BRIGHTNESS = 9,
    MENU_AUDIO_MUTE = 10,
    MENU_NIGHT_MODE = 11,
    MENU_TEST_TONE = 12,
    
    MENU_FIREWARE_INFO = 0x0d,                              // ��ʾ�̼�����

    MENU_SD_INSERT = 14,                                    // ��ʾSD����
    MENU_UD_INSERT = 15,                                    // ��ʾU�̲���
    MENU_SD_REMOVE = 16,                                    // ��ʾSD�γ�
    MENU_UD_REMOVE = 17,                                    // ��ʾU�̰γ�
    MENU_PC_REMOVE = 18,                 	                // ��ʾUSB�����γ�
    MENU_BT_REMOVE = 19,                 	                // ��ʾ������Ƶ�γ�
    MENU_HDMI_REMOVE = 20,                                  // ��ʾHDMI�γ� 
    MENU_PLAY_TRACK = 21,                                   // ��ʾ��ý���ļ���Ϣ
    MENU_PLAY_TIME = 22,                                    // ��ʾ��ý�岥��ʱ��

    MENU_TESTTONE_FL = 36,                                  // ��������ǰ��������
    MENU_TESTTONE_CE = 37,                                  // ����������������
    MENU_TESTTONE_SW = 38,                                  // ������������������
    MENU_TESTTONE_FR = 39,                                  // ��������ǰ��������
    MENU_TESTTONE_SR = 40,                                  // ������������������
    MENU_TESTTONE_BR = 41,                                  // ������������������
    MENU_TESTTONE_BL = 42,                                  // ������������������
    MENU_TESTTONE_SL = 43,                                  // ������������������

    MENU_CH_TRIM_FL = 46,                                   // ����΢��ǰ��������
    MENU_CH_TRIM_CE = 47,                                   // ����΢����������
    MENU_CH_TRIM_SW = 48,                                   // ����΢������������
    MENU_CH_TRIM_FR = 49,                                   // ����΢��ǰ��������
    MENU_CH_TRIM_SR = 50,                                   // ����΢������������
    MENU_CH_TRIM_BR = 51,                                   // ����΢������������
    MENU_CH_TRIM_BL = 52,                                   // ����΢������������
    MENU_CH_TRIM_SL = 53,                                   // ����΢������������

    MENU_MASTER_VOL = 56,                                   // �˵�����������
    MENU_MIC_VOL1 = 57,                                     // �˵���Ͳ1����
    MENU_MIC_VOL2 = 58,                                     // �˵���Ͳ2����
    MENU_MIC_ECHO = 59,                                     // �˵���Ͳ��������
    MENU_MIC_REPEAT = 60,                                   // �˵���Ͳ�ظ�����
    MENU_MIC_DELAY = 61,                                    // �˵���Ͳ�ӳ�ʱ��
    MENU_MIC_BASS = 62,                                     // �˵���Ͳ������������
    MENU_MIC_TREBLE = 63,                                   // �˵���Ͳ������������

    MENU_BT_1 = 65,                                         // �������
    MENU_BT_2 = 66,                                         // ����������
    MENU_MODE_DSP = 67,

    MENU_LIP_SYNC = 70,
    MENU_DELAY_FRONT = 71,
    MENU_DELAY_CENTER = 72,					                // �˵����������ӳ�ʱ�����
    MENU_DELAY_SURR = 73,					                // �˵����������ӳ�ʱ�����
    MENU_DELAY_BACK = 74,					                // �˵����������ӳ�ʱ�����
    MENU_SPEAK_FRONT = 75,
    MENU_SPEAK_CENTER = 76,
    MENU_SPEAK_WOOFER = 77,
    MENU_SPEAK_SURR = 78,
    MENU_SPEAK_BACK = 79,
    MENU_SPEAK_FILTER = 80,


    MENU_TRIM_WOOFER = 86,                                  // �˵�����΢������������
    MENU_TRIM_CENTER = 87,                                  // �˵�����΢����������
    MENU_TRIM_SURROUND = 88,                                // �˵�����΢����������
    MENU_TRIM_BACK = 89                                     // �˵�����΢����������

} MENU_STATE;


typedef enum {
    MENU_SET_NORMAL = 0,                                        // ��������ʾģʽ
    MENU_SET_TWINKLE_ON = 1,                                    // ��˸����
    MENU_SET_TWINKLE_OFF = 2,                                   // ��˸Ϩ��
    MENU_SET_ADJ_DOWN = 3,                                      // ����-
    MENU_SET_ADJ_UP = 4                                         // ����+
} MENU_SET;


typedef enum {
    LISTEN_MODE_HIFI = 0,                                   // ����ģʽ��������HIFI
    LISTEN_MODE_2_1CH = 1,                                  // ����ģʽ��������2.1CH
    LISTEN_MODE_SURROUND1 = 2,				                // ����ģʽ��������Դ��ģʽ��û���κζ������㷨
    LISTEN_MODE_SURROUND2 = 3,				                // ����ģʽ��������ģʽ�㷨0
    LISTEN_MODE_SURROUND3 = 4,				                // ����ģʽ��������ģʽ�㷨1
    LISTEN_MODE_SURROUND4 = 5				                // ����ģʽ����������Ч
} LISTEN_MODE_STATE;



typedef enum {
	CHANNEL_FL,												// ǰ��������
	CHANNEL_FR,												// ǰ��������
	CHANNEL_CE,												// ��������
	CHANNEL_SW,												// ����������
	CHANNEL_SL,												// ����������
	CHANNEL_SR,												// ����������
	CHANNEL_BL,												// ����������
	CHANNEL_BR												// ����������
} SPKER_CHANNEL;

typedef enum {
	SPKER_NONE,												// ��Ӧ����������û�а�װ
	SPKER_SMALL,											// ��Ӧ����ΪС����
	SPKER_LARGE											    // ��Ӧ����Ϊ������
} SPKER_SETUP;

typedef enum {
    MEM_BRIGHTNESS = 0,                                     // ������ʾ������
    MEM_SOURCE_AUTO = 1,                                   	// �����Զ�����Ļָ� 
    MEM_MIC_BASS = 2,                                       // ���仰Ͳ������������
    MEM_MIC_TREBLE = 3                                      // ���仰Ͳ������������
} MEMORY_USER;                                              // �û�����


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��Ƶ���ֵĶ��� //////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
    INPUT_SWITCH_AUX = 0,                                   // ģ������
    INPUT_SWITCH_OPTIC = 1,                                 // ����1   
    INPUT_SWITCH_COA1 = 2,                                  // ����2   
    INPUT_SWITCH_COA2 = 3,                                  // ����3   
    INPUT_SWITCH_SD = 4,                                    // SD      
    INPUT_SWITCH_UDISK = 5,                                 // UDISK   
    INPUT_SWITCH_USBA = 6,                                 	// USB����
    INPUT_SWITCH_E8CH = 7,                                  // ����7.1����
    INPUT_SWITCH_BT = 8,                                 	// ������Ƶ
    INPUT_SWITCH_HDMI1 = 9,                                 // HDMI1   
    INPUT_SWITCH_HDMI2 = 10,                                // HDMI2   
    INPUT_SWITCH_HDMI3 = 11,                                // HDMI3   
    INPUT_SWITCH_H_ARC = 12,                                // HDMI-ARC
    INPUT_SWITCH_NONE = 13                                  // ѭ����ʽ��û��ѡ��    
} INPUT_SWITCH;

#define cDIP_SURR_STEREO               	0
#define cDIP_SURR_ST_SW               	1
#define cDIP_SURR_MODE1                	2
#define cDIP_SURR_MODE2                 3
#define cDIP_SURR_MODE3                 4
#define cDIP_SURR_MODE4                 5


#define cI2C_ACK						0
#define cI2C_NAK						1

#define cI2C_ADDRESS					0xcc				// ����I2C�Ĵӻ���ַ

