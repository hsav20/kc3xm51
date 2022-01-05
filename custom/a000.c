
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#define ROOT     		  
#include "main.h"    

BYTE gDelayInput;
BYTE gA000Input;
BYTE gA000Mode;
BYTE gMuteTime;
BYTE gBtCtrlTime;
BOOL isStandKey;
BOOL isBtInput;
BOOL isPlayCtrl;
BOOL isAc3Dts;
BOOL isSaveAc3Dts;

#define MEMORY_INPUT		(KCM_EXTR_MEMORY + 4)	
#define MEMORY_MODE			(KCM_EXTR_MEMORY + 5)	



void MA000_ListenMode(BOOL isChange, BOOL isText){          // ��������ģʽѡ��
	BYTE listen;
	BYTE config;
	g2DIP_ShowBuffer[6] &= ~0x00d0;							// B7:PROLOGIC B6:NEO6 B4:DSP
	if (!isAc3Dts){
		if (isChange){
			if (++gA000Mode > 3){
				gA000Mode = 0;
			}
			MKCM_WriteRegister(MEMORY_MODE, gA000Mode);
			MKCM_WriteRegister(KCM_LPF_FREQ, 200);			// �������ĵ�ͨ�˲����ĸ�Ƶ��ֹƵ��
			MKCM_WriteRegister(KCM_HPF_FREQ, 200);			// ��ѡ��С����ʱ����Ӧ��������ʹ�ñ��Ĵ������õ�Ƶ��
		}
	
		// ��[6] B15:D1 B14:D1 B13:D1 B12:D1 B11:WIFI B10:LPCM B9:HD    B8:DTS  B7:PROLOGIC B6:NEO6  B5:��  B4:DSP B3:ATMOS B2:DD  B1:AUTO B0:ST
		if ((gA000Mode & 1) == 0){
			config = 0xab;
			listen = 0x00;
		}else{
			config = 0x55;
			listen = 0x01;
		}
		if (gA000Mode == 0){								// ģʽ1����ʾ����ʾNEO6��7.1�������
			// ������ģʽ�����ص�������򿪣����úϳ���������ͻ����������·������Ϊ���������
			listen |= 0x20;
			g2DIP_ShowBuffer[6] |= 0x0040;	
		}else if (gA000Mode == 1){							// ģʽ 2����ʾ����ʾPROLOGII��7.1 �������
			// С����ģʽ�����ص����򿪣����úϳ���������ͻ����������·���Ƹ���ǰ�����������źŲ����κδ���
			// ���轫ǰ�ã����ã����ƹ�7 ��������ͨ��ֹƵ����Ϊ200Hz
			listen |= 0x20;
			g2DIP_ShowBuffer[6] |= 0x0080;	
		}else{												// ģʽ 3-n����ʾ����ʾdsp
			g2DIP_ShowBuffer[6] |= 0x0010;
			if (isText){
				MDIP_MenuNormal(MENU_MODE_DSP);
			}
			listen |= 0x30;
		}
	}else{													// DTS5.1��ű�5.1
		listen = 0x20;
		config = 0xab;
	}
	MKCM_WriteRegister(KCM_LISTEN_MODE, listen);
	MKCM_WriteRegister(KCM_SPK_CONFIG, config);
	FDIP_ScreenUpdata = 1;
}



CONST_CHAR Tab_InputTextA000[] = {
	"AUXOPTCO1 BTSD USBUSB   BT HD1HD2HD3"
//	 +++---+++---+++---+++---+++---+++---+++
};

void MDIP_CustomInput(){									// �û�������Դ������ʾ
	if (FSYS_Standby){
		return;
	}
	g2DIP_ShowBuffer[6] &= ~(0x4000|0x0020);				// Ϩ���Ʒ��ż�:
	if (mINPUT_SWITCH >= INPUT_SWITCH_SD && mINPUT_SWITCH <= INPUT_SWITCH_BT){
		g2DIP_ShowBuffer[6] |= 0x0020;						// �����Ʒ���
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
				g2DIP_ShowBuffer[6] |= 0x4000;                          // ����:
				return;
			}
		}
	}
