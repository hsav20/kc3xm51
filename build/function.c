
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www,hsav.com/download/kc3xm51.zip


#include "main.h"           

void MKCM_Initialize(){										// KCMģ���ʼ��  	
	gDIP_Brightness = 0;
	MKCM_SetPowerOn();										// KCM����
}	  
void MKCM_SetPowerOn(){ 									// KCM����
	MKCM_WriteRegister(KCM_POWER_ON, 1);
    MDIP_MenuSelect(cMenu_PowerOn, MENU_NORMAL);
	gDIP_MenuLock = 100;									// ��ʱ������ʾ10��
	gAUD_SrcFormat = 0;
    gAUD_BpsRate = 0;
    g2SUB_SrcValid = 0;
    gSUB_SrcAuto = INPUT_SWITCH_NONE;                       // �Զ�ѡ��ʧЧ
    MLOG("KCM_POWER_ON\r\n");
}	  



void MKCM_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
    if (!HAL_KCM_I2C_INT()){                           		// detect KCM interrupt ��⵽I2C�ӻ������ж�,INTΪ��
	    BYTE gLocal_1;
	    BYTE gLocal_2;

		gLocal_1 = MKCM_ReadRegister(KCM_READ_IRQ);				// Read interrupt ��ȡKCM�ж�
		MKCM_WriteRegister(KCM_CLEAR_IRQ, gLocal_1);  			// Clear interrupt �����Ӧ���ж�
		if (FKCM_I2C_Error){
            MLOG("KCM_READ_IRQ Error\r\n");
			return;
		}
        MLOG("KCM_CLEAR_IRQ %02x\r\n", (u32)gLocal_1);
		if ((gLocal_1 & KCM_IRQ_SYSTEM_INIT) > 0){			// ģʽ��ʼ������жϣ���Ҫд��"KCM_POWER_ON"�Ĵ�����
            MKCM_RestoreMemory();
		}
		if ((gLocal_1 & KCM_IRQ_FORMAT_INFO) > 0){          // �����ź������ʽ�ı��жϣ���Ҫ��ȡ"KCM_SRC_FORMAT"�Ĵ���
			gAUD_SrcFormat = MKCM_ReadRegister(KCM_SRC_FORMAT);
            gAUD_BpsRate = MKCM_ReadRegister(KCM_BPS_RATE);
			if (gDIP_MenuLock == 0){						// 
				if (!FSYS_TestTone){						// û�д���������
					MDIP_MenuSelect(cMenu_SrcFormat, MENU_NORMAL);
				}
			}
			if (gAUD_SrcFormat == 0){
				MDIP_CleanSpectrum();
	    	}
			MDIP_SrcFormatSymbol();
            MDIP_SurroundSymbol();
        	if (mINPUT_SWITCH == INPUT_SWITCH_SD || mINPUT_SWITCH == INPUT_SWITCH_UDISK){
                g2TimeLength = MKCM_Read2Byte(KCM_RD_FILE_TIME);
            }
		}
		if ((gLocal_1 & KCM_IRQ_SRC_VALID) > 0){            // ��Ч����Դ����ı��жϣ���Ҫ��ȡ"KCM_SRC_VALID"�Ĵ���
            MKCM_ReadSrcValid();
		}
		if ((gLocal_1 & KCM_IRQ_VOLUME) > 0){               // ��Ч����Դ����ı��жϣ���Ҫ��ȡ"KCM_SRC_VALID"�Ĵ���
			gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);     // ��ȡ��ǰ����ֵ
			if (gDIP_MenuSelect == cMenu_MasterVolume){
				MDIP_MenuSelect(cMenu_MasterVolume, 0);
			}
		}
        if ((gLocal_1 & KCM_IRQ_FIRMWARE) > 0){             // �̼����£���Ҫ��ȡ"KCM_RD_INFO"�Ĵ���
            MDIP_MenuSelect(cMenu_Fireware, MENU_NORMAL);
        }
        if ((gLocal_1 & KCM_IRQ_MEDIA_TIME) > 0){           // ��ý�岥��ʱ��ı�
            g2PlayTime = MKCM_Read2Byte(KCM_PLAY_TIME);
            if (g2PlayTime){                                // ����ʱ��ı� 
                MDIP_MenuSelect(cMenu_PlayTime, MENU_NORMAL);
            }else {                                         // ���������
                WORD g2Local_1 = (mINPUT_SWITCH == INPUT_SWITCH_SD) ? g2SdQty : g2UDiskQty;
                if (++g2PlayIndex >= g2Local_1){
                    g2PlayIndex = 0;
                }
                MKCM_Write2Byte(KCM_PLAY_INDEX, g2PlayIndex);  // ����g2PlayIndex
                MDIP_MenuSelect(cMenu_PlayTrack, MENU_NORMAL);
            }
        }
		if ((gLocal_1 & KCM_IRQ_WIFI_RCV) > 0){
		    WORD g2Local_1 = MKCM_Read2Byte(KCM_COMMAND_RCV);
			MKCM_WifiCommand(g2Local_1, g2Local_1 >> 8);
			gSYS_WifiCommand = 7;
		}
		if ((gLocal_1 & KCM_IRQ_WIFI_STATUS) > 0){
			MDIP_WifiSymbol(0xff);	    	 
		}
    }
    return;
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

	// ����˿�ѡ��ʹ��KCM_INPUT�Ĵ����ļ���ֵ
	value = MKCM_ReadRegister(KCM_INPUT_SOURCE);			// ���������˿�ѡ��
	if (FKCM_I2C_Error){									// ��ȡKCM����
		MDIP_MenuSelect(cMenu_Restore, 0);
		return;
	}
	gSYS_ModelType = MKCM_ReadRegister(KCM_RD_INFO);
	mINPUT_SWITCH = MKCM_FromRegister(KCM_INPUT_SOURCE, value);	// ͨ���Ĵ�������ѡ�����ֵ�ָ�
//MDEBUG(0xf8);MDEBUG(value);MDEBUG(mINPUT_SWITCH);
	// ������ʹ��KCM_VOLUME�Ĵ����ļ���ֵ
	gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);	// ���������ֵ
	// ��Ч����ͨ����ʹ��KCM_EQ_SELECT�����ԵļĴ����ļ���ֵ
	gDIP_SoundEffect = MKCM_ReadRegister(KCM_EQ_SELECT);			// �������Ч����ͨ��ѡ��

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

	// KCM��չ���䣺ʹ��KCM_MEMORYRD�Ĵ����ļ���ֵ
	MKCM_ReadXByte(KCM_EXTR_MEMORY, 4, temp);               // �û�����
	gDIP_Surround[0] = temp[MEM_SURROUND_2CH];              // ������ģʽ������
	gDIP_Surround[1] = temp[MEM_SURROUND_8CH];              // ������ģʽ������
	gDIP_Select2Ch = temp[MEM_SELECT_2CH];                  // ѡ��Ϊ������
	gDIP_Brightness = temp[MEM_BRIGHTNESS];                 // ��ʾ������
    MDIP_Brightness(0, gDIP_Brightness);
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
    MKCM_WriteXByte(KCM_CUSTOM_CODE, 4, temp);              // �����û��Զ��幦�ܴ��뼰����ģ��̼��Ĵ���
                      
    for (select = 0; select < 4; select++){
        // ��·����EQ��Ч�������ã�һ��д��һ�����ڳ�ʼ��
        for (counter = 0; counter < sizeof(TabEqSetup); counter++){ 
            temp[counter + 1] = TabEqSetup[counter];
        }
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_SETUP, 1 + sizeof(TabEqSetup), temp);   // ��·����EQ��Ч��������

        // ��·����EQ��ЧƵ�ʣ�һ��д��һ�����ڳ�ʼ��
        for (counter = 0; counter < sizeof(TabEqFreq); counter++){ // д��EQƵ��
            temp[counter + 1] = TabEqFreq[counter];
        }
        for (counter = 0; counter < sizeof(TabEqFreq); counter++){ // д��EQƵ�ʵ�ƽ
            temp[counter + 1] |= TabEqLevel[select][counter];
        }
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_VALUE, 1 + sizeof(TabEqFreq), temp);   // ��·����EQ��Ч��������
    }
    MKCM_WriteXByte(KCM_WR_SPECTRUM, sizeof(TabSpectrum), TabSpectrum);   // ����Ƶ��ģʽ

    g2SUB_SrcValid = 0;                                     // ���¿�ʼ���
	FSYS_Standby = 0;
	FSYS_TestTone = 0;
	FSYS_MuteEnable = 0;
    MDIP_MenuSelect(cMenu_PowerOn, 0);
	gDIP_MenuLock = 10;										// �����˳���ʱ������ʾ��
    MDIP_SurroundSymbol();
    MDIP_SrcFormatSymbol();
	MDIP_WifiSymbol(0xff);	    	 
    return;
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
	gLocal_Buffer[MEM_SURROUND_2CH] = 0;                    // ������ģʽ������
	gLocal_Buffer[MEM_SURROUND_8CH] = 0;                    // ������ģʽ������
	gLocal_Buffer[MEM_SELECT_2CH] = 0;                      // ѡ��Ϊ������
	gLocal_Buffer[MEM_BRIGHTNESS] = 2;                      // ��ʾ������
	MKCM_WriteXByte(KCM_EXTR_MEMORY, 4, gLocal_Buffer);

	MKCM_WriteRegister(KCM_VOLUME_CTRL, 39); 				// ����ֵ

	// ����΢����ʹ��KCM_FL_TRIM�ȼĴ����ļ���ֵ
	gLocal_1 = 0;
	do {
		MKCM_WriteRegister(KCM_FL_TRIM + gLocal_1, 0);
	} while (++gLocal_1 < 8);

	MKCM_WriteRegister(KCM_SPK_CONFIG, 0xab);				// ���ô󡢻��ƴ����ô�ǰ�ô��г�����
	MKCM_WriteRegister(KCM_POWER_ON, 1);
    MDIP_MenuSelect(cMenu_PowerOn, 0);
	gDIP_MenuLock = 30;										// ��ʱ������ʾ3��
    return;
}

char MKCM_HdmiInsert(WORD flag, WORD g2Local_1){            // ���HDMI���룬�в��뷵��1
    if (g2Local_1 & flag){                                  // �����б��ε�HDMI
        if (!(g2SUB_SrcValid & flag)){                      // �ϴ�û�б��ε�HDMI
            if (mINPUT_SWITCH < INPUT_SWITCH_HDMI1 && mINPUT_SWITCH > INPUT_SWITCH_H_ARC){ // ���ԭ������ѡ��HDMI
                if (mINPUT_SWITCH != INPUT_SWITCH_SD && mINPUT_SWITCH != INPUT_SWITCH_UDISK){ // ���ԭ������ѡ��SD/U��
                    gSUB_SrcAuto = mINPUT_SWITCH;           // ����ԭ����ѡ��
                }
            }
            if (flag == KCM_SRC_VALID_HDMI1){
                mINPUT_SWITCH = INPUT_SWITCH_HDMI1;         // �Զ�����HDMI1
            }else if (flag == KCM_SRC_VALID_HDMI2){
                mINPUT_SWITCH = INPUT_SWITCH_HDMI2;         // �Զ�����HDMI2
            }else {
                mINPUT_SWITCH = INPUT_SWITCH_HDMI3;         // �Զ�����HDMI3
            }
            //MKCM_WriteRegister(KCM_INPUT_SOURCE, MKCM_ToRegister(KCM_INPUT_SOURCE, mINPUT_SWITCH));  
			gDIP_MenuSelect = cMenu_Restore;				// ���ܲ˵����������Ҫ���϶���
            MAUD_MixInputSource(mINPUT_SWITCH);
//            MLOG("HdmiInsertB:%d %02x\r\n", (u32)mINPUT_SWITCH, (u32)MKCM_ToRegister(KCM_INPUT_SOURCE, mINPUT_SWITCH));
            return 1;
        }
    }
    return 0;
}
char MKCM_HdmiRemove(WORD flag, WORD g2Local_1){            // ���HDMI�γ����аγ�����1
    if (!(g2Local_1 & flag)){                               // ����û��HDMI
        if (g2SUB_SrcValid & flag){                         // �ϴ���HDMI
            if (gSUB_SrcAuto != INPUT_SWITCH_NONE){         // �����Զ�ѡ��ʧЧ
                MKCM_WriteRegister(KCM_INPUT_SOURCE, MKCM_ToRegister(KCM_INPUT_SOURCE, gSUB_SrcAuto));  // �ָ�ԭ��������
                MLOG("MKCM_HdmiRemove:%d\r\n", gSUB_SrcAuto);
            }
            return 1;
        }
    }
    return 0;
}

char MKCM_SdUdiskInsert(WORD flag, WORD g2Local_1){         // ���SD/U�̲��룬�в��뷵��1
    if (g2Local_1 & flag){                                  // ������SD/U��
        if (!(g2SUB_SrcValid & flag)){                      // �ϴ�û��SD/U��
            WORD track = MKCM_Read2Byte((flag == KCM_SRC_VALID_SD) ? KCM_RD_SD_QTY : KCM_RD_UDISK_QTY);
    //MDEBUG(0xe8);MDEBUG(total);
            if (track){                                     // �ҵ��ļ�
                if (mINPUT_SWITCH != INPUT_SWITCH_SD && mINPUT_SWITCH != INPUT_SWITCH_UDISK){ // ���ԭ������ѡ��SD/U��
                    gSUB_SrcAuto = mINPUT_SWITCH;           // ����ԭ����ѡ��
                }
                if (flag == KCM_SRC_VALID_SD){
                    mINPUT_SWITCH = INPUT_SWITCH_SD;
                    g2SdQty = track;
                }else {
                    mINPUT_SWITCH = INPUT_SWITCH_UDISK;
                    g2UDiskQty = track;
                }
                MKCM_WriteRegister(KCM_INPUT_SOURCE, MKCM_ToRegister(KCM_INPUT_SOURCE, mINPUT_SWITCH));
                g2PlayIndex = 0;
                MKCM_Write2Byte(KCM_PLAY_INDEX, g2PlayIndex);  // ���ŵ�0��
            }else {
                MDIP_MenuSelect((flag == KCM_SRC_VALID_SD) ? cMenu_SdInsert : cMenu_UDiskInsert, 0);     // ��ʾSD/U�̲���
            }
            return 1;
        }
    }
    return 0;
}
char MKCM_SdUdiskRemove(WORD flag, WORD g2Local_1){         // ���SD/U�̰γ����аγ�����1
    if (!(g2Local_1 & flag)){                               // ����û��SD/U��
        if (g2SUB_SrcValid & flag){                         // �ϴ���SD/U��
            if (flag == KCM_SRC_VALID_SD){
                g2SdQty = 0;
            }else {
                g2UDiskQty = 0;
            }
            if (gSUB_SrcAuto != INPUT_SWITCH_NONE){         // �����Զ�ѡ��ʧЧ
                MKCM_WriteRegister(KCM_INPUT_SOURCE, MKCM_ToRegister(KCM_INPUT_SOURCE, gSUB_SrcAuto));  // �ָ�ԭ��������
            }else {
                MDIP_MenuSelect((flag == KCM_SRC_VALID_SD) ? cMenu_UDiskRemove : cMenu_UDiskRemove, 0);         // ��ʾSD/U�̲��
            }
            return 1;
        }
    }
    return 0;
}
void MKCM_ReadSrcValid(){
	WORD g2Local_1 = MKCM_Read2Byte(KCM_SRC_VALID);         // ���ε���Ч��Դ
//if(g2Local_1==KCM_SRC_VALID_UDISK){g2Local_1=KCM_SRC_VALID_HDMI1|KCM_SRC_VALID_HDMI2;}
    if (g2SUB_SrcValid != g2Local_1){                       // �������ϴε���Ч��Դ�ı�
        MLOG("KCM_SRC_VALID:%04x Last:%04x\r\n", (u32)g2Local_1, (u32)g2SUB_SrcValid);
        if (!MKCM_HdmiInsert(KCM_SRC_VALID_HDMI1, g2Local_1)){         // ���HDMI1���룬û�вż��HDMI2����
            if (!MKCM_HdmiInsert(KCM_SRC_VALID_HDMI2, g2Local_1)){     // ���HDMI2���룬û�вż��HDMI3����
                MKCM_HdmiInsert(KCM_SRC_VALID_HDMI3, g2Local_1);       // ���HDMI3����
            }
        }
        MKCM_HdmiRemove(KCM_SRC_VALID_HDMI1, g2Local_1);
        MKCM_HdmiRemove(KCM_SRC_VALID_HDMI2, g2Local_1);
        MKCM_HdmiRemove(KCM_SRC_VALID_HDMI3, g2Local_1);

        if (!MKCM_SdUdiskInsert(KCM_SRC_VALID_UDISK, g2Local_1)){   // ���U�̲��룬û�вż��SD����
            MKCM_SdUdiskInsert(KCM_SRC_VALID_SD, g2Local_1);    // ���SD����
        }
        MKCM_SdUdiskRemove(KCM_SRC_VALID_SD, g2Local_1);
        MKCM_SdUdiskRemove(KCM_SRC_VALID_UDISK, g2Local_1);
        g2SUB_SrcValid = g2Local_1;
    }else {
        MLOG("KCM_SRC_VALID = Last:%04x\r\n", (u32)g2Local_1);
    }
}

void MKCM_WifiCommand(BYTE regNumber, BYTE value){		// �յ�Զ��APP��ָ��
//MDEBUG(0xa9);MDEBUG(regNumber);MDEBUG(value);
	switch (regNumber){
	case KCM_VOLUME_MUTE:									// ��Ƶ�����������Ӽ�����
		if (FSYS_MuteEnable != (value & 0x01)){				// MUTE�ı�
			MKEY_AudioMute();
		}
		else if (value & 0x04){								// �����ӻ��
			gDIP_MenuSelect = cMenu_MasterVolume;
			MKCM_WriteRegister(KCM_VOLUME_MUTE, value);
		}
		break;
	case KCM_INPUT_SOURCE:										// ������Դѡ��
		mINPUT_SWITCH = MKCM_FromRegister(KCM_INPUT_SOURCE, value);	// ͨ���Ĵ�������ѡ�����ֵ�ָ�
		MAUD_AutoCanclMute();
		MAUD_AutoCanclTestTone();
		MDIP_MenuSelect(cMenu_Restore, 0);
		break;
	}
}


// //////////////////////////////////////////////////
// KCM�Ĵ�����ֵ�뱾�����ڵ�ֵ�໥ת��
// //////////////////////////////////////////////////
CONST_CHAR Tab_InputSwitch[] = {							// KCM_INPUT_SOURCE
	KCM_INPUT_ANALOG,                                       // 0=ģ������
	KCM_INPUT_DIGITAL | 0,                                  // 1=����1
    KCM_INPUT_DIGITAL | 1,                                  // 2=����2
    KCM_INPUT_DIGITAL | 2, 			                        // 3=����3
    KCM_INPUT_MEDIA | 0,                                    // 4=SD
    KCM_INPUT_MEDIA | 1,		                            // 5=UDISK
    KCM_INPUT_HDMI | 0,				                        // 6=HDMI1
    KCM_INPUT_HDMI | 1,                                     // 7=HDMI2
    KCM_INPUT_HDMI | 2,                                     // 8=HDMI3
    KCM_INPUT_HDMI | 4,	                                    // 9=HDMI-ARC 
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
