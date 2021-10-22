
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           

void MDIP_BaseInit(){										// 显示模块基础层初始化								  
	MDIP_Brightness(0, 0);
	MDIP_ScreenFill(0x00);
	gSYS_WifiCommand = 0;
}	  
void MDIP_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
    if (FDIP_ScreenUpdata){
    	FDIP_ScreenUpdata = 0;
    	MDIP_ScreenUpdata();
	}

	if ((baseTimer & 0x02) > 0){ 							// 100ms
		if (gSYS_WifiCommand){
			if (--gSYS_WifiCommand == 0){
				MDIP_WifiSymbol(1);	
			}
			else {
				MDIP_WifiSymbol(gSYS_WifiCommand & 0x01);	
			}
		}
		
		if (gDIP_MenuLock > 0){								// 暂时锁定显示
			gDIP_MenuLock = 0;								// 暂时在这里解除锁定
			return;
		}
		if (FSYS_MuteEnable && (gDIP_MenuTimer == 0)){
			MDIP_MenuNormal(MENU_AUDIO_MUTE);
		}
		if (!FSYS_Standby && gDIP_MenuTimer){				// 100ms如果菜单打开
			if (--gDIP_MenuTimer == 0) {					// 菜单计时	
            	if (!FSYS_TestTone){
					MDIP_MenuNormal(MENU_RESTORE);
				}
				else {                                      // 测试噪音打开之中
					BYTE channel = MDIP_GetNextChannel(gDIP_MenuSelect - MENU_TESTTONE_FL);
					MDIP_MenuNormal(channel + MENU_TESTTONE_FL);
                    MAUD_TestToneChannel(channel);
//MDEBUG(0xc7);MDEBUG(gDIP_MenuSelect);MDEBUG(gDIP_MenuTimer);
				}
			}
			else if (FDIP_MenuTwinkle){						// 菜单需要闪烁
				if ((gDIP_MenuTimer % 5) == 0){	  			// 每0.5秒钟闪烁
					BYTE twinkle = (gDIP_MenuTimer / 5);
					MDIP_MenuSelect(gDIP_MenuSelect, (twinkle & 0x01) ? MENU_SET_TWINKLE_OFF : MENU_SET_TWINKLE_ON);
				}
			}
        }
		if (!FSYS_Standby && !FSYS_MuteEnable && !FSYS_TestTone && FDIP_FreqSymbol && ((gAUD_SrcFormat & 0x0f) >= KCM_SRC_NOS)){
#ifdef SPECTRUM_ENABLE
            MDIP_ReadSpectrum();
#endif			
		}else {
//		MLOG("FSYS_TestTone:%d %d", FSYS_TestTone, FSYS_TestTone);
		}
	}
    return;
}
void MDIP_MenuNormal(BYTE index){							// 菜单选择一般模式 
	MDIP_MenuSelect(index, MENU_SET_NORMAL);				// 菜单选择
}
void MDIP_SetNormal(){                                      // 设置菜单一般模式显示的时间
	gDIP_MenuTimer = 20;
	FDIP_MenuTwinkle = 0;
}
void MDIP_SetState(MENU_STATE state){                       // 设置菜单状态
    gDIP_MenuSelect = state;
    MDIP_SetNormal();
}
void MDIP_CleanSymbol(){									// 统一清除屏幕及符号
	FDIP_FreqSymbol = 0;
    HAL_DIP_LED(0);                                         // 熄灭显示屏固定的符号
	MDIP_ClearSpectrum();
    DIP_DOT_OFF();
}

