
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           

void MDIP_BaseInit(){										// ��ʾģ��������ʼ��								  
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
		
		if (gDIP_MenuLock > 0){								// ��ʱ������ʾ
			return;
		}
		if (FSYS_MuteEnable && (gDIP_MenuTimer == 0)){
			MDIP_MenuNormal(cMenu_AudioMute);
		}
		if (!FSYS_Standby && gDIP_MenuTimer){				// 100ms����˵���
			if (--gDIP_MenuTimer == 0) {					// �˵���ʱ	
            	if (!FSYS_TestTone){
					MDIP_MenuNormal(MENU_RESTORE);
				}
				else {                                      // ����������֮��
					BYTE channel = MDIP_GetNextChannel(gDIP_MenuSelect - MENU_TESTTONE_FL);
					MDIP_MenuNormal(channel + MENU_TESTTONE_FL);
                    MAUD_TestToneChannel(channel);
//MDEBUG(0xc7);MDEBUG(gDIP_MenuSelect);MDEBUG(gDIP_MenuTimer);
				}
			}
			else if (FDIP_MenuTwinkle){						// �˵���Ҫ��˸
				if ((gDIP_MenuTimer % 5) == 0){	  			// ÿ0.5������˸
					BYTE twinkle = (gDIP_MenuTimer / 5);
					MDIP_MenuSelect(gDIP_MenuSelect, (twinkle & 0x01) ? MENU_SET_TWINKLE_OFF : MENU_SET_TWINKLE_ON);
				}
			}
        }
		if (!FSYS_Standby && !FSYS_MuteEnable && !FSYS_TestTone && FDIP_FreqSymbol && ((gAUD_SrcFormat & 0x0f) >= KCM_SRC_NOS)){
            MDIP_ReadSpectrum();
		}else {
//		MLOG("FSYS_TestTone:%d %d", FSYS_TestTone, FSYS_TestTone);
		}
	}
    return;
}
void MDIP_MenuNormal(BYTE index){							// �˵�ѡ��һ��ģʽ 
	MDIP_MenuSelect(index, MENU_SET_NORMAL);					// �˵�ѡ��
}
void MDIP_SetNormal(){                                      // ���ò˵�һ��ģʽ��ʾ��ʱ��
	gDIP_MenuTimer = 20;
	FDIP_MenuTwinkle = 0;
}
void MDIP_SetState(MENU_STATE state){                       // ���ò˵�״̬
    gDIP_MenuSelect = state;
    MDIP_SetNormal();
}

