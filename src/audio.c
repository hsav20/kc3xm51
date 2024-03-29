
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           

void MAUD_Initialize(){										// 按键模块初始化  	
}	  
void MAUD_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
    if (gRemoveTimer && ++gRemoveTimer > 200){			    // 大约2秒后 
		INPUT_SWITCH select;
		BYTE value = MKCM_ReadRegister(KCM_EXTR_MEMORY + MEM_SOURCE_AUTO);	// 自动输入的恢复
    	select = MKCM_FromRegister(KCM_INPUT_SOURCE, value);				// 从KCM来的寄存器，转换到本机处理的值
		MLOG("RemoveA %02x %d ", value, select,mINPUT_SWITCH);
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
//MLOG("MAUD_MixTrimAdjust A %d %d %d", index, KCM_FL_TRIM + address, value);
    MKCM_WriteRegister(KCM_FL_TRIM + address, value);
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
//MLOG("MenuSelectA %02x", select);
		mINPUT_SWITCH = select;
		MAUD_InputWrite(mINPUT_SWITCH, mINPUT_SWITCH);
		if (mINPUT_SWITCH == INPUT_SWITCH_SD){
			MKCM_AutoTrack(KCM_PLAY_SD_QTY);				// 输入KCM_PLAY_SD_QTY，如果可以播放返回1
		}else if (mINPUT_SWITCH == INPUT_SWITCH_UDISK){
			MKCM_AutoTrack(KCM_PLAY_UDISK_QTY);				// 输入KCM_PLAY_UDISK_QTY，如果可以播放返回1
		}
	}
	MDIP_MenuNormal(MENU_INPUT_SOURCE);                    	// 菜单状态:输入音源选择
}
void MAUD_Preemptible(){									// 抢占式输入选择 
	MLOG("MKCM_Preemptible:%d %d/%d", gPreemptibleIn[gPreemptibleStep], gPreemptibleStep, gPreemptibleQty);
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
	INPUT_SWITCH_AUX,                                   	// 0=模拟输入
    INPUT_SWITCH_OPTIC,                                 	// 1=数码1   
    INPUT_SWITCH_COA1,                                  	// 2=数码2   
    INPUT_SWITCH_COA2,                                  	// 3=数码3   
};

    // INPUT_SWITCH_HDMI1 = 9,                                 // HDMI1   
    // INPUT_SWITCH_HDMI2 = 10,                                // HDMI2   
    // INPUT_SWITCH_HDMI3 = 11,                                // HDMI3   
    // INPUT_SWITCH_H_ARC = 12,                                // HDMI-ARC