void MDIP_MenuSelect(BYTE index, MENU_SET mode){			// 菜单选择高级模式，mode 0一般模式 1闪烁点亮 2闪烁熄灭 3调整- 4调整+ 
	if (mode == MENU_SET_NORMAL){							// 正常的显示模式
		MDIP_SetNormal();
		//gDIP_MenuTimer = 20;
		//FDIP_MenuTwinkle = 0;
	}
	MDIP_CleanSymbol();
	switch (index){
	case MENU_RESTORE:                                      // 菜单恢复状态
		if (!FKCM_I2C_Error){
			MDIP_InputSource();
		}
		else {
			MDIP_WrString("KCM-NO");
		}
		gDIP_MenuSelect = 0;
		gDIP_MenuTimer = 0;
		FDIP_FreqSymbol = 1;
		break;
	case MENU_INPUT_SOURCE:
		MDIP_InputSource();
		break;
	case MENU_STANDBY:
		MDIP_WrString(" -OFF-");
		break;
	case MENU_POWER_ON:                                     // 菜单状态:电源打开
		MDIP_WrString(" HELLO");
		MDIP_ScreenUpdata();
		gDIP_MenuTimer = 10;
		break;
	case MENU_BRIGHTNESS:
		if (mode == 4){
			gDIP_MenuTimer = 20;
			if (++gDIP_Brightness > 2){
				gDIP_Brightness = 0;
			}
			MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_BRIGHTNESS, gDIP_Brightness);
		}
		MDIP_Brightness(1, gDIP_Brightness);
		break;

	case MENU_VIDEO_SRC:
		gDIP_MenuTimer = 50;
		MDIP_VideoSrc();
		break;
	case MENU_SRC_FORMAT:									// 菜单显示输入码流格式
		MDIP_SourceFormat();
		break;
	case MENU_NIGHT_MODE:
		MDIP_NightMode();
		break;
	case MENU_TEST_TONE:
		MDIP_NoiseSignal();
		break;

    case MENU_SD_INSERT:                                   // 显示SD插入
    case MENU_UD_INSERT:                                // 显示U盘插入
    case MENU_SD_REMOVE:                                   // 显示SD拔出 
    case MENU_UD_REMOVE:                                // 显示U盘拔出
    case MENU_PC_REMOVE:               	               // 显示USB声卡拔出
    case MENU_HDMI_REMOVE:                                 // 显示HDMI拔出 
    case MENU_BT_REMOVE:               	                 	// 显示蓝牙音频拔出
		MDIP_InsertRemove(index - MENU_SD_INSERT);			// 显示外置音源插入/拔出
		break;
    case MENU_PLAY_TRACK:                                  // 显示多媒体文件信息
        MDIP_PlayTrack();
		break;


	case MENU_AUDIO_MUTE:
		if (mode == 0 || mode == 4){ 						 // 0一般模式 或者 4静音有效
			gDIP_MenuTimer = 80;
			FDIP_MenuTwinkle = 1;
		}
		if (mode < 2 || mode == 4){
			MDIP_WrString("MUTE  ");
		}	
		else if (mode == 2){
			MDIP_WrString("MUTE--");
		}
		if (mode == 4){										// 静音有效
			MKCM_WriteRegister(KCM_VOLUME_MUTE, 0x03);      // B1为控制音频的静音 B0=1静音打开
		}
		else if (mode == 3){								// 取消静音
			gDIP_MenuTimer = 20;
			MKCM_WriteRegister(KCM_VOLUME_MUTE, 0x02);      // B1为控制音频的静音 B0=1静音打开 
		}
//MDEBUG(0xa9);MDEBUG(mode); MDEBUG(FSYS_MuteEnable);
		break;
	case MENU_MASTER_VOL:
		if (mode >= 3){										// 3调整- 4调整+
			if (gDIP_MenuSelect == MENU_MASTER_VOL){
				MAUD_MixMasterVolume((mode == 4) ? 1 : 0); 	
			}
		}
		if (mode == 2){										// 2闪烁熄灭
			MDIP_WrString("VOL   ");
		}
		else {
			MDIP_WrString("VOL ");
			MDIP_Write2Digit(4, gAUD_MasterVolume);
			if (mode == 0){
				gDIP_MenuTimer = 80;
				FDIP_MenuTwinkle = 1;
			}
		}
		break;

	case MENU_TESTTONE_FL:                                     // 测试噪音前置左声道
	case MENU_TESTTONE_CE:                                     // 测试噪音中置声道
	case MENU_TESTTONE_SW:                                     // 测试噪音超低音声道
	case MENU_TESTTONE_FR:                                     // 测试噪音前置右声道
	case MENU_TESTTONE_SR:                                     // 测试噪音环绕右声道
	case MENU_TESTTONE_BR:                                     // 测试噪音后置右声道
	case MENU_TESTTONE_BL:                                     // 测试噪音后置左声道
	case MENU_TESTTONE_SL:                                     // 测试噪音环绕左声道
//MDEBUG(0xb2);MDEBUG(index);
        MDIP_TestTone(index, mode);                         // 声道微调参数调节
        break;
	case MENU_CH_TRIM_FL:                                    // 声道微调前置左声道
	case MENU_CH_TRIM_CE:                                    // 声道微调中置声道
	case MENU_CH_TRIM_SW:                                    // 声道微调超低音声道
	case MENU_CH_TRIM_FR:                                    // 声道微调前置右声道
	case MENU_CH_TRIM_SR:                                    // 声道微调环绕右声道
	case MENU_CH_TRIM_BR:                                    // 声道微调后置右声道
	case MENU_CH_TRIM_BL:                                    // 声道微调后置左声道
	case MENU_CH_TRIM_SL:                                    // 声道微调环绕左声道
        MAUD_AutoCanclTestTone();
		MDIP_TrimControl(index, mode);                      // 声道微调参数调节
		break;

	case MENU_MIC_VOL1:                                  	// 话筒1音量
    case MENU_MIC_VOL2:                                  	// 话筒2音量
    case MENU_MIC_ECHO:                                     // 话筒回声比例
    case MENU_MIC_REPEAT:                                   // 话筒重复比例
    case MENU_MIC_DELAY:                                    // 话筒延迟时间
    case MENU_MIC_BASS:                                   	// 话筒音调低音调节
    case MENU_MIC_TREBLE:                                   // 话筒音调高音调节
        MAUD_AutoCanclTestTone();
		MEQMIC_MicDisplay(index, mode);                     // 显示话筒各种参数调节
		break;
	case MENU_SPEAK_FRONT:
	case MENU_SPEAK_CENTER:
	case MENU_SPEAK_WOOFER:
	case MENU_SPEAK_SURR:
	case MENU_SPEAK_BACK:
        MAUD_AutoCanclTestTone();
		MDIP_SpeakSetup(index, mode);
		break;
	case MENU_LIP_SYNC:
	case MENU_DELAY_FRONT:
	case MENU_DELAY_CENTER:
	case MENU_DELAY_SURR:
	case MENU_DELAY_BACK:
        MAUD_AutoCanclTestTone();
		MDIP_DelayTime(index, mode);
		break;
	default: MDIP_MenuCustom(index, mode); break;	
	}
	gDIP_MenuSelect = index;
}
void MDIP_NoiseSignal(){
	BYTE gLocal_1 = MKCM_ReadRegister(KCM_TEST_TONE);
	if (FSYS_TestTone){									// 已经打开了噪音测试
		if (gDIP_MenuSelect == MENU_TEST_TONE){	
			if (++gLocal_1 > 7){
				gLocal_1 = 0;
			}
		}
	}
	MKCM_WriteRegister(KCM_TEST_TONE, gLocal_1); 
	MDIP_WrString("NOISE ");
	MDIP_SingleChar(5, gLocal_1 + '1');
	if (FSYS_TestTone){									// 已经打开了噪音测试
		gDIP_TestToneChannel = 0;
		MAUD_TestToneChannel(gDIP_TestToneChannel);
	}
}
BYTE MDIP_GetNextChannel(BYTE index){                       // 测试噪音声道微调获取下一个声道
    switch (index){						                    // 0前左 1中置 2超低音 3前右 4环右 5后右 6后左 7环左
	case 0 : 											    // 前左时
		if (gDIP_SpeakSetup[1]){						    // 有中置
			return 1;                                       // 下一个中置
		}
		if (gDIP_SpeakSetup[2]){						    // 有超低音
			return 2;                                       // 下一个超低音
		}
		return 3;						                    // 没有中置及超低音 直接跳到前右
	case 1 : 											    // 中置时
		if (gDIP_SpeakSetup[2]){						    // 有超低音
			return 2;                                       // 下一个超低音
		}
		return 3;						                    // 没有超低音 直接跳到前右
	case 2 : 											    // 超低音时
		return 3;						                    // 直接跳到前右
	case 3 : 											    // 前右时
		if (gDIP_SpeakSetup[3]){						    // 有环绕
			return 4;                                       // 环绕右声道
		}
        if (gDIP_SpeakSetup[4]){						    // 有后置声道
			return 5;                                       // 后置声道
		}
		return 0;						                    // 没有环绕及后置，直接跳到前左
	case 4 : 											    // 环右时
		if (gDIP_SpeakSetup[4]){						    // 有后置声道
			return 5;                                       // 后置声道
		}
		return 0;						                    // 没有后置，直接跳到前左
	case 5 : 											    // 后右时
		return 6;						                    // 直接跳到后左
	case 6 : 											    // 后右左
		return 7;						                    // 直接跳到后左
	default : 											    // 左右时
		return 0;						                    // 直接跳到后左
	}
}

