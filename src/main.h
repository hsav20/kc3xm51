
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

// ����������汾ѡ��

//#define CUSTOM_CODE                     0x12123000          // ��׼�汾 ʹ��PT2258����IC
//#define CUSTOM_CODE                     0x12123018          // ������� ʹ��PT2258����IC���ֽ�1(B3:PT2258)�ֽ�0(B3:ģ���MIC����)
//#define CUSTOM_CODE                     0x12120000          // ��׼�汾
#define CUSTOM_CODE                     0x44443000          // ����Ƶ�壬���ó������뻷���� �Ե�  ʹ��PT2258����IC

// ����������������ѡ��һ���ʺ��Լ��ı��룬���ұ���ѡ��һ��(ֻ��һ��û��//ע��)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "c51tiny.h"
#include "Kc3xType.h"
#include "const.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I2C�ӿڲ��ֵĶ��� ///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ʱ�Ӽ�ϵͳ���ֵĶ��� ////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXTR idata BYTE gTBascTimer;  
EXTR idata BYTE g10msTimer;  
EXTR idata BYTE g1000msTimer;  
EXTR idata BYTE gRmStatus;  
EXTR idata BYTE gRmTimer;  
EXTR idata BYTE gRmBuffer;  
EXTR idata BYTE GRmCodeData;  
EXTR idata BYTE gRmKeyContinCanclTm;  
EXTR idata BYTE gRmKeyContinSpeed;  
EXTR idata BYTE GPKeyData;
EXTR idata BYTE gPKeyBuffer;
EXTR idata BYTE gKeyScan;

EXTR idata BYTE gKeyJopLockTm;
EXTR idata BYTE gKeyJopInputTm;
EXTR BOOL FKeyJogUp;
EXTR BOOL FKeyJogSwOK;         
EXTR BOOL FKeyJopSta00;
EXTR BOOL FKeyJopSta01;         
EXTR BOOL FKeyJogUpSave;
EXTR BOOL FKeyJopInputEnl;

EXTR BOOL FSYS_WIFI_Connect;  
EXTR xdata BYTE gSYS_WifiCommand;  
EXTR xdata BYTE gSYS_ModelType;  
EXTR xdata BYTE gBrightnessTimer;  

EXTR xdata BYTE gPKeyConfirmTm;
EXTR xdata BYTE gDSP_KCONFIG;
EXTR xdata BYTE gMED_PlayMode;
EXTR xdata BYTE gSUR_C_Delay;
EXTR xdata BYTE gSUR_S_Delay;

EXTR xdata BYTE gPKey_PressTime;
EXTR xdata BYTE gPKey_Status;
EXTR xdata BYTE gSUBInitialize;

EXTR xdata BYTE gSeekAutoTm;
EXTR xdata BYTE gSeekAutoTime;
EXTR xdata BYTE gDelay_SeekAutoTime;
EXTR BOOL FSeekAuto;



EXTR BOOL FAUD_MasterVolume;
EXTR xdata BYTE gAUD_AutoInputSrc;
EXTR xdata BYTE gAUD_MasterVolume;
EXTR xdata BYTE gAUD_SrcFormat;
EXTR xdata BYTE gAUD_BpsRate;
EXTR xdata WORD g2SUB_SrcValid;
EXTR xdata BYTE gSUB_SrcAuto;
EXTR xdata BYTE gPlayOperate;
EXTR xdata WORD g2SdQty;
EXTR xdata WORD g2UDiskQty;
EXTR xdata WORD g2PlayIndex;
EXTR xdata WORD g2TimeLength;
EXTR xdata WORD g2PlayTime;
EXTR xdata BYTE gPlayStatus;

EXTR xdata BYTE gDIP_MenuSelect;
EXTR xdata BYTE gDIP_MenuTimer;
EXTR xdata BYTE gDIP_MenuLock;
EXTR xdata BYTE gDIP_JogMenu;
EXTR BOOL FDIP_MenuTwinkle;
EXTR BOOL FSYS_Standby;
EXTR BOOL FSYS_MuteEnable;
EXTR BOOL FSYS_TestTone;
EXTR BOOL FDIP_FreqSymbol;
EXTR xdata BYTE gDIP_Brightness;
EXTR xdata INPUT_SWITCH mINPUT_SWITCH;
EXTR xdata BYTE gDIP_SoundEffect;                     
EXTR xdata BYTE gDIP_TestToneChannel;
EXTR xdata BYTE gDIP_Select2Ch;         // ѡ��Ϊ������
EXTR xdata BYTE gDIP_Surround[2];       // 0������ 1������
EXTR xdata BYTE gDIP_SpeakSetup[5];		// 0ǰ�� 1���� 2������ 3���� 4����
EXTR xdata BYTE gDIP_DelayTime[5];		// 0=LINSYNC 1ǰ�� 2���� 3���� 4����

