
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip


#include "main.h"           

void MKCM_Initialize(){										// KCMģ���ʼ��  	
	gDIP_Brightness = 0;
	MKCM_SetPowerOn();										// KCM����
}	  
void MKCM_SetPowerOn(){ 									// KCM����
	MKCM_WriteRegister(KCM_POWER_ON, 1);
    MDIP_MenuNormal(MENU_POWER_ON);                         // �˵�״̬:��Դ��
	gDIP_MenuLock = 100;									// ��ʱ������ʾ10��
    MLOG("KCM_POWER_ON");
}	  



void MKCM_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
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
// MLOG("KCM_READ_IRQ %02x", gLocal_1);
		}
		if ((gLocal_1 & KCM_IRQ_SYSTEM_INIT) > 0){			// ģʽ��ʼ������жϣ���Ҫд��"KCM_POWER_ON"�Ĵ�����
            MKCM_RestoreMemory();
		}
		if ((gLocal_1 & KCM_IRQ_FORMAT_INFO) > 0){          // �����ź������ʽ�ı��жϣ���Ҫ��ȡ"KCM_SRC_FORMAT"�Ĵ���
			gAUD_SrcFormat = MKCM_ReadRegister(KCM_SRC_FORMAT);
			//MLOG("gAUD_SrcFormat %02x", gAUD_SrcFormat);
            gAUD_BpsRate = MKCM_ReadRegister(KCM_BPS_RATE);
			if (gDIP_MenuLock == 0){						// 
				if (!FSYS_TestTone){						// û�д���������
					MDIP_MenuNormal(cMenu_SrcFormat);
				}
			}
			if (gAUD_SrcFormat == 0){
				MDIP_CleanSpectrum();
	    	}
			MDIP_SrcFormatSymbol();
            MDIP_SurroundSymbol();
        	if (mINPUT_SWITCH == INPUT_SWITCH_SD || mINPUT_SWITCH == INPUT_SWITCH_UDISK){
                g2TimeLength = MKCM_Read2Byte(KCM_PLAY_FILE_TIME);
            }
		}
		if ((gLocal_1 & KCM_IRQ_SRC_VALID) > 0){            	// ��Ч����Դ����ı��жϣ���Ҫ��ȡ"KCM_SRC_VALID"�Ĵ���
            MKCM_ReadSrcValid();
		}
		if ((gLocal_1 & KCM_IRQ_VOLUME) > 0){               	// �������ڸı��жϣ���Ҫ��ȡ"KCM_VOLUME_CTRL"�Ĵ�����ȡ���µ�����ֵ
			gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);     // ��ȡ��ǰ����ֵ
			if (gDIP_MenuSelect == cMenu_MasterVolume){
				MDIP_MenuNormal(cMenu_MasterVolume);
			}
		}
        if ((gLocal_1 & KCM_IRQ_FIRMWARE) > 0){             // �̼����£���Ҫ��ȡ"KCM_RD_INFO"�Ĵ���
            MDIP_FirewareInfo();                            // ��ʾ�̼�����
//            MDIP_MenuNormal(cMenu_Fireware);
        }
        if ((gLocal_1 & KCM_IRQ_PLAY_TIME) > 0){           		// ��ý�岥��ʱ��ı�
            g2PlayTime = MKCM_Read2Byte(KCM_PLAY_TIME);
            if (g2PlayTime){                                	// ����ʱ��ı� 
				if (gDIP_MenuSelect != MENU_INPUT_SOURCE){
	                MDIP_MenuNormal(cMenu_PlayTime);
				}
            }else {                                         	// ���������
                MDIP_MenuNormal(cMenu_PlayTrack);
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
	  




       
CONST_CHAR TabEqSetup[5] = {                                // ��·����EQ��Ч��������
    0x03, 0xc0 | 5, 0, 8,                                   // ǰ��ͨ��ʹ��EQ��5�μ�HSF&LSF��MICû����Ч��������Qֵ8
};     
CONST_CHAR TabEqFreq[9] = {                                 // ��·�ξ���EQ��Ч����Ƶ��
    0x00|6,  0x20|3,  0,                                    // 50Hz(50-20/5=6),             350Hz(300-150/50=3)
    0x20|17, 0x40|18, 0,                                    // 1000Hz(1000-150/50=17),      3300Hz(3300-1500/100=18)
    0x60|21, 0,       0,                                    // 15000Hz(15000-4500/500=21)
    
};     

CONST_CHAR TabEqLevel[4][9] = {                             // ��·�ξ���EQ��Ч�����ƽ
{   
    0x00, 0x80, 15|(2<<4),                                  // +15dB, -2dB
    0x80, 0x00, 12|(5<<4),                                  // -12dB, +5dB,
    0x00, 0x00, 15,                                         // +15dB,
},{
    0x80, 0x00, 12|(2<<4),                                  // -12dB, +2dB
    0x00, 0x80, 10|(5<<4),                                  // +10dB, -5dB,
    0x80, 0x00, 8,                                          // -8dB,
},{
    0x00, 0x00, 8|(5<<4),                                   // +8dB, +5dB
    0x80, 0x00, 2|(3<<4),                                   // -2dB,  +3dB,
    0x00, 0x00, 6,                                          // +6dB,
},{
    0x00, 0x80, 5|(2<<4),                                   // +5dB, -2dB
    0x80, 0x00, 4|(11<<4),                                  // -4dB, +11dB,
    0x00, 0x00, 13,                                         // +13dB,
},
};
CONST_CHAR TabSpectrum[] = {
    0x01,                                                   // �ֽ�0ѡ��Ƶ�׷�ʽ��1ΪƵ�ʵ�ƽ��ʽ
    5, 8, 0,                                                // �ֽ�1���ֽ�3Ϊ�������12λ����ʾ�����ȼ��߶�����
    100 & 0xff,   (100 >> 8) & 0xff,                        // 100Hz
    350 & 0xff,   (350 >> 8) & 0xff,                        // 350Hz
    1000 & 0xff,  (1000 >> 8) & 0xff,                       // 1000Hz
    3300 & 0xff,  (3300 >> 8) & 0xff,                       // 3300Hz
    10000 & 0xff, (10000 >> 8) & 0xff,                      // 10000Hz
};


void MKCM_RestoreMemory(){ 									// ��������KCM֮�лָ�����
    BYTE counter;
    BYTE select;
	BYTE value;
	BYTE gLocal_1;
    BYTE temp[10];

	// KCM��չ���䣺ʹ��KCM_MEMORYRD�Ĵ����ļ���ֵ
	MKCM_ReadXByte(KCM_EXTR_MEMORY, temp, 4);               // ��ȡ�û��������ļ��䵽temp
	if (FKCM_I2C_Error){									// ��ȡKCM����
		MDIP_MenuNormal(MENU_RESTORE);						// �Ѿ������Ͳ��� 
		return;
	}
    value = temp[MEM_SOURCE_AUTO];
//	gDIP_Surround[0] = temp[MEM_SURROUND_2CH] & 0x01;       // ����ģʽ������
//	gDIP_Surround[1] = temp[MEM_SURROUND_8CH] & 0x03;       // ����ģʽ������
	//gDIP_Select2Ch = temp[MEM_SELECT_2CH] & 0x01;           // ѡ��Ϊ������
	gDIP_Brightness = temp[MEM_BRIGHTNESS] & 0x03;          // ��ʾ������
//MLOG("BrightA_%02x_%02x_", temp[MEM_SOURCE_AUTO], temp[MEM_BRIGHTNESS]);

	gLocal_1 = MKCM_FromRegister(KCM_INPUT_SOURCE, value);	// ͨ���Ĵ�������ѡ�����ֵ�ָ�
	MLOG("RestoreA %02x %02x %02x", value, gLocal_1, temp[MEM_SOURCE_AUTO]);
	if (gLocal_1 >= INPUT_SWITCH_SD && gLocal_1 <= INPUT_SWITCH_BT){	// SD/UDISK/USB����/������Ƶ
		mINPUT_SWITCH = temp[MEM_SOURCE_AUTO];  			// �Զ�����Ļָ� 
	}else{
		mINPUT_SWITCH = gLocal_1;							// ʹ��KCM���������˿�ѡ��
	}
//MLOG("mINPUT_B %02x %02x", mINPUT_SWITCH, gLocal_1);
	
	// ����˿�ѡ��ʹ��KCM_INPUT�Ĵ����ļ���ֵ
	value = MKCM_ReadRegister(KCM_INPUT_SOURCE);			// ���������˿�ѡ��
	MKCM_ReadXByte(KCM_RD_INFO, temp, 2);                   // ��ȡģ����Ϣ�Ĵ���
    gSYS_ModelType = temp[1];                               // [0]�ļ������ٷֱ�[1]ģ���ͺ�[2-n]ÿ8�ֽ�Ϊ���ֹ̼������ڰ汾


	// ������ʹ��KCM_VOLUME�Ĵ����ļ���ֵ
	gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);	// ���������ֵ
	// ��Ч����ͨ����ʹ��KCM_EQ_SELECT�����ԵļĴ����ļ���ֵ
//	gDIP_SoundEffect = MKCM_ReadRegister(KCM_EQ_SELECT);			// �������Ч����ͨ��ѡ��

	for (counter = 0; counter < sizeof(gDIP_TrimCtrl); counter++){ // ����΢����ʹ��KCM_FL_TRIM�ȼĴ����ļ���ֵ
		gLocal_1 = MKCM_FromRegister(KCM_TEST_TONE, counter);	// ����������˳��
		value = MKCM_ReadRegister(KCM_FL_TRIM + gLocal_1);	// ����΢��ֵ
		gDIP_TrimCtrl[gLocal_1] = MKCM_FromRegister(KCM_FL_TRIM, value);	// ΢�� �Ĵ���->����ֵ
//MDEBUG(0xf9);MDEBUG(gLocal_1);MDEBUG(gDIP_TrimCtrl[gLocal_1]);
	};

	value = MKCM_ReadRegister(KCM_MIC_VOLUME);	            // ��Ͳ1����Ͳ2��������
	gDIP_MicCtrl[0] = MKCM_FromRegister(KCM_MIC_VOLUME, value & 0x0f);  // 0-15ת����0-9
	gDIP_MicCtrl[1] = MKCM_FromRegister(KCM_MIC_VOLUME, (value >> 4) & 0x0f);

	value = MKCM_ReadRegister(KCM_MIC_ECHO_EQ);	            // ��Ͳ��������Ͳ���EQ������Ч����ѡ�����
	gDIP_MicCtrl[2] = MKCM_FromRegister(KCM_MIC_DELAY, value & 0x0f);   // 0-15ת����0-9
	
	value = MKCM_ReadRegister(KCM_MIC_REPEAT);	            // ��Ͳ�ظ�����Ͳֱ��������
	gDIP_MicCtrl[3] = MKCM_FromRegister(KCM_MIC_DELAY, value & 0x0f);   // 0-15ת����0-9
	
	value = MKCM_ReadRegister(KCM_MIC_DELAY);	            // ��Ͳ�ӳ�ʱ�� �������ֵ
	gDIP_MicCtrl[4] = MKCM_FromRegister(KCM_MIC_DELAY, value);	// 255��ת����0-9
	
	value = MKCM_ReadRegister(KCM_MIC_REVERB);	            // ��Ͳ������� �������ֵ
	gDIP_MicCtrl[5] = MKCM_FromRegister(KCM_MIC_DELAY, value & 0x0f);   // 0-15ת����0-9

//MDEBUG(0xf6);MDEBUG(gDIP_MicCtrl[0]);MDEBUG(gDIP_MicCtrl[1]);


	// ��������ѡ��ʹ��KCM_SPK_CONFIG�ļ���ֵ
	gLocal_1 = MKCM_ReadRegister(KCM_SPK_CONFIG);			// �������������
	MKCM_FromRegister(KCM_SPK_CONFIG, gLocal_1);

    
//MDEBUG(0xf9);MDEBUG(gDIP_Surround[0]);MDEBUG(gDIP_Surround[1]);MDEBUG(gDIP_Select2Ch);

	MKCM_WriteRegister(KCM_VOLUME_MAX, 80);                 // 80 �����������ֵ
	MKCM_WriteRegister(KCM_LPF_FREQ, 70);                   // 70Hz ������ͨ��LPF��ͨ�˲���Ƶ��
	MKCM_WriteRegister(KCM_HPF_FREQ, 70);                   // 70Hz ������С����HPF��ͨ�˲���Ƶ��
//KCM_DYN_COMPRES           KCM_DYN_COMPRES
//KCM_LIP_SYNC_SET  ����ͬ���ӳ�ʱ��

    temp[0] = (BYTE)(CUSTOM_CODE >> 24);
    temp[1] = (BYTE)(CUSTOM_CODE >> 16);
    temp[2] = (BYTE)(CUSTOM_CODE >> 8);
    temp[3] = (BYTE)(CUSTOM_CODE >> 0);
    MKCM_WriteXByte(KCM_CUSTOM_CODE, temp, 4);              // �����û��Զ��幦�ܴ��뼰����ģ��̼��Ĵ���
                      
    for (select = 0; select < 4; select++){
        // ��·����EQ��Ч�������ã�һ��д��һ�����ڳ�ʼ��
        for (counter = 0; counter < sizeof(TabEqSetup); counter++){ 
            temp[counter + 1] = TabEqSetup[counter];
        }
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_SETUP, temp, 1 + sizeof(TabEqSetup));   // ��·����EQ��Ч��������

        // ��·����EQ��ЧƵ�ʣ�һ��д��һ�����ڳ�ʼ��
        for (counter = 0; counter < sizeof(TabEqFreq); counter++){ // д��EQƵ��
            temp[counter + 1] = TabEqFreq[counter];
        }
        for (counter = 0; counter < sizeof(TabEqFreq); counter++){ // д��EQƵ�ʵ�ƽ
            temp[counter + 1] |= TabEqLevel[select][counter];
        }
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_VALUE, temp, 1 + sizeof(TabEqFreq));   // ��·����EQ��Ч��������
    }
    MKCM_WriteXByte(KCM_WR_SPECTRUM, TabSpectrum, sizeof(TabSpectrum));   // ����Ƶ��ģʽ

	// ���һЩ�ϵ���Ҫȷ��ֵ�ı�������־
	gAUD_SrcFormat = 0;
    gAUD_BpsRate = 0;
    gPlayStatus = 0;
	FSYS_Standby = 0;
	FSYS_TestTone = 0;
	FSYS_MuteEnable = 0;
	gRemoveTimer = 0;
	MAUD_MakePreemptible(KCM_SRC_VALID_E8CH);				// ������ռʽ����ѡ��ģ��汾��KC35H����Ҫ��������7.1���� 

    MDIP_MenuNormal(MENU_POWER_ON);                         // �˵�״̬:��Դ��
	gDIP_MenuLock = 10;										// �����˳���ʱ������ʾ��
    MDIP_SurroundSymbol();
    MDIP_SrcFormatSymbol();
	MDIP_WifiSymbol(0xff);	    	 
	MDIP_PlaySymbol(gPlayStatus);
	MDIP_Brightness(0, gDIP_Brightness);
}