CONST_CHAR Tab_DIP_TToneShow[] = {
	"FLTtCETtSWTtFRTtSRTtBRTtBLTtSLTt"
//	 ++++----++++----++++----++++----
};

void MDIP_TestTone(BYTE index, MENU_SET mode){             // 声道微调参数调节
	if (mode >= MENU_SET_ADJ_DOWN){							    // MENU_SET_ADJ_DOWN或MENU_SET_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // 上次已经进入当前菜单
		    MAUD_MixTrimAdjust(index - MENU_TESTTONE_FL, (mode == MENU_SET_ADJ_UP) ? 1 : 0); 	
			gDIP_MenuTimer = 80;
		}
	}
    index -= MENU_TESTTONE_FL;                             // 从声道微调前置左声道开始
    MDIP_WrString((char*)&Tab_DIP_TToneShow[index * 4]);
	if (mode == MENU_SET_TWINKLE_OFF){							// MENU_SET_TWINKLE_OFF闪烁熄灭
		MDIP_SingleChar(4, ' ');
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_SET_NORMAL或MENU_SET_TWINKLE_ON闪烁点亮
		MDIP_ShowNegative9(gDIP_TrimCtrl[index]);			// 在显示最后两个字的位置显示-9到+9的数值
/*
    	BYTE value = gDIP_TrimCtrl[index];
//if(mode!=MENU_SET_TWINKLE_ON&&mode!=MENU_SET_TWINKLE_OFF){MDEBUG(0xee);MDEBUG(index);MDEBUG(value);}
        if (value < 9){
            MDIP_SingleChar(4, '-');
            MDIP_SingleChar(5, (9 - value) + '0');
        }else {
            MDIP_SingleChar(4, ' ');
            MDIP_SingleChar(5, (value - 9) + '0');
        }
		*/
		FDIP_MenuTwinkle = 1;
	}
}

