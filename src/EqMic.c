
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           


#ifdef MICROPHONE
#define MAIN_EQ                         3                   // 如果打开MIC声道EQ音效则第4组为话筒的音效
#else
#define MAIN_EQ                         4                   // 没有打开MIC声道EQ音效则可用4组为主声道音效
#endif

CONST_CHAR TabEqSetup[5] = {                                // 多路均衡EQ音效处理设置，注意从字节1开始的
    0x03,                                                   // KCM_EQ_SETUP字节1 前置通道使用EQ
    (0<<6) | (0<<5) | (5&0x1f),                             // 主声道使用5段EQ 自动的衰减值配合音量芯片
    (3<<6) | (0<<5) | (2&0x1f),                             // 话筒声道HSF LSF共2段 自动的衰减值配合音量芯片
    (6<<4) | 8,                                             // 滤波器Q值 话筒=6 主声道=8 
    (9<<4) | 15,                                            // 预衰减值 话筒高低音最大为+9dB，主声道最大为+15dB
}; 
CONST_CHAR TabEqFreq[9] = {                                 // 多路段均衡EQ音效处理频率，注意从字节1开始的
    0x00|6,  0x20|3,  0,                                    // 50Hz(50-20/5=6),             350Hz(300-150/50=3)
    0x20|17, 0x40|18, 0,                                    // 1000Hz(1000-150/50=17),      3300Hz(3300-1500/100=18)
    0x60|21, 0,       0,                                    // 15000Hz(15000-4500/500=21)
};     

CONST_CHAR TabEqLevel[4][9] = {                             // 多路段均衡EQ音效处理电平，注意从字节1开始的
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

void MEQMIC_EqRestore(){									// EQ恢复记忆
	BYTE value;
    BYTE counter;
    BYTE select;
    BYTE temp[10];
    
    // 多路均衡EQ音效处理初始化
	for (select = 0; select < MAIN_EQ; select++){           // 多路均衡EQ音效处理
        // 多路均衡EQ音效处理设置，一般写入一次用于初始化
        memcpy(&temp[1], TabEqSetup, sizeof(TabEqSetup));
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_SETUP, temp, 1 + sizeof(TabEqSetup));   // 多路均衡EQ音效处理设置

        // 多路均衡EQ音效频率，一般写入一次用于初始化
        memcpy(&temp[1], TabEqFreq, sizeof(TabEqFreq));
        for (counter = 0; counter < sizeof(TabEqFreq); counter++){ // 写入EQ频率电平
            temp[counter + 1] |= TabEqLevel[select][counter];
        }
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_VALUE, temp, 1 + sizeof(TabEqFreq));   // 多路均衡EQ音效处理设置
    }
}