//MLOG("MDIP_CustomInput  %d %d", gA000Input, mINPUT_SWITCH);
	MDIP_WrText((char*)&Tab_InputTextA000[mINPUT_SWITCH * 3], 3);
	MDIP_SingleChar(3, ' ');
	MDIP_ShowVolume();										// ��4��5��λ����ʾ������ֵ
}

CONST_CHAR Tab_InputA000[] = {
    INPUT_SWITCH_HDMI1, INPUT_SWITCH_HDMI2, INPUT_SWITCH_HDMI3,
	INPUT_SWITCH_OPTIC, INPUT_SWITCH_COA1, INPUT_SWITCH_COA2, 
	INPUT_SWITCH_UDISK, INPUT_SWITCH_AUX
};
void MAUD_InputA000(){         								// ����������һ������ѡ��
	isBtInput = (Tab_InputA000[gA000Input] != INPUT_SWITCH_COA2) ? 0 : 1;
	isPlayCtrl = (Tab_InputA000[gA000Input] != INPUT_SWITCH_UDISK) ? 0 : 1;
	P27 = isBtInput;
	gDIP_MenuSelect = MENU_RESTORE;							// �˵����̽�������Ļָ� 
	MAUD_InputSelect(Tab_InputA000[gA000Input]);
	MA000_ListenMode(0, 0);
}
void MAUD_InputSave(){         								// ����������һ������ѡ��
	if (++gA000Input >= sizeof(Tab_InputA000)){
		gA000Input = 0;
	}
	MKCM_WriteRegister(MEMORY_INPUT, gA000Input);
}
void MAUD_SET_BT(BYTE type){								// 0=�ָ� 1=���� 2=��һ�� 3=��һ��
	if (type != 0){
		if (type == 1){
			P26 = 0;
		}else if (type == 2){
			P24 = 0;
		}else {
			P23 = 0;
		}
		gBtCtrlTime = 10;									// 10 * 100����
	}else{
		P26 = 1;
		P24 = 1;
		P23 = 1;
	}
}

CONST_CHAR Tab_TrimMenu[] = {
	"SW CENSURBAC"
//	 +++---+++---
};

void MAUD_TrimAdjust(SPKER_CHANNEL channel, BYTE directUp){
    BYTE address;
    BYTE value;

	MAUD_AutoCanclMute();
	if (directUp){
		if (gDIP_TrimCtrl[channel] < 18){
			++gDIP_TrimCtrl[channel];
		}
	}
	else {
		if (gDIP_TrimCtrl[channel] > 0){
			--gDIP_TrimCtrl[channel];
		}
	}
    
    address = MKCM_ToRegister(KCM_TEST_TONE, channel);
    value = MKCM_ToRegister(KCM_FL_TRIM, gDIP_TrimCtrl[channel]);
//MLOG("MAUD_TrimAdjust A %d %02x %d", channel, KCM_FL_TRIM + address, value);
    MKCM_WriteRegister(KCM_FL_TRIM + address, value);
	if (channel == CHANNEL_SL){								// ����������
		MKCM_WriteRegister(KCM_SR_TRIM, value);				// ����������һ���޸�
	}else if (channel == CHANNEL_BL){						// ����������
		MKCM_WriteRegister(KCM_BR_TRIM, value);				// ����������һ���޸�
	}

//MLOG("MAUD_TrimAdjust B %d %d", MKCM_ReadRegister(KCM_CE_TRIM), MKCM_ReadRegister(KCM_SW_TRIM));	
}
void MDIP_TrimMenu(BYTE index, BYTE mode){					// mode 0һ��ģʽ 1��˸���� 2��˸Ϩ�� 3����- 4����+  
	BYTE temp0;
	BYTE value;
	temp0 = (index == MENU_TRIM_WOOFER) ? 2 : (index == MENU_TRIM_CENTER) ? 1 : (index == MENU_TRIM_SURROUND) ? 7 : 6;
	if (mode >= MENU_SET_ADJ_DOWN){							    // MENU_SET_ADJ_DOWN��MENU_SET_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // �ϴ��Ѿ����뵱ǰ�˵�
            MAUD_TrimAdjust(temp0, (mode == MENU_SET_ADJ_UP) ? 1 : 0); 	
			gDIP_MenuTimer = 80;
		}
	}
	MDIP_WrString((char*)&Tab_TrimMenu[(index - MENU_TRIM_WOOFER) * 3]);
	MDIP_SingleChar(3, ' ');
	//  MLOG("MDIP_TrimMenu %d %d", index, mode);
	value = gDIP_TrimCtrl[temp0];			// 0=FL 1=CN 2=SW 3=FR 4=SR 5=BR 6=BL 7=SL