CONST_CHAR Tab_DIP_TrimShow[] = {
	"FLTrCETrSWTrFRTrSRTrBRTrBLTrSLTr"
//	 ++++----++++----++++----++++----
};
void MDIP_TrimControl(BYTE index, MENU_SET mode){          // 声道微调参数调节
	if (mode >= MENU_SET_ADJ_DOWN){							    // MENU_SET_ADJ_DOWN或MENU_SET_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // 上次已经进入当前菜单
            MAUD_MixTrimAdjust(index - MENU_CH_TRIM_FL, (mode == MENU_SET_ADJ_UP) ? 1 : 0); 	
			gDIP_MenuTimer = 80;
		}
	}
    index -= MENU_CH_TRIM_FL;                            // 从声道微调前置左声道开始
    MDIP_WrString((char*)&Tab_DIP_TrimShow[index * 4]);
	if (mode == MENU_SET_TWINKLE_OFF){							// MENU_SET_TWINKLE_OFF闪烁熄灭
		MDIP_SingleChar(4, ' ');
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_SET_NORMAL或MENU_SET_TWINKLE_ON闪烁点亮
    	BYTE value = gDIP_TrimCtrl[index];
//if(mode!=MENU_SET_TWINKLE_ON&&mode!=MENU_SET_TWINKLE_OFF){MDEBUG(0xee);MDEBUG(index);MDEBUG(value);}
        if (value < 9){
            MDIP_SingleChar(4, '-');
            MDIP_SingleChar(5, (9 - value) + '0');
        }else {
            MDIP_SingleChar(4, ' ');
            MDIP_SingleChar(5, (value - 9) + '0');
        }
		if (mode == MENU_SET_NORMAL){                           // 正常的显示模式
			gDIP_MenuTimer = 80;
			FDIP_MenuTwinkle = 1;
		}
       
	}
}

