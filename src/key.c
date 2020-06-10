
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           


void MKEY_Initialize(){										  
}	  
void MKEY_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
	MPKey_Scan();
	if (gDIP_MenuLock > 0){
		if (!FSYS_Standby){									// �����ڴ���ģʽ
			if ((baseTimer & 0x02) > 0){					// 100msʱ��
				--gDIP_MenuLock;
			}
		}
		return;	
	}
	if (FSYS_Standby){
		if (FRmDecodDone){
			FRmDecodDone = 0;
			if (GRmCodeData == cRmKey_Standby){
				MKCM_SetPowerOn();						// KCM����
			}
		}
	   	if (FPKeyDecodeDone){
			FPKeyDecodeDone = 0;
//			FDirectUp = 0;
			FPKeyStep = 1;
			if (GPKeyData == cPanKey_InputSource){
				MKCM_SetPowerOn();						// KCM����
			}
		}
		return;
	}
   	if (FPKeyDecodeDone){
		FPKeyDecodeDone = 0;
//		FDirectUp = 0;
		FPKeyStep = 1;
//MDEBUG(0xaa);MDEBUG(GPKeyData);

		switch (GPKeyData){
		case cPanKey_InputSource:
			MAUD_MixInputSource(INPUT_SWITCH_NONE);         // ѭ����ʽ
			break;
		case cPanKey_Stereo:
			MDIP_MenuSelect(cMenu_Surround2Ch, MENU_NORMAL);
			break;
		case cPanKey_SurroundMode:
			MDIP_MenuSelect(cMenu_Surround8Ch, MENU_NORMAL);
			break;
 		case cPanKey_SoundEffect:
			MDIP_MenuSelect(cMenu_SoundEffect, MENU_NORMAL);
			break;

		case cPanKey_PlayPause:
			break;
		case cPanKey_SkipDown:
			break;
		case cPanKey_SkipUp:
			break;

  		case cPanKey_AudioMute: 							// MUTE
			if (gDIP_MenuSelect >= cMenu_MasterVolume && gDIP_MenuSelect <= cMenu_MicReverb){		// �Ѿ���JOG�˵� 
				MDIP_MenuSelect(cMenu_Restore, MENU_NORMAL);
			}
			else {
				MKEY_AudioMute();
			}
 			break;

  		case cPanKey_JogMenu:								// JOG
			
			if (gDIP_MenuSelect < cMenu_MasterVolume || gDIP_MenuSelect > cMenu_MicVolume2){		// ��û�н���JOG�˵� 
				MDIP_MenuSelect(cMenu_MasterVolume, MENU_NORMAL);
			}
			else {
				if (++gDIP_MenuSelect > cMenu_MicVolume2){
					gDIP_MenuSelect = cMenu_MasterVolume;
				}
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_NORMAL);
			}
			break;
		}
	}
    
    if ((gRmKeyContinCanclTm != 0) && (++gRmKeyContinCanclTm > 60)){	
        gRmKeyContinCanclTm = 0;
		gRmKeyContinSpeed = 0;
    }
	if (FRmDecodDone){
		BYTE gLocal_1;
		FRmDecodDone = 0;
//MDEBUG(0xf9);MDEBUG(GRmCodeData);
		switch (GRmCodeData){
		case cRmKey_Standby:
			FSYS_Standby = ~FSYS_Standby;
			if (FSYS_Standby){
				MDIP_MenuSelect(cMenu_Standby, MENU_NORMAL);
			}
			else {
				MKCM_WriteRegister(KCM_POWER_ON, 1);
			    MDIP_MenuSelect(cMenu_PowerOn, MENU_NORMAL);
				gDIP_MenuLock = 30;							// ��ʱ������ʾ3��
			}
			break;
		case cRmKey_Brightness:
			gLocal_1 = (gDIP_MenuSelect == cMenu_Brightness) ? MENU_ADJ_UP : MENU_NORMAL;
			MDIP_MenuSelect(cMenu_Brightness, gLocal_1);
			break;
		case cRmKey_FactorySet:
			MDIP_WriteString(" RESET");
			MDIP_ScreenUpdata();
			MKCM_FactorySet();
			break;
		case cRmKey_MicCtrl:
			if (CUSTOM_CODE != 0x44443000){			// ��������汾������Ƶ�壬���ó������뻷���� �Ե�  ʹ��PT2258����IC
				if (gDIP_MenuSelect < cMenu_MicVolume1){	    // ��û�н���Mic�˵� 
					MDIP_MenuSelect(cMenu_MicVolume1, MENU_NORMAL);
				}else {
					if (++gDIP_MenuSelect > cMenu_MicReverb){
						gDIP_MenuSelect = cMenu_MicVolume1;
					}
					MDIP_MenuSelect(gDIP_MenuSelect, MENU_NORMAL);
				}
			}
			break;
		case cRmKey_TrimUp:
            if (gDIP_MenuSelect >= cMenu_ChTrimFL && gDIP_MenuSelect <= cMenu_ChTrimSL){	// �Ѿ���������΢���˵� 
                gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_ADJ_UP);
            }else if (gDIP_MenuSelect >= cMenu_TToneFL && gDIP_MenuSelect <= cMenu_TToneSL){	// �Ѿ�������������˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_ADJ_UP);
			}
			break;
		case cRmKey_TrimDown:
            if (gDIP_MenuSelect >= cMenu_ChTrimFL && gDIP_MenuSelect <= cMenu_ChTrimSL){	// �Ѿ���������΢���˵� 
                gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_ADJ_DOWN);
            }else if (gDIP_MenuSelect >= cMenu_TToneFL && gDIP_MenuSelect <= cMenu_TToneSL){	// �Ѿ�������������˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_ADJ_DOWN);
			}
			break;
		case cRmKey_MicUp:
			if (CUSTOM_CODE != 0x44443000){			// ��������汾������Ƶ�壬���ó������뻷���� �Ե�  ʹ��PT2258����IC
				if (gDIP_MenuSelect >= cMenu_MicVolume1 && gDIP_MenuSelect <= cMenu_MicReverb){	// �Ѿ�����Mic�˵� 
					gRmKeyContinCanclTm = 1;
					MDIP_MenuSelect(gDIP_MenuSelect, MENU_ADJ_UP);
				}
			}
			break;
		case cRmKey_MicDown:
			if (CUSTOM_CODE != 0x44443000){			// ��������汾������Ƶ�壬���ó������뻷���� �Ե�  ʹ��PT2258����IC
				if (gDIP_MenuSelect >= cMenu_MicVolume1 && gDIP_MenuSelect <= cMenu_MicReverb){	// �Ѿ�����Mic�˵� 
					gRmKeyContinCanclTm = 1;
					MDIP_MenuSelect(gDIP_MenuSelect, MENU_ADJ_DOWN);
				}
			}
			break;
		case cRmKey_VolumeUp:
			gRmKeyContinCanclTm = 1;
			gLocal_1 = (gDIP_MenuSelect == cMenu_MasterVolume) ? MENU_ADJ_UP : MENU_NORMAL;
			MDIP_MenuSelect(cMenu_MasterVolume, gLocal_1);
			break;
		case cRmKey_VolumeDown:
			if (!FSYS_MuteEnable){
				gRmKeyContinCanclTm = 1;
				gLocal_1 = (gDIP_MenuSelect == cMenu_MasterVolume) ? MENU_ADJ_DOWN : MENU_NORMAL;
				MDIP_MenuSelect(cMenu_MasterVolume, gLocal_1);
			}
			break;

		case cRmKey_TestTone:
			MKEY_TestTone();
			break;
		case cRmKey_AudioMute:
    		MKEY_AudioMute();
			break;
		case cRmKey_MeunCtrl:
			if (gDIP_MenuSelect >= cMenu_LipSync && gDIP_MenuSelect <= cMenu_SpeakFilter){	// �Ѿ��������ò˵� 
				MDIP_MenuSelect(cMenu_Restore, MENU_NORMAL);
			}
			else {
				MDIP_MenuSelect(cMenu_LipSync, MENU_NORMAL);
			}
			break;
		case cRmKey_MeunUp:
			if (gDIP_MenuSelect >= cMenu_LipSync && gDIP_MenuSelect <= cMenu_SpeakFilter){	// �Ѿ��������ò˵� 
				++gDIP_MenuSelect;
				if (gDIP_MenuSelect == cMenu_DelayCenter){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[1] == 0){			// û����������
						++gDIP_MenuSelect;					// ����
					}
				}
				if (gDIP_MenuSelect == cMenu_DelaySurround){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[3] == 0){			// û�л���������
						++gDIP_MenuSelect;					// ����
					}
				}
				if (gDIP_MenuSelect == cMenu_DelayBack){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[4] == 0){			// û�к�������
						++gDIP_MenuSelect;					// ����
					}
				}
				if (gDIP_MenuSelect > cMenu_SpeakFilter){
					gDIP_MenuSelect = cMenu_LipSync;
				}
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_NORMAL);
				gDIP_MenuTimer = 50;
			}
