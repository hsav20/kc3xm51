
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           


void MKEY_Initialize(){										  
}

void MKEY_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
	MPKey_Scan();
/*
	if (gDIP_MenuLock > 0){
		if (!FSYS_Standby){									// �����ڴ���ģʽ
			if ((baseTimer & 0x02) > 0){					// 100msʱ��
				--gDIP_MenuLock;
			}
		}
		return;	
	}*/
	if (FSYS_Standby){
		if (FRmDecodDone){
			FRmDecodDone = 0;
			if (GRmCodeData == cRmKey_Standby){
				MKCM_SetPowerOn();						// KCM����
			}
		}
	   	if (FPKeyDecodeDone){
			FPKeyDecodeDone = 0;
			FPKeyStep = 1;
			if (GPKeyData == cPanKey_InputSource){
				MKCM_SetPowerOn();						// KCM����
			}
		}
		return;
	}
   	if (FPKeyDecodeDone){
		FPKeyDecodeDone = 0;
		FPKeyStep = 1;

// MLOG("FPKeyDecodeDone %02x", GRmCodeData);		
		switch (GPKeyData){
		case cPanKey_InputSource:
			MAUD_InputOneKey();         					// ����������һ������ѡ�� 
			break;
		case cPanKey_Stereo:                                // �������������
			MKEY_ListenMode(1);                             // ��������ģʽѡ��    
			break;
		case cPanKey_SurroundMode:                          // ��廷��������
			MKEY_ListenMode(0);                             // ��������ģʽѡ��
			break;
 		case cPanKey_EqSelect:                              // ���EQ������ѡ��
			MEQMIC_KeyEqSelect();                           // ����EQ������ѡ��
			break;

		case cPanKey_PlayPause:								// ��尴�� 
			MKEY_PlayPause();								// ��ý�岥��/��ͣ
			break;
		case cPanKey_SkipDown:
			MDIP_PlaySkip(KCM_OPERATE_SKIP_DOWN);			// ��ý�岥�ź�һ��
			break;
		case cPanKey_SkipUp:
			MDIP_PlaySkip(KCM_OPERATE_SKIP_UP);				// ��ý�岥��ǰһ��
			break;
  		case cPanKey_AudioMute: 							// MUTE
			if (gDIP_MenuSelect >= MENU_MASTER_VOL && gDIP_MenuSelect <= MENU_MIC_TREBLE){		// �Ѿ���JOG�˵� 
				MDIP_MenuNormal(MENU_RESTORE);
			}
			else {
				MKEY_AudioMute();
			}
 			break;

  		case cPanKey_JogMenu:								// JOG
			
			if (gDIP_MenuSelect < MENU_MASTER_VOL || gDIP_MenuSelect > MENU_MIC_VOL2){		// ��û�н���JOG�˵� 
				MDIP_MenuNormal(MENU_MASTER_VOL);
			}
			else {
				if (++gDIP_MenuSelect > MENU_MIC_VOL2){
					gDIP_MenuSelect = MENU_MASTER_VOL;
				}
				MDIP_MenuNormal(gDIP_MenuSelect);
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
// MLOG("FRmDecodDone %02x ", GRmCodeData);	
		switch (GRmCodeData){
		case cRmKey_Standby:
			FSYS_Standby = ~FSYS_Standby;
			if (FSYS_Standby){
				MDIP_MenuNormal(MENU_STANDBY);
			}
			else {
				MKCM_WriteRegister(KCM_POWER_ON, KCM_SET_POWER_ON);		// ����ģ�������������״̬
			    MDIP_MenuNormal(MENU_POWER_ON);             // �˵�״̬:��Դ��
				gDIP_MenuLock = 30;							// ��ʱ������ʾ3��
			}
			break;
		case cRmKey_Brightness:
			gLocal_1 = (gDIP_MenuSelect == MENU_BRIGHTNESS) ? MENU_SET_ADJ_UP : MENU_SET_NORMAL;
			MDIP_MenuSelect(MENU_BRIGHTNESS, gLocal_1);
			break;
		case cRmKey_FactorySet:
			MDIP_WrString(" RESET");
			MDIP_ScreenUpdata();
			MKCM_FactorySet();
			break;
		case cRmKey_MicCtrl:
			MEQMIC_KeyCtrl();								// EQ��MIC����CTRL���
			break;
		case cRmKey_TrimUp:
            if (gDIP_MenuSelect >= MENU_CH_TRIM_FL && gDIP_MenuSelect <= MENU_CH_TRIM_SL){	// �Ѿ���������΢���˵� 
                gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_UP);
            }else if (gDIP_MenuSelect >= MENU_TESTTONE_FL && gDIP_MenuSelect <= MENU_TESTTONE_SL){	// �Ѿ�������������˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_UP);
			}
			break;
		case cRmKey_TrimDown:
            if (gDIP_MenuSelect >= MENU_CH_TRIM_FL && gDIP_MenuSelect <= MENU_CH_TRIM_SL){	// �Ѿ���������΢���˵� 
                gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_DOWN);
            }else if (gDIP_MenuSelect >= MENU_TESTTONE_FL && gDIP_MenuSelect <= MENU_TESTTONE_SL){	// �Ѿ�������������˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_DOWN);
			}
			break;
		case cRmKey_MicUp:
			MEQMIC_KeyUp();									// EQ��MIC��������+���
			break;
		case cRmKey_MicDown:
			MEQMIC_KeyDown();								// EQ��MIC��������-���
			break;
		case cRmKey_VolumeUp:
			gRmKeyContinCanclTm = 1;
			gLocal_1 = (gDIP_MenuSelect == MENU_MASTER_VOL) ? MENU_SET_ADJ_UP : MENU_SET_NORMAL;
			MDIP_MenuSelect(MENU_MASTER_VOL, gLocal_1);
			break;
		case cRmKey_VolumeDown:
			if (!FSYS_MuteEnable){
				gRmKeyContinCanclTm = 1;
				gLocal_1 = (gDIP_MenuSelect == MENU_MASTER_VOL) ? MENU_SET_ADJ_DOWN : MENU_SET_NORMAL;
				MDIP_MenuSelect(MENU_MASTER_VOL, gLocal_1);
			}
			break;

		case cRmKey_TestTone:
			MKEY_TestTone();
			break;
		case cRmKey_AudioMute:
    		MKEY_AudioMute();
			break;
		case cRmKey_MeunCtrl:
			if (gDIP_MenuSelect >= MENU_LIP_SYNC && gDIP_MenuSelect <= MENU_SPEAK_FILTER){	// �Ѿ��������ò˵� 
				MDIP_MenuNormal(MENU_RESTORE);
			}
			else {
				MDIP_MenuNormal(MENU_LIP_SYNC);
			}
			break;
		case cRmKey_MeunUp:
			if (gDIP_MenuSelect >= MENU_LIP_SYNC && gDIP_MenuSelect <= MENU_SPEAK_FILTER){	// �Ѿ��������ò˵� 
				++gDIP_MenuSelect;
				if (gDIP_MenuSelect == MENU_DELAY_CENTER){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[1] == 0){			// û����������
						++gDIP_MenuSelect;					// ����
					}
				}
				if (gDIP_MenuSelect == MENU_DELAY_SURR){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[3] == 0){			// û�л���������
						++gDIP_MenuSelect;					// ����
					}
				}
				if (gDIP_MenuSelect == MENU_DELAY_BACK){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[4] == 0){			// û�к�������
						++gDIP_MenuSelect;					// ����
					}
				}
				if (gDIP_MenuSelect > MENU_SPEAK_FILTER){
					gDIP_MenuSelect = MENU_LIP_SYNC;
				}
				MDIP_MenuNormal(gDIP_MenuSelect);
				gDIP_MenuTimer = 50;
			}