void MDIP_MenuSelect(BYTE index, MENU_SET mode){			// �˵�ѡ��߼�ģʽ��mode 0һ��ģʽ 1��˸���� 2��˸Ϩ�� 3����- 4����+ 
	if (mode == MENU_SET_NORMAL){								// ��������ʾģʽ
		MDIP_SetNormal();
		//gDIP_MenuTimer = 20;
		//FDIP_MenuTwinkle = 0;
	}
	FDIP_FreqSymbol = 0;
    HAL_DIP_LED(0);                                         // Ϩ����ʾ���̶��ķ���
	MDIP_ClearSpectrum();
    
    DIP_DOT_OFF();
	
//g2DIP_ShowBuffer[6] &= ~(0x0700 | 0x0004);
	switch (index){
	case MENU_RESTORE:                                      // �˵��ָ�״̬
		if (!FKCM_I2C_Error){
			if (mINPUT_SWITCH == INPUT_SWITCH_SD || mINPUT_SWITCH == INPUT_SWITCH_UDISK){
				MDIP_PlayTime();
			}else {
				MDIP_InputSource();
			}
		}
		else {
			MDIP_WriteString("KCM-NO");
		}
		gDIP_MenuSelect = 0;
		gDIP_MenuTimer = 0;
		FDIP_FreqSymbol = 1;
		break;
	case MENU_INPUT_SOURCE:
		MDIP_InputSource();
		break;
	case cMenu_Standby:
		MDIP_WriteString(" -OFF-");
		break;
	case MENU_POWER_ON:                                     // �˵�״̬:��Դ��
		MDIP_WriteString(" HELLO");
		MDIP_ScreenUpdata();
		gDIP_MenuTimer = 10;
		break;
	case cMenu_Brightness:
		if (mode == 4){
			gDIP_MenuTimer = 20;
			if (++gDIP_Brightness > 2){
				gDIP_Brightness = 0;
			}
			MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_BRIGHTNESS, gDIP_Brightness);
		}
		MDIP_Brightness(1, gDIP_Brightness);
		break;
	/*case cMenu_Surround2Ch:
        MAUD_AutoCanclTestTone();
		MDIP_SurroundMode(cMenu_Surround2Ch, mode);
		break;
	case cMenu_Surround8Ch:
        MAUD_AutoCanclTestTone();
		MDIP_SurroundMode(cMenu_Surround8Ch, mode);
		break;
	case cMenu_SoundEffect:
        MAUD_AutoCanclTestTone();
		MDIP_SoundEffect(4);
		break;
        */
	case cMenu_VideoSrc:
		gDIP_MenuTimer = 50;
		MDIP_VideoSrc();
		break;
	case MENU_SRC_FORMAT:									// �˵���ʾ����������ʽ
		MDIP_SourceFormat();
		break;
	case cMenu_NightMode:
		MDIP_NightMode();
		break;
	case cMenu_NoiseSignal:
		MDIP_NoiseSignal();
		break;
//	case cMenu_Fireware:                                    // ��ʾ�̼�����
//		MDIP_Fireware();
//		break;
    case cMenu_SdInsert:                                   // ��ʾSD����
    case cMenu_UDiskInsert:                                // ��ʾU�̲���
    case cMenu_SdRemove:                                   // ��ʾSD�γ� 
    case cMenu_UDiskRemove:                                // ��ʾU�̰γ�
    case cMenu_UsbaRemove:               	               // ��ʾUSB�����γ�
    case cMenu_HdmiRemove:                                 // ��ʾHDMI�γ� 
    case cMenu_BtRemove:               	                 	// ��ʾ������Ƶ�γ�
		MDIP_InsertRemove(index - cMenu_SdInsert);			// ��ʾ������Դ����/�γ�
		break;
    case cMenu_PlayTrack:                                  // ��ʾ��ý���ļ���Ϣ
        MDIP_PlayTrack();
		break;
    case cMenu_PlayTime:                                   // ��ʾ��ý�岥��ʱ��
        MDIP_PlayTime();
        FDIP_FreqSymbol = 1;								// ��Ҫ�����־�ſ�����ʾƵ��
		break;


	case cMenu_AudioMute:
		if (mode == 0 || mode == 4){ 						 // 0һ��ģʽ ���� 4������Ч
			gDIP_MenuTimer = 80;
			FDIP_MenuTwinkle = 1;
		}
		if (mode < 2 || mode == 4){
			MDIP_WriteString("MUTE  ");
		}	
		else if (mode == 2){
			MDIP_WriteString("MUTE--");
		}
		if (mode == 4){										// ������Ч
			MKCM_WriteRegister(KCM_VOLUME_MUTE, 0x03);      // B1Ϊ������Ƶ�ľ��� B0=1������
		}
		else if (mode == 3){								// ȡ������
			gDIP_MenuTimer = 20;
			MKCM_WriteRegister(KCM_VOLUME_MUTE, 0x02);      // B1Ϊ������Ƶ�ľ��� B0=1������ 
		}
//MDEBUG(0xa9);MDEBUG(mode); MDEBUG(FSYS_MuteEnable);
		break;
	case MENU_MASTER_VOL:
		if (mode >= 3){										// 3����- 4����+
			if (gDIP_MenuSelect == MENU_MASTER_VOL){
				MAUD_MixMasterVolume((mode == 4) ? 1 : 0); 	
			}
		}
		if (mode == 2){										// 2��˸Ϩ��
			MDIP_WriteString("VOL   ");
		}
		else {
			MDIP_WriteString("VOL ");
			MDIP_Write2Digit(4, gAUD_MasterVolume);
			if (mode == 0){
				gDIP_MenuTimer = 80;
				FDIP_MenuTwinkle = 1;
			}
		}
		break;

	case MENU_TESTTONE_FL:                                     // ��������ǰ��������
	case MENU_TESTTONE_CE:                                     // ����������������
	case MENU_TESTTONE_SW:                                     // ������������������
	case MENU_TESTTONE_FR:                                     // ��������ǰ��������
	case MENU_TESTTONE_SR:                                     // ������������������
	case MENU_TESTTONE_BR:                                     // ������������������
	case MENU_TESTTONE_BL:                                     // ������������������
	case MENU_TESTTONE_SL:                                     // ������������������
//MDEBUG(0xb2);MDEBUG(index);
        MDIP_TestTone(index, mode);                         // ����΢����������
        break;
	case MENU_CH_TRIM_FL:                                    // ����΢��ǰ��������
	case MENU_CH_TRIM_CE:                                    // ����΢����������
	case MENU_CH_TRIM_SW:                                    // ����΢������������
	case MENU_CH_TRIM_FR:                                    // ����΢��ǰ��������
	case MENU_CH_TRIM_SR:                                    // ����΢������������
	case MENU_CH_TRIM_BR:                                    // ����΢������������
	case MENU_CH_TRIM_BL:                                    // ����΢������������
	case MENU_CH_TRIM_SL:                                    // ����΢������������
        MAUD_AutoCanclTestTone();
		MDIP_TrimControl(index, mode);                      // ����΢����������
		break;

	case MENU_MIC_VOL1:                                  	// ��Ͳ1����
    case MENU_MIC_VOL2:                                  	// ��Ͳ2����
    case MENU_MIC_ECHO:                                     // ��Ͳ��������
    case MENU_MIC_REPEAT:                                   // ��Ͳ�ظ�����
    case MENU_MIC_DELAY:                                    // ��Ͳ�ӳ�ʱ��
    case MENU_MIC_BASS:                                   	// ��Ͳ������������
    case MENU_MIC_TREBLE:                                   // ��Ͳ������������
        MAUD_AutoCanclTestTone();
		MEQMIC_MicDisplay(index, mode);                     // ��ʾ��Ͳ���ֲ�������
		break;
	case cMenu_SpeakFront:
	case cMenu_SpeakCenter:
	case cMenu_SpeakWoofer:
	case cMenu_SpeakSurround:
	case cMenu_SpeakBack:
        MAUD_AutoCanclTestTone();
		MDIP_SpeakSetup(index, mode);
		break;
	case cMenu_LipSync:
	case cMenu_DelayFront:
	case cMenu_DelayCenter:
	case cMenu_DelaySurround:
	case cMenu_DelayBack:
        MAUD_AutoCanclTestTone();
		MDIP_DelayTime(index, mode);
		break;
	}
	gDIP_MenuSelect = index;
}
void MDIP_NoiseSignal(){
	BYTE gLocal_1 = MKCM_ReadRegister(KCM_TEST_TONE);
	if (FSYS_TestTone){									// �Ѿ�������������
		if (gDIP_MenuSelect == cMenu_NoiseSignal){	
			if (++gLocal_1 > 7){
				gLocal_1 = 0;
			}
		}
	}
	MKCM_WriteRegister(KCM_TEST_TONE, gLocal_1); 
	MDIP_WriteString("NOISE ");
	MDIP_SingleChar(5, gLocal_1 + '1');
	if (FSYS_TestTone){									// �Ѿ�������������
		gDIP_TestToneChannel = 0;
		MAUD_TestToneChannel(gDIP_TestToneChannel);
	}
}
BYTE MDIP_GetNextChannel(BYTE index){                       // ������������΢����ȡ��һ������
    switch (index){						                    // 0ǰ�� 1���� 2������ 3ǰ�� 4���� 5���� 6���� 7����
	case 0 : 											    // ǰ��ʱ
		if (gDIP_SpeakSetup[1]){						    // ������
			return 1;                                       // ��һ������
		}
		if (gDIP_SpeakSetup[2]){						    // �г�����
			return 2;                                       // ��һ��������
		}
		return 3;						                    // û�����ü������� ֱ������ǰ��
	case 1 : 											    // ����ʱ
		if (gDIP_SpeakSetup[2]){						    // �г�����
			return 2;                                       // ��һ��������
		}
		return 3;						                    // û�г����� ֱ������ǰ��
	case 2 : 											    // ������ʱ
		return 3;						                    // ֱ������ǰ��
	case 3 : 											    // ǰ��ʱ
		if (gDIP_SpeakSetup[3]){						    // �л���
			return 4;                                       // ����������
		}
        if (gDIP_SpeakSetup[4]){						    // �к�������
			return 5;                                       // ��������
		}
		return 0;						                    // û�л��Ƽ����ã�ֱ������ǰ��
	case 4 : 											    // ����ʱ
		if (gDIP_SpeakSetup[4]){						    // �к�������
			return 5;                                       // ��������
		}
		return 0;						                    // û�к��ã�ֱ������ǰ��
	case 5 : 											    // ����ʱ
		return 6;						                    // ֱ����������
	case 6 : 											    // ������
		return 7;						                    // ֱ����������
	default : 											    // ����ʱ
		return 0;						                    // ֱ����������
	}
}

