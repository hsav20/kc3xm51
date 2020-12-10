
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           

void MAUD_Initialize(){										// 按键模块初始化  	
}	  
void MAUD_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
    if (gRemoveTimer && ++gRemoveTimer > 200){			// 大约2秒后 
		INPUT_SWITCH select = MKCM_ReadRegister(KCM_EXTR_MEMORY + MEM_SOURCE_AUTO);	// 自动输入的恢复
    	MLOG("gRemoveTimer %d\r\n", (u32)select);
		gDIP_MenuSelect = MENU_RESTORE;				        // 菜单即刻进入输入的恢复 
		MAUD_InputSelect(select);
    	gRemoveTimer = 0;
    }
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
/*
void MAUD_MixSoundEffect(){
	MAUD_AutoCanclMute();
	MAUD_AutoCanclTestTone();
	if (++gDIP_SoundEffect > 4){
		gDIP_SoundEffect = 0;
	}
	MKCM_WriteRegister(KCM_EQ_SELECT, gDIP_SoundEffect);	// 选择需要的音效处理通道
//MDEBUG(0xa7);MDEBUG(gDIP_SoundEffect);
}
*/
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 音源输入选择及抢占式处理  //////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MAUD_InputSelect(INPUT_SWITCH select){
    DIP_SURROUND_OFF();
    DIP_SRC_FORMAT_OFF();
	MAUD_AutoCanclMute();
	MAUD_AutoCanclTestTone();
	
	if (gDIP_MenuSelect == MENU_RESTORE || gDIP_MenuSelect == MENU_INPUT_SOURCE){	// 只有菜单已经换到MENU_RESTORE后，才可以改变当前的输入
		mINPUT_SWITCH = select;
		MAUD_InputWrite(mINPUT_SWITCH, mINPUT_SWITCH);
	}
	MDIP_MenuNormal(MENU_INPUT_SOURCE);                    // 菜单状态:输入音源选择
}
void MAUD_Preemptible(){							// 抢占式输入选择 
	MLOG("MKCM_Preemptible:%d %d/%d\r\n", (u32)gPreemptibleIn[gPreemptibleStep], (u32)gPreemptibleStep, (u32)gPreemptibleQty);
	if (gPreemptibleQty){
		MAUD_InputSelect(gPreemptibleIn[gPreemptibleStep]);
		if (++gPreemptibleStep >= gPreemptibleQty){
			gPreemptibleStep = 0;
		}
	}else {
		MAUD_InputSelect(mINPUT_SWITCH);
	}
}
CONST_CHAR TabInputOneKey[] = {
	INPUT_SWITCH_AUX,                                   // 0=模拟输入
    INPUT_SWITCH_OPTIC,                                 // 1=数码1   
    INPUT_SWITCH_COA1,                                  // 2=数码2   
    INPUT_SWITCH_COA2,                                  // 3=数码3   
};
void MAUD_InputOneKey(){								// 所有输入用一个按键选择 
	gDIP_MenuSelect = MENU_RESTORE;					// 菜单即刻进入输入的恢复 
	if (gWithHdmiQty && gWithHdmiStep < gWithHdmiQty){	 // 循环方式
		MAUD_InputSelect(gPreemptibleIn[gWithHdmiStep]);
	}else {
		MAUD_InputSelect(TabInputOneKey[gWithHdmiStep - gWithHdmiQty]);
	}
	if (++gWithHdmiStep >= (sizeof(TabInputOneKey) + gWithHdmiQty)){
		gWithHdmiStep = 0;
	}
}
void MAUD_InputWrite(INPUT_SWITCH select, INPUT_SWITCH last){
	MKCM_WriteRegister(KCM_INPUT_SOURCE, MKCM_ToRegister(KCM_INPUT_SOURCE, select));
    MLOG("InputSource:%d last:%02x %02x\r\n", (u32)select, (u32)last, (u32)MKCM_ReadRegister(KCM_INPUT_SOURCE));
	if (last <= INPUT_SWITCH_COA2){ 								// 如果原来不是抢占式输入
	    MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_SOURCE_AUTO, last);	// 自动输入的恢复
		MLOG("InputSourceB:%d last:%02x\r\n", (u32)select, (u32)MKCM_ReadRegister(KCM_INPUT_SOURCE));
//		return;
	}
}
CONST_WORD TabSrcValid[] = {
	KCM_SRC_VALID_SD, KCM_SRC_VALID_UDISK,
	KCM_SRC_VALID_USBA, KCM_SRC_VALID_BT,
	KCM_SRC_VALID_HDMI1, KCM_SRC_VALID_HDMI2, KCM_SRC_VALID_HDMI3, 
	KCM_SRC_VALID_E8CH
}; 
CONST_CHAR TabValidSwitch[] = {
	INPUT_SWITCH_SD, INPUT_SWITCH_UDISK,
	INPUT_SWITCH_USBA, INPUT_SWITCH_BT,
	INPUT_SWITCH_HDMI1, INPUT_SWITCH_HDMI2, INPUT_SWITCH_HDMI3,
    INPUT_SWITCH_E8CH
};
void MAUD_MakePreemptible(WORD g2Local_1){			// 生成抢占式输入选择 
    if (g2Local_1){									// 有抢占式输入 
    	BYTE qty = 0;
    	BYTE index;
    	WORD flag;
    	for (index = 0; index < sizeof(TabSrcValid)/2; index++){
        	flag = TabSrcValid[index];
        	if (g2Local_1 & flag){    				// 有非抢占式
        		if (flag == KCM_SRC_VALID_SD){
	    			if (!g2SdQty){
			    		continue;	
		    		} 
	        	}else if (flag == KCM_SRC_VALID_UDISK){
	    			if (g2SdQty){
			    		continue;	
		    		} 
	        	}
				gPreemptibleIn[qty++] = TabValidSwitch[index];
        	}
    	}
		if (gWithHdmiQty != qty){						// 带HDMI的抢占式输入  
			gWithHdmiStep = 0;
			gWithHdmiQty = qty;
		}
		for (index = 0; index < gWithHdmiQty; index++){
			if (gPreemptibleIn[index] >= INPUT_SWITCH_HDMI1 && gPreemptibleIn[index] <= INPUT_SWITCH_H_ARC){
				break;
			}
		}
		if (gPreemptibleQty != index){						// 不带HDMI的抢占式输入  
			gPreemptibleStep = 0;
			gPreemptibleQty = qty;
		}
  	  	MLOG("KCM_SRC_VALID E %d %d\r\n", (u32)gPreemptibleQty, (u32)gWithHdmiQty);  
    }else {												// 没有了抢占式输入
		gPreemptibleQty = 0;
		gWithHdmiQty = 0; 
		gPreemptibleStep = 0;
		gWithHdmiStep = 0;
		g2SdQty = 0;
		g2UDiskQty = 0;
    }
    g2AUD_SrcValid = g2Local_1;
}