void MKCM_FactorySet(){										// ��������
	BYTE gLocal_1;
	BYTE gLocal_2;
	BYTE gLocal_Buffer[4];

	MDIP_WriteString("SET ");
	gLocal_1 = 0;
	do {
		MDIP_Write2Digit(4, gLocal_1);
		MDIP_ScreenUpdata();
		MUSDELAY(255);
		MUSDELAY(255);
	} while (++gLocal_1 < 100);

	// KCM��չ���䣺ʹ��KCM_MEMORYRD�Ĵ����ļ���ֵ
	gLocal_Buffer[MEM_SOURCE_AUTO] = 0;                    	// �Զ�����Ļָ�
	gLocal_Buffer[MEM_SURROUND_2CH] = 0;                    // ������ģʽ������
	gLocal_Buffer[MEM_SURROUND_8CH] = 0;                    // ������ģʽ������
	gLocal_Buffer[MEM_SELECT_2CH] = 0;                      // ѡ��Ϊ������
	gLocal_Buffer[MEM_BRIGHTNESS] = 2;                      // ��ʾ������
	MKCM_WriteXByte(KCM_EXTR_MEMORY, gLocal_Buffer, 5);

	MKCM_WriteRegister(KCM_VOLUME_CTRL, 39); 				// ����ֵ

	// ����΢����ʹ��KCM_FL_TRIM�ȼĴ����ļ���ֵ
	gLocal_1 = 0;
	do {
		MKCM_WriteRegister(KCM_FL_TRIM + gLocal_1, 0);
	} while (++gLocal_1 < 8);

	MKCM_WriteRegister(KCM_SPK_CONFIG, 0xab);				// ���ô󡢻��ƴ����ô�ǰ�ô��г�����
	MKCM_WriteRegister(KCM_POWER_ON, 1);
    MDIP_MenuNormal(MENU_POWER_ON);                         // �˵�״̬:��Դ��
	gDIP_MenuLock = 30;										// ��ʱ������ʾ3��
    return;
}
void MKCM_AppCommand(){
    BYTE inData[8];
    BYTE length = MKCM_ReadAutoByte(KCM_APP_COMMAND, inData, 8);
    if (length == 2){
//        MKCM_WriteRegister(inData[0], inData[1]);
    	switch (inData[0]){
    	case KCM_VOLUME_CTRL:									// ��Ƶ�����������Ӽ�����
            MLOG("KCM_VOLUME_CTRL %d", inData[1]);
            gAUD_MasterVolume = inData[1];
            MDIP_MenuNormal(cMenu_MasterVolume);
    		break;
    	case KCM_INPUT_SOURCE:									// ������Դѡ��
            MLOG("KCM_INPUT_SOURCE %02x", inData[1]);
    		mINPUT_SWITCH = MKCM_FromRegister(KCM_INPUT_SOURCE, inData[1]);	// ͨ���Ĵ�������ѡ�����ֵ�ָ�
    		MAUD_AutoCanclMute();
    		MAUD_AutoCanclTestTone();
    		MDIP_MenuNormal(MENU_RESTORE);
    		break;
	    case KCM_LISTEN_MODE:				                    // ����ģʽѡ��
            MLOG("KCM_LISTEN_MODE %02x", inData[1]);
            MDIP_ListenMode(inData[1]);
            break;
	    case KCM_EQ_SELECT:				                        // ��·������Ч����ѡ��
            MLOG("KCM_LISTEN_MODE %02x", inData[1]);
            MDIP_EqSelect(inData[1]);
            break;

    	case KCM_VOLUME_MUTE:									// ��Ƶ�����������Ӽ�����
    		if (FSYS_MuteEnable != (inData[1] & 0x01)){			// MUTE�ı�
    			//MKEY_AudioMute();
    		}
    		else if (inData[1] & 0x04){							// �����ӻ��
    			gDIP_MenuSelect = cMenu_MasterVolume;
    			//MKCM_WriteRegister(KCM_VOLUME_MUTE, inData[1]);
    		}
    		break;
    	default :
            if (inData[0] >= KCM_FL_TRIM && inData[0] <= KCM_BR_TRIM){    // ��������΢��
            }
        MLOG("AppCommand %02x %02x %02x", length, inData[0], inData[1]);
    	    break;
    	}
    }
    MDIP_WifiSymbol(1);
}