void MDIP_SpeakSetup(BYTE index, BYTE mode){				// mode 0一般模式 1闪烁点亮 2闪烁熄灭 3调整- 4调整+  
	BYTE gLocal_1;
	g2DIP_ShowBuffer[6] |= 0x5000;	
	gDIP_MenuTimer = 80;
	MDIP_SingleChar(4, ' ');
	MDIP_SingleChar(5, ' ');
	switch (index){
	case MENU_SPEAK_FRONT :
		MDIP_WrString("FRSP ");
		if (mode >= 3){
			if (++gDIP_SpeakSetup[0] > 1){
				gDIP_SpeakSetup[0] = 0;
			}
			MKCM_WriteRegister(KCM_SPK_CONFIG, MKCM_ToRegister(KCM_SPK_CONFIG, 0));
		}
		MDIP_SingleChar(5, MDIP_GetSpeakerChar(0));
		break;
	case MENU_SPEAK_CENTER :
		MDIP_WrString("CESP ");
		if (mode >= 3){
			if (++gDIP_SpeakSetup[1] > 2){
				gDIP_SpeakSetup[1] = 0;
			}
			MKCM_WriteRegister(KCM_SPK_CONFIG, MKCM_ToRegister(KCM_SPK_CONFIG, 0));
		}
		MDIP_SingleChar(5, MDIP_GetSpeakerChar(1));
		break;
	case MENU_SPEAK_WOOFER :
		MDIP_WrString("SWSP ");
		if (mode >= 3){
			if (++gDIP_SpeakSetup[2] > 1){
				gDIP_SpeakSetup[2] = 0;
			}
			MKCM_WriteRegister(KCM_SPK_CONFIG, MKCM_ToRegister(KCM_SPK_CONFIG, 0));
		}
		MDIP_SingleChar(5, MDIP_GetSpeakerChar(2));
		break;
	case MENU_SPEAK_SURR :
		MDIP_WrString("SRSP ");
		if (mode >= 3){
			if (++gDIP_SpeakSetup[3] > 2){
				gDIP_SpeakSetup[3] = 0;
			}
			MKCM_WriteRegister(KCM_SPK_CONFIG, MKCM_ToRegister(KCM_SPK_CONFIG, 0));
		}
		MDIP_SingleChar(5, MDIP_GetSpeakerChar(3));
		break;
	case MENU_SPEAK_BACK :
		MDIP_WrString("BKSP ");
		if (mode >= 3){
			if (++gDIP_SpeakSetup[4] > 2){
				gDIP_SpeakSetup[4] = 0;
			}
			MKCM_WriteRegister(KCM_SPK_CONFIG, MKCM_ToRegister(KCM_SPK_CONFIG, 0));
		}
		MDIP_SingleChar(5, MDIP_GetSpeakerChar(4));
		break;
	case MENU_SPEAK_FILTER :
		MDIP_WrString("LPF ");
		if (mode >= 3){
		}
		break;
	}
}
void MDIP_DelayTime(BYTE index, BYTE mode){					// mode 0一般模式 1闪烁点亮 2闪烁熄灭 3调整- 4调整+  
	switch (index){
	case MENU_LIP_SYNC :
		g2DIP_ShowBuffer[6] |= 0x4000;	
		MDIP_WrString("LIPS");
		if (mode >= 3){
			MDIP_AdjDelayTime(0, mode);
		}
		break;
	case MENU_DELAY_FRONT :
		MDIP_WrString("FRDL ");
		g2DIP_ShowBuffer[6] |= 0x5000;	
		if (mode >= 3){
			MDIP_AdjDelayTime(1, mode);
		}
		break;
	case MENU_DELAY_CENTER :
		MDIP_WrString("CEDL ");
		g2DIP_ShowBuffer[6] |= 0x5000;	
		if (mode >= 3){
			MDIP_AdjDelayTime(2, mode);
		}
		break;
	case MENU_DELAY_SURR :
		MDIP_WrString("SWDL ");
		g2DIP_ShowBuffer[6] |= 0x5000;	
		if (mode >= 3){
			MDIP_AdjDelayTime(3, mode);
		}
		break;
	case MENU_DELAY_BACK :
		MDIP_WrString("BKDL ");
		g2DIP_ShowBuffer[6] |= 0x5000;	
		if (mode >= 3){
			MDIP_AdjDelayTime(4, mode);
		}
		break;
	}
	gDIP_MenuTimer = 50;
//MDEBUG(0xc8);MDEBUG(index - MENU_LIP_SYNC);
	MDIP_Write2Digit(4, gDIP_DelayTime[index - MENU_LIP_SYNC] * 5);
}