//MDEBUG(0xa9);MDEBUG(gDIP_MenuSelect);
			break;
		case cRmKey_MeunDown:
			if (gDIP_MenuSelect >= cMenu_LipSync && gDIP_MenuSelect <= cMenu_SpeakFilter){	// �Ѿ��������ò˵� 
				--gDIP_MenuSelect;
				if (gDIP_MenuSelect == cMenu_DelayBack){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[4] == 0){			// û�к�������
						--gDIP_MenuSelect;					// ����
					}
				}
				if (gDIP_MenuSelect == cMenu_DelaySurround){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[3] == 0){			// û�л���������
						--gDIP_MenuSelect;					// ����
					}
				}

				if (gDIP_MenuSelect == cMenu_DelayCenter){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[1] == 0){			// û����������
						--gDIP_MenuSelect;					// ����
					}
				}
				if (gDIP_MenuSelect < cMenu_LipSync){
					gDIP_MenuSelect = cMenu_SpeakFilter;
				}
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_NORMAL);
				gDIP_MenuTimer = 50;
			}
//MDEBUG(0xa8);MDEBUG(gDIP_MenuSelect);
			break;
		case cRmKey_MeunLeft:
			if (gDIP_MenuSelect >= cMenu_LipSync && gDIP_MenuSelect <= cMenu_SpeakFilter){	// �Ѿ��������ò˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, 3);
			}
			break;
		case cRmKey_MeunRight:
			if (gDIP_MenuSelect >= cMenu_LipSync && gDIP_MenuSelect <= cMenu_SpeakFilter){	// �Ѿ��������ò˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, 4);
			}
			break;
		case cRmKey_TrimCtrl:                               // ����΢��
			if (gDIP_MenuSelect < cMenu_ChTrimFL || gDIP_MenuSelect > cMenu_ChTrimSL){	    // ��û�н�������΢���˵� 
            	MAUD_AutoCanclTestTone();
				MDIP_MenuSelect(cMenu_ChTrimFL, MENU_NORMAL);   // ��������΢���˵� 
			}else {                                         // �Ѿ���������΢���˵� 
                BYTE next = MDIP_GetNextChannel(gDIP_MenuSelect - cMenu_ChTrimFL);    
//MDEBUG(0xee);MDEBUG(gDIP_MenuSelect - cMenu_ChTrimFL);MDEBUG(next);//MDEBUG(gDIP_SpeakSetup[1]);MDEBUG(gDIP_SpeakSetup[2]);MDEBUG(gDIP_SpeakSetup[3]);MDEBUG(gDIP_SpeakSetup[4]);MDEBUG(gDIP_MenuSelect);MDEBUG(cMenu_ChTrimFL);
                gDIP_MenuSelect = next + cMenu_ChTrimFL;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_NORMAL);
			}
			break;
		case cRmKey_Record:                                        // ¼������ͣ���ܣ�������ʱ��������I2C���߶�ȡ
			FDIP_ScreenFill = ~FDIP_ScreenFill;
			if (FDIP_ScreenFill){
				MDIP_ScreenFill(0xff);	
				MDIP_ScreenUpdata();
			}else {
				MDIP_SurroundSymbol();
				MDIP_SrcFormatSymbol();
				MDIP_MenuSelect(cMenu_Restore, 0);
			}
			break;
		case cRmKey_Random:
			MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_PLAY_RANDOM0);  // ��ý��������ţ�ʱ��0
			break;
		case cRmKey_Repeat:
			MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_PLAY_REPEAT0);  // ��ý���ظ����ţ�����0
			break;

		case cRmKey_Stop:
            MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_PLAY_STOP);  // ��ý�岥��ֹͣ
			break;
		case cRmKey_PlayPause:
            MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_PLAY_PAUSE);  // ��ý�岥����ͣ/����
			break;
		case cRmKey_FastBack:
			//MKCM_PlayOperate(KCM_PLAY_FAST_BACK);  // ��ý�岥�Ų��ſ���
			break;
		case cRmKey_FastForward:
			MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_PLAY_FAST_FORW0);  // ��ý�岥�ſ��
			break;
		case cRmKey_SkipDown:
			MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_PLAY_SKIP_DOWN);  // ��ý�岥�ź�һ��
			break;
		case cRmKey_SkipUp:
			MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_PLAY_SKIP_UP);  // ��ý�岥��ǰһ��
			break;
		case cRmKey_SoundEffect:
			MDIP_MenuSelect(cMenu_SoundEffect, MENU_NORMAL);
			break;
		case cRmKey_NightMode:
			MDIP_MenuSelect(cMenu_NightMode, MENU_NORMAL);
			break;
		case cRmKey_NoiseSignal:
			MDIP_MenuSelect(cMenu_NoiseSignal, MENU_NORMAL);
			break;
		case cRmKey_MediaType:
			break;
		case cRmKey_Stereo:
			MDIP_MenuSelect(cMenu_Surround2Ch, MENU_NORMAL);
			break;
		case cRmKey_Surround:
			MDIP_MenuSelect(cMenu_Surround8Ch, MENU_NORMAL);
			break;
		case cRmKey_VideoSrc:
			MKEY_VideoSelect();
			break;
		case cRmKey_InputNet:
			MAUD_MixInputSource(INPUT_SWITCH_UDISK);
			break;
		case cRmKey_InputHdmi1:
			MAUD_MixInputSource(INPUT_SWITCH_HDMI1);
			break;
		case cRmKey_InputHdmi2:
			MAUD_MixInputSource(INPUT_SWITCH_HDMI2);
			break;
		case cRmKey_InputHdmi3:
			MAUD_MixInputSource(INPUT_SWITCH_HDMI3);
			break;
		case cRmKey_InputHdmiArc:
			MAUD_MixInputSource(INPUT_SWITCH_H_ARC);
			break;
		case cRmKey_InputOptica:
			MAUD_MixInputSource(INPUT_SWITCH_OPTIC);
			break;
		case cRmKey_InputCoaxal1:
			MAUD_MixInputSource(INPUT_SWITCH_COA1);
			break;
		case cRmKey_InputCoaxal2:
			MAUD_MixInputSource(INPUT_SWITCH_COA2);
			break;
		case cRmKey_InputAux:
			MAUD_MixInputSource(INPUT_SWITCH_AUX);
			break;

		}
	}
	if (FKeyJogSwOK){
		FKeyJogSwOK = 0;
		if (gDIP_MenuSelect >= cMenu_MasterVolume && gDIP_MenuSelect <= cMenu_MicVolume2){		// �Ѿ�����JOG�˵� 
			MDIP_MenuSelect(gDIP_MenuSelect, FKeyJogUp ? 4 : 3);
		}
		else {
			MDIP_MenuSelect(cMenu_MasterVolume, MENU_NORMAL);
		}
	}
    return;
}	  
void MKEY_AudioMute(){
	BYTE gLocal_1;
	FSYS_MuteEnable = ~FSYS_MuteEnable;
	gLocal_1 = FSYS_MuteEnable ? 4 : 3;
	MDIP_MenuSelect(cMenu_AudioMute, gLocal_1);
	if (!FSYS_MuteEnable){									// �������
		MDIP_MenuSelect(cMenu_MasterVolume, MENU_NORMAL);
	}
    MAUD_AutoCanclTestTone();
    return;
}
void MKEY_TestTone(){                                       // MENU_MODE mode
	FSYS_TestTone = ~FSYS_TestTone;

	if (FSYS_TestTone){										// ����������
		MDIP_MenuSelect(cMenu_TToneFL, MENU_NORMAL);
        MAUD_TestToneChannel(0);
//MDEBUG(0x99);MDEBUG(gDIP_MenuSelect);MDEBUG(gDIP_MenuTimer);MDEBUG(FSYS_TestTone);

	}
	else {													// �ر���������
		MDIP_MenuSelect(cMenu_Restore, MENU_NORMAL);
		MAUD_TestToneChannel(0xff);
	}
    return;
}

