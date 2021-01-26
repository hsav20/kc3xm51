
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
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


#define EQ_FREQ(F) (                        \
    (F<150)  ? ((0<<5) | ((F-20)/5)) :      \
    (F<1500) ? ((1<<5) | ((F-150)/50)) :    \
    (F<4500) ? ((2<<5) | ((F-1500)/100)) :  \
               ((3<<5) | ((F-4500)/500)))
           
#define EQ_LEVEL(L) (                       \
    (L<0) ? (0x80 | (((L&0x1f)^0x1f)+1)) : (L&0x1f))

CONST_CHAR TabEqSetup[5] = {                                // 多路均衡EQ音效处理设置，注意从字节1开始的
    0x03,                                                   // KCM_EQ_SETUP字节1 前置通道使用EQ
    (0<<6) | (0<<5) | (5&0x1f),                             // 主声道使用5段EQ 自动的衰减值配合音量芯片
    (3<<6) | (0<<5) | (2&0x1f),                             // 话筒声道HSF LSF共2段 自动的衰减值配合音量芯片
    (6<<4) | 8,                                             // 滤波器Q值 话筒=6 主声道=8 
    (9<<4) | 15,                                            // 预衰减值 话筒高低音最大为+9dB，主声道最大为+15dB
}; 
CONST_CHAR TabEqFreq[6] = {                                 // 多路段均衡EQ音效处理频率，注意从字节1开始的
    EQ_FREQ(50), EQ_FREQ(350), EQ_FREQ(1000), EQ_FREQ(3300), EQ_FREQ(15000), 0, // 后面增加一个字节用于双数对齐
};     
CONST_CHAR TabEqLevel[4][6] = {                             // 多路段均衡EQ音效处理电平，注意从字节1开始的
    {EQ_LEVEL(8), EQ_LEVEL(5), EQ_LEVEL(-2), EQ_LEVEL(3), EQ_LEVEL(6), 0,}, // 后面增加一个字节用于双数对齐
    {EQ_LEVEL(15), EQ_LEVEL(-2), EQ_LEVEL(-12), EQ_LEVEL(5), EQ_LEVEL(15), 0,},
    {EQ_LEVEL(-12), EQ_LEVEL(2), EQ_LEVEL(10), EQ_LEVEL(-5), EQ_LEVEL(-8), 0,},
    {EQ_LEVEL(5), EQ_LEVEL(-3), EQ_LEVEL(-4), EQ_LEVEL(11), EQ_LEVEL(13), 0,},
};