CONST_CHAR Tab_DIP_InputSwitch[] = {
	"AUX-INOPTICACOA-D1COA-D2 -SD-  -USB- -PC-  E8CH -BT-  HDMI-1HDMI-2HDMI-3HDMI-A"
//	 ++++++------++++++------++++++------++++++------++++++------++++++------++++++
};
CONST_CHAR Tab_DIP_Bps[] = {
    "32K48K56K64K80K96K112128160192224256320384"
//	 +++---+++---+++---+++---+++---+++---+++---
	
};
void MDIP_InputSource(){									// 音源输入显示
#ifdef CUSTOM_DIP_INPUT
	MDIP_CustomInput();										// 用户定制音源输入显示
	return;
#endif
	g2DIP_ShowBuffer[6] &= ~(0x4000|0x0020);				// 熄灭云符号及:
	if (mINPUT_SWITCH >= INPUT_SWITCH_SD && mINPUT_SWITCH <= INPUT_SWITCH_BT){
		g2DIP_ShowBuffer[6] |= 0x0020;						// 点亮云符号
		if (mINPUT_SWITCH == INPUT_SWITCH_SD || mINPUT_SWITCH == INPUT_SWITCH_UDISK){
			if (g2PlayTime > 0){
				if (g2PlayTime < 3){
					MDIP_PlayTrack();
					return;
				}
				if (g2PlayTime < 5){
					 BYTE temp = gAUD_SrcFreq >> 3;
					if ((gAUD_SrcFormat & 0x0f) == KCM_SRC_MP3){
						gDIP_MenuTimer = 20;
						if (temp > 0 && temp < 16){
							BYTE gLocal_Buffer[6];
							MAPI_COPY_BUFF8(3, "MP3", gLocal_Buffer);
							MAPI_COPY_BUFF8(3, &Tab_DIP_Bps[(temp-1) * 3], &gLocal_Buffer[3]);
							MDIP_WrString((char*)gLocal_Buffer);
							return;
						}
					}
				}
				MDIP_WrString("  ");
				MDIP_Write2Digit(2, g2PlayTime/60);
				MDIP_Write2Digit(4, g2PlayTime%60);
				g2DIP_ShowBuffer[6] |= 0x4000;                          // 点亮:
				return;
			}
		}
	}
// MLOG("Source: %02x %02x", mINPUT_SWITCH, g2DIP_ShowBuffer[6]);
	MDIP_WrString((char*)&Tab_DIP_InputSwitch[mINPUT_SWITCH * 6]);
}

void MDIP_PlayTrack(){
    BYTE temp;
	WORD index;

	index = MKCM_Read2Byte(KCM_PLAY_INDEX) + 1;
    g2DIP_ShowBuffer[6] &= ~0x4000;                     // 熄灭符号:
    MDIP_WrString("      ");
    temp = MDIP_WriteDec(0, index);
    MDIP_SingleChar(temp++, '/');
    MDIP_WriteDec(temp, (mINPUT_SWITCH == INPUT_SWITCH_SD) ? g2SdQty : g2UDiskQty);
	// MLOG("MDIP_PlayTrack %d %d ", index, g2UDiskQty);
}

void MDIP_PlaySkip(BYTE operate){
	g2PlayTime = 0;
	gAUD_SrcFreq = 0;
	MDIP_InputSource();		
	MKCM_WriteRegister(KCM_PLAY_OPERATE, operate);  // 多媒体播放后一首
	MLOG("MDIP_PlaySkip A %d", operate);
}
void MDIP_PlayTime(){
}


void MDIP_VideoSrc(){
//	MDIP_WrString("VIDE0 ");
//	MDIP_SingleChar(5, (gAUD_AutoInputSrc >> 5) + '1');
}


CONST_CHAR Tab_DIP_Surround[] = {
	"  HIFI 2+1CHMODE  "
//	 ++++++------++++++
};

