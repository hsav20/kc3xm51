
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

// ！！！编译版本选择

//#define CUSTOM_CODE                     0x12123000          // 标准版本 使用PT2258音量IC
//#define CUSTOM_CODE                     0x12123018          // 数码输出 使用PT2258音量IC，字节1(B3:PT2258)字节0(B3:模拟从MIC输入)
//#define CUSTOM_CODE                     0x12120000          // 标准版本
#define CUSTOM_CODE                     0x44443000          // 旧音频板，中置超低音与环绕声 对调  使用PT2258音量IC

// ！！！以上设置请选择一个适合自己的编译，而且必须选择一个(只有一个没有//注释)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "c51tiny.h"
#include "Kc3xType.h"
#include "const.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I2C接口部分的定义 ///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 时钟及系统部分的定义 ////////////////////////////////////////////////////////////////////////////////////////
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
EXTR xdata BYTE gDIP_Select2Ch;         // 选择为立体声
EXTR xdata BYTE gDIP_Surround[2];       // 0立体声 1多声道
EXTR xdata BYTE gDIP_SpeakSetup[5];		// 0前置 1中置 2超低音 3环绕 4后置
EXTR xdata BYTE gDIP_DelayTime[5];		// 0=LINSYNC 1前置 2中置 3环绕 4后置

EXTR xdata BYTE gDIP_TrimCtrl[8];	    // 0=FL 1=CN 2=SW 3=FR 4=SR 5=BR 6=BL 7=SL
EXTR xdata BYTE gDIP_MicCtrl[6];		// 0话筒1音量 1话筒2  2回声 3重复 4延迟 5混响


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
void MPKey_Scan();											// 面板按键扫描

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
void MDIP_ScreenFill(BYTE value);							// 填充所有显示元素的数值，0x00为全部熄灭
void MDIP_ScreenUpdata();    
void MDIP_WriteByte(BYTE gLocal_1);
void MDIP_TrimShow(BYTE gLocal_0,BYTE gLocal_1);

void MDIP_Initialize();    
void MDIP_WriteString(char* string);
void MPKey_Scan();
void MDIP_ScreenUpdata();    
void MDIP_SingleChar(BYTE gLocal_1, BYTE gLocal_2);
void MDIPSpectrum(						// 频谱
					BYTE gLocal_Style,	// 倒转  	
					BYTE gLocal_1,		// 0-5  	
					BYTE gLocal_2,		// 当前频谱
					BYTE gLocal_3);		// 顶点频谱	 

void MKEY_CheckJop();
void MAUD_MixInputSource(BYTE index);
void MAUD_MixSoundEffect();
void MAUD_MixMasterVolume(BYTE directUp);
void MAUD_MixTrimAdjust(BYTE menuMic, BYTE directUp);
void MAUD_MixMicAdjust(BYTE menuMic, BYTE directUp);
void MAUD_AutoCanclMute();
void MAUD_TestToneChannel(BYTE channel);					// 噪音测试
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
void MDIP_TestTone(BYTE index, MENU_MODE mode);             // 声道微调参数调节
void MDIP_TrimControl(BYTE index, MENU_MODE mode);          // 声道微调参数调节
void MDIP_MicControl(BYTE index, MENU_MODE mode);           // 话筒各种参数调节
void MDIP_SpeakSetup(BYTE index, BYTE mode);				// mode 0一般模式 1闪烁点亮 2闪烁熄灭 3调整- 4调整+  
void MDIP_DelayTime(BYTE index, BYTE mode);					// mode 0一般模式 1闪烁点亮 2闪烁熄灭 3调整- 4调整+  
BYTE MDIP_WriteDec(BYTE number, WORD value);                // 显示十进制数，返回下一个字开始的位置
void MDIP_Write2Digit(BYTE number, BYTE value);             // 显示2个数字
void MDIP_WriteHex(BYTE number, BYTE value);                // 显示进二进制的数
char MDIP_Hex2String(BYTE hex);                             // 返回十六进制的值
void MDIP_ReadSpectrum();
void MDIP_ClearSpectrum();
void MDIP_NightMode();
void MDIP_NoiseSignal();
void MDIP_Fireware();
void MDIP_ExtrInOut(BYTE type);								// 显示外置音源插入/插出 
void MDIP_PlayTrack();
void MDIP_PlayTime();
BYTE MDIP_GetNextChannel(BYTE index);                       // 测试噪音声道微调获取下一个声道

#define DIP_SURROUND_OFF()              {g2DIP_ShowBuffer[6] &= ~(0x0001 | 0x0002 | 0x0080 | 0x0040 | 0x0010);}
#define DIP_SRC_FORMAT_OFF()            {g2DIP_ShowBuffer[6] &= ~(0x0700 | 0x0004);}
#define DIP_DOT_OFF()                   {g2DIP_ShowBuffer[6] &= ~0xf000;}
#define DIP_PLAY_OFF()              	{g2DIP_ShowBuffer[7] &= ~(0x0001 | 0x0002 | 0x0004 | 0x0008);}

void MKCM_SetPowerOn(); 									// KCM开机
void MKCM_RestoreMemory();									// 开机，从KCM之中恢复记忆
void MKCM_FactorySet();
void MKCM_ReadSrcValid();
void MAPI_COPY_BUFF8(BYTE length, BYTE* in_data, BYTE* out_data);
void MDIP_MenuSelect(BYTE index, MENU_MODE mode);			// mode 0一般模式 1闪烁点亮 2闪烁熄灭 3调整- 4调整+ 
BYTE MDIP_GetSpeakerChar(BYTE index);						// 0前置 1中置 2超低音 3环绕 4后置
void MDIP_AdjDelayTime(BYTE index, BYTE mode);				// 0=LINSYNC 1前置 2中置 3环绕 4后置 


BYTE MKCM_ToRegister(BYTE index, BYTE counter);				// 从本机处理的值，转换到KCM寄存器的值
BYTE MKCM_FromRegister(BYTE index, BYTE value);				// 从KCM来的寄存器，转换到本机处理的值

BYTE MKCM_PresetEQ(BYTE counter, BYTE tone);				// 预设EQ

void MKEY_AudioMute();
void MKEY_TestTone();
void MKEY_VideoSelect();
void MDIP_Brightness(BYTE show, BYTE bright);
void MKCM_WifiCommand(BYTE regNumber, BYTE value);			// 收到远程APP的指令

