
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

#define HAL_DIP_LED(b)                	{P25=b;}
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

#define cPanKey_InputSource	    		0x18				// ������Դѡ��
#define cPanKey_Stereo    				0x14				// ������
#define cPanKey_SurroundMode    		0x12
#define cPanKey_SoundEffect    			0x11
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
#define cMenu_Fireware                  13                  // ��ʾ�̼�����
#define cMenu_SdInsert                  14                  // ��ʾSD����
#define cMenu_SdRemove                  15                  // ��ʾSD���
#define cMenu_UDiskInsert               16                  // ��ʾU�̲���
#define cMenu_UDiskRemove               17                  // ��ʾU�̲��
#define cMenu_PlayTrack                 18                  // ��ʾ��ý���ļ���Ϣ
#define cMenu_PlayTime                  19                  // ��ʾ��ý�岥��ʱ��



#define cMenu_TToneFL                   36                  // ��������ǰ��������
#define cMenu_TToneCE                   37                  // ����������������
#define cMenu_TToneSW                   38                  // ������������������
#define cMenu_TToneFR                   39                  // ��������ǰ��������
#define cMenu_TToneSR                   40                  // ������������������
#define cMenu_TToneBR                   41                  // ������������������
#define cMenu_TToneBL                   42                  // ������������������
#define cMenu_TToneSL                   43                  // ������������������

#define cMenu_ChTrimFL                  46                  // ����΢��ǰ��������
#define cMenu_ChTrimCE                  47                  // ����΢����������
#define cMenu_ChTrimSW                  48                  // ����΢������������
#define cMenu_ChTrimFR                  49                  // ����΢��ǰ��������
#define cMenu_ChTrimSR                  50                  // ����΢������������
#define cMenu_ChTrimBR                  51                  // ����΢������������
#define cMenu_ChTrimBL                  52                  // ����΢������������
#define cMenu_ChTrimSL                  53                  // ����΢������������

#define cMenu_MasterVolume              56
#define cMenu_MicVolume1                57                  // ��Ͳ1����
#define cMenu_MicVolume2                58                  // ��Ͳ2����
#define cMenu_MicEcho                   59                  // ��Ͳ��������
#define cMenu_MicRepeat                 60                  // ��Ͳ�ظ�����
#define cMenu_MicDelay                  61                  // ��Ͳ�ӳ�ʱ��
#define cMenu_MicReverb                 62                  // ��Ͳ�������


#define cMenu_LipSync           		70
#define cMenu_DelayFront           		71
#define cMenu_DelayCenter          		72					// �˵����������ӳ�ʱ�����
#define cMenu_DelaySurround        		73					// �˵����������ӳ�ʱ�����
#define cMenu_DelayBack           		74					// �˵����������ӳ�ʱ�����
#define cMenu_SpeakFront           		75
#define cMenu_SpeakCenter          		76
#define cMenu_SpeakWoofer          		77
#define cMenu_SpeakSurround        		78
#define cMenu_SpeakBack           		79
#define cMenu_SpeakFilter          		80

typedef enum {
    MENU_NORMAL = 0,                                        // ��������ʾģʽ
    MENU_TWINKLE_ON = 1,                                    // ��˸����
    MENU_TWINKLE_OFF = 2,                                   // ��˸Ϩ��
    MENU_ADJ_DOWN = 3,                                      // ����-
    MENU_ADJ_UP = 4                                         // ����+
} MENU_MODE;


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
    MEM_SURROUND_2CH = 0,                                   // ������ģʽ������
    MEM_SURROUND_8CH = 1,                                   // ������ģʽ������
    MEM_SELECT_2CH = 2,                                     // ѡ��Ϊ������
    MEM_BRIGHTNESS = 3                                      // ��ʾ������
} MEMORY_USER;                                              // �û�����


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��Ƶ���ֵĶ��� //////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
    INPUT_SWITCH_AUX = 0,                                   // 0=ģ������
    INPUT_SWITCH_OPTIC = 1,                                 // 1=����1   
    INPUT_SWITCH_COA1 = 2,                                  // 2=����2   
    INPUT_SWITCH_COA2 = 3,                                  // 3=����3   
    INPUT_SWITCH_SD = 4,                                    // 4=SD      
    INPUT_SWITCH_UDISK = 5,                                 // 5=UDISK   
    INPUT_SWITCH_HDMI1 = 6,                                 // 6=HDMI1   
    INPUT_SWITCH_HDMI2 = 7,                                 // 7=HDMI2   
    INPUT_SWITCH_HDMI3 = 8,                                 // 8=HDMI3   
    INPUT_SWITCH_H_ARC = 9,                                 // 9=HDMI-ARC
    INPUT_SWITCH_NONE = 10                                  // ѭ����ʽ��û��ѡ��    
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

EXTR BOOL FKCM_I2C_Error;

void MKCM_WriteRegister(					  				// дDA32Ux��I2C�Ĵ���, �ɹ�����1
					BYTE address, 							// ��ַ����ֵ
					BYTE vaule); 							// ����
BYTE MKCM_ReadRegister(										// Read byte from DA32C. ��ȡ8λ�ļĴ���
					BYTE address);      					// Index. �Ĵ�������ֵ
void MKCM_Write2Byte(										// Write 2 bytes to DA32C. д��2���ֽ�
					BYTE address,      						// Index. �Ĵ�������ֵ
					WORD vaule);      						// 16 bit Data. 16λ����
void MKCM_WriteXByte(										// Write buffer to DA32C. д�����ֽ�
					BYTE address,      						// Index. �Ĵ�������ֵ
					WORD length,      						// Length. ����
					BYTE* in_data);   						// Buffer address. ���ݻ���
WORD MKCM_Read2Byte(										// Read word from DA32C. ��ȡ16λ�ļĴ���
					BYTE address);      					// Index. �Ĵ�������ֵ
void MKCM_ReadXByte(										// Read buffer from DA32C. ��ȡ����ֽ�
					BYTE address,      						// Index. �Ĵ�������ֵ
					WORD length,    						// Length. ����
					BYTE* out_data);      					// Buffer ���ݻ���




void MI2C_Bus_Write(BYTE gLocal_1);      					// ��I2C����д��һ���ֽڣ�gLocalΪ��д���ݣ�����Ϊ0��ʾ�ɹ�
BYTE MI2C_Bus_Read(BOOL FLocal_NAK);						// ��I2C���߶�һ���ֽڣ������һ�ζ�ʱ��FLocal_NAK����Ϊ1
void MI2C_Bus_Start();  									// I2C���߿�ʼ
void MI2C_Bus_Stop();										// I2C����ֹͣ
void MI2C_100K_DELAY();										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
void MI2C_400K_DELAY();										// ����400Kbps�ı�׼��ʱ��������Ч�����ͨѶ���ٶ�

#define HAL_KCM_I2C_INT()				(P14)
#define HAL_KCM_I2C_SCL(b)				{P16=b;}
#define HAL_KCM_I2C_SDA(b)				{P15=b;}
#define HAL_KCM_I2C_IN_SDA()			(P15)

