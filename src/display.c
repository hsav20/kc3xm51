
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           

void MDIP_Initialize(){										  
    BYTE gLocal_0;

	HAL_DIP_LED(0);
    HAL_DIP_STB(0);											// TA6932Ƭѡ
    MUSDELAY(1);
	MDIP_WriteByte(0x40);		  							// ��ʼ���ϵ�ʱ����ַ������1
    MUSDELAY(1);
    HAL_DIP_STB(1);    

    MUSDELAY(1);
    HAL_DIP_STB(0);
    MUSDELAY(1);
	MDIP_WriteByte(0xc0);		  							// �����ַ 
    MUSDELAY(1);
	gLocal_0 = 0;
	do{
		MDIP_WriteByte(0x00);		  						// ����1
	    MUSDELAY(1);
	}while(++gLocal_0 < 16);
	MUSDELAY(10);
    HAL_DIP_STB(1);    

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
			MDIP_MenuSelect(cMenu_AudioMute, 0);
		}
		if (!FSYS_Standby && gDIP_MenuTimer){
			if (--gDIP_MenuTimer == 0) {
            	if (!FSYS_TestTone){
					MDIP_MenuSelect(cMenu_Restore, 0);
				}
				else {                                      // ����������֮��
					BYTE channel = MDIP_GetNextChannel(gDIP_MenuSelect - cMenu_TToneFL);
					MDIP_MenuSelect(channel + cMenu_TToneFL, MENU_NORMAL);
                    MAUD_TestToneChannel(channel);
//MDEBUG(0xc7);MDEBUG(gDIP_MenuSelect);MDEBUG(gDIP_MenuTimer);
				}
			}
			else if (FDIP_MenuTwinkle){						// �˵���Ҫ��˸
				if ((gDIP_MenuTimer % 5) == 0){	  			// ÿ0.5������˸
					BYTE twinkle = (gDIP_MenuTimer / 5);
					MDIP_MenuSelect(gDIP_MenuSelect, (twinkle & 0x01) ? 2 : 1);
				}
			}
        }
		if (!FSYS_Standby && !FSYS_MuteEnable && !FSYS_TestTone && FDIP_FreqSymbol && gAUD_SrcFormat > KCM_SRC_NOS){
            MDIP_ReadSpectrum();
		}

	}
    return;
}
void MDIP_MenuSelect(BYTE index, MENU_MODE mode){			// �˵�ѡ��
	if (mode == MENU_NORMAL){								// ��������ʾģʽ
		gDIP_MenuTimer = 20;
		FDIP_MenuTwinkle = 0;
	}
	FDIP_FreqSymbol = 0;
    HAL_DIP_LED(0);
	MDIP_ClearSpectrum();
    
    DIP_DOT_OFF();
	
//g2DIP_ShowBuffer[6] &= ~(0x0700 | 0x0004);
	switch (index){
	case cMenu_Restore:
		if (!FKCM_I2C_Error){
			MDIP_InputSource();
		}
		else {
			MDIP_WriteString("KCM-NO");
		}
		gDIP_MenuSelect = 0;
		gDIP_MenuTimer = 0;
		FDIP_FreqSymbol = 1;
		break;
	case cMenu_InputSource:
		MDIP_InputSource();
		break;
	case cMenu_Standby:
		MDIP_WriteString(" -OFF-");
		break;
	case cMenu_PowerOn:
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
	case cMenu_Surround2Ch:
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
	case cMenu_VideoSrc:
		gDIP_MenuTimer = 50;
		MDIP_VideoSrc();
		break;
	case cMenu_SrcFormat:
		MDIP_SourceFormat();
		break;
	case cMenu_NightMode:
		MDIP_NightMode();
		break;
	case cMenu_NoiseSignal:
		MDIP_NoiseSignal();
		break;
	case cMenu_Fireware:                                    // ��ʾ�̼�����
		MDIP_Fireware();
		break;
    case cMenu_SdInsert:                                   // ��ʾSD����
    case cMenu_SdRemove:                                   // ��ʾSD���
    case cMenu_UDiskInsert:                                // ��ʾU�̲���
    case cMenu_UDiskRemove:                                // ��ʾU�̲��
		MDIP_DiskInOut(index - cMenu_SdInsert);
		break;
    case cMenu_PlayTrack:                                  // ��ʾ��ý���ļ���Ϣ
        MDIP_PlayTrack();
		break;
    case cMenu_PlayTime:                                   // ��ʾ��ý�岥��ʱ��
		gDIP_MenuTimer = 20;
        MDIP_PlayTime();
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
	case cMenu_MasterVolume:
		if (mode >= 3){										// 3����- 4����+
			if (gDIP_MenuSelect == cMenu_MasterVolume){
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

	case cMenu_TToneFL:                                     // ��������ǰ��������
	case cMenu_TToneCE:                                     // ����������������
	case cMenu_TToneSW:                                     // ������������������
	case cMenu_TToneFR:                                     // ��������ǰ��������
	case cMenu_TToneSR:                                     // ������������������
	case cMenu_TToneBR:                                     // ������������������
	case cMenu_TToneBL:                                     // ������������������
	case cMenu_TToneSL:                                     // ������������������
//MDEBUG(0xb2);MDEBUG(index);
        MDIP_TestTone(index, mode);                         // ����΢����������
        break;
	case cMenu_ChTrimFL:                                    // ����΢��ǰ��������
	case cMenu_ChTrimCE:                                    // ����΢����������
	case cMenu_ChTrimSW:                                    // ����΢������������
	case cMenu_ChTrimFR:                                    // ����΢��ǰ��������
	case cMenu_ChTrimSR:                                    // ����΢������������
	case cMenu_ChTrimBR:                                    // ����΢������������
	case cMenu_ChTrimBL:                                    // ����΢������������
	case cMenu_ChTrimSL:                                    // ����΢������������
        MAUD_AutoCanclTestTone();
		MDIP_TrimControl(index, mode);                      // ����΢����������
		break;

	case cMenu_MicVolume1:                                  // ��Ͳ1����
    case cMenu_MicVolume2:                                  // ��Ͳ2����
    case cMenu_MicEcho:                                     // ��Ͳ��������
    case cMenu_MicRepeat:                                   // ��Ͳ�ظ�����
    case cMenu_MicDelay:                                    // ��Ͳ�ӳ�ʱ��
    case cMenu_MicReverb:                                   // ��Ͳ�������
        MAUD_AutoCanclTestTone();
		MDIP_MicControl(index, mode);                       // ��Ͳ���ֲ�������
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

void MDIP_TestTone(BYTE index, MENU_MODE mode){             // ����΢����������
	if (mode >= MENU_ADJ_DOWN){							    // MENU_ADJ_DOWN��MENU_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // �ϴ��Ѿ����뵱ǰ�˵�
		    MAUD_MixTrimAdjust(index - cMenu_TToneFL, (mode == MENU_ADJ_UP) ? 1 : 0); 	
			gDIP_MenuTimer = 80;
		}
	}
    index -= cMenu_TToneFL;                             // ������΢��ǰ����������ʼ
    MDIP_WriteString((char*)&Tab_DIP_TToneShow[index * 4]);
	if (mode == MENU_TWINKLE_OFF){							// MENU_TWINKLE_OFF��˸Ϩ��
		MDIP_SingleChar(4, ' ');
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_NORMAL��MENU_TWINKLE_ON��˸����
    	BYTE value = gDIP_TrimCtrl[index];
//if(mode!=MENU_TWINKLE_ON&&mode!=MENU_TWINKLE_OFF){MDEBUG(0xee);MDEBUG(index);MDEBUG(value);}
        if (value < 9){
            MDIP_SingleChar(4, '-');
            MDIP_SingleChar(5, (9 - value) + '0');
        }else {
            MDIP_SingleChar(4, ' ');
            MDIP_SingleChar(5, (value - 9) + '0');
        }
		FDIP_MenuTwinkle = 1;
	}
}

CONST_CHAR Tab_DIP_TrimShow[] = {
	"FLTrCETrSWTrFRTrSRTrBRTrBLTrSLTr"
//	 ++++----++++----++++----++++----
};
void MDIP_TrimControl(BYTE index, MENU_MODE mode){          // ����΢����������
	if (mode >= MENU_ADJ_DOWN){							    // MENU_ADJ_DOWN��MENU_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // �ϴ��Ѿ����뵱ǰ�˵�
            MAUD_MixTrimAdjust(index - cMenu_ChTrimFL, (mode == MENU_ADJ_UP) ? 1 : 0); 	
			gDIP_MenuTimer = 80;
		}
	}
    index -= cMenu_ChTrimFL;                            // ������΢��ǰ����������ʼ
    MDIP_WriteString((char*)&Tab_DIP_TrimShow[index * 4]);
	if (mode == MENU_TWINKLE_OFF){							// MENU_TWINKLE_OFF��˸Ϩ��
		MDIP_SingleChar(4, ' ');
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_NORMAL��MENU_TWINKLE_ON��˸����
    	BYTE value = gDIP_TrimCtrl[index];
//if(mode!=MENU_TWINKLE_ON&&mode!=MENU_TWINKLE_OFF){MDEBUG(0xee);MDEBUG(index);MDEBUG(value);}
        if (value < 9){
            MDIP_SingleChar(4, '-');
            MDIP_SingleChar(5, (9 - value) + '0');
        }else {
            MDIP_SingleChar(4, ' ');
            MDIP_SingleChar(5, (value - 9) + '0');
        }
		if (mode == MENU_NORMAL){                           // ��������ʾģʽ
			gDIP_MenuTimer = 80;
			FDIP_MenuTwinkle = 1;
		}
       
	}
}
// ��Ͳ1���� ��Ͳ2���� ��Ͳ�������� ��Ͳ�ظ����� ��Ͳ�ӳ�ʱ�� ��Ͳ�������
CONST_CHAR Tab_DIP_MicShow[] = {
	"MIC1MIC2ECHOREPTDELYREVB"
//	 ++++----++++----++++----
};
void MDIP_MicControl(BYTE index, MENU_MODE mode){
//if(mode!=MENU_TWINKLE_ON&&mode!=MENU_TWINKLE_OFF){MDEBUG(0xee);MDEBUG(index);MDEBUG(mode);}
	if (mode >= MENU_ADJ_DOWN){							    // MENU_ADJ_DOWN��MENU_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // �ϴ��Ѿ����뵱ǰ�˵�
			MAUD_MixMicAdjust(index, (mode == MENU_ADJ_UP) ? 1 : 0); 	
			gDIP_MenuTimer = 80;
		}
	}
    index -= cMenu_MicVolume1;                              // �ӻ�Ͳ1������ʼ
	MDIP_WriteString((char*)&Tab_DIP_MicShow[index * 4]);
    MDIP_SingleChar(4, ' ');

	if (mode == MENU_TWINKLE_OFF){							// MENU_TWINKLE_OFF��˸Ϩ��
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_NORMAL��MENU_TWINKLE_ON��˸����
    	BYTE gLocal_1 = gDIP_MicCtrl[index];
        MDIP_SingleChar(5, gLocal_1 + '0');
		if (mode == MENU_NORMAL){                           // ��������ʾģʽ
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
	"AUX-INOPTICACOA-D1COA-D2 -SD-  -USB-HDMI-1HDMI-2HDMI-3HDMI-A"
//	 ++++++------++++++------++++++------++++++------++++++------
};

void MDIP_InputSource(){
	BYTE index = (gSUB_SrcAuto == INPUT_SWITCH_NONE) ? mINPUT_SWITCH : gSUB_SrcAuto;
    MLOG("InSource:%d %d %d\r\n", (u32)index, (u32)gSUB_SrcAuto, (u32)mINPUT_SWITCH);
	if (index == INPUT_SWITCH_SD || index == INPUT_SWITCH_UDISK){
		g2DIP_ShowBuffer[6] |= 0x0020;	
	}
	else {
		g2DIP_ShowBuffer[6] &= ~0x0020;	
	}

	MDIP_WriteString((char*)&Tab_DIP_InputSwitch[index * 6]);
}
void MDIP_VideoSrc(){
	MDIP_WriteString("VIDE0 ");
	MDIP_SingleChar(5, (gAUD_AutoInputSrc >> 5) + '1');
}



CONST_CHAR Tab_DIP_Surround2Ch[] = {
	"  HIFI 2+1CH"
//	 ++++++------
};
CONST_CHAR Tab_DIP_Surround8Ch[] = {
	"MODE"
};
void MDIP_SurroundMode(BYTE index, MENU_MODE mode){
	// [6] B15:D1 B14:D1 B13:D1 B12:D1 B11:WIFI B10:LPCM B9:HD    B8:DTS  B7:PROLOGIC B6:NEO6  B5:��  B4:DSP B3:ATMOS B2:DD  B1:AUTO B0:ST
//MDEBUG(0x99);MDEBUG(change);MDEBUG(gDIP_MenuSelect);MDEBUG(index);
    if (index == gDIP_MenuSelect){                          // �ϴ��Ѿ����뵱ǰ�˵�
        if (index == cMenu_Surround2Ch){                    // ������
    		gDIP_Surround[0] ^= 1;
        	MKCM_WriteRegister(KCM_LISTEN_MODE, gDIP_Surround[0]);

            MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_SURROUND_2CH, gDIP_Surround[0]);
            if (gDIP_Select2Ch != 1){
                gDIP_Select2Ch = 1;
                MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_SELECT_2CH, 1);
            }
        }else {                                             // ������
            if (++gDIP_Surround[1] >= 4){
                gDIP_Surround[1] = 0;
            }
//MDEBUG(0x99);MDEBUG(gDIP_Surround[1]);
            if (gDIP_Surround[1] < 3){
                MKCM_WriteRegister(KCM_LISTEN_MODE, 0x10 | gDIP_Surround[1]);
            }else {
                MKCM_WriteRegister(KCM_LISTEN_MODE, 0x30);  // ѡ���������Ч��B1:0Ϊ���ֲ�ͬ�㷨�Ķ�������Ч
            }
            MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_SURROUND_8CH, gDIP_Surround[1]);
            if (gDIP_Select2Ch != 0){
                gDIP_Select2Ch = 0;
                MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_SELECT_2CH, 0);
            }
        }
    }else if (gDIP_MenuSelect == cMenu_Surround8Ch && index == cMenu_Surround2Ch){  // �ϴ��Ƕ���������ǰΪ������
        MKCM_WriteRegister(KCM_LISTEN_MODE, gDIP_Surround[0]);
        gDIP_Select2Ch = 1;
        MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_SELECT_2CH, 1);
    }else if (gDIP_MenuSelect == cMenu_Surround2Ch && index == cMenu_Surround8Ch){  // �ϴ�������������ǰΪ������
        if (gDIP_Surround[1] < 3){
            MKCM_WriteRegister(KCM_LISTEN_MODE, 0x10 | gDIP_Surround[1]);
        }else {
            MKCM_WriteRegister(KCM_LISTEN_MODE, 0x30);      // ѡ���������Ч��B1:0Ϊ���ֲ�ͬ�㷨�Ķ�������Ч
        }
        gDIP_Select2Ch = 0;
        MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_SELECT_2CH, 0);
    }

    if (index == cMenu_Surround2Ch){
		MDIP_WriteString((char*)&Tab_DIP_Surround2Ch[gDIP_Surround[0] * 6]);
    }else {
        MDIP_WriteString((char*)Tab_DIP_Surround8Ch);
        MDIP_SingleChar(4, ' ');
        MDIP_SingleChar(5, gDIP_Surround[1] + '1');
    }
	MDIP_SurroundSymbol();
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