void MAUD_InputOneKey(){									// 所有输入用一个按键选择 
	gDIP_MenuSelect = MENU_RESTORE;							// 菜单即刻进入输入的恢复 
	if (gWithHdmiQty && gWithHdmiStep < gWithHdmiQty){	 	// 循环方式
		MAUD_InputSelect(gPreemptibleIn[gWithHdmiStep]);
	}else {
		MAUD_InputSelect(TabInputOneKey[gWithHdmiStep - gWithHdmiQty]);
	}
	if (++gWithHdmiStep >= (sizeof(TabInputOneKey) + gWithHdmiQty)){
		gWithHdmiStep = 0;
	}
}
void MAUD_InputWrite(INPUT_SWITCH select, INPUT_SWITCH last){
	BYTE value1 = MKCM_ToRegister(KCM_INPUT_SOURCE, select);
	BYTE value2 = MKCM_ToRegister(KCM_INPUT_SOURCE, last);
	MKCM_WriteRegister(KCM_INPUT_SOURCE, value1);
    MLOG("SourceW:%d(%d)%02x(%02x)", select, last, value1, value2);
	if (last <= INPUT_SWITCH_COA2){ 								// 如果原来不是抢占式输入
	    MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_SOURCE_AUTO, value2);	// 自动输入的恢复
		MLOG("SourceJ:%d %02x", select, value2);
	}
}
CONST_WORD TabSrcValid[] = {
	KCM_SRC_VALID_SD, KCM_SRC_VALID_UDISK,
	KCM_SRC_VALID_USBA, KCM_SRC_VALID_BT, KCM_SRC_VALID_E8CH
	// KCM_SRC_VALID_HDMI1, KCM_SRC_VALID_HDMI2, KCM_SRC_VALID_HDMI3
	
}; 
CONST_CHAR TabValidSwitch[] = {
	INPUT_SWITCH_SD, INPUT_SWITCH_UDISK,
	INPUT_SWITCH_USBA, INPUT_SWITCH_BT, INPUT_SWITCH_E8CH
	// INPUT_SWITCH_HDMI1, INPUT_SWITCH_HDMI2, INPUT_SWITCH_HDMI3
};
void MAUD_MakePreemptible(WORD validFlag){			        // 生成抢占式输入选择 
	BYTE qty = 0;
    // if (gSYS_ModelType == KCM_MODEL_35H){                   // 模块版本是KC35H，需要加入外置7.1声道
    //     gPreemptibleIn[qty++] = INPUT_SWITCH_E8CH;
    // }    
    if (validFlag){									// 有抢占式输入 
    	BYTE index;
    	WORD flag;
    	for (index = 0; index < sizeof(TabSrcValid)/2; index++){
        	flag = TabSrcValid[index];
        	if (validFlag & flag){    				// 有非抢占式
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
  	  	MLOG("KCM_SRC_VALID E %d %d", gPreemptibleQty, gWithHdmiQty);  
    }else {												// 没有了抢占式输入
		gPreemptibleQty = qty;
		gWithHdmiQty = 0; 
		gPreemptibleStep = 0;
		gWithHdmiStep = 0;
		g2SdQty = 0;
		g2UDiskQty = 0;
    }
    g2AUD_SrcValid = validFlag;
}


BYTE MKCM_AutoTrack(BYTE value){							// 输入KCM_PLAY_SD_QTY或KCM_PLAY_UDISK_QTY，如果可以播放返回1
	WORD qty = MKCM_Read2Byte(value);
	if (value == KCM_PLAY_SD_QTY){
		g2SdQty = qty;
	}else{
		g2UDiskQty = qty;
	}
	if (qty){												// 有播放文件 
		MKCM_Write2Byte(KCM_PLAY_INDEX, 0xffff);  			// 播放上次记忆的曲目
		return 1;
	}
	return 0;
}


void MKCM_ReadSrcInfo(){									// 收到中断读取格式、通道、采样率、码流率等信息
//  MKCM_ReadXByte(KCM_SRC_FORMAT, &gAUD_SrcFormat, 3);   // 连续读3字节到gAUD_SrcFormat gAUD_ChSr gAUD_SrcFreq
	gAUD_SrcFormat = MKCM_ReadRegister(KCM_SRC_FORMAT);	// 数码信号输入格式指示
	gAUD_SrcChannel = MKCM_ReadRegister(KCM_SRC_CHANNEL);	// 数码信号输入通道信息及超低音指示
	gAUD_SrcRate = MKCM_ReadRegister(KCM_SRC_RATE);	// 数码信号输入采样率及实际播放采样率指示
	gAUD_SrcBps = MKCM_ReadRegister(KCM_SRC_BPS);	// 数码信号输入码流率指示
					
	// MLOG("gAUD_SrcFormat %02x %02x %02x", gAUD_SrcFormat, gAUD_ChSr, gAUD_SrcFreq);

//	MLOG("SrcFormat %02x %02x", gAUD_SrcFormat, gAUD_SrcFreq);
	if (mINPUT_SWITCH == INPUT_SWITCH_SD || mINPUT_SWITCH == INPUT_SWITCH_UDISK){
		char fileName[16];
		BYTE length = MKCM_ReadAutoByte(KCM_PLAY_FILE_NAME, fileName, 16);
		// MLOG("FILE_NAME A %d %02x %02x %02x", length, fileName[0], fileName[1], fileName[2]);
		g2TimeLength = MKCM_Read2Byte(KCM_PLAY_FILE_TIME);
	}else {
		if (gDIP_MenuLock == 0){						// 
			if (!FSYS_TestTone){						// 没有打开噪音测试
				MDIP_MenuNormal(MENU_SRC_FORMAT);		// 菜单显示输入码流格式
			}
		}
	}
	if (gAUD_SrcFormat == 0){
		MDIP_CleanSpectrum();
	}
	MDIP_SrcFormatSymbol();
	MDIP_SurroundSymbol();
}

void MKCM_ReadSrcValid(){									// 收到中断读取有效的音源输入改变
	WORD srcValid = MKCM_Read2Byte(KCM_SRC_VALID);         // 本次的有效音源
//if(g2Local_1==KCM_SRC_VALID_UDISK){g2Local_1=KCM_SRC_VALID_HDMI1|KCM_SRC_VALID_HDMI2;}
    if (g2AUD_SrcValid != srcValid){                       // 本次与上次的有效音源改变
		BYTE index;
    	WORD flag;
    	BOOL found = 0;
        MLOG("KCM_SRC_VALID:%02x%02x Last:%02x%02x", srcValid>>8, srcValid, g2AUD_SrcValid>>8, g2AUD_SrcValid);
        for (index = 0; index < sizeof(TabSrcValid)/2; index++){
        	flag = TabSrcValid[index];
        	if ((srcValid & flag) && !(g2AUD_SrcValid & flag)){    // 本次有及上次没有非抢占式
		       	found = 1;
	        	switch (flag){
	            case KCM_SRC_VALID_SD:
					if (MKCM_AutoTrack(KCM_PLAY_SD_QTY) == 0){
						MDIP_MenuNormal(MENU_SD_INSERT); 
		            	found = 0;							// 没有播放文件
					}
					break;	
	            case KCM_SRC_VALID_UDISK: 
					if (MKCM_AutoTrack(KCM_PLAY_UDISK_QTY) == 0){
						MDIP_MenuNormal(MENU_UD_INSERT); 
		            	found = 0;							// 没有播放文件
					}
					break;	
	            case INPUT_SWITCH_E8CH: 
	            	found = 0;
					break;	
				// case KCM_SRC_VALID_HDMI1:                    // HDMI1   
				// case KCM_SRC_VALID_HDMI2:                    // HDMI2   
				// case KCM_SRC_VALID_HDMI3:                    // HDMI3   
				// case KCM_SRC_VALID_HDMIS:                    // HDMI-ARC
	            // 	found = 0;
				// 	break;	
      			}
      			if (found){
					MAUD_InputWrite(TabValidSwitch[index], mINPUT_SWITCH);
					mINPUT_SWITCH = TabValidSwitch[index];
MLOG("mINPUT_A %02x", mINPUT_SWITCH);
					gDIP_MenuSelect = MENU_RESTORE;			// 菜单即刻进入输入的恢复 
					MDIP_MenuNormal(MENU_INPUT_SOURCE);     // 菜单状态:输入音源选择
					gRemoveTimer = 0;						// 取消拔出恢复到非抢占式插入
			    }
	        	break;										// 只处理一个抢占式插入
        	}
        }
        if (!found){										// 没有非抢占式插入
			for (index = 0; index < sizeof(TabSrcValid)/2; index++){
	        	flag = TabSrcValid[index];
	    //MLOG("KCM_SRC_VALID C %d %02x%02x", index, flag>>8, flag);  
				if (!(srcValid & flag) && (g2AUD_SrcValid & flag)){    // 本次没有及上次有非抢占式
		            switch (flag){
		            case KCM_SRC_VALID_SD:
						g2SdQty = 0; 
						MDIP_PlaySymbol(0);
						MDIP_MenuNormal(MENU_SD_REMOVE); 
						break;	
		            case KCM_SRC_VALID_UDISK: 
						g2UDiskQty = 0;
						MDIP_PlaySymbol(0);
						MDIP_MenuNormal(MENU_UD_REMOVE); 
						break;	
		            case KCM_SRC_VALID_USBA: 
						MDIP_MenuNormal(MENU_PC_REMOVE); 
						break;	
		            case KCM_SRC_VALID_BT: 
						MDIP_MenuNormal(MENU_BT_REMOVE); 
						break;	
		            // case KCM_SRC_VALID_HDMI1: 
		            // case KCM_SRC_VALID_HDMI2: 
		            // case KCM_SRC_VALID_HDMI3: 
			        //     break;
		            }
					if (mINPUT_SWITCH == TabValidSwitch[index]){	// 有抢占式拔出而且是当前的输入
						gRemoveTimer = 1;					// 大约2秒后 
			        }					
		            break;									// 只处理一个抢占式拔出
	        	}
	        }
        }
		MAUD_MakePreemptible(srcValid);					// 生成抢占式输入选择 
    }else {
        MLOG("KCM_SRC_VALID = Last:%02x%02x", srcValid>>8, srcValid);
    }
}