void MEQMIC_KeyEqSelect(){                                  // 按键EQ均衡器选择
    BYTE value = MKCM_ReadRegister(KCM_EQ_SELECT);          // 多路均衡音效处理选择
    if (gDIP_MenuSelect == MENU_EQ_SELECT){                 // 只有进入对应的菜单才改变模式
        if (++value > MAIN_EQ){                             // 1至3分别为3组多段EQ均衡器。或第4组用于话筒的高低音控制
            value = 0;                                      // 0为停止使用音效
        }
        MKCM_WriteRegister(KCM_EQ_SELECT, value);
    }
    MDIP_EqSelect(value);
}
CONST_CHAR Tab_DIP_EqMode[] = {
	" FLAT  EQ   "
//	 ++++++------
};
void MDIP_EqSelect(BYTE value){                               // 显示EQ均衡器选择
    MDIP_WriteString((char*)&Tab_DIP_EqMode[value ? 6 : 0]);
    if (value){
        MDIP_SingleChar(4, value + '0');
    }
    MDIP_SetState(MENU_EQ_SELECT);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIC_VOL_PT2259

#ifdef MIC_VOL_PT2259
#define MIC_CHIP 1
#else
#define MIC_CHIP 0
#endif

CONST_CHAR TabMicAdjMax[5] = {                              // 话筒各种参数控制最大值设置KCM_MIC_ADJ_MAX
    // B7为加倍延迟时间选择 B5:4话筒合成到主通道选择 B3话筒声道支持EQ ；B1:0为话筒1及2音量合成芯片类型
    (0<<7) | (0<<4) | (1<<3) | MIC_CHIP,                    // 字节0为合成及音量方式设置
    (9<<4) | 9,                                             // 话筒1及2音量比例KCM_MIC_VOLUME控制最大值
    (9<<4) | 0x0f,                                          // 话筒延迟时间及回声比例，最大不需要100%的
    (0x0f<<4) | 0x0f,                                       // 话筒重复及直达声最大值，最大不需要100%的
    (0<<4) | 0,                                             // 话筒混响1及混响2比例KCM_MIC_REVERB控制最大值
};  

void MEQMIC_MicRestore(){									// 话筒恢复记忆
	BYTE value;
    BYTE temp[5];

#ifdef MICROPHONE											// 定义就加入话筒控制
    // MKCM_RestoreMemory 从KCM_EXTR_MEMORY记忆之中恢复话筒低、高音	
	if (gDIP_MicTone[0] > 18){								// 话筒低音	-9到+9 超出范围的处理
        gDIP_MicTone[0] = 9;                                // 设置默认值
	}
	if (gDIP_MicTone[1] > 18){								// 话筒高音	-9到+9 超出范围的处理
        gDIP_MicTone[1] = 9;                                // 设置默认值
	}

	value = MKCM_ReadRegister(KCM_MIC_VOLUME);	            // 话筒1及话筒2音量比例
	FMicTurnOn = value > 0 ? 1 : 0;                         // 话筒打开标志    
    gDIP_MicCtrl[0] = (value >> 4) & 0x0f;                  // B7:4为话筒1音量
	gDIP_MicCtrl[1] = value & 0x0f;                         // B3:0为话筒2音量
    if (gDIP_MicCtrl[0] > 9){                               // 超出范围的处理
        gDIP_MicCtrl[0] = 2;                                // 设置默认值
    }
    if (gDIP_MicCtrl[1] > 9){                               // 超出范围的处理
        gDIP_MicCtrl[1] = 2;                                // 设置默认值
    }
    MEQMIC_MicAutoMixer();                                  // 根据话筒1及2的音量自动调节话筒声音与主声道合成比例

    value = MKCM_ReadRegister(KCM_MIC_ECHO);	            // 话筒直达声及回声比例
                                                            // B7:4为话筒直达声合成的比例
	gDIP_MicCtrl[2] = value & 0x0f;                         // B3:0为话筒回声音量
    if (gDIP_MicCtrl[2] > 9){                               // 超出范围的处理
        gDIP_MicCtrl[2] = 2;                                // 设置默认值
    }	

    value = MKCM_ReadRegister(KCM_MIC_DELAY);	            // 话筒延迟时间及重复比例
	gDIP_MicCtrl[4] = (value >> 4) & 0x0f;	                // B7:4为话筒延迟时间，与KCM_MIC_ADJ_MAX对应的最大值比例
    if (gDIP_MicCtrl[4] > 9){                               // 超出范围的处理
        gDIP_MicCtrl[4] = 2;                                // 设置默认值
    }
	gDIP_MicCtrl[3] = value & 0x0f;                         // B3:0为话筒重复比例
	 if (gDIP_MicCtrl[3] > 9){                              // 超出范围的处理
        gDIP_MicCtrl[3] = 2;                                // 设置默认值
    }

    MKCM_WriteXByte(KCM_MIC_ADJ_MAX, TabMicAdjMax, sizeof(TabMicAdjMax));   // 话筒各种参数最大值设置
 
    temp[0] = 4;                                            // 第4组固定为话筒的EQ预置音效
    memcpy(&temp[1], TabEqSetup, sizeof(TabEqSetup));
    MKCM_WriteXByte(KCM_EQ_SETUP, temp, 1 + sizeof(TabEqSetup));   // 多路均衡EQ音效处理设置    
    MEQMIC_MicSetTone();                                    // 写入话筒的音调到KCM
#endif      // MICROPHONE 定义就加入话筒控制
}
void MEQMIC_KeyCtrl(){										// EQ或MIC按键CTRL入口
#ifdef MICROPHONE											// 定义就将遥控器的音调、音调+、音调-改变为话筒控制
	if (gDIP_MenuSelect < MENU_MIC_VOL1){	    		// 还没有进入Mic菜单 
		MDIP_MenuNormal(MENU_MIC_VOL1);
	}else {
		if (++gDIP_MenuSelect > MENU_MIC_TREBLE){
			gDIP_MenuSelect = MENU_MIC_VOL1;
		}
		MDIP_MenuNormal(gDIP_MenuSelect);
	}
#endif			
}
void MEQMIC_KeyUp(){										// EQ或MIC按键调节+入口
#ifdef MICROPHONE											// 定义就将遥控器的音调、音调+、音调-改变为话筒控制
	if (gDIP_MenuSelect >= MENU_MIC_VOL1 && gDIP_MenuSelect <= MENU_MIC_TREBLE){	// 已经进入Mic菜单 
		gRmKeyContinCanclTm = 1;
		MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_UP);
	}
#endif			
}
void MEQMIC_KeyDown(){										// EQ或MIC按键调节-入口
#ifdef MICROPHONE											// 定义就将遥控器的音调、音调+、音调-改变为话筒控制
	if (gDIP_MenuSelect >= MENU_MIC_VOL1 && gDIP_MenuSelect <= MENU_MIC_TREBLE){	// 已经进入Mic菜单 
		gRmKeyContinCanclTm = 1;
		MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_DOWN);
	}