// //////////////////////////////////////////////////
// KCM�Ĵ�����ֵ�뱾�����ڵ�ֵ�໥ת��
// //////////////////////////////////////////////////
CONST_CHAR Tab_InputSwitch[] = {							// KCM_INPUT_SOURCE     KC3X_INPUT_TYPE
	KCM_INPUT_ANALOG,			                            // ��Դѡ��ģ������
	KCM_INPUT_RX1,				                            // ��Դѡ��RX1����
	KCM_INPUT_RX2,				                            // ��Դѡ��RX2����
	KCM_INPUT_RX3,				                            // ��Դѡ��RX3����
	KCM_INPUT_SD,				                            // ��Դѡ��SD/TF������
	KCM_INPUT_UDISK,				                        // ��Դѡ��U������
	KCM_INPUT_PC,				                            // ��Դѡ��USB��������
	KCM_INPUT_E8CH,				                            // ��Դѡ������7.1��������
	KCM_INPUT_BT,				                            // ��Դѡ����������
	KCM_INPUT_HDMI1,				                        // ��Դѡ��HDMI1����
	KCM_INPUT_HDMI2,				                        // ��Դѡ��HDMI2����
	KCM_INPUT_HDMI3,				                        // ��Դѡ��HDMI3����
	KCM_INPUT_ARC,				                            // ��Դѡ��HDMI ARC����
};  						 