void MKCM_ReadSrcValid(){
	WORD g2Local_1 = MKCM_Read2Byte(KCM_SRC_VALID);         // 本次的有效音源
//if(g2Local_1==KCM_SRC_VALID_UDISK){g2Local_1=KCM_SRC_VALID_HDMI1|KCM_SRC_VALID_HDMI2;}
    if (g2AUD_SrcValid != g2Local_1){                       // 本次与上次的有效音源改变
		BYTE index;
    	WORD flag;
    	BOOL found = 0;

        MLOG("KCM_SRC_VALID:%04x Last:%04x\r\n", (u32)g2Local_1, (u32)g2AUD_SrcValid);

        for (index = 0; index < sizeof(TabSrcValid)/2; index++){
        	flag = TabSrcValid[index];
        	if ((g2Local_1 & flag) && !(g2AUD_SrcValid & flag)){    // 本次有及上次没有非抢占式
		       	found = 1;
	        	switch (flag){
	            case KCM_SRC_VALID_SD:
		            g2SdQty = MKCM_Read2Byte(KCM_PLAY_SD_QTY);
		            if (g2SdQty){								// 有播放文件 
			            g2PlayIndex = 0;
	                	MKCM_Write2Byte(KCM_PLAY_INDEX, g2PlayIndex);  // 播放第0首	         
            		}else {
		            	MDIP_MenuNormal(cMenu_SdInsert); 
		            	found = 0;								// 没有播放文件 
		            }
					break;	
	            case KCM_SRC_VALID_UDISK: 
					g2UDiskQty = MKCM_Read2Byte(KCM_PLAY_UDISK_QTY);
					if (g2UDiskQty){								// 有播放文件 
			            g2PlayIndex = 0;
	                	MKCM_Write2Byte(KCM_PLAY_INDEX, g2PlayIndex);  // 播放第0首	         
            		}else {										// 没有播放文件 
		            	MDIP_MenuNormal(cMenu_UDiskInsert); 
		            	found = 0;
		            }
					break;	
      			}
      			if (found){
					MAUD_InputWrite(TabValidSwitch[index], mINPUT_SWITCH);
					mINPUT_SWITCH = TabValidSwitch[index];
					gDIP_MenuSelect = MENU_RESTORE;				// 菜单即刻进入输入的恢复 
					MDIP_MenuNormal(MENU_INPUT_SOURCE);     // 菜单状态:输入音源选择
					gRemoveTimer = 0;					// 取消拔出恢复到非抢占式插入
			    }
	        	break;									// 只处理一个抢占式插入
        	}
        }
        if (!found){									// 没有非抢占式插入
			for (index = 0; index < sizeof(TabSrcValid)/2; index++){
	        	flag = TabSrcValid[index];
	    //MLOG("KCM_SRC_VALID C %d %04x\r\n", (u32)index, (u32)flag);  
				if (!(g2Local_1 & flag) && (g2AUD_SrcValid & flag)){    // 本次没有及上次有非抢占式
		            switch (flag){
		            case KCM_SRC_VALID_SD:
						g2SdQty = 0; 
						MDIP_PlaySymbol(0);
						MDIP_MenuNormal(cMenu_SdRemove); 
						break;	
		            case KCM_SRC_VALID_UDISK: 
						g2UDiskQty = 0;
						MDIP_PlaySymbol(0);
						MDIP_MenuNormal(cMenu_UDiskRemove); 
						break;	
		            case KCM_SRC_VALID_USBA: 
						MDIP_MenuNormal(cMenu_UsbaRemove); 
						break;	
		            case KCM_SRC_VALID_BT: 
						MDIP_MenuNormal(cMenu_BtRemove); 
						break;	
		            case KCM_SRC_VALID_HDMI1: 
		            case KCM_SRC_VALID_HDMI2: 
		            case KCM_SRC_VALID_HDMI3: 
			            break;
		            }
					if (mINPUT_SWITCH == TabValidSwitch[index]){	// 有抢占式拔出而且是当前的输入
						gRemoveTimer = 1;					// 大约2秒后 
			        }					
		            break;									// 只处理一个抢占式拔出
	        	}
	        }
        }
		MAUD_MakePreemptible(g2Local_1);					// 生成抢占式输入选择 
    }else {
        MLOG("KCM_SRC_VALID = Last:%04x\r\n", (u32)g2Local_1);
    }
}