LISTEN_MODE_STATE GetListenModeIndex(BYTE value){
    switch (value & 0x30){                                  // B5:4为聆听模式类型选择
    case 0x00: return (value & 1) ? LISTEN_MODE_2_1CH : LISTEN_MODE_HIFI;   // 选择为双声道立体声，B0为0关闭超低音；为1打开超低音
    case 0x10: return LISTEN_MODE_SURROUND1;                // 选择为多声道源码模式，没有任何多声道算法
    case 0x20: return (value & 1) ? LISTEN_MODE_SURROUND3 : LISTEN_MODE_SURROUND2;  // 选择多声道模式，B1:0为各种不同算法的多声道模式
    }
    return LISTEN_MODE_SURROUND4;                           // 选择多声道音效，B1:0为各种不同算法的多声道音效
}
void MDIP_ListenMode(BYTE value){                           // 显示聆听模式
	LISTEN_MODE_STATE state = GetListenModeIndex(value);
    MDIP_CleanSymbol();                                     // 统一清除屏幕及符号
    DIP_SURROUND_OFF();
    if (state < LISTEN_MODE_SURROUND1){
        MDIP_WrString((char*)&Tab_DIP_Surround[state * 6]);
		g2DIP_ShowBuffer[6] |= 0x0001;	
    }else {
        MDIP_WrString((char*)&Tab_DIP_Surround[LISTEN_MODE_SURROUND1 * 6]);
        MDIP_SingleChar(5, (state-LISTEN_MODE_SURROUND1) + '1');
    	switch (value){
    	case LISTEN_MODE_SURROUND1 :
    		g2DIP_ShowBuffer[6] |= 0x0002;	
    		break;
    	case LISTEN_MODE_SURROUND2 :
    		g2DIP_ShowBuffer[6] |= 0x0080;	
    		break;
    	case LISTEN_MODE_SURROUND3 :
    		g2DIP_ShowBuffer[6] |= 0x0040;	
    		break;
    	case LISTEN_MODE_SURROUND4 :
    		g2DIP_ShowBuffer[6] |= 0x0010;	
    		break;
    	}
    }
    MDIP_SetNormal();
    MDIP_SetState(MENU_LISTEN_MODE);
}
CONST_CHAR Tab_DIP_SRC_CH[] = {
	"2/01/02/03/02/13/12/23/23/33/42/32/4"
//	 +++---+++---+++---+++---+++---+++---
};
void MDIP_SourceFormat(){
	if ((gAUD_SrcFormat & 0x0f) >= KCM_SRC_PCM){
    	BYTE gLocal_1;
    	BYTE gLocal_Buffer[4];

    	gLocal_Buffer[3] = 0;
    	gLocal_1 = (gAUD_SrcFormat >> 4) * 3;
    	MDIP_WrString("      ");
	
    //MDEBUG(0xa8);MDEBUG(gAUD_SrcFormat);	
    	MDIP_SrcFormatSymbol();
    	switch (gAUD_SrcFormat & 0x0f){
    	case KCM_SRC_PCM :
    		MDIP_WrString("PCM");
    		break;
    	case KCM_SRC_AC3 :                                      // 输入AC-3信号
    	case KCM_SRC_E_AC3 :                                    // 输入Enhanced AC-3信号
    	case KCM_SRC_AC3_HD :                                   // 输入杜比TRUE HD信号    
    		MAPI_COPY_BUFF8(3, &Tab_DIP_SRC_CH[gLocal_1], gLocal_Buffer);
    		MDIP_WrString(gLocal_Buffer);
    		break;
    	case KCM_SRC_DTS :                                      // 输入DTS信号
        case KCM_SRC_DTS_CD:				                    // 输入DTS CD信号
        case KCM_SRC_DTS_ES:				                    // 输入DTS Extended Surround信号
        case KCM_SRC_DTS_HRA:				                    // 输入DTS HD High Resolution Audio信号
        case KCM_SRC_DTS_MA:				                    // 输入DTS HD Master Audio信号
    		MAPI_COPY_BUFF8(3, &Tab_DIP_SRC_CH[gLocal_1], gLocal_Buffer);
    		MDIP_WrString(gLocal_Buffer);
    		break;
    	case KCM_SRC_LPCM :
    		MDIP_WrString("2/4");
    		break;
    	}
	}else {
        MDIP_InputSource();    
    }
}
BYTE MDIP_GetSpeakerChar(BYTE index){						// 0前置 1中置 2超低音 3环绕 4后置
	if (index == 0){										// 前置
		return gDIP_SpeakSetup[0] ? 'L' : 'S';				// 0=小喇叭 1=大喇叭
	}
	if (index == 2){										// 超低音
		return gDIP_SpeakSetup[2] ? 'Y' : 'N';				// 0=没有 1=有
	}
	if (gDIP_SpeakSetup[index] == 2){						// 大喇叭
		return 'L';											
	}
	if (gDIP_SpeakSetup[index] == 1){						// 小喇叭
		return 'S';											
	}
	return 'N';												// 没有安装
}



