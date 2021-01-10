
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define cRmNCustom                      0xbf       
#define cRmCustom                       0x00

//	����		����		����		��������
//	ͨ��+			����+				����+
//	ͨ��-			����-				����-
//	��������		�˵���				����
//	�˵���			�˵�				�˵���
//	��ʽ��ʾ		�˵���				¼����ͣ
//	���		�ظ�		ֹͣ		������ͣ
//	<<			>>			|<<			>>|
//	��Ч		ҹ��ģʽ 	��������	������
//	������		������		��Ƶ		������
//	HDMI1		HDMI2		HDMI3		HDMI-ARC
//	����		ͬ��1		ͬ��2		ģ��

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

#define cRmKey_EqSelect    			    0x4e
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

#define cPanKey_InputSource	    		0x18				// ������Դѡ��
#define cPanKey_Stereo    				0x14				// ������
#define cPanKey_SurroundMode    		0x12
#define cPanKey_EqSelect    			0x11
#define cPanKey_SkipDown   				0x01
#define cPanKey_SkipUp    				0x02
#define cPanKey_PlayPause    			0x04
#define cPanKey_AudioMute    			0x08
#define cPanKey_JogMenu    				0x28


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

    cMenu_VideoSrc = 6,
    MENU_SRC_FORMAT = 7,                                    // �˵���ʾ����������ʽ
    cMenu_Standby = 8,
    cMenu_Brightness = 9,
    cMenu_AudioMute = 10,
    cMenu_NightMode = 11,
    cMenu_NoiseSignal = 12,
    
    MENU_FIREWARE_INFO = 0x0d,                              // ��ʾ�̼�����

    cMenu_SdInsert = 14,                  // ��ʾSD����
    cMenu_UDiskInsert = 15,                  // ��ʾU�̲���
    cMenu_SdRemove = 16,                  // ��ʾSD�γ�
    cMenu_UDiskRemove = 17,                  // ��ʾU�̰γ�
    cMenu_UsbaRemove = 18,                 	// ��ʾUSB�����γ�
    cMenu_BtRemove = 19,                 	// ��ʾ������Ƶ�γ�
    cMenu_HdmiRemove = 20,                  // ��ʾHDMI�γ� 
    cMenu_PlayTrack = 21,                  // ��ʾ��ý���ļ���Ϣ
    cMenu_PlayTime = 22,                  // ��ʾ��ý�岥��ʱ��



    cMenu_TToneFL = 36,                  // ��������ǰ��������
    cMenu_TToneCE = 37,                  // ����������������
    cMenu_TToneSW = 38,                  // ������������������
    cMenu_TToneFR = 39,                  // ��������ǰ��������
    cMenu_TToneSR = 40,                  // ������������������
    cMenu_TToneBR = 41,                  // ������������������
    cMenu_TToneBL = 42,                  // ������������������
    cMenu_TToneSL = 43,                  // ������������������

    cMenu_ChTrimFL = 46,                  // ����΢��ǰ��������
    cMenu_ChTrimCE = 47,                  // ����΢����������
    cMenu_ChTrimSW = 48,                  // ����΢������������
    cMenu_ChTrimFR = 49,                  // ����΢��ǰ��������
    cMenu_ChTrimSR = 50,                  // ����΢������������
    cMenu_ChTrimBR = 51,                  // ����΢������������
    cMenu_ChTrimBL = 52,                  // ����΢������������
    cMenu_ChTrimSL = 53,                  // ����΢������������

    cMenu_MasterVolume = 56,
    cMenu_MicVolume1 = 57,                  // ��Ͳ1����
    cMenu_MicVolume2 = 58,                  // ��Ͳ2����
    cMenu_MicEcho = 59,                  // ��Ͳ��������
    cMenu_MicRepeat = 60,                  // ��Ͳ�ظ�����
    cMenu_MicDelay = 61,                  // ��Ͳ�ӳ�ʱ��
    cMenu_MicReverb = 62,                  // ��Ͳ�������


    cMenu_LipSync = 70,
    cMenu_DelayFront = 71,
    cMenu_DelayCenter = 72,					// �˵����������ӳ�ʱ�����
    cMenu_DelaySurround = 73,					// �˵����������ӳ�ʱ�����
    cMenu_DelayBack = 74,					// �˵����������ӳ�ʱ�����
    cMenu_SpeakFront = 75,
    cMenu_SpeakCenter = 76,
    cMenu_SpeakWoofer = 77,
    cMenu_SpeakSurround = 78,
    cMenu_SpeakBack = 79,
    cMenu_SpeakFilter = 80


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
    MEM_BRIGHTNESS = 0,                                     // ��ʾ������
    MEM_SOURCE_AUTO = 1,                                   	// �Զ�����Ļָ� 
    MEM_SELECT_2CH = 2                                      // ѡ��Ϊ������
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

