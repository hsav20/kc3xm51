
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www,hsav.com/download/kc3xm51.zip

#include "main.h"           

void MAUD_Initialize(){										// 按键模块初始化  	
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
	if (gDIP_MenuSelect == cMenu_Restore){					// 只有菜单已经换到cMenu_Restore后，才可以改变当前的输入
		if (index == INPUT_SWITCH_NONE){                    // 循环方式
			if (++mINPUT_SWITCH > INPUT_SWITCH_H_ARC){
				mINPUT_SWITCH = INPUT_SWITCH_AUX;
			}
		}
		else {
            gSUB_SrcAuto = INPUT_SWITCH_NONE;               // 自动选择失效
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
            value |= 0x02;                                  // B1为控制音频的静音 B0=0静音关闭 
        }
	}
	else {
		value = 0x20;
	}
	MKCM_WriteRegister(KCM_VOLUME_MUTE, value);	            // B5为控制音量的加减；B4为0表示音量值减
//MDEBUG(0xb1);MDEBUG(value);
}
void MAUD_TestToneChannel(BYTE channel){					// 噪音测试
	if (channel < 0xff){
		MKCM_WriteRegister(KCM_TEST_TONE, 0x10 | MKCM_ToRegister(KCM_TEST_TONE, channel));	// 噪音测试通道
//MDEBUG(0xb1);MDEBUG(0x10 | MKCM_ToRegister(KCM_TEST_TONE, channel));
	}
	else {									 				// 关闭
		MKCM_WriteRegister(KCM_TEST_TONE, 0);				// 噪音测试关闭
	}
}
void MAUD_MixSoundEffect(){
	MAUD_AutoCanclMute();
	MAUD_AutoCanclTestTone();
	if (++gDIP_SoundEffect > 4){
		gDIP_SoundEffect = 0;
	}
	MKCM_WriteRegister(KCM_EQ_SELECT, gDIP_SoundEffect);	// 选择需要的音效处理通道
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
    switch (index){                                     // 0话筒1音量 1话筒2  2回声 3重复 4延迟 5混响
    case cMenu_MicVolume1:                               // 话筒1音量
    case cMenu_MicVolume2:                               // 话筒2音量
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[0]);
        temp0 = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[1]);
        MKCM_WriteRegister(KCM_MIC_VOLUME, temp0 << 4 | value);   // 话筒1及话筒2音量比例
        break;
    case cMenu_MicEcho:                                 // 话筒回声比例
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[2]);
        temp0 = 0;                                  // 话筒多段EQ均衡音效处理选择比例
        MKCM_WriteRegister(KCM_MIC_ECHO_EQ, temp0 << 4 | value);   // 话筒回声及话筒多段EQ均衡音效处理选择比例
        break;
    case cMenu_MicRepeat:                               // 话筒重复比例
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[3]);
        temp0 = 8;                                  // 直达声比例固定为8
        MKCM_WriteRegister(KCM_MIC_REPEAT, temp0 << 4 | value);
        break;
    case cMenu_MicReverb:                               // 话筒混响比例
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[5]);
        temp0 = 1;                                  // 话筒混响2固定为1
        MKCM_WriteRegister(KCM_MIC_REVERB, temp0 << 4 | value);
        break;
    case cMenu_MicDelay:                                // 话筒延迟时间，每步20毫秒
        value = MKCM_ToRegister(KCM_MIC_DELAY, gDIP_MicCtrl[4]);    // 0-255转换到0-9
        MKCM_WriteRegister(KCM_MIC_DELAY, value);
        break;
    }
}
void MAUD_AutoCanclTestTone(){
//MDEBUG(0xa7);
	if (FSYS_TestTone){
		FSYS_TestTone = 0;
		MKCM_WriteRegister(KCM_TEST_TONE, 0);				// 噪音测试关闭
	}

}
void MAUD_AutoCanclMute(){
	if (FSYS_MuteEnable){
		FSYS_MuteEnable = 0;
	}
}