//MLOG("MDIP_TrimMenu A %d %d %d %d", index, temp0, value, gDIP_TrimCtrl[1]);
//if(mode!=MENU_SET_TWINKLE_ON&&mode!=MENU_SET_TWINKLE_OFF){MDEBUG(0xee);MDEBUG(index);MDEBUG(value);}
	if (mode == MENU_SET_TWINKLE_OFF){							// MENU_SET_TWINKLE_OFF��˸Ϩ��
		MDIP_SingleChar(4, ' ');
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_SET_NORMAL��MENU_SET_TWINKLE_ON��˸����

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

void MDIP_MenuCustom(BYTE index, MENU_SET mode){
	switch (index){
    case MENU_BT_1 :                                   		// �������
		MDIP_WrString("BT----");
		break;
    case MENU_BT_2 :                                   		// ����������
		MDIP_WrString("000000");
		break;
    case MENU_MODE_DSP :                               		// ����������
		MDIP_WrString(" DSP  ");
		MDIP_SingleChar(5, (gA000Mode - 2) + '0');
		break;
    case MENU_TRIM_WOOFER :                                 // �˵�����΢������������
    case MENU_TRIM_CENTER :                                 // �˵�����΢����������
    case MENU_TRIM_SURROUND :                               // �˵�����΢����������
    case MENU_TRIM_BACK :                                   // �˵�����΢����������
        MAUD_AutoCanclTestTone();
		MDIP_TrimMenu(index, mode);
		break;
		
	}
}
void MA000_Standby(){
	P27 = 0;
	P33 = 1;												// ��Ϊ��������Ϊ�������
	P34 = 1;												// ��Ϊ�����������Ϊ����������35H�ĵ�Դ
	P00 = 1;												// ��Ƭ��39��(P00)����ߵ�ƽ����LED��ɫ�Ƶ���
	P01 = 0;												// ��Ƭ��38��(P01)����ߵ�ƽ����LED��ɫ����
	FSYS_Standby = 1;
	MKCM_WriteRegister(KCM_POWER_ON, KCM_SET_STANDBY);		// ����ģ��������״̬
	MDIP_MenuNormal(MENU_STANDBY);
	DIP_SURROUND_OFF();
	DIP_SRC_FORMAT_OFF();
	DIP_DOT_OFF();
	DIP_PLAY_OFF();
	MDIP_WrString("      ");
}
void MA000_SetPowerOn(){
	P34 = 0;												// ��Ϊ�����������Ϊ����������35H�ĵ�Դ
	P00 = 0;												// ��Ƭ��39��(P00)����ߵ�ƽ����LED��ɫ����
	P01 = 1;												// ��Ƭ��38��(P01)����ߵ�ƽ����LED��ɫ�Ƶ���
	FSYS_Standby = 0;
	MKCM_SetPowerOn();										// KCM����
	gMuteTime = 30;	
}
void MA000_KEY_SKIP(BOOL isUp){
	if (isPlayCtrl){
		MDIP_PlaySkip(isUp ? KCM_OPERATE_SKIP_DOWN : KCM_OPERATE_SKIP_UP);		// ��ý�岥��ǰ/��һ��
	}else if (isBtInput && !gBtCtrlTime){
		
		if (P21){											// û���������밴��
			MAUD_SET_BT(isUp ? 3 : 2);						// 0=�ָ� 1=���� 2=��һ�� 3=��һ��			
		}else{												// ��ס�������밴��
			P26 = 0;
			MDIP_MenuNormal(isUp ? MENU_BT_1 : MENU_BT_2);  // �������/���������� 
			gBtCtrlTime = isUp ? 30 : 50;					// ��3��������� ��5������������
		}
	
	}
}
void MA000_KEY_10msTimer(BYTE baseTimer){   				// B3=1000ms B2=500ms B1=100ms B0=10ms 
	MPKey_Scan();
	if (FSYS_Standby){
		if (FRmDecodDone){
			FRmDecodDone = 0;
			if (GRmCodeData == cRmKey_Standby){
				MA000_SetPowerOn();
			}
		}
	   	if (FPKeyDecodeDone){
			FPKeyDecodeDone = 0;
			FPKeyStep = 1;
			if (GPKeyData == cPanKey_Standby){
				MA000_SetPowerOn();	
			}
		}
		return;
	}
   	if (FPKeyDecodeDone){
		FPKeyDecodeDone = 0;
		FPKeyStep = 1;

// MLOG("FPKeyDecodeDone %02x", GPKeyData);		
		switch (GPKeyData){
  		case cPanKey_Standby:								// JOG
		 	MA000_Standby();
            break;
  		case cPanKey_JogMenu:								// JOG			
			MAUD_InputSave();
			MAUD_InputA000();         						// ����������һ������ѡ��
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
//MLOG("FRmDecodDone %02x ", GRmCodeData);	
		switch (GRmCodeData){
		case cRmKey_Standby:
			MA000_Standby();
			break;
		case cRmKey_FactorySet:
			MDIP_WrString(" RESET");
			MDIP_ScreenUpdata();
			MKCM_FactorySet();
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
		case cRmKey_AudioMute:
    		MKEY_AudioMute();
			break;
		case cRmKey_MeunCtrl:
			if (gDIP_MenuSelect >= MENU_TRIM_WOOFER && gDIP_MenuSelect <= MENU_TRIM_BACK){	// �Ѿ��������ò˵� 
			//	MDIP_MenuNormal(MENU_RESTORE);
				++gDIP_MenuSelect;
				if (gDIP_MenuSelect > MENU_TRIM_BACK){
					gDIP_MenuSelect = MENU_TRIM_WOOFER;
				}
				MDIP_MenuNormal(gDIP_MenuSelect);
				gDIP_MenuTimer = 50;
			}
			else {
				MDIP_MenuNormal(MENU_TRIM_WOOFER);
			}
			break;
		case cRmKey_MeunLeft:
			if (gDIP_MenuSelect >= MENU_TRIM_WOOFER && gDIP_MenuSelect <= MENU_TRIM_BACK){	// �Ѿ��������ò˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_DOWN);
			}
			break;
		case cRmKey_MeunRight:
			if (gDIP_MenuSelect >= MENU_TRIM_WOOFER && gDIP_MenuSelect <= MENU_TRIM_BACK){	// �Ѿ��������ò˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_UP);
			}
			break;
		case cRmKey_PlayPause:								// ��������/��ͣ
			if (isPlayCtrl){
				MKEY_PlayPause();
			}else if (isBtInput){
				MAUD_SET_BT(1);								// 0=�ָ� 1=���� 2=��һ�� 3=��һ��			
			}
			break;
		case cRmKey_SkipDown:
			MA000_KEY_SKIP(0);
			break;
		case cRmKey_SkipUp:
			MA000_KEY_SKIP(1);
			break;
		case cRmKey_Surround:                               // ��廷��������
            MA000_ListenMode(1, 1);                            // ��������ģʽѡ��
			break;
		case cRmKey_InputOnKey:
			MAUD_InputSave();
			MAUD_InputA000();         						// ����������һ������ѡ��
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
void MA000_KCM_10msTimer(BYTE baseTimer){   				// B3=1000ms B2=500ms B1=100ms B0=10ms 
    if (!HAL_KCM_I2C_INT()){                           		// detect KCM interrupt ��⵽I2C�ӻ������ж�,INTΪ��
	    BYTE gLocal_1;
	    BYTE gLocal_2;

		gLocal_1 = MKCM_ReadRegister(KCM_READ_IRQ);			// Read interrupt ��ȡKCM�ж�
		MKCM_WriteRegister(KCM_CLEAR_IRQ, gLocal_1);  		// Clear interrupt �����Ӧ���ж�
		if (FKCM_I2C_Error){
            MLOG("KCM_READ_IRQ Error");
			return;
		}
		if (gLocal_1 != KCM_IRQ_PLAY_TIME){					// ���Ƕ�ý�岥��ʱ��ı�
//MLOG("KCM_READ_IRQ %02x", gLocal_1);
// MLOG("FSYS_Standby %D", FSYS_Standby?1:0);
		}
		if ((gLocal_1 & KCM_IRQ_SYSTEM_INIT) > 0){			// KCMģʽ��ʼ������ж�
            if (!FSYS_Standby){
				MKCM_RestoreMemory();						// ��KCM�ָ����صļ���
				gA000Input = MKCM_ReadRegister(MEMORY_INPUT);	// �ָ�����ѡ��
				gA000Mode = MKCM_ReadRegister(MEMORY_MODE);
				MAUD_InputA000();
				MA000_ListenMode(0, 0);
			}else{
				MKCM_WriteRegister(KCM_POWER_ON, KCM_SET_STANDBY);	// ����ģ��������״̬
				gLocal_1 = 0;								//����Ķ����ٴ���
			}
		}
		if (FSYS_Standby){									// ����״̬�²����������ж���
			return;
		}
		if ((gLocal_1 & KCM_IRQ_FORMAT_INFO) > 0){          // �����ź������ʽ�ı��жϣ���Ҫ��ȡ"KCM_SRC_FORMAT"�Ĵ���
			BYTE mode;
			gAUD_SrcFormat = MKCM_ReadRegister(KCM_SRC_FORMAT);
			mode = gAUD_SrcFormat & 0x0f;
			isAc3Dts = (mode == KCM_SRC_AC3 || mode == KCM_SRC_DTS) ? 1 : 0;
			if (isSaveAc3Dts != isAc3Dts){
				if (isAc3Dts){
					MKCM_WriteRegister(KCM_LISTEN_MODE, 0x20);
				}else{

				}
				
				isSaveAc3Dts = isAc3Dts;
			}
			//MLOG("gAUD_SrcFormat %02x", gAUD_SrcFormat);
            gAUD_SrcFreq = MKCM_ReadRegister(KCM_SRC_FREQ);
			if (mINPUT_SWITCH == INPUT_SWITCH_SD || mINPUT_SWITCH == INPUT_SWITCH_UDISK){
                g2TimeLength = MKCM_Read2Byte(KCM_PLAY_FILE_TIME);
            }else {
				if (gDIP_MenuLock == 0){						// 
					if (!FSYS_TestTone){						// û�д���������
						MDIP_MenuNormal(MENU_SRC_FORMAT);		// �˵���ʾ����������ʽ
					}
				}
			}
			if (gAUD_SrcFormat == 0){
				MDIP_CleanSpectrum();
	    	}
			MDIP_SrcFormatSymbol();
            MDIP_SurroundSymbol();
			MA000_ListenMode(0, 0);        	
		}
		// if ((gLocal_1 & KCM_IRQ_SRC_VALID) > 0){            	// ��Ч����Դ����ı��жϣ���Ҫ��ȡ"KCM_SRC_VALID"�Ĵ���
        //     MKCM_ReadSrcValid();
		// }
		if ((gLocal_1 & KCM_IRQ_VOLUME) > 0){               	// �������ڸı��жϣ���Ҫ��ȡ"KCM_VOLUME_CTRL"�Ĵ�����ȡ���µ�����ֵ
			gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);     // ��ȡ��ǰ����ֵ
			if (gDIP_MenuSelect == MENU_MASTER_VOL){
				MDIP_MenuNormal(MENU_MASTER_VOL);
			}
		}
        if ((gLocal_1 & KCM_IRQ_FIRMWARE) > 0){             // �̼����£���Ҫ��ȡ"KCM_RD_INFO"�Ĵ���
            MDIP_FirewareInfo();                            // ��ʾ�̼�����
//            MDIP_MenuNormal(cMenu_Fireware);
        }
        if ((gLocal_1 & KCM_IRQ_PLAY_TIME) > 0){           	// ��ý�岥��ʱ��ı�
            if (gDIP_MenuSelect == MENU_RESTORE || gDIP_MenuSelect == MENU_INPUT_SOURCE){	// ֻ���ڲ˵��ָ���������Դѡ��״̬
				g2PlayTime = MKCM_Read2Byte(KCM_PLAY_TIME);
				MDIP_InputSource();
			}
        }
        if ((gLocal_1 & KCM_IRQ_PLAY_STATE) > 0){           // ��ý���ļ�����״̬�ı�
        	gPlayStatus = MKCM_ReadRegister(KCM_PLAY_STATE);     // ��ȡ��ý���ļ�����״̬
			MLOG("KCM_IRQ_PLAY_STATE %02x", gPlayStatus);
        	MDIP_PlaySymbol(gPlayStatus);
        }
		if ((gLocal_1 & KCM_IRQ_APP_COMMAND) > 0){          // �յ��ֻ�/Զ��APP����ָ���Ҫ��ȡ"KCM_APP_COMMAND"�Ĵ���
            MKCM_AppCommand();
        }
    }
    
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void main(){
	P27 = 0;
	MSYS_Initialize(); 										// CPU����������ʼ��
	MDIP_HalInit();											// ��ʾģ��Ӳ���ײ��ʼ��	
	MDIP_BaseInit();										// ��ʾģ��������ʼ��
	MAUD_Initialize();       								// ��Ƶģ���ʼ��
	gDIP_Brightness = 0;
	MA000_Standby();
	while (1){ 					   
        if ((gTBascTimer & 0x01) > 0){						// 10ms (B3=1000ms B2=500ms B1=100ms B0=10ms) 
	        MA000_KCM_10msTimer(gTBascTimer);               // �Ӵ���ģ��10msʱ�䴦�� 		
			MA000_KEY_10msTimer(gTBascTimer);				// ����ģ��10msʱ�䴦��           
			MDIP_10msTimer(gTBascTimer);  					// ��ʾģ��10msʱ�䴦��
           	MAUD_10msTimer(gTBascTimer);                	// ��Ƶģ��10msʱ�䴦��
			if ((gTBascTimer & 0x08) > 0){					// 1000ms
			//	 MLOG("gDIP_MenuSelect %d %d", FSYS_Standby?1:0, gDIP_MenuLock);
			}
			if ((gTBascTimer & 0x02) > 0){					// 100msʱ��
				if (gMuteTime > 0 && --gMuteTime == 0){		// 3���ȡ������
					P33 = 0;								// ��Ϊ��������Ϊ�������
				}
				if (gBtCtrlTime > 0 && --gBtCtrlTime == 0){	// 0.6��������������
					P26 = 1;
					P24 = 1;
					P23 = 1;
				}
			}			
			gTBascTimer = 0;
		}
    }
} 


void MSYS_Initialize(){
    TMOD = 0x21; 											// 8bit Auto Timer1 / 16bit Timer0
    TCON = 0x40 | 0x10 | 0x01;								// TR1 + TR0 + EX0�½��ش���
    SCON = 0x50;											// 8bit UART
	TH1 = 0xff;												// 115200bps UARTʹ��115200bpsͨѶ
	PCON  |= 0x80;											// UART����
    IE = 0x80 | 0x02 | 0x01;								// EA + ET0 + EX0
}


void MUSDELAY(BYTE gLocal_1){      
    if (gLocal_1 > 3){
        while (--gLocal_1 != 0);
    }
    return;
}

void MPKey_Scan(){
    BYTE gLocal_1;
    BYTE gLocal_2;

	P05 = 0;
	if (!P22){
		if (++gPKeyConfirmTm > 8){ 
			gPKeyConfirmTm = 0;
			if (!FPKeyStep){
				GPKeyData = cPanKey_JogMenu;
				FPKeyDecodeDone = 1;
				FPKeyStep = 1;

			}
		}
		return;
	}else if (!P21){
		isStandKey = 1;
	}else if (isStandKey){
		isStandKey = 0;
		if (!gBtCtrlTime){
			GPKeyData = cPanKey_Standby;
			FPKeyDecodeDone = 1;
			FPKeyStep = 1;
		}
	}else{
		gPKeyConfirmTm = 0;
		FPKeyStep = 0; 
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

// ʱ���ж� ----------------------------------------------------------------
void TBascTimer(void) interrupt 1 {
    TL0 = (BYTE)(cSYS_TIMER0);                              // 100us
    TH0 = (BYTE)(cSYS_TIMER0 >> 8);

//P34=~P34;
	if ((++g10msTimer % 10) == 0){							// 1ms
		MKEY_CheckJop();
		if ((g10msTimer % 100) == 0){						// 10ms
			g10msTimer = 0;
		    gTBascTimer = 0x01;
			if ((++g1000msTimer % 10) == 0){				// 100ms
				gTBascTimer |= 0x02;
				if ((g1000msTimer % 50) == 0){				// 500ms
					gTBascTimer |= 0x04;
					if ((g1000msTimer % 100) == 0){			// 1000ms
						g1000msTimer = 0;	
						gTBascTimer |= 0x08;
					}
				}
			}
		}
		if (FDIP_FreqSymbol){
			if (++gBrightnessTimer > 19){
				gBrightnessTimer = 0;
				HAL_DIP_LED(1);                             // ������ʾ���̶��ķ���
			}
			else {
				if ((gDIP_Brightness == 0) && (gBrightnessTimer == 1)){
					HAL_DIP_LED(0);                         // ����Ϊ0ʱϨ����ʾ���̶��ķ���
				}
				else if ((gDIP_Brightness == 1) && (gBrightnessTimer == 4)){
					HAL_DIP_LED(0);                         // ����Ϊ1ʱϨ����ʾ���̶��ķ���
				}else if ((gBrightnessTimer == 8)){
					HAL_DIP_LED(0);                         // ����Ϊ2ʱϨ����ʾ���̶��ķ���
				}
			}
		}
	}

    if (gRmTimer != 0 && ++gRmTimer > 159){
        if(!FRmError && (gRmStatus == 2) && (gRmKeyContinCanclTm != 0)){
            if (gRmKeyContinSpeed < 8){
				if ((gRmKeyContinSpeed & 3) == 3){
					FRmDecodDone = 1;
				}
				++gRmKeyContinSpeed;
			}
			else {
	            FRmDecodDone = 1;
			}
        } 
        gRmStatus = 0;
        FRmError = 0; 
        gRmTimer = 0;
    }
    return;
}

// �����ж�0 ---------------------------------------------------------------
void EX0_int (void) interrupt 0 {		  
    if (!FRmError){
        ++gRmStatus;                                    
        if (gRmStatus == 2){    
            if (gRmTimer > 145) FRmError = 1; 
        }
        else{
            gRmBuffer >>= 1;
            if (gRmTimer > 16) gRmBuffer |= 0x80;   
            if ((gRmStatus == 2+8) || (gRmStatus == 2+24)){ 
                 GRmCodeData = gRmBuffer;
            }
            else if (gRmStatus == 2+16){
                if ((gRmBuffer != cRmNCustom) || (GRmCodeData != cRmCustom)){
                     gRmKeyContinCanclTm = 0;
                     FRmError = 1;
                } 
//                gTemp0=gRmBuffer;gTemp1=GRmCodeData;				
            }
            else if (gRmStatus == 2+32){ 
                 if ((gRmBuffer ^ 0xff) == GRmCodeData){
                    FRmDecodDone = 1;
                    gRmKeyContinSpeed = 0;
                 }else{
                    FRmDecodDone = 0; 
                    FRmError = 1;
                 }
            }
        }
    }
    gRmTimer = 1; 
    return;
}		 