CONST_CHAR Tab_DIP_TToneShow[] = {
	"FLTtCETtSWTtFRTtSRTtBRTtBLTtSLTt"
//	 ++++----++++----++++----++++----
};

void MDIP_TestTone(BYTE index, MENU_SET mode){             // ����΢����������
	if (mode >= MENU_SET_ADJ_DOWN){							    // MENU_SET_ADJ_DOWN��MENU_SET_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // �ϴ��Ѿ����뵱ǰ�˵�
		    MAUD_MixTrimAdjust(index - MENU_TESTTONE_FL, (mode == MENU_SET_ADJ_UP) ? 1 : 0); 	
			gDIP_MenuTimer = 80;
		}
	}
    index -= MENU_TESTTONE_FL;                             // ������΢��ǰ����������ʼ
    MDIP_WriteString((char*)&Tab_DIP_TToneShow[index * 4]);
	if (mode == MENU_SET_TWINKLE_OFF){							// MENU_SET_TWINKLE_OFF��˸Ϩ��
		MDIP_SingleChar(4, ' ');
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_SET_NORMAL��MENU_SET_TWINKLE_ON��˸����
		MDIP_ShowNegative9(gDIP_TrimCtrl[index]);			// ����ʾ��������ֵ�λ����ʾ-9��+9����ֵ
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
void MDIP_TrimControl(BYTE index, MENU_SET mode){          // ����΢����������
	if (mode >= MENU_SET_ADJ_DOWN){							    // MENU_SET_ADJ_DOWN��MENU_SET_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // �ϴ��Ѿ����뵱ǰ�˵�
            MAUD_MixTrimAdjust(index - MENU_CH_TRIM_FL, (mode == MENU_SET_ADJ_UP) ? 1 : 0); 	
			gDIP_MenuTimer = 80;
		}
	}
    index -= MENU_CH_TRIM_FL;                            // ������΢��ǰ����������ʼ
    MDIP_WriteString((char*)&Tab_DIP_TrimShow[index * 4]);
	if (mode == MENU_SET_TWINKLE_OFF){							// MENU_SET_TWINKLE_OFF��˸Ϩ��
		MDIP_SingleChar(4, ' ');
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_SET_NORMAL��MENU_SET_TWINKLE_ON��˸����
    	BYTE value = gDIP_TrimCtrl[index];
//if(mode!=MENU_SET_TWINKLE_ON&&mode!=MENU_SET_TWINKLE_OFF){MDEBUG(0xee);MDEBUG(index);MDEBUG(value);}
        if (value < 9){
            MDIP_SingleChar(4, '-');
            MDIP_SingleChar(5, (9 - value) + '0');
        }else {
            MDIP_SingleChar(4, ' ');
            MDIP_SingleChar(5, (value - 9) + '0');
        }
		if (mode == MENU_SET_NORMAL){                           // ��������ʾģʽ
			gDIP_MenuTimer = 80;
			FDIP_MenuTwinkle = 1;
		}
       
	}
}