void MDIP_SoundEffect(BYTE mode){
	if (mode >= 3){
		if (gDIP_MenuSelect == cMenu_SoundEffect){      
			MAUD_MixSoundEffect();
		}
	}
    if (gDIP_SoundEffect == 0){
    	MDIP_WriteString(" FLAT ");
    }else {
    	MDIP_WriteString("SOUND");
    	MDIP_SingleChar(5, gDIP_SoundEffect + '0');
    }
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
void MDIP_Fireware(){
	gDIP_MenuTimer = 50;
    MDIP_WriteString("FWDONE");
}


CONST_CHAR Tab_DIP_DiskInOut[] = {
	"SD-IN SD-OUTUD-IN UD-OUT"
//	 ++++++------++++++------
};

void MDIP_DiskInOut(BYTE type){
	gDIP_MenuTimer = 50;
//MDEBUG(0xd9);MDEBUG(type);
    MDIP_WriteString((char*)&Tab_DIP_DiskInOut[type * 6]);
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
    gDIP_MenuTimer = 10;
	MDIP_WriteString("  ");
    MDIP_Write2Digit(2, g2PlayTime/60);
    MDIP_Write2Digit(4, g2PlayTime%60);
    g2DIP_ShowBuffer[6] |= 0x4000;                          // :
}


CONST_CHAR Tab_DIP_Brightness[] = {
	0x00,0x02,0x07,  
};
void MDIP_Brightness(BYTE show, BYTE bright){
	if (show > 0){
		MDIP_WriteString("BRIGH");
		MDIP_SingleChar(5, bright + '1');
	}

    HAL_DIP_STB(0);
    MUSDELAY(1);
	MDIP_WriteByte(0x88 | Tab_DIP_Brightness[bright]);		  					// ��ʾ������
    MUSDELAY(1);
    HAL_DIP_STB(1);    
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

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MDIP_SurroundSymbol(){
    BYTE surround = gDIP_Select2Ch ? gDIP_Surround[0] : (2 + gDIP_Surround[1]);
	//g2DIP_ShowBuffer[6] &= ~(0x0001 | 0x0002 | 0x0080 | 0x0040 | 0x0010);	
    DIP_SURROUND_OFF();
	switch (surround){
	case cDIP_SURR_STEREO :
	case cDIP_SURR_ST_SW :
		g2DIP_ShowBuffer[6] |= 0x0001;	
		break;
	case cDIP_SURR_MODE1 :
		g2DIP_ShowBuffer[6] |= 0x0002;	
		break;
	case cDIP_SURR_MODE2 :
		g2DIP_ShowBuffer[6] |= 0x0080;	
		break;
	case cDIP_SURR_MODE3 :
		g2DIP_ShowBuffer[6] |= 0x0040;	
		break;
	case cDIP_SURR_MODE4 :
		g2DIP_ShowBuffer[6] |= 0x0010;	
		break;
	}
	FDIP_ScreenUpdata = 1;
}
void MDIP_SrcFormatSymbol(){
	DIP_SRC_FORMAT_OFF();
	//g2DIP_ShowBuffer[6] &= ~(0x0700 | 0x0004);	
	// ��[6] B15:D1 B14:D1 B13:D1 B12:D1 B11:WIFI B10:LPCM B9:HD    B8:DTS  B7:PROLOGIC B6:NEO6  B5:��  B4:DSP B3:ATMOS B2:DD  B1:AUTO B0:ST
	// ��[6] B15:D1 B14:D1 B13:D1 B12:D1 B11:WIFI B10:��   B9:PAUSE B8:PLAY B7:RPALL    B6:RP1   B5:DTS B4:DD  B3:AUTO  B2:DSP B1:PLII B0:ST
	
//MDEBUG(0xa9);MDEBUG(gSYS_ModelType);MDEBUG(gAUD_SrcFormat);
	switch (gAUD_SrcFormat & 0x0f){
	case KCM_SRC_PCM :
		if (gSYS_ModelType != KCM_MODEL_35H && gSYS_ModelType != KCM_MODEL_36H){
			g2DIP_ShowBuffer[6] |= 0x0400;	
		}
		break;
	case KCM_SRC_AC3 :                                      // ����AC-3�ź�
	case KCM_SRC_E_AC3 :                                    // ����Enhanced AC-3�ź�
	case KCM_SRC_AC3_HD :                                   // ����ű�TRUE HD�ź�    
		g2DIP_ShowBuffer[6] |= 0x0004;	
		break;
	case KCM_SRC_DTS :                                      // ����DTS�ź�
    case KCM_SRC_DTS_CD:				                    // ����DTS CD�ź�
    case KCM_SRC_DTS_ES:				                    // ����DTS Extended Surround�ź�
    case KCM_SRC_DTS_HRA:				                    // ����DTS HD High Resolution Audio�ź�
    case KCM_SRC_DTS_MA:				                    // ����DTS HD Master Audio�ź�
		g2DIP_ShowBuffer[6] |= 0x0100;	
		break;
	case KCM_SRC_LPCM :
		g2DIP_ShowBuffer[6] |= 0x0400;	
		break;
	}
	FDIP_ScreenUpdata = 1;
}
void MDIP_WifiSymbol(BYTE turnOn){
	if (turnOn == 0xff){
		turnOn = (MKCM_ReadRegister(KCM_WIFI_STATUS) & 0x80) ? 1 : 0;
	}
	if (turnOn){
		g2DIP_ShowBuffer[6] |= 0x0800;	
	}
	else {
		g2DIP_ShowBuffer[6] &= ~0x0800;	
	}
	FDIP_ScreenUpdata = 1;
}

void MDIP_WriteString(char* string){
	BYTE gLocal_1;
	char counter = 0;
	do {
		gLocal_1 = *string++;
		if (gLocal_1 == 0){
			break;
		}
		MDIP_SingleChar(counter, gLocal_1);
	} while (++counter < 6);
}
BYTE MDIP_WriteDec(BYTE number, WORD value){                // ��ʾʮ��������������һ���ֿ�ʼ��λ��
    if (value / 10000){
        MDIP_SingleChar(number++, (value / 10000) + '0');
        value = value % 10000;
    }
    if (value / 1000){
        MDIP_SingleChar(number++, (value / 1000) + '0');
        value = value % 1000;
    }
    if (value / 100){
        MDIP_SingleChar(number++, (value / 100) + '0');
        value = value % 100;
    }
    if (value / 10){
        MDIP_SingleChar(number++, (value / 10) + '0');
        value = value % 10;
    }
    MDIP_SingleChar(number++, value + '0');
    return number;
}
void MDIP_Write2Digit(BYTE number, BYTE value){             // ��ʾ2������
	MDIP_SingleChar(number, (value / 10) + '0');
	MDIP_SingleChar(number+1, (value % 10) + '0');
}
/*
void MDIP_WriteHex(BYTE number, BYTE value){                // ��ʾ�������Ƶ���
    MDIP_SingleChar(number, MDIP_Hex2String(value >> 4));
	MDIP_SingleChar(number+1, MDIP_Hex2String(value & 0x0f));
} 
char MDIP_Hex2String(BYTE hex){                             // ����ʮ�����Ƶ�ֵ
    if (hex > 9){
        return hex - 10 + 'A';
    }
    return hex + '0';
}
*/

CONST_CHAR TabDIPmiLED_BASE[] = { // ���ֹ�
    0x00,0x00,         // ' '
    0x24,0x3f,         // '0'
    0x00,0x06,         // '1'		   		____A____  
    0x00,0xdb,         //  2		       |\	|  	/| 	  A 0x0001 B 0x0002	C  0x0004 D  0x0008 
    0x00,0xcf,         //  3		       | H	J  K | 	  E 0x0010 F 0x0020 G1 0x0040 G2 0x0080
    0x00,0xe6,         //  4		       F  \	| /  B 	  H 0x0100 J 0x0200 K  0x0400 L  0x0800
    0x00,0xed,         //  5		       | 	|	 | 	  M 0x1000 N 0x2000
    0x00,0xfd,         //  6		       |_G1___G2_| 
    0x00,0x07,         //  7		       |		 | 
    0x00,0xff,         //  8		       |  	|    | 
    0x00,0xef,         //  9		       E  /	M \  C 
    0x00,0xf7,         //  'A'		       | N	|  L | 
    0x12,0x8f,         //  'B'		       |/___D___\| 
    0x00,0x39,         //  'C'		                  
    0x12,0x0f,         //  'D'
    0x00,0xf9,         //  'E'
    0x00,0xf1,         //  'F'
    0x00,0xbd,         //  'G'
    0x00,0xf6,         //  'H'
    0x12,0x09,         //  'I'
    0x00,0x1e,         //  'J' 
    0x0C,0x70,         //  'K'
    0x00,0x38,         //  'L'
    0x05,0x36,         //  'M'
    0x09,0x36,         //  'N'
    0x00,0x3f,         //  'O'
    0x00,0xf3,         //  'P'
    0x08,0x3f,         //  'Q'
    0x08,0xf3,         //  'R'
    0x00,0xed,         //  'S'
    0x12,0x01,         //  'T'
    0x00,0x3e,         //  'U'
    0x24,0x30,         //  'V'
    0x28,0x36,         //  'W'
    0x2d,0x00,         //  'X'
    0x00,0xee,         //  'y'
    0x24,0x09,         //  'Z'
    0x00,0xc0,         //  '-'
    0x24,0x00,         //  '/'
    0x12,0xc0,         //  '+'
};  						 

void MDIP_SingleChar(
					BYTE gLocal_1,		  			// 0-2 (���ֹ�) 3-5 (8�ֹ�) 6 (STEREO-WIFI+3��DOT) 7-12 (Ƶ�׵�1-6��)
					BYTE gLocal_2){			 
	BYTE gLocal_Tab;

	if (gLocal_1 < 3){ 								// 0-2 (���ֹ�)
	    if ((gLocal_2 >= '0') && (gLocal_2 <= '9')) {
	       gLocal_2 -= '0';
	       gLocal_Tab = gLocal_2+1;
	    }
	    else if ((gLocal_2 >= 'a') && (gLocal_2 <= 'z')){
	        gLocal_Tab = gLocal_2 - 'a' + 0x0b;            // ���д
	    }
	    else if ((gLocal_2 >= 'A') && (gLocal_2 <= 'Z')){
	        gLocal_Tab = (gLocal_2 - 'A') + 0x0b; // 0x14
	    }
	    else {
			switch (gLocal_2){
			case '-': gLocal_Tab = 37; break;
			case '/': gLocal_Tab = 38; break;
			case '+': gLocal_Tab = 39; break;
	    	default :
	    		gLocal_Tab = 0; 
				break;
			}
		}
		g2DIP_ShowBuffer[gLocal_1] = ((WORD)TabDIPmiLED_BASE[gLocal_Tab*2])<<8;
		g2DIP_ShowBuffer[gLocal_1] |= TabDIPmiLED_BASE[gLocal_Tab*2+1];
	}
	else if (gLocal_1 < 6){ 								// 3-5 (8�ֹ�) 
	    if ((gLocal_2 >= '0') && (gLocal_2 <= '9')) {
	       gLocal_2 -= '0';
	       gLocal_Tab = gLocal_2+1;
			g2DIP_ShowBuffer[gLocal_1] = ((WORD)TabDIPmiLED_BASE[gLocal_Tab*2])<<8;
			g2DIP_ShowBuffer[gLocal_1] |= TabDIPmiLED_BASE[gLocal_Tab*2+1];
	    }
	    else {
			switch (gLocal_2){
			case '-': gLocal_2 = 0xc0; break;
			case 'A': gLocal_2 = 0xf7; break;
			case 'B': gLocal_2 = 0x7c; break;
			case 'C': gLocal_2 = 0x39; break;
			case 'D': gLocal_2 = 0xde; break;
			case 'E': gLocal_2 = 0xf9; break;
			case 'F': gLocal_2 = 0xf1; break;
			case 'G': gLocal_2 = 0xef; break;
			case 'I': gLocal_2 = 0x06; break;
			case 'H': gLocal_2 = 0xf6; break;
			case 'L': gLocal_2 = 0x38; break;
			case 'N': gLocal_2 = 0x37; break;
			case 'O': gLocal_2 = 0x3f; break;
			case 'P': gLocal_2 = 0xf3; break;
			case 'R':
			case 'r': gLocal_2 = 0x50; break;
			case 'S': gLocal_2 = 0xed; break;
			case 'T':
			case 't': gLocal_2 = 0xf8; break;
			case 'U': gLocal_2 = 0x3e; break;
			case 'Y': gLocal_2 = 0x6e; break;
	    	default :
				gLocal_2 = 0x0000;
				break;
			}
			g2DIP_ShowBuffer[gLocal_1] = gLocal_2;

		}
	}
	else {
		if(gLocal_1 > 6 && gLocal_1 < 13){ // ����
		    if ((gLocal_2 >= 1) && (gLocal_2 <= 5)) {
		       gLocal_Tab = gLocal_2+10+3+26-1;
		    }
		}
		else {
		}

		g2DIP_ShowBuffer[gLocal_1] = ((WORD)TabDIPmiLED_BASE[gLocal_Tab*2])<<8;
		g2DIP_ShowBuffer[gLocal_1] |= TabDIPmiLED_BASE[gLocal_Tab*2+1];
	//	g2DIP_ShowBuffer[gLocal_1]=0xffff;
//MDEBUG(0xbc);MDEBUG(gLocal_Tab);MDEBUG(g2DIP_ShowBuffer[gLocal_1]>>8);MDEBUG(g2DIP_ShowBuffer[gLocal_1]);
	}
	FDIP_ScreenUpdata = 1;
    return;
}
CONST_CHAR Tab_DIP_Spectrum1[] = {
	// 			   �ظ�		 �ظ�
	0x00,0x10,0x18,0x1c,0x1c,0x1e,0x1e,0x1f,
};

void MDIP_ClearSpectrum(){
	BYTE gLocal_1 = 0;
    do {
		g2DIP_ShowBuffer[8 + gLocal_1] = 0;
    } while (++gLocal_1 < 5);
}

BYTE MDIP_GetLevel(BYTE level){
//MDEBUG(0xaa);MDEBUG(level& 0x07);
    return Tab_DIP_Spectrum1[level & 0x07];
}
void MDIP_ReadSpectrum(){
	BYTE counter;
	BYTE outData[3];

	// 5��Ƶ�ף��ֽ�0��B2:0Ϊ��1�У�B5:3Ϊ��2�У��ֽ�1�� B2:0Ϊ��3�У�B5:3Ϊ��4�У��ֽ�2�� B2:0Ϊ��5��
	MKCM_ReadXByte(KCM_RD_SPECTRUM, 3, outData);
//MDEBUG(0xaa);/*MDEBUG(outData[0]);MDEBUG(outData[1]);*/MDEBUG(outData[2]);
	if (FKCM_I2C_Error){
		MDIP_ClearSpectrum();
		return;
	}
    g2DIP_ShowBuffer[8 + 0] = MDIP_GetLevel(outData[0]);
    g2DIP_ShowBuffer[8 + 1] = MDIP_GetLevel(outData[0]>>3);
    g2DIP_ShowBuffer[8 + 2] = MDIP_GetLevel(outData[1]);
    g2DIP_ShowBuffer[8 + 3] = MDIP_GetLevel(outData[1]>>3);
    g2DIP_ShowBuffer[8 + 4] = MDIP_GetLevel(outData[2]);
    FDIP_ScreenUpdata = 1;
}
void MDIP_CleanSpectrum(){
	BYTE gLocal_1;
    gLocal_1 = 0;
    do {
		g2DIP_ShowBuffer[8] = 0;
    } while (++gLocal_1 < 5);
	FDIP_ScreenUpdata = 1;
}

void MDIP_ScreenFill(BYTE value){							// ���������ʾԪ�ص���ֵ��0x00Ϊȫ��Ϩ��
	BYTE gLocal_1;
    gLocal_1 = 0;
    do{
		g2DIP_ShowBuffer[gLocal_1] = value;    
    } while (++gLocal_1 < 13);
}
void MDIP_ScreenUpdata(){    
	BYTE gLocal_0;
	BYTE gLocal_1;

    gLocal_1 = 0;
    do{
		gDIP_Buffer[gLocal_1] = 0;    
    } while (++gLocal_1 < 16);    						  

	// ���ֹ�1
	if (g2DIP_ShowBuffer[0] & 0x0001) gDIP_Buffer[14] |= 0x08;
	if (g2DIP_ShowBuffer[0] & 0x0002) gDIP_Buffer[7]  |= 0x08;
	if (g2DIP_ShowBuffer[0] & 0x0004) gDIP_Buffer[0]  |= 0x08;
	if (g2DIP_ShowBuffer[0] & 0x0008) gDIP_Buffer[3]  |= 0x08;
	if (g2DIP_ShowBuffer[0] & 0x0010) gDIP_Buffer[11] |= 0x08;
	if (g2DIP_ShowBuffer[0] & 0x0020) gDIP_Buffer[15] |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0040) gDIP_Buffer[9]  |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0080) gDIP_Buffer[6]  |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0100) gDIP_Buffer[13] |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0200) gDIP_Buffer[12] |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0400) gDIP_Buffer[5]  |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0800) gDIP_Buffer[1]  |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x1000) gDIP_Buffer[2]  |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x2000) gDIP_Buffer[8]  |= 0x08;

	// ���ֹ�2
	if (g2DIP_ShowBuffer[1] & 0x0001) gDIP_Buffer[14] |= 0x20;
	if (g2DIP_ShowBuffer[1] & 0x0002) gDIP_Buffer[7]  |= 0x20;
	if (g2DIP_ShowBuffer[1] & 0x0004) gDIP_Buffer[0]  |= 0x20;
	if (g2DIP_ShowBuffer[1] & 0x0008) gDIP_Buffer[3]  |= 0x20;
	if (g2DIP_ShowBuffer[1] & 0x0010) gDIP_Buffer[11] |= 0x20;
	if (g2DIP_ShowBuffer[1] & 0x0020) gDIP_Buffer[15] |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0040) gDIP_Buffer[9]  |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0080) gDIP_Buffer[6]  |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0100) gDIP_Buffer[13] |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0200) gDIP_Buffer[12] |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0400) gDIP_Buffer[5]  |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0800) gDIP_Buffer[1]  |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x1000) gDIP_Buffer[2]  |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x2000) gDIP_Buffer[8]  |= 0x20;

	// ���ֹ�3
	if (g2DIP_ShowBuffer[2] & 0x0001) gDIP_Buffer[14] |= 0x10;
	if (g2DIP_ShowBuffer[2] & 0x0002) gDIP_Buffer[7]  |= 0x10;
	if (g2DIP_ShowBuffer[2] & 0x0004) gDIP_Buffer[0]  |= 0x10;
	if (g2DIP_ShowBuffer[2] & 0x0008) gDIP_Buffer[3]  |= 0x10;
	if (g2DIP_ShowBuffer[2] & 0x0010) gDIP_Buffer[11] |= 0x10;
	if (g2DIP_ShowBuffer[2] & 0x0020) gDIP_Buffer[15] |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0040) gDIP_Buffer[9]  |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0080) gDIP_Buffer[6]  |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0100) gDIP_Buffer[13] |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0200) gDIP_Buffer[12] |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0400) gDIP_Buffer[5]  |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0800) gDIP_Buffer[1]  |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x1000) gDIP_Buffer[2]  |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x2000) gDIP_Buffer[8]  |= 0x10;

	// ���ֹ�1
	if (g2DIP_ShowBuffer[3] & 0x0001) gDIP_Buffer[14] |= 0x04;
	if (g2DIP_ShowBuffer[3] & 0x0002) gDIP_Buffer[7]  |= 0x04;
	if (g2DIP_ShowBuffer[3] & 0x0004) gDIP_Buffer[0]  |= 0x04;
	if (g2DIP_ShowBuffer[3] & 0x0008) gDIP_Buffer[3]  |= 0x04;
	if (g2DIP_ShowBuffer[3] & 0x0010) gDIP_Buffer[11] |= 0x04;
	if (g2DIP_ShowBuffer[3] & 0x0020) gDIP_Buffer[15] |= 0x04;
  	if (g2DIP_ShowBuffer[3] & 0x0040) gDIP_Buffer[9]  |= 0x04;

	// ���ֹ�2
	if (g2DIP_ShowBuffer[4] & 0x0001) gDIP_Buffer[14] |= 0x80;
	if (g2DIP_ShowBuffer[4] & 0x0002) gDIP_Buffer[7]  |= 0x80;
	if (g2DIP_ShowBuffer[4] & 0x0004) gDIP_Buffer[0]  |= 0x80;
	if (g2DIP_ShowBuffer[4] & 0x0008) gDIP_Buffer[3]  |= 0x80;
	if (g2DIP_ShowBuffer[4] & 0x0010) gDIP_Buffer[11] |= 0x80;
	if (g2DIP_ShowBuffer[4] & 0x0020) gDIP_Buffer[15] |= 0x80;
  	if (g2DIP_ShowBuffer[4] & 0x0040) gDIP_Buffer[9]  |= 0x80;

	// ���ֹ�3
	if (g2DIP_ShowBuffer[5] & 0x0001) gDIP_Buffer[14] |= 0x02;
	if (g2DIP_ShowBuffer[5] & 0x0002) gDIP_Buffer[7]  |= 0x02;
	if (g2DIP_ShowBuffer[5] & 0x0004) gDIP_Buffer[0]  |= 0x02;
	if (g2DIP_ShowBuffer[5] & 0x0008) gDIP_Buffer[3]  |= 0x02;
	if (g2DIP_ShowBuffer[5] & 0x0010) gDIP_Buffer[11] |= 0x02;
	if (g2DIP_ShowBuffer[5] & 0x0020) gDIP_Buffer[15] |= 0x02;
  	if (g2DIP_ShowBuffer[5] & 0x0040) gDIP_Buffer[9]  |= 0x02;

	// STEREO -- WIFI ��
	if (g2DIP_ShowBuffer[6] & 0x0001) gDIP_Buffer[14] |= 0x01;
	if (g2DIP_ShowBuffer[6] & 0x0002) gDIP_Buffer[7]  |= 0x01;
	if (g2DIP_ShowBuffer[6] & 0x0004) gDIP_Buffer[0]  |= 0x01;
	if (g2DIP_ShowBuffer[6] & 0x0008) gDIP_Buffer[3]  |= 0x01;
	if (g2DIP_ShowBuffer[6] & 0x0010) gDIP_Buffer[11] |= 0x01;
	if (g2DIP_ShowBuffer[6] & 0x0020) gDIP_Buffer[15] |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0040) gDIP_Buffer[9]  |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0080) gDIP_Buffer[6]  |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0100) gDIP_Buffer[13] |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0200) gDIP_Buffer[12] |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0400) gDIP_Buffer[5]  |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0800) gDIP_Buffer[1]  |= 0x01;
  	// dot ':''.' ':'
  	if (g2DIP_ShowBuffer[6] & 0x1000) gDIP_Buffer[10]  |= 0x20;
  	if (g2DIP_ShowBuffer[6] & 0x2000) gDIP_Buffer[10]  |= 0x10;
  	if (g2DIP_ShowBuffer[6] & 0x4000) gDIP_Buffer[10]  |= 0x04;

	// Ƶ����ʾ
	// P5 P15 P25 
	// P31	(60 330 1K 3K3 10K 20K)
	// �ӵ�Դ ��������

	// p1 ���ϵ��� ��������
	if (g2DIP_ShowBuffer[7] & 0x01) gDIP_Buffer[5]  |= 0x04;
	if (g2DIP_ShowBuffer[7] & 0x02) gDIP_Buffer[12] |= 0x04;
	if (g2DIP_ShowBuffer[7] & 0x04) gDIP_Buffer[13] |= 0x04;
	if (g2DIP_ShowBuffer[7] & 0x08) gDIP_Buffer[6]  |= 0x04;

	if (g2DIP_ShowBuffer[8] & 0x01) gDIP_Buffer[1]  |= 0x80;
  	if (g2DIP_ShowBuffer[8] & 0x02) gDIP_Buffer[5]  |= 0x80;
  	if (g2DIP_ShowBuffer[8] & 0x04) gDIP_Buffer[12] |= 0x80;
  	if (g2DIP_ShowBuffer[8] & 0x08) gDIP_Buffer[13] |= 0x80;
  	if (g2DIP_ShowBuffer[8] & 0x10) gDIP_Buffer[6]  |= 0x80;

	//p11
  	if (g2DIP_ShowBuffer[9] & 0x01) gDIP_Buffer[5]  |= 0x02;
  	if (g2DIP_ShowBuffer[9] & 0x02) gDIP_Buffer[12] |= 0x02;
  	if (g2DIP_ShowBuffer[9] & 0x04) gDIP_Buffer[13] |= 0x02;
  	if (g2DIP_ShowBuffer[9] & 0x08) gDIP_Buffer[6]  |= 0x02;

	// p16
  	if (g2DIP_ShowBuffer[10] & 0x01) gDIP_Buffer[11] |= 0x40;
	if (g2DIP_ShowBuffer[10] & 0x02) gDIP_Buffer[3]  |= 0x40;
	if (g2DIP_ShowBuffer[10] & 0x04) gDIP_Buffer[0]  |= 0x40;
	if (g2DIP_ShowBuffer[10] & 0x08) gDIP_Buffer[7]  |= 0x40;
	if (g2DIP_ShowBuffer[10] & 0x10) gDIP_Buffer[14] |= 0x40;

	// p21
	if (g2DIP_ShowBuffer[11] & 0x01) gDIP_Buffer[13] |= 0x40;
	if (g2DIP_ShowBuffer[11] & 0x02) gDIP_Buffer[6]  |= 0x40;
  	if (g2DIP_ShowBuffer[11] & 0x04) gDIP_Buffer[9]  |= 0x40;
  	if (g2DIP_ShowBuffer[11] & 0x08) gDIP_Buffer[15] |= 0x40;

	// p26
  	if (g2DIP_ShowBuffer[12] & 0x01) gDIP_Buffer[8]  |= 0x40;
  	if (g2DIP_ShowBuffer[12] & 0x02) gDIP_Buffer[2]  |= 0x40;
  	if (g2DIP_ShowBuffer[12] & 0x04) gDIP_Buffer[1]  |= 0x40;
  	if (g2DIP_ShowBuffer[12] & 0x08) gDIP_Buffer[5]  |= 0x40;
  	if (g2DIP_ShowBuffer[12] & 0x10) gDIP_Buffer[12] |= 0x40;

    HAL_DIP_STB(0);
    MUSDELAY(1);
	MDIP_WriteByte(0x40);		  // ��ʼ��	�ϵ�ʱ����ַ��Ϊ00H(00H-0DH) //  �ϵ�ʱ��Ĭ������Ϊ������Ϊ1/16����ʾ�أ���ַ������1
    MUSDELAY(1);
    HAL_DIP_STB(1);    
    MUSDELAY(1);
    HAL_DIP_STB(0);
    MUSDELAY(1);
	MDIP_WriteByte(0xc0);		  // �����ַ 
	gLocal_0 = 0;

	do{
		MDIP_WriteByte(gDIP_Buffer[gLocal_0]);		  // ����
	}while(++gLocal_0 < 16);	  // TA6932 һ�����16��BYTE����
    HAL_DIP_STB(1);    
    return;
}

void MDIP_WriteByte(BYTE gLocal_1){
    BYTE gLocal_0;

    gLocal_0 = 8;
    do{
        HAL_DIP_CLK(0);
        HAL_DIP_DAT(gLocal_1 & 0x01);
        gLocal_1 >>= 1;
        HAL_DIP_CLK(1);
    } while (--gLocal_0 > 0); 
    return;
}