CONST_CHAR Tab_VideoSelect[] = {
	0x01,0x02,0x04,
};
void MKEY_VideoSelect(){
	if (mINPUT_SWITCH < INPUT_SWITCH_HDMI1){
		BYTE gLocal_1 = gAUD_AutoInputSrc >> 5;					// B7:B5Ϊѡ�����Ƶ
		BYTE gLocal_2 = gAUD_AutoInputSrc & 0x07;				// B4:B0Ϊ�������Ƶλ
		BYTE gLocal_3 = 0;
		do {
			if (++gLocal_1 > 2){
				gLocal_1 = 0;
			}
			if (gLocal_2 & Tab_VideoSelect[gLocal_1]){			// ѡ�����Ƶ�в���
				MKCM_WriteRegister(KCM_INPUT_VIDEO, (gLocal_1 << 5) | gLocal_2);
			    return;
			}
		} while (++gLocal_3 < 3);
	    MDIP_MenuSelect(cMenu_VideoSrc, MENU_NORMAL);
	}
	else {
		MDIP_MenuSelect(cMenu_Restore, MENU_NORMAL);
	}
    return;
}

void MPKey_Scan(){
    BYTE gLocal_1;
    BYTE gLocal_2;

	if(gKeyScan == 0){
		HAL_PKEY_0(0);
		HAL_PKEY_1(1);
		HAL_PKEY_2(1);
	}		
	else if(gKeyScan == 1){
		HAL_PKEY_0(1);
		HAL_PKEY_1(0);
		HAL_PKEY_2(1);
	}		
	else {
		HAL_PKEY_0(1);
		HAL_PKEY_1(1);
		HAL_PKEY_2(0);
	}

	gLocal_1 = 0;
	if(!HAL_IN_PKEY_0()) gLocal_1 |= 0x01;		
	if(!HAL_IN_PKEY_1()) gLocal_1 |= 0x02;		
	if(!HAL_IN_PKEY_2()) gLocal_1 |= 0x04;		
	if(!HAL_IN_PKEY_3()) gLocal_1 |= 0x08;		

    if (gLocal_1){
        gLocal_1 = (gKeyScan << 4) + gLocal_1;    
        if (GPKeyData == gLocal_1){
            if (++gPKeyConfirmTm > 8){ 
		        if (!FPKeyStep){
	                FPKeyDecodeDone = 1;
	            }
				gPKeyConfirmTm = 0;
            }
        }
		else {
			FPKeyStep = 0; 
			gPKeyConfirmTm = 0; 
			GPKeyData = gLocal_1;
		}
    }
    else{ 
        if (++gKeyScan > 2){ 
            gKeyScan = 0;  
        }           
		FPKeyStep = 0; 
		gPKeyConfirmTm = 0; 
    }
    return;
}
void MKEY_CheckJop(){										// ��ת��ť ��ת
    if (!FKeyJopInputEnl){
        if (HAL_IN_JOP0() && HAL_IN_JOP1() && (++gKeyJopInputTm > 3)){
            gKeyJopInputTm = 0; 
            FKeyJopInputEnl = 1;
        }
        FKeyJopSta00 = 0;
        FKeyJopSta01 = 0;
    }                 
    else if (HAL_IN_JOP0() != HAL_IN_JOP1()){
        if (FKeyJopInputEnl){
            FKeyJogUp = HAL_IN_JOP1();
            if ((FKeyJogUp == FKeyJogUpSave) || (gKeyJopLockTm == 0)){
                FKeyJogSwOK = 1;
            }
            FKeyJogUpSave = FKeyJogUp;
            gKeyJopLockTm = 3;
            gKeyJopInputTm = 0;
            FKeyJopInputEnl = 0;
            FKeyJopSta01 = 0;
        }
        else {
            if (FKeyJopSta01){
                gKeyJopLockTm = 3;
                gKeyJopInputTm = 0;                          
                FKeyJopInputEnl = 0;
            }
            FKeyJopSta01 = 1;
        }                                
        FKeyJopSta00 = 0;
    }
    else if (!HAL_IN_JOP0() && !HAL_IN_JOP1()){ 
        if (FKeyJopSta00){
            gKeyJopLockTm = 3;
            gKeyJopInputTm = 0; 
            FKeyJopInputEnl = 0;
        }
        FKeyJopSta00 = 1;
        FKeyJopSta01 = 0;
    }
    return;
}