CONST_CHAR Tab_SurroundMode[] = {
	0x00, 0x01, 0x10, 0x11, 0x12, 0x13,
};  						 
CONST_CHAR Tab_TestToneChannel[] = {
// �淶��ͨ��˳��FL FR CN SW SL SR BL BR
// ����������˳��FL CN SW FR SR BR BL SL 					// �������������ȷֲ���˳ʱ��
	0,2,3,1, 5,7, 6,4,
};

CONST_CHAR Tab_ChannelTrim[] = {
	0x10 | 9, 0x10 | 8, 0x10 | 7, 0x10 | 6, 0x10 | 5,       // -9 -8 -7 -6 -5 dB     
	0x10 | 4, 0x10 | 3, 0x10 | 2, 0x10 | 1, 0x00,		    // -4 -3 -2 -1 0  dB
	1,2,3,4,5,6,7,8,9, 								        // +1 ~ +9 dB
};  						 
CONST_CHAR Tab_MicDelay[] = {                               // 0-255ת����0-9
    60/20,80/20,100/20,120/20,      160/20,200/20,300/20,500/20,    1000/20,2000/20,    // ��λ����    
};
CONST_CHAR Tab_Mic16To10[] = {                              // 0-15ת����0-9
    0,2,4,6,     8,10,12,13,    14,15,         
};