void MDIP_NightMode(){
	BYTE gLocal_1 = MKCM_ReadRegister(KCM_DYN_COMPRES);

	if (gDIP_MenuSelect == MENU_NIGHT_MODE){	
		if (gLocal_1 < 50){                                 
            gLocal_1 = 50;                                  // 50%
        }else {
			gLocal_1 = 0;                                   // 0%
		}

		MKCM_WriteRegister(KCM_DYN_COMPRES, gLocal_1); 
	}
	if (gLocal_1){
		MDIP_WrString("NIG-ON");
	}
	else {
		MDIP_WrString("NIG-OF");
	}
}
CONST_CHAR Tab_DIP_ExtrInOut[] = {
	"SD-IN UD-IN SD-OUTUD-OUTPC-OUTBT-OUTHD-OUT"
//	 ++++++------++++++------++++++------++++++
};

void MDIP_InsertRemove(BYTE type){						// 显示外置音源插入/插出 
	gDIP_MenuTimer = 50;
//MDEBUG(0xd9);MDEBUG(type);
    MDIP_WrString((char*)&Tab_DIP_ExtrInOut[type * 6]);
}




void MDIP_AdjDelayTime(BYTE index, BYTE mode){				// 0=LINSYNC 1前置 2中置 3环绕 4后置 
	BYTE gLocal_1;
	switch (index == 0){
	case 0: gLocal_1 = 19; break;
	case 1: gLocal_1 = 9; break;
	case 2: gLocal_1 = 9; break;
	case 3: gLocal_1 = 9; break;
	case 4: gLocal_1 = 9; break;
	}
	if (mode == 4){	   											// mode 0一般模式 1闪烁点亮 2闪烁熄灭 3调整- 4调整+
		if (gDIP_DelayTime[index] > gLocal_1){
			gDIP_DelayTime[index] = gLocal_1;
		}
		if (gDIP_DelayTime[index] < gLocal_1){
			++gDIP_DelayTime[index];
		}
	}
	else if (mode == 3){
		if (gDIP_DelayTime[index] > 0){
			--gDIP_DelayTime[index];
		}
	}
//MDEBUG(0xc7);MDEBUG(index);MDEBUG(gDIP_DelayTime[index]);
}
/*
void MDIP_Fireware(){
	gDIP_MenuTimer = 50;
    MDIP_WrString("FWDONE");
}
  */


void MDIP_FirewareInfo(){				                    // 显示固件更新
    BYTE tmpData[10];
    MKCM_ReadXByte(KCM_RD_INFO, tmpData, 2);
    if (tmpData[0] == 100){
        MDIP_WrString("FWDONE");
    }else if (tmpData[0] == 104){
        MDIP_WrString("FW Err");
    }else {
        MDIP_WrString("FW -");
        MDIP_Write2Digit(4, tmpData[0]);
    }
    MDIP_SetState(MENU_FIREWARE_INFO);
    gDIP_MenuTimer = 60;
}
void MDIP_ShowVolume(){										// 在4及5的位置显示音量的值
	MDIP_Write2Digit(4, gAUD_MasterVolume);
}
void MDIP_ShowNegative9(BYTE value){						// 在显示最后两个字的位置显示-9到+9的数值
	if (value < 9){											// 负数
		MDIP_SingleChar(4, '-');
		MDIP_SingleChar(5, (9 - value) + '0');
	}else {													// 0或正数
		MDIP_SingleChar(4, ' ');
		MDIP_SingleChar(5, (value - 9) + '0');
	}
}
void MDIP_SetUpDown(BYTE directUp, BYTE upMax, BYTE* value){	// 显示加减调节
    if (directUp){                                          // 加数值
        if (*value < upMax){               					// 还没有到达最大值
            *value += 1;									// 可以继续加
		}
    }else {													// 减数值
        if (*value > 0){									// 还没有到达最小值
            *value -= 1;									// 可以继续减
        }
    }

} 