#endif			
}


// 话筒1音量 话筒2音量 话筒回声比例 话筒重复比例 话筒延迟时间 话筒低音 话筒高音
CONST_CHAR Tab_DIP_MicShow[] = {
	"MIC1MIC2ECHOREPTDELYMBASMTRE"
//	 ++++----++++----++++----++++
};
void MEQMIC_MicDisplay(BYTE index, MENU_SET mode){          // 显示话筒各种参数调节
    BYTE value;
    BYTE temp0;
	if (mode >= MENU_SET_ADJ_DOWN){							    // MENU_SET_ADJ_DOWN或MENU_SET_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // 上次已经进入当前菜单
			MEQMIC_MicKcm(index, (mode == MENU_SET_ADJ_UP) ? 1 : 0); 	// 话筒各种参数调节写入到KCM
			gDIP_MenuTimer = 80;
		}
	}
    temp0 = index - MENU_MIC_VOL1;                          // 从话筒1音量开始
	MDIP_WriteString((char*)&Tab_DIP_MicShow[temp0 * 4]);
    MDIP_SingleChar(4, ' ');

	if (mode == MENU_SET_TWINKLE_OFF){							// MENU_SET_TWINKLE_OFF闪烁熄灭
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_SET_NORMAL或MENU_SET_TWINKLE_ON闪烁点亮
    	if (index < MENU_MIC_BASS){
            value = gDIP_MicCtrl[temp0];
            MDIP_SingleChar(5, value + '0');
        }else {
            MDIP_ShowNegative9(gDIP_MicTone[index - MENU_MIC_BASS]);    
        }
		if (mode == MENU_SET_NORMAL){                       // 正常的显示模式
			gDIP_MenuTimer = 80;
			FDIP_MenuTwinkle = 1;
		}
	}
}