BYTE MKCM_ToRegister(BYTE index, BYTE counter){				// �ӱ��������ֵ��ת����KCM�Ĵ�����ֵ
	switch (index){
	case KCM_INPUT_SOURCE :									// ����˿�ѡ��
    	return Tab_InputSwitch[counter];
	case KCM_LISTEN_MODE :									// ������ģʽ
    	return Tab_SurroundMode[counter];
	case KCM_TEST_TONE :									// ��������
    	return Tab_TestToneChannel[counter];
	case KCM_FL_TRIM :										// ����΢��
	    return Tab_ChannelTrim[counter];
	case KCM_SPK_CONFIG :									// ��������
		// B7-B6����B5-B4����B3-B2����(0=û�а�װ 1=С���� 2=������)B1ǰ��(0=С����)B0������(0=û�а�װ)
		// 0ǰ�� 1���� 2������ 3���� 4����
		if (1){
			BYTE gLocal_1 = 0;
			if (gDIP_SpeakSetup[2] > 0){					// ������
				gLocal_1 |= 0x01;
			}
			if (gDIP_SpeakSetup[0] > 0){					// ǰ�ô�����
				gLocal_1 |= 0x02;
			}
			gLocal_1 |= gDIP_SpeakSetup[1] << 2;			// ����
			gLocal_1 |= gDIP_SpeakSetup[3] << 4;			// ����
			gLocal_1 |= gDIP_SpeakSetup[4] << 6;			// ����
			return gLocal_1;
		}
	case KCM_MIC_VOLUME :									// ��Ͳ����
	    return Tab_Mic16To10[counter];
	case KCM_MIC_DELAY :				                    // ��Ͳ�ӳ�ʱ��
	    return Tab_MicDelay[counter];
	}
	return 0;
}
BYTE MKCM_FromRegister(BYTE index, BYTE value){				// ��KCM���ļĴ�����ת�������������ֵ
	BYTE gLocal_1 = 0;
	switch (index){
	case KCM_INPUT_SOURCE :									// ����˿�ѡ��
		do {
			if (Tab_InputSwitch[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_InputSwitch));
		return INPUT_SWITCH_AUX;								// ������Χ������Ĭ��ֵAUX
	case KCM_LISTEN_MODE :									// ������ģʽ
		do {
			if (Tab_SurroundMode[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_SurroundMode));
		return 0x01;										// ������Χ������Ĭ��ֵ
	case KCM_TEST_TONE :									// ��������
		do {
			if (Tab_TestToneChannel[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_TestToneChannel));
		return 0;											// ������Χ������Ĭ��ֵ0
	case KCM_FL_TRIM :										// ����΢��
		do {
			if (Tab_ChannelTrim[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_ChannelTrim));
		return 0;											// ������Χ������Ĭ��ֵ0dB
    case KCM_SPK_CONFIG :								// ��������
		// B7:6����B5:4����B3:2����B1ǰ��B0������ ��������0=û�а�װ��ǰ�ã�0=С���ȣ�������0=û�а�װ 1=С���� 2=������
		// 0ǰ�� 1���� 2������ 3���� 4����
		gDIP_SpeakSetup[0] = (value & 0x02) ? 1 : 0;		// ǰ�ã�С(0)��(1)ѡ��
		gDIP_SpeakSetup[1] = (value >> 2) & 0x03;
		gDIP_SpeakSetup[2] = (value & 0x01) ? 1 : 0;		// ��������û�а�װ(0)/��װ(1)
	    gDIP_SpeakSetup[3] = (value >> 4) & 0x03;
	    gDIP_SpeakSetup[4] = (value >> 6) & 0x03;
		break;
	case KCM_MIC_VOLUME :								    // 0��Ͳ1���� 1��Ͳ2  2���� 3�ظ�
		do {
			if (Tab_Mic16To10[gLocal_1] == value){          // 0-15ת����0-9
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_Mic16To10));
	    return 0;
    case KCM_MIC_DELAY:                                     // ��Ͳ�ӳ�ʱ��
		do {
			if (Tab_MicDelay[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_MicDelay));
        return 0;
	}
	return 0;												// ������Χ������Ĭ��ֵ0dB
}
void MAPI_COPY_BUFF8(BYTE length, BYTE* in_data, BYTE* out_data){
	for (; length != 0; length--){
        *out_data++ = *in_data++;
    };
    return;
}