EXTR xdata BYTE gDIP_TrimCtrl[8];	    // 0=FL 1=CN 2=SW 3=FR 4=SR 5=BR 6=BL 7=SL
EXTR xdata BYTE gDIP_MicCtrl[6];		// 0��Ͳ1���� 1��Ͳ2  2���� 3�ظ� 4�ӳ� 5����


EXTR BOOL FPowerCTRL;
EXTR BOOL FPKeyDetect;
EXTR BOOL FRmDecodDone;
EXTR BOOL FRmError;
EXTR BOOL FPKeyDecodeDone;
EXTR BOOL FPKeyStep;
EXTR BOOL FPKeyEnabel;
EXTR BOOL FSeekAuto;
EXTR BOOL FInitialize;
EXTR BOOL FAUD_PowerOffOn;  
EXTR BOOL FKey_ChildLock;  
EXTR BOOL FDIP_ScreenUpdata; 
EXTR BOOL FDIP_ScreenFill;  

EXTR xdata BYTE gDelay_111;

EXTR xdata BYTE g10Plus;
EXTR xdata WORD g2SelectNum;
EXTR xdata BYTE gSelectPos;

EXTR BOOL FPlayStates;
EXTR xdata WORD g2PlayTime;
EXTR xdata WORD g2SongTotal;
EXTR xdata WORD g2PlaySel;
EXTR xdata BYTE gPlayMode;
EXTR xdata BYTE gPlayState;

EXTR xdata BYTE gDIP_SpectMode;
EXTR xdata BYTE gDIP_Spectrum[5];
EXTR xdata BYTE gDIP_Buffer[32];
EXTR xdata WORD g2DIP_ShowBuffer[13]; 




void MUSDELAY(BYTE gLocal_1);                
BYTE MKEY_FirstPress(BYTE gLocal_0);
void MKCM_10msTimer(BYTE baseTimer);   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
void MPKey_Scan();											// ��尴��ɨ��

void MDEBUG(BYTE gLocal_1);

void MTestToneShow();
void MMenuSelect();
void MPowerCtrl();



void MAUD_VolumeSend();
void MAUD_10msTimer(BYTE baseTimer);   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
void MSUR_TEST_TONE();
void MDSP_INPUT();

BYTE MVol_Number(BYTE gLocal_0, BYTE gLocal_1,BOOL FLocal_1);
BYTE MVol_Number1(BYTE gLocal_0, BYTE gLocal_1,BOOL Focal_1);
void MAUDTrimNumber(BYTE gLocal_0, BYTE gLocal_1);
void MAUD_VolumeData(BYTE gLocal_2, BYTE gLocal_1);
void MDSP_WR_DELAY(BYTE gLocal_0, BYTE gLocal_1);

void MSYS_Initialize();
void MKCM_Initialize();
void MAUD_Initialize();
BYTE MEEP_ByteRead(BYTE gLocal_1);
void MFLASH_EraseSector(WORD g2Local_1);
#define MMEM_WriteByte(v1, v2) {}
void MFLASH_WriteByte(WORD g2Local_1, BYTE gLocal_1);
BYTE MFLASH_ReadByte(WORD g2Local_1);
void MI2C_Polling();

void MKEY_Initialize();
void MKEY_10msTimer(BYTE baseTimer);

void MAUD_10msTimer(BYTE baseTimer);   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
void MAUD_100msTimer();
void MAUD_500msTimer();

void MDIP_10msTimer(BYTE baseTimer);
void MDIP_CleanSpectrum();
void MDIP_ScreenFill(BYTE value);							// ���������ʾԪ�ص���ֵ��0x00Ϊȫ��Ϩ��
void MDIP_ScreenUpdata();    
void MDIP_WriteByte(BYTE gLocal_1);
void MDIP_TrimShow(BYTE gLocal_0,BYTE gLocal_1);

void MDIP_Initialize();    
void MDIP_WriteString(char* string);
void MPKey_Scan();
void MDIP_ScreenUpdata();    
void MDIP_SingleChar(BYTE gLocal_1, BYTE gLocal_2);
void MDIPSpectrum(						// Ƶ��
					BYTE gLocal_Style,	// ��ת  	
					BYTE gLocal_1,		// 0-5  	
					BYTE gLocal_2,		// ��ǰƵ��
					BYTE gLocal_3);		// ����Ƶ��	 