void MDIP_SpeakSetup(BYTE index, BYTE mode){				// mode 0һ��ģʽ 1��˸���� 2��˸Ϩ�� 3����- 4����+  
	BYTE gLocal_1;
	g2DIP_ShowBuffer[6] |= 0x5000;	
	gDIP_MenuTimer = 80;
	MDIP_SingleChar(4, ' ');
	MDIP_SingleChar(5, ' ');
	switch (index){
	case cMenu_SpeakFront :
		MDIP_WriteString("FRSP ");
		if (mode >= 3){
			if (++gDIP_SpeakSetup[0] > 1){
				gDIP_SpeakSetup[0] = 0;
			}
			MKCM_WriteRegister(KCM_SPK_CONFIG, MKCM_ToRegister(KCM_SPK_CONFIG, 0));
		}
		MDIP_SingleChar(5, MDIP_GetSpeakerChar(0));
		break;
	case cMenu_SpeakCenter :
		MDIP_WriteString("CESP ");
		if (mode >= 3){
			if (++gDIP_SpeakSetup[1] > 2){
				gDIP_SpeakSetup[1] = 0;
			}
			MKCM_WriteRegister(KCM_SPK_CONFIG, MKCM_ToRegister(KCM_SPK_CONFIG, 0));
		}
		MDIP_SingleChar(5, MDIP_GetSpeakerChar(1));
		break;
	case cMenu_SpeakWoofer :
		MDIP_WriteString("SWSP ");
		if (mode >= 3){
			if (++gDIP_SpeakSetup[2] > 1){
				gDIP_SpeakSetup[2] = 0;
			}
			MKCM_WriteRegister(KCM_SPK_CONFIG, MKCM_ToRegister(KCM_SPK_CONFIG, 0));
		}
		MDIP_SingleChar(5, MDIP_GetSpeakerChar(2));
		break;
	case cMenu_SpeakSurround :
		MDIP_WriteString("SRSP ");
		if (mode >= 3){
			if (++gDIP_SpeakSetup[3] > 2){
				gDIP_SpeakSetup[3] = 0;
			}
			MKCM_WriteRegister(KCM_SPK_CONFIG, MKCM_ToRegister(KCM_SPK_CONFIG, 0));
		}
		MDIP_SingleChar(5, MDIP_GetSpeakerChar(3));
		break;
	case cMenu_SpeakBack :
		MDIP_WriteString("BKSP ");
		if (mode >= 3){
			if (++gDIP_SpeakSetup[4] > 2){
				gDIP_SpeakSetup[4] = 0;
			}
			MKCM_WriteRegister(KCM_SPK_CONFIG, MKCM_ToRegister(KCM_SPK_CONFIG, 0));
		}
		MDIP_SingleChar(5, MDIP_GetSpeakerChar(4));
		break;
	case cMenu_SpeakFilter :
		MDIP_WriteString("LPF ");
		if (mode >= 3){
		}
		break;
	}
}
void MDIP_DelayTime(BYTE index, BYTE mode){					// mode 0һ��ģʽ 1��˸���� 2��˸Ϩ�� 3����- 4����+  
	switch (index){
	case cMenu_LipSync :
		g2DIP_ShowBuffer[6] |= 0x4000;	
		MDIP_WriteString("LIPS");
		if (mode >= 3){
			MDIP_AdjDelayTime(0, mode);
		}
		break;
	case cMenu_DelayFront :
		MDIP_WriteString("FRDL ");
		g2DIP_ShowBuffer[6] |= 0x5000;	
		if (mode >= 3){
			MDIP_AdjDelayTime(1, mode);
		}
		break;
	case cMenu_DelayCenter :
		MDIP_WriteString("CEDL ");
		g2DIP_ShowBuffer[6] |= 0x5000;	
		if (mode >= 3){
			MDIP_AdjDelayTime(2, mode);
		}
		break;
	case cMenu_DelaySurround :
		MDIP_WriteString("SWDL ");
		g2DIP_ShowBuffer[6] |= 0x5000;	
		if (mode >= 3){
			MDIP_AdjDelayTime(3, mode);
		}
		break;
	case cMenu_DelayBack :
		MDIP_WriteString("BKDL ");
		g2DIP_ShowBuffer[6] |= 0x5000;	
		if (mode >= 3){
			MDIP_AdjDelayTime(4, mode);
		}
		break;
	}
	gDIP_MenuTimer = 50;
//MDEBUG(0xc8);MDEBUG(index - cMenu_LipSync);
	MDIP_Write2Digit(4, gDIP_DelayTime[index - cMenu_LipSync] * 5);
}