void MEQMIC_EqRestore(){									// EQ恢复记忆
    BYTE index;
    BYTE counter;
    BYTE select;
	BYTE value1;
	BYTE value2;
    BYTE temp[10];
    
    // 多路均衡EQ音效处理初始化
	for (select = 0; select < MAIN_EQ; select++){           // 多路均衡EQ音效处理
        // 多路均衡EQ音效处理设置，一般写入一次用于初始化
        memcpy(&temp[1], TabEqSetup, sizeof(TabEqSetup));
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_SETUP, temp, 1 + sizeof(TabEqSetup));   // 多路均衡EQ音效处理设置

        counter = 1;                                        // 每3个字节为两段12位的EQ设置值
        for (index = 0; index < sizeof(TabEqFreq); index+=2){ // 写入EQ频率及电平
            value1 = TabEqLevel[select][index];
            value2 = TabEqLevel[select][index+1];
            temp[counter + 0] = (value1&0x80) | (TabEqFreq[index]&0x7f);
            temp[counter + 1] = (value2&0x80) | (TabEqFreq[index+1]&0x7f);
            temp[counter + 2] = (value1&0x0f) | ((value2&0x0f)<<4);
            counter += 3;
        }
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_VALUE, temp, counter);   // 多路均衡EQ音效处理设置
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
void MDIP_EqSelect(BYTE value){                             // 显示EQ均衡器选择
    MDIP_CleanSymbol();                                     // 统一清除屏幕及符号
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
    // 字节0 B7为加倍延迟时间选择 B5:4话筒合成到主通道选择 B3话筒声道支持EQ ；B1:0为话筒1及2音量合成芯片类型
    (0<<7) | (0<<4) | (1<<3) | MIC_CHIP,                    // 字节0为合成及音量方式设置
    (9<<4) | 9,                                             // KCM_MIC_VOLUME 话筒1及2音量比例KCM_MIC_VOLUME控制最大值
    (0x09<<4) | 0x09,                                       // KCM_MIC_ECHO 话筒直达声及回声最大值
    (9<<4) | 0x0f,                                          // KCM_MIC_DELAY 话筒延迟时间及重复比例，重复最大不需要100%
    (0<<4) | 0,                                             // KCM_MIC_REVERB 话筒混响1及混响2比例KCM_MIC_REVERB控制最大值
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
	if (mode >= MENU_SET_ADJ_DOWN){							// MENU_SET_ADJ_DOWN或MENU_SET_ADJ_UP 
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

WORD makeEqValue(BYTE level, WORD freq){
    if (level < 9){                                         // 负数
        freq |= 0x80;                                       // B7为1时B11:8的值为衰减-0至-15dB。 
        freq |= (9 - level) << 8;                           // B11:8为dB的数值不包括符号
    }else{                                                  // 0或正数
        freq |= (level - 9) << 8;                           // B11:8为dB的数值不包括符号
    }
    return freq;
}
CONST_CHAR TabDirectSound[10] = {                           // 话筒的直达声混合比例
    9, 7, 7, 7, 5, 5, 5, 5, 3, 3,                           // 一般回声调大就意味着直达声需要小点
}; 
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
            value = TabDirectSound[gDIP_MicCtrl[2]];        // 如果关闭回声，将直达声开到最大
            MKCM_WriteRegister(KCM_MIC_ECHO, (value<<4) | gDIP_MicCtrl[2]);   // 话筒回声及话筒多段EQ均衡音效处理选择比例
            break;
        case MENU_MIC_DELAY:                                // 话筒延迟时间
        case MENU_MIC_REPEAT:                               // 话筒重复比例
            MKCM_WriteRegister(KCM_MIC_DELAY, (gDIP_MicCtrl[4]<<4) | gDIP_MicCtrl[3]);
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
    bass = makeEqValue(gDIP_MicTone[0], EQ_FREQ(300));
    treble = makeEqValue(gDIP_MicTone[1], EQ_FREQ(8000));
    
    temp[0] = 4;                                            // 第4组为话筒的音效
    // 字节1开始每3个字节为两段12位的EQ设置值
    temp[1] = bass & 0xff;                                  // 第一字节为第1段低8位
    temp[2] = treble & 0xff;                                // 第二字节为第2段低8位
    temp[3] = ((bass >> 8) & 0x0f) | ((treble >> 4) & 0xf0);  // 第三字节的B3:0为第1段高4位，B7:4为第2段高4位
//MLOG("EQ_VALUE %02x %02x %02x", temp[1], temp[2], temp[3]);    
    MKCM_WriteXByte(KCM_EQ_VALUE, temp, 1 + 3);             // 多路均衡EQ音效处理设置
}
CONST_CHAR TabMicMain[10] = {                               // 话筒与音乐混合比例
    6,  4,                                                  // 话筒75%  音乐50%
    8,  4,                                                  // 话筒100% 音乐50%
    9,  4,                                                  // 话筒113% 音乐50%
    10, 3,                                                  // 话筒125% 音乐38%
    11, 2,                                                  // 话筒138% 音乐25%    
}; 
void MEQMIC_MicAutoMixer(){                                 // 根据话筒1及2的音量自动调节话筒声音与主声道合成比例
    if (gDIP_MicCtrl[0] < 5 && gDIP_MicCtrl[1] < 5){        // 话筒1及2的音量都小于5
        MKCM_WriteRegister(KCM_MIC_MIXER, (4<<4) | 4);      // 话筒声音与主声道合成比例都为50% (4除8)
    }else {                                                 // 只要有一个大于或等于5，表示原来的话筒与音乐的比例已经不够
        BYTE maxVol = (gDIP_MicCtrl[0] > gDIP_MicCtrl[1]) ? gDIP_MicCtrl[0] : gDIP_MicCtrl[1];
        maxVol = (maxVol - 5) * 2;                         // 转换为索引值
        MKCM_WriteRegister(KCM_MIC_MIXER, (TabMicMain[maxVol]<<4) | TabMicMain[maxVol+1]);  // 话筒的声音比主声道大
    }
}