#define MIC_BASS_300HZ                  (0x20 | 3)          // B6:5=1  B4:0=3*50+150=300Hz
#define MIC_TREBL_5000HZ                (0x60 | 1)          // B6:5=3  B4:0=1*500+4500=5000Hz
WORD makeEqValue(BYTE level, WORD freq){
    if (level < 9){                                         // 负数
        freq |= 0x80;                                       // B7为1时B11:8的值为衰减-0至-15dB。 
        freq |= (9 - level) << 8;                           // B11:8为dB的数值不包括符号
    }else{                                                  // 0或正数
        freq |= (level - 9) << 8;                           // B11:8为dB的数值不包括符号
    }
    return freq;
}
void MEQMIC_MicKcm(BYTE index, BYTE directUp){              // 话筒各种参数调节写入到KCM
	BYTE value;

	MAUD_AutoCanclMute();
    if (index < MENU_MIC_BASS){                             // 0话筒1音量 1话筒2  2回声 3重复 4延迟
        MDIP_SetUpDown(directUp, 9, &gDIP_MicCtrl[index - MENU_MIC_VOL1]);
        switch (index){                                     // 0话筒1音量 1话筒2  2回声 3重复 4延迟 5混响
        case MENU_MIC_VOL1:                                 // 话筒1音量
        case MENU_MIC_VOL2:                                 // 话筒2音量
            MKCM_WriteRegister(KCM_MIC_VOLUME, (gDIP_MicCtrl[0]<<4) | gDIP_MicCtrl[1]);   // 话筒1及话筒2音量比例
            MEQMIC_MicAutoMixer();
            break;
        case MENU_MIC_ECHO:                                 // 话筒回声比例
            value = 8;                                      // 直达声比例固定为8
            MKCM_WriteRegister(KCM_MIC_ECHO, (value<<4) | gDIP_MicCtrl[2]);   // 话筒回声及话筒多段EQ均衡音效处理选择比例
            break;
        case MENU_MIC_DELAY:                                // 话筒延迟时间
        case MENU_MIC_REPEAT:                               // 话筒重复比例
            MKCM_WriteRegister(KCM_MIC_DELAY, (gDIP_MicCtrl[4]<<4) | gDIP_MicCtrl[3]);
        break;
            default:
        break;
        }
    }else{                                                  // 话筒音调低、高音调节
        MDIP_SetUpDown(directUp, 18, &gDIP_MicTone[index - MENU_MIC_BASS]);
        MEQMIC_MicSetTone();
        if (index == MENU_MIC_BASS){                        // 将改变的低音值写入记忆
            MKCM_WriteRegister(KCM_EXTR_MEMORY+MEM_MIC_BASS, gDIP_MicTone[0]);
        }else{                                              // 将改变的高音值写入记忆
            MKCM_WriteRegister(KCM_EXTR_MEMORY+MEM_MIC_TREBLE, gDIP_MicTone[1]);      
        }
    }
}
void MEQMIC_MicSetTone(){                                   // 写入话筒的音调到KCM
    BYTE temp[4];
    WORD bass;
    WORD treble;
    bass = makeEqValue(gDIP_MicTone[0], MIC_BASS_300HZ);
    treble = makeEqValue(gDIP_MicTone[1], MIC_TREBL_5000HZ);
    
    temp[0] = 4;                                        // 第4组为话筒的音效
    // 字节1开始每3个字节为两段12位的EQ设置值
    temp[1] = bass & 0xff;                              // 第一字节为第1段低8位
    temp[2] = treble & 0xff;                            // 第二字节为第2段低8位
    temp[3] = ((bass >> 8) & 0x0f) | ((treble >> 4) & 0xf0);  // 第三字节的B3:0为第1段高4位，B7:4为第2段高4位
//MLOG("EQ_VALUE %02x %02x %02x", temp[1], temp[2], temp[3]);    
    MKCM_WriteXByte(KCM_EQ_VALUE, temp, 1 + 3);         // 多路均衡EQ音效处理设置
}

void MEQMIC_MicAutoMixer(){                                 // 根据话筒1及2的音量自动调节话筒声音与主声道合成比例
    if (gDIP_MicCtrl[0] < 7 && gDIP_MicCtrl[1] < 7){        // 话筒1及2的音量都小于7
        MKCM_WriteRegister(KCM_MIC_MIXER, (8<<4) | 8);      // 话筒声音与主声道合成比例都为50% (8除16)
    }else {
        BYTE maxVol = (gDIP_MicCtrl[0] > gDIP_MicCtrl[1]) ? gDIP_MicCtrl[0] : gDIP_MicCtrl[1];
        if (maxVol == 7){                                   // 最大的话筒为7
            MKCM_WriteRegister(KCM_MIC_MIXER, (10<<4) | 6);  // 话筒的声音比主声道大
        }else if (maxVol == 8){                             // 最大的话筒为8
            MKCM_WriteRegister(KCM_MIC_MIXER, (12<<4) | 4);  // 话筒的声音比主声道大
        }else {                                             // 最大的话筒为9
            MKCM_WriteRegister(KCM_MIC_MIXER, (14<<4) | 2);  // 话筒的声音比主声道大
        }
    }
}