//MDEBUG(0xa9);MDEBUG(gDIP_MenuSelect);
			break;
		case cRmKey_MeunDown:
			if (gDIP_MenuSelect >= MENU_LIP_SYNC && gDIP_MenuSelect <= MENU_SPEAK_FILTER){	// �Ѿ��������ò˵� 
				--gDIP_MenuSelect;
				if (gDIP_MenuSelect == MENU_DELAY_BACK){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[4] == 0){			// û�к�������
						--gDIP_MenuSelect;					// ����
					}
				}
				if (gDIP_MenuSelect == MENU_DELAY_SURR){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[3] == 0){			// û�л���������
						--gDIP_MenuSelect;					// ����
					}
				}

				if (gDIP_MenuSelect == MENU_DELAY_CENTER){	// �˵����������ӳ�ʱ�����
					if (gDIP_SpeakSetup[1] == 0){			// û����������
						--gDIP_MenuSelect;					// ����
					}
				}
				if (gDIP_MenuSelect < MENU_LIP_SYNC){
					gDIP_MenuSelect = MENU_SPEAK_FILTER;
				}
				MDIP_MenuNormal(gDIP_MenuSelect);
				gDIP_MenuTimer = 50;
			}
//MDEBUG(0xa8);MDEBUG(gDIP_MenuSelect);
			break;
		case cRmKey_MeunLeft:
			if (gDIP_MenuSelect >= MENU_LIP_SYNC && gDIP_MenuSelect <= MENU_SPEAK_FILTER){	// �Ѿ��������ò˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_DOWN);
			}
			break;
		case cRmKey_MeunRight:
			if (gDIP_MenuSelect >= MENU_LIP_SYNC && gDIP_MenuSelect <= MENU_SPEAK_FILTER){	// �Ѿ��������ò˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_UP);
			}
			break;
		case cRmKey_TrimCtrl:                               // ����΢��
			if (gDIP_MenuSelect < MENU_CH_TRIM_FL || gDIP_MenuSelect > MENU_CH_TRIM_SL){	    // ��û�н�������΢���˵� 
            	MAUD_AutoCanclTestTone();
				MDIP_MenuNormal(MENU_CH_TRIM_FL);   			// ��������΢���˵� 
			}else {                                         // �Ѿ���������΢���˵� 
                BYTE next = MDIP_GetNextChannel(gDIP_MenuSelect - MENU_CH_TRIM_FL);    
//MDEBUG(0xee);MDEBUG(gDIP_MenuSelect - MENU_CH_TRIM_FL);MDEBUG(next);//MDEBUG(gDIP_SpeakSetup[1]);MDEBUG(gDIP_SpeakSetup[2]);MDEBUG(gDIP_SpeakSetup[3]);MDEBUG(gDIP_SpeakSetup[4]);MDEBUG(gDIP_MenuSelect);MDEBUG(MENU_CH_TRIM_FL);
                gDIP_MenuSelect = next + MENU_CH_TRIM_FL;
				MDIP_MenuNormal(gDIP_MenuSelect);
			}
			break;
		case cRmKey_Record:                                        // ¼������ͣ���ܣ�������ʱ��������I2C���߶�ȡ
			// FDIP_ScreenFill = ~FDIP_ScreenFill;
			// if (FDIP_ScreenFill){
			// 	MDIP_ScreenFill(0xff);	
			// 	MDIP_ScreenUpdata();
			// }else {
			// 	MDIP_SurroundSymbol();
			// 	MDIP_SrcFormatSymbol();
			// 	MDIP_MenuNormal(MENU_RESTORE);
			// }
			break;
		case cRmKey_Random:
			// MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_PLAY_RANDOM0);  // ��ý��������ţ�ʱ��0
			break;
		case cRmKey_Repeat:
			// MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_PLAY_REPEAT0);  // ��ý���ظ����ţ�����0
			break;

		case cRmKey_Stop:
            MKCM_WriteRegister(KCM_PLAY_STATE, KC3X_STATE_PLAY_STOP);  // ��ý�岥��ֹͣ
			break;
		case cRmKey_PlayPause:								// ��������/��ͣ
			MKEY_PlayPause();
			break;
		case cRmKey_FastBack:
			MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_OPERATE_FAST_BACK);  // ��ý�岥�Ų��ſ���
			break;
		case cRmKey_FastForward:
			MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_OPERATE_FAST_FORW);  // ��ý�岥�ſ��
			break;
		case cRmKey_SkipDown:
			MDIP_PlaySkip(KCM_OPERATE_SKIP_DOWN);			// ��ý�岥�ź�һ��
			break;
		case cRmKey_SkipUp:
			MDIP_PlaySkip(KCM_OPERATE_SKIP_UP);				// ��ý�岥��ǰһ��
			break;
		case cRmKey_EqSelect:
            MEQMIC_KeyEqSelect();                           // ����EQ������ѡ��
			break;
		case cRmKey_NightMode:
			MDIP_MenuNormal(MENU_NIGHT_MODE);
			break;
		case cRmKey_NoiseSignal:
			MDIP_MenuNormal(MENU_TEST_TONE);
			break;
		case cRmKey_MediaType:
			break;
		case cRmKey_Stereo:                                 // �������������
            MKEY_ListenMode(1);                             // ��������ģʽѡ��
			break;
		case cRmKey_Surround:                               // ��廷��������
            MKEY_ListenMode(0);                             // ��������ģʽѡ��
			break;
		case cRmKey_VideoSrc:								// SD����
			MKEY_VideoSelect();
			break;
		case cRmKey_InputNet:								// UDisk����
			// MAUD_InputSelect(INPUT_SWITCH_UDISK);			// UDISK   
			//  MAUD_InputSelect(INPUT_SWITCH_BT);			// BT
			MAUD_Preemptible();								// ��ռʽ����ѡ�� 
			break;
		case cRmKey_InputHdmi1:
			MAUD_InputSelect(INPUT_SWITCH_HDMI1);
			break;
		case cRmKey_InputHdmi2:
			MAUD_InputSelect(INPUT_SWITCH_HDMI2);
			break;
		case cRmKey_InputHdmi3:
			MAUD_InputSelect(INPUT_SWITCH_HDMI3);
			break;
		case cRmKey_InputHdmiArc:							// HDMI_ARC������7.1����
			MAUD_InputSelect(INPUT_SWITCH_E8CH);			// ����7.1����
			// MAUD_InputSelect(INPUT_SWITCH_H_ARC);
			break;
		case cRmKey_InputOptica:
			MAUD_InputSelect(INPUT_SWITCH_OPTIC);
			break;
		case cRmKey_InputCoaxal1:
			MAUD_InputSelect(INPUT_SWITCH_COA1);
			break;
		case cRmKey_InputCoaxal2:
			MAUD_InputSelect(INPUT_SWITCH_COA2);
			break;
		case cRmKey_InputAux:
			MAUD_InputSelect(INPUT_SWITCH_AUX);
			break;

		}
	}
	if (FKeyJogSwOK){
		FKeyJogSwOK = 0;
		if (gDIP_MenuSelect >= MENU_MASTER_VOL && gDIP_MenuSelect <= MENU_MIC_VOL2){		// �Ѿ�����JOG�˵� 
			MDIP_MenuSelect(gDIP_MenuSelect, FKeyJogUp ? MENU_SET_ADJ_UP : MENU_SET_ADJ_DOWN);
		}
		else {
			MDIP_MenuNormal(MENU_MASTER_VOL);
		}
	}
    return;
}	  
void MKEY_AudioMute(){
	BYTE gLocal_1;
	FSYS_MuteEnable = ~FSYS_MuteEnable;
	gLocal_1 = FSYS_MuteEnable ? 4 : 3;
	MDIP_MenuSelect(MENU_AUDIO_MUTE, gLocal_1);
	if (!FSYS_MuteEnable){									// �������
		MDIP_MenuNormal(MENU_MASTER_VOL);
	}
    MAUD_AutoCanclTestTone();
    return;
}
void MKEY_TestTone(){                                       // MENU_SET mode
	FSYS_TestTone = ~FSYS_TestTone;

	if (FSYS_TestTone){										// ����������
		MDIP_MenuNormal(MENU_TESTTONE_FL);
        MAUD_TestToneChannel(0);
//MDEBUG(0x99);MDEBUG(gDIP_MenuSelect);MDEBUG(gDIP_MenuTimer);MDEBUG(FSYS_TestTone);

	}
	else {													// �ر���������
		MDIP_MenuNormal(MENU_RESTORE);
		MAUD_TestToneChannel(0xff);
	}
    return;
}
void MKEY_PlayPause(){										// ��������/��ͣ
MLOG("MKEY_PlayPause A %d %02x", mINPUT_SWITCH, gPlayStatus);
	if (mINPUT_SWITCH == INPUT_SWITCH_SD || mINPUT_SWITCH == INPUT_SWITCH_UDISK){
		BYTE flag = gPlayStatus & KC3X_STATE_PLAY_FLAG;
		if (flag == KC3X_STATE_PLAY_PLAY){					// ����Ѿ��ڲ���֮��
			MKCM_WriteRegister(KCM_PLAY_STATE, KC3X_STATE_PLAY_PAUSE);  // ��ͣ
			gPlayStatus &= ~KC3X_STATE_PLAY_FLAG;					
			gPlayStatus |= KC3X_STATE_PLAY_PAUSE;
		}else {
			MKCM_WriteRegister(KCM_PLAY_STATE, KC3X_STATE_PLAY_PLAY);  // ����
			gPlayStatus &= ~KC3X_STATE_PLAY_FLAG;
			gPlayStatus |= KC3X_STATE_PLAY_PLAY;
		}
		MDIP_PlaySymbol(gPlayStatus);								// �ӿ�ͼ����ʾ���ò����о��õ�
	}
}