void MKEY_CheckJop();
void MAUD_MixInputSource(BYTE index);
void MAUD_MixSoundEffect();
void MAUD_MixMasterVolume(BYTE directUp);
void MAUD_MixTrimAdjust(BYTE menuMic, BYTE directUp);
void MAUD_MixMicAdjust(BYTE menuMic, BYTE directUp);
void MAUD_AutoCanclMute();
void MAUD_TestToneChannel(BYTE channel);					// ��������
void MAUD_AutoCanclTestTone();
void MAUD_InputSource(BYTE source);
void MAUD_SurroundMode(BYTE mode);
BYTE MAUD_GetInputSource(BYTE memory);

void MDIP_SourceFormat();
void MDIP_InputSource();
void MDIP_VideoSrc();
void MDIP_SurroundMode(BYTE index, MENU_MODE mode);
void MDIP_SurroundSymbol();
void MDIP_SrcFormatSymbol();
void MDIP_PlaySymbol(BYTE status);
void MDIP_WifiSymbol(BYTE turnOn);
void MDIP_SoundEffect(BYTE mode);
void MDIP_TestTone(BYTE index, MENU_MODE mode);             // ����΢����������
void MDIP_TrimControl(BYTE index, MENU_MODE mode);          // ����΢����������
void MDIP_MicControl(BYTE index, MENU_MODE mode);           // ��Ͳ���ֲ�������
void MDIP_SpeakSetup(BYTE index, BYTE mode);				// mode 0һ��ģʽ 1��˸���� 2��˸Ϩ�� 3����- 4����+  
void MDIP_DelayTime(BYTE index, BYTE mode);					// mode 0һ��ģʽ 1��˸���� 2��˸Ϩ�� 3����- 4����+  
BYTE MDIP_WriteDec(BYTE number, WORD value);                // ��ʾʮ��������������һ���ֿ�ʼ��λ��
void MDIP_Write2Digit(BYTE number, BYTE value);             // ��ʾ2������
void MDIP_WriteHex(BYTE number, BYTE value);                // ��ʾ�������Ƶ���
char MDIP_Hex2String(BYTE hex);                             // ����ʮ�����Ƶ�ֵ
void MDIP_ReadSpectrum();
void MDIP_ClearSpectrum();
void MDIP_NightMode();
void MDIP_NoiseSignal();
void MDIP_Fireware();
void MDIP_ExtrInOut(BYTE type);								// ��ʾ������Դ����/��� 
void MDIP_PlayTrack();
void MDIP_PlayTime();
BYTE MDIP_GetNextChannel(BYTE index);                       // ������������΢����ȡ��һ������

#define DIP_SURROUND_OFF()              {g2DIP_ShowBuffer[6] &= ~(0x0001 | 0x0002 | 0x0080 | 0x0040 | 0x0010);}
#define DIP_SRC_FORMAT_OFF()            {g2DIP_ShowBuffer[6] &= ~(0x0700 | 0x0004);}
#define DIP_DOT_OFF()                   {g2DIP_ShowBuffer[6] &= ~0xf000;}
#define DIP_PLAY_OFF()              	{g2DIP_ShowBuffer[7] &= ~(0x0001 | 0x0002 | 0x0004 | 0x0008);}

void MKCM_SetPowerOn(); 									// KCM����
void MKCM_RestoreMemory();									// ��������KCM֮�лָ�����
void MKCM_FactorySet();
void MKCM_ReadSrcValid();
void MAPI_COPY_BUFF8(BYTE length, BYTE* in_data, BYTE* out_data);
void MDIP_MenuSelect(BYTE index, MENU_MODE mode);			// mode 0һ��ģʽ 1��˸���� 2��˸Ϩ�� 3����- 4����+ 
BYTE MDIP_GetSpeakerChar(BYTE index);						// 0ǰ�� 1���� 2������ 3���� 4����
void MDIP_AdjDelayTime(BYTE index, BYTE mode);				// 0=LINSYNC 1ǰ�� 2���� 3���� 4���� 


BYTE MKCM_ToRegister(BYTE index, BYTE counter);				// �ӱ��������ֵ��ת����KCM�Ĵ�����ֵ
BYTE MKCM_FromRegister(BYTE index, BYTE value);				// ��KCM���ļĴ�����ת�������������ֵ

BYTE MKCM_PresetEQ(BYTE counter, BYTE tone);				// Ԥ��EQ

void MKEY_AudioMute();
void MKEY_TestTone();
void MKEY_VideoSelect();
void MDIP_Brightness(BYTE show, BYTE bright);
void MKCM_WifiCommand(BYTE regNumber, BYTE value);			// �յ�Զ��APP��ָ��

