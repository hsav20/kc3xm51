
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www,hsav.com/download/kc3xm51.zip

#include "main.h"           

void MAUD_Initialize(){										// ����ģ���ʼ��  	
}	  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MAUD_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
    return;
}
void MAUD_MixInputSource(BYTE index){
    DIP_SURROUND_OFF();
    DIP_SRC_FORMAT_OFF();
	MAUD_AutoCanclMute();
	MAUD_AutoCanclTestTone();
	if (gDIP_MenuSelect == cMenu_Restore){					// ֻ�в˵��Ѿ�����cMenu_Restore�󣬲ſ��Ըı䵱ǰ������
		if (index == INPUT_SWITCH_NONE){                    // ѭ����ʽ
			if (++mINPUT_SWITCH > INPUT_SWITCH_H_ARC){
				mINPUT_SWITCH = INPUT_SWITCH_AUX;
			}
		}
		else {
            gSUB_SrcAuto = INPUT_SWITCH_NONE;               // �Զ�ѡ��ʧЧ
			mINPUT_SWITCH = index;
		}
		MKCM_WriteRegister(KCM_INPUT_SOURCE, MKCM_ToRegister(KCM_INPUT_SOURCE, mINPUT_SWITCH));
		MLOG("MixInputSource:%d R:%02x\r\n", (u32)index, (u32)MKCM_ReadRegister(KCM_INPUT_SOURCE));
	}else {
		MLOG("MixInputSourceB:%d R:%d\r\n", (u32)gDIP_MenuSelect, (u32)cMenu_Restore);
	}
	MDIP_MenuSelect(cMenu_Restore, MENU_NORMAL);
}
void MAUD_MixMasterVolume(BYTE directUp){
	BYTE value;

	MAUD_AutoCanclTestTone();
	FAUD_MasterVolume = 1;
	if (directUp){
        value = 0x30;
        if (FSYS_MuteEnable){
		    FSYS_MuteEnable = 0;
            value |= 0x02;                                  // B1Ϊ������Ƶ�ľ��� B0=0�����ر� 
        }
	}
	else {
		value = 0x20;
	}
	MKCM_WriteRegister(KCM_VOLUME_MUTE, value);	            // B5Ϊ���������ļӼ���B4Ϊ0��ʾ����ֵ��
//MDEBUG(0xb1);MDEBUG(value);
}
void MAUD_TestToneChannel(BYTE channel){					// ��������
	if (channel < 0xff){
		MKCM_WriteRegister(KCM_TEST_TONE, 0x10 | MKCM_ToRegister(KCM_TEST_TONE, channel));	// ��������ͨ��
//MDEBUG(0xb1);MDEBUG(0x10 | MKCM_ToRegister(KCM_TEST_TONE, channel));
	}
	else {									 				// �ر�
		MKCM_WriteRegister(KCM_TEST_TONE, 0);				// �������Թر�
	}
}
void MAUD_MixSoundEffect(){
	MAUD_AutoCanclMute();
	MAUD_AutoCanclTestTone();
	if (++gDIP_SoundEffect > 4){
		gDIP_SoundEffect = 0;
	}
	MKCM_WriteRegister(KCM_EQ_SELECT, gDIP_SoundEffect);	// ѡ����Ҫ����Ч����ͨ��
//MDEBUG(0xa7);MDEBUG(gDIP_SoundEffect);
}
void MAUD_MixTrimAdjust(BYTE index, BYTE directUp){
    BYTE address;
    BYTE value;

	MAUD_AutoCanclMute();
	if (directUp){
		if (gDIP_TrimCtrl[index] < 18){
			++gDIP_TrimCtrl[index];
		}
	}
	else {
		if (gDIP_TrimCtrl[index] > 0){
			--gDIP_TrimCtrl[index];
		}
	}
    
    address = MKCM_ToRegister(KCM_TEST_TONE, index);
    value = MKCM_ToRegister(KCM_FL_TRIM, gDIP_TrimCtrl[index]);
//MDEBUG(0xaa);MDEBUG(index);MDEBUG(KCM_FL_TRIM + address);MDEBUG(value);
    MKCM_WriteRegister(KCM_FL_TRIM + address, value);
}
void MAUD_MixMicAdjust(BYTE index, BYTE directUp){
	BYTE value;
    BYTE temp0;

	MAUD_AutoCanclMute();
    temp0 = index - cMenu_MicVolume1;
	if (directUp){
		if (gDIP_MicCtrl[temp0] < 9){
			++gDIP_MicCtrl[temp0];
		}
	}
	else {
		if (gDIP_MicCtrl[temp0] > 0){
			--gDIP_MicCtrl[temp0];
		}
	}
    switch (index){                                     // 0��Ͳ1���� 1��Ͳ2  2���� 3�ظ� 4�ӳ� 5����
    case cMenu_MicVolume1:                               // ��Ͳ1����
    case cMenu_MicVolume2:                               // ��Ͳ2����
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[0]);
        temp0 = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[1]);
        MKCM_WriteRegister(KCM_MIC_VOLUME, temp0 << 4 | value);   // ��Ͳ1����Ͳ2��������
        break;
    case cMenu_MicEcho:                                 // ��Ͳ��������
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[2]);
        temp0 = 0;                                  // ��Ͳ���EQ������Ч����ѡ�����
        MKCM_WriteRegister(KCM_MIC_ECHO_EQ, temp0 << 4 | value);   // ��Ͳ��������Ͳ���EQ������Ч����ѡ�����
        break;
    case cMenu_MicRepeat:                               // ��Ͳ�ظ�����
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[3]);
        temp0 = 8;                                  // ֱ���������̶�Ϊ8
        MKCM_WriteRegister(KCM_MIC_REPEAT, temp0 << 4 | value);
        break;
    case cMenu_MicReverb:                               // ��Ͳ�������
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[5]);
        temp0 = 1;                                  // ��Ͳ����2�̶�Ϊ1
        MKCM_WriteRegister(KCM_MIC_REVERB, temp0 << 4 | value);
        break;
    case cMenu_MicDelay:                                // ��Ͳ�ӳ�ʱ�䣬ÿ��20����
        value = MKCM_ToRegister(KCM_MIC_DELAY, gDIP_MicCtrl[4]);    // 0-255ת����0-9
        MKCM_WriteRegister(KCM_MIC_DELAY, value);
        break;
    }
}
void MAUD_AutoCanclTestTone(){
//MDEBUG(0xa7);
	if (FSYS_TestTone){
		FSYS_TestTone = 0;
		MKCM_WriteRegister(KCM_TEST_TONE, 0);				// �������Թر�
	}

}
void MAUD_AutoCanclMute(){
	if (FSYS_MuteEnable){
		FSYS_MuteEnable = 0;
	}
}