CONST_CHAR Tab_DIP_InputSwitch[] = {
	"AUX-INOPTICACOA-D1COA-D2 -SD-  -USB- -PC-  E8CH -BT-  HDMI-1HDMI-2HDMI-3HDMI-A"
//	 ++++++------++++++------++++++------++++++------++++++------++++++------++++++
};

void MDIP_InputSource(){
	if (mINPUT_SWITCH >= INPUT_SWITCH_SD && mINPUT_SWITCH <= INPUT_SWITCH_BT){
		g2DIP_ShowBuffer[6] |= 0x0020;	
	}
	else {
		g2DIP_ShowBuffer[6] &= ~0x0020;	
	}
MLOG("Source: %02x %02x", mINPUT_SWITCH, g2DIP_ShowBuffer[6]);
	MDIP_WriteString((char*)&Tab_DIP_InputSwitch[mINPUT_SWITCH * 6]);
}
void MDIP_VideoSrc(){
//	MDIP_WriteString("VIDE0 ");
//	MDIP_SingleChar(5, (gAUD_AutoInputSrc >> 5) + '1');
}


CONST_CHAR Tab_DIP_Surround[] = {
	"  HIFI 2+1CHMODE  "
//	 ++++++------++++++
};

LISTEN_MODE_STATE GetListenModeIndex(BYTE value){
    switch (value & 0x30){                                  // B5:4Ϊ����ģʽ����ѡ��
    case 0x00: return (value & 1) ? LISTEN_MODE_2_1CH : LISTEN_MODE_HIFI;   // ѡ��Ϊ˫������������B0Ϊ0�رճ�������Ϊ1�򿪳�����
    case 0x10: return LISTEN_MODE_SURROUND1;                // ѡ��Ϊ������Դ��ģʽ��û���κζ������㷨
    case 0x20: return (value & 1) ? LISTEN_MODE_SURROUND3 : LISTEN_MODE_SURROUND2;  // ѡ�������ģʽ��B1:0Ϊ���ֲ�ͬ�㷨�Ķ�����ģʽ
    }
    return LISTEN_MODE_SURROUND4;                           // ѡ���������Ч��B1:0Ϊ���ֲ�ͬ�㷨�Ķ�������Ч
}
void MDIP_ListenMode(BYTE value){                           // ��ʾ����ģʽ
	LISTEN_MODE_STATE state = GetListenModeIndex(value);
	
//	MDIP_SurroundSymbol();                                          

    DIP_SURROUND_OFF();
    if (state < LISTEN_MODE_SURROUND1){
        MDIP_WriteString((char*)&Tab_DIP_Surround[state * 6]);
		g2DIP_ShowBuffer[6] |= 0x0001;	
    }else {
        MDIP_WriteString((char*)&Tab_DIP_Surround[LISTEN_MODE_SURROUND1 * 6]);
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
CONST_CHAR Tab_DIP_EqMode[] = {
	" FLAT SOUND "
//	 ++++++------
};
void MDIP_EqSelect(BYTE value){                               // ��ʾEQ������ѡ��
    MDIP_WriteString((char*)&Tab_DIP_EqMode[value ? 6 : 0]);
    if (value){
        MDIP_SingleChar(5, value + '0');
    }
    MDIP_SetState(MENU_EQ_SELECT);
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
    	MDIP_WriteString("      ");
	
    //MDEBUG(0xa8);MDEBUG(gAUD_SrcFormat);	
    	MDIP_SrcFormatSymbol();
    	switch (gAUD_SrcFormat & 0x0f){
    	case KCM_SRC_PCM :
    		MDIP_WriteString("PCM");
    		break;
    	case KCM_SRC_AC3 :                                      // ����AC-3�ź�
    	case KCM_SRC_E_AC3 :                                    // ����Enhanced AC-3�ź�
    	case KCM_SRC_AC3_HD :                                   // ����ű�TRUE HD�ź�    
    		MAPI_COPY_BUFF8(3, &Tab_DIP_SRC_CH[gLocal_1], gLocal_Buffer);
    		MDIP_WriteString(gLocal_Buffer);
    		break;
    	case KCM_SRC_DTS :                                      // ����DTS�ź�
        case KCM_SRC_DTS_CD:				                    // ����DTS CD�ź�
        case KCM_SRC_DTS_ES:				                    // ����DTS Extended Surround�ź�
        case KCM_SRC_DTS_HRA:				                    // ����DTS HD High Resolution Audio�ź�
        case KCM_SRC_DTS_MA:				                    // ����DTS HD Master Audio�ź�
    		MAPI_COPY_BUFF8(3, &Tab_DIP_SRC_CH[gLocal_1], gLocal_Buffer);
    		MDIP_WriteString(gLocal_Buffer);
    		break;
    	case KCM_SRC_LPCM :
    		MDIP_WriteString("2/4");
    		break;
    	}
	}else {
        MDIP_InputSource();    
    }
}
BYTE MDIP_GetSpeakerChar(BYTE index){						// 0ǰ�� 1���� 2������ 3���� 4����
	if (index == 0){										// ǰ��
		return gDIP_SpeakSetup[0] ? 'L' : 'S';				// 0=С���� 1=������
	}
	if (index == 2){										// ������
		return gDIP_SpeakSetup[2] ? 'Y' : 'N';				// 0=û�� 1=��
	}
	if (gDIP_SpeakSetup[index] == 2){						// ������
		return 'L';											
	}
	if (gDIP_SpeakSetup[index] == 1){						// С����
		return 'S';											
	}
	return 'N';												// û�а�װ
}



void MDIP_NightMode(){
	BYTE gLocal_1 = MKCM_ReadRegister(KCM_DYN_COMPRES);

	if (gDIP_MenuSelect == cMenu_NightMode){	
		if (gLocal_1 < 50){                                 
            gLocal_1 = 50;                                  // 50%
        }else {
			gLocal_1 = 0;                                   // 0%
		}

		MKCM_WriteRegister(KCM_DYN_COMPRES, gLocal_1); 
	}
	if (gLocal_1){
		MDIP_WriteString("NIG-ON");
	}
	else {
		MDIP_WriteString("NIG-OF");
	}
}
CONST_CHAR Tab_DIP_ExtrInOut[] = {
	"SD-IN UD-IN SD-OUTUD-OUTPC-OUTBT-OUTHD-OUT"
//	 ++++++------++++++------++++++------++++++
};

void MDIP_InsertRemove(BYTE type){						// ��ʾ������Դ����/��� 
	gDIP_MenuTimer = 50;
//MDEBUG(0xd9);MDEBUG(type);
    MDIP_WriteString((char*)&Tab_DIP_ExtrInOut[type * 6]);
}
void MDIP_PlayTrack(){
    BYTE temp;

    gDIP_MenuTimer = 20;
    g2DIP_ShowBuffer[6] &= ~0x4000;                     // :
    MDIP_WriteString("      ");
    temp = MDIP_WriteDec(0, g2PlayIndex+1);
    MDIP_SingleChar(temp++, '/');
    MDIP_WriteDec(temp, (mINPUT_SWITCH == INPUT_SWITCH_SD) ? g2SdQty : g2UDiskQty);
}
CONST_CHAR Tab_DIP_Bps[] = {
    "32K48K56K64K80K96K112128160192224256320384"
//	 +++---+++---+++---+++---+++---+++---+++---
	
};
void MDIP_PlayTime(){
    if (g2PlayTime < 3){
        MDIP_PlayTrack();
        return;
    }
    if (g2PlayTime < 5){
        BYTE temp = gAUD_BpsRate >> 3;
        if ((gAUD_SrcFormat & 0x0f) == KCM_SRC_MP3){
            gDIP_MenuTimer = 20;
            if (temp > 0 && temp < 16){
                BYTE gLocal_Buffer[6];
                MAPI_COPY_BUFF8(3, "MP3", gLocal_Buffer);
                MAPI_COPY_BUFF8(3, &Tab_DIP_Bps[(temp-1) * 3], &gLocal_Buffer[3]);
                MDIP_WriteString((char*)gLocal_Buffer);
                return;
            }
        }
    }
	MDIP_WriteString("  ");
    MDIP_Write2Digit(2, g2PlayTime/60);
    MDIP_Write2Digit(4, g2PlayTime%60);
    g2DIP_ShowBuffer[6] |= 0x4000;                          // :
}




void MDIP_AdjDelayTime(BYTE index, BYTE mode){				// 0=LINSYNC 1ǰ�� 2���� 3���� 4���� 
	BYTE gLocal_1;
	switch (index == 0){
	case 0: gLocal_1 = 19; break;
	case 1: gLocal_1 = 9; break;
	case 2: gLocal_1 = 9; break;
	case 3: gLocal_1 = 9; break;
	case 4: gLocal_1 = 9; break;
	}
	if (mode == 4){	   											// mode 0һ��ģʽ 1��˸���� 2��˸Ϩ�� 3����- 4����+
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
    MDIP_WriteString("FWDONE");
}
  */


void MDIP_FirewareInfo(){				                    // ��ʾ�̼�����
    BYTE tmpData[10];
    MKCM_ReadXByte(KCM_RD_INFO, tmpData, 2);
    if (tmpData[0] == 100){
        MDIP_WriteString("FWDONE");
    }else if (tmpData[0] == 104){
        MDIP_WriteString("FW Err");
    }else {
        MDIP_WriteString("FW -");
        MDIP_Write2Digit(4, tmpData[0]);
    }
    MDIP_SetState(MENU_FIREWARE_INFO);
    gDIP_MenuTimer = 60;
}
void MDIP_ShowNegative9(BYTE value){						// ����ʾ��������ֵ�λ����ʾ-9��+9����ֵ
	if (value < 9){											// ����
		MDIP_SingleChar(4, '-');
		MDIP_SingleChar(5, (9 - value) + '0');
	}else {													// 0������
		MDIP_SingleChar(4, ' ');
		MDIP_SingleChar(5, (value - 9) + '0');
	}
}
void MDIP_SetUpDown(BYTE directUp, BYTE upMax, BYTE* value){	// ��ʾ�Ӽ�����
    if (directUp){                                          // ����ֵ
        if (*value < upMax){               					// ��û�е������ֵ
            *value += 1;									// ���Լ�����
		}
    }else {													// ����ֵ
        if (*value > 0){									// ��û�е�����Сֵ
            *value -= 1;									// ���Լ�����
        }
    }

} 