CONST_CHAR Tab_VideoSelect[] = {
	0x01,0x02,0x04,
};
void MKEY_VideoSelect(){
	if (++gVideoSelect >= 3){
		gVideoSelect = 0;
	}
	MKCM_WriteRegister(KCM_INPUT_VIDEO, 1 << gVideoSelect);
	MDIP_MenuNormal(MENU_VIDEO_SRC);
	// if (mINPUT_SWITCH < INPUT_SWITCH_HDMI1){
	// 	BYTE gLocal_1 = gAUD_AutoInputSrc >> 5;					// B7:B5Ϊѡ�����Ƶ
	// 	BYTE gLocal_2 = gAUD_AutoInputSrc & 0x07;				// B4:B0Ϊ�������Ƶλ
	// 	BYTE gLocal_3 = 0;
	// 	do {
	// 		if (++gLocal_1 > 2){
	// 			gLocal_1 = 0;
	// 		}
	// 		if (gLocal_2 & Tab_VideoSelect[gLocal_1]){			// ѡ�����Ƶ�в���
	// 			MKCM_WriteRegister(KCM_INPUT_VIDEO, (gLocal_1 << 5) | gLocal_2);
	// 		    return;
	// 		}
	// 	} while (++gLocal_3 < 3);
	//     MDIP_MenuNormal(MENU_VIDEO_SRC);
	// }
	// else {
	// 	MDIP_MenuNormal(MENU_RESTORE);
	// }
}
CONST_CHAR Tab_ListenModeRegister[] = {
    0x00, 0x01, 											// ˫������������B0Ϊ0�رճ�������Ϊ1�򿪳�����
	0x10, 													// ������Դ��ģʽ���������Զ�
	0x20, 0x21, 											// ������ģʽ��B1:0Ϊ���ֲ�ͬ�㷨�Ķ�����ģʽ��
	0x30,													// ��������Ч��B1:0Ϊ���ֲ�ͬ�㷨�Ķ�������Ч��
};

void MKEY_ListenMode(BYTE stereo){                          // ��������ģʽѡ��
    BYTE value = MKCM_ReadRegister(KCM_LISTEN_MODE);        // ����ģʽѡ��
    if (gDIP_MenuSelect == MENU_LISTEN_MODE){               // ֻ�н����Ӧ�Ĳ˵��Ÿı�ģʽ
        LISTEN_MODE_STATE state = GetListenModeIndex(value);
        if (stereo){                                        // ѡ��������
            state = (state == LISTEN_MODE_HIFI) ? LISTEN_MODE_2_1CH : LISTEN_MODE_HIFI;
        }else {
            if (state < LISTEN_MODE_SURROUND1){                                // ֻ������������ֱ�ӻ���MODE1
                state = LISTEN_MODE_SURROUND1;
            }else if (++state > LISTEN_MODE_SURROUND4){
                state = LISTEN_MODE_SURROUND1;
            }
        }
        value = Tab_ListenModeRegister[state];
        MKCM_WriteRegister(KCM_LISTEN_MODE, value);
// MLOG("ListenMode %d %02x", state, value);		
    }
    MDIP_ListenMode(value);
}

