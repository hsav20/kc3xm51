
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           


CONST_CHAR TabEqSetup[5] = {                                // 多路均衡EQ音效处理设置
    0x03, 0xc0 | 5, 0, 8,                                   // 前置通道使用EQ，5段及HSF&LSF，MIC没有音效，主声道Q值8
};     
CONST_CHAR TabEqFreq[9] = {                                 // 多路段均衡EQ音效处理频率
    0x00|6,  0x20|3,  0,                                    // 50Hz(50-20/5=6),             350Hz(300-150/50=3)
    0x20|17, 0x40|18, 0,                                    // 1000Hz(1000-150/50=17),      3300Hz(3300-1500/100=18)
    0x60|21, 0,       0,                                    // 15000Hz(15000-4500/500=21)
};     

CONST_CHAR TabEqLevel[4][9] = {                             // 多路段均衡EQ音效处理电平
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

CONST_CHAR TabMicAdjMax[5] = {                              // 话筒各种参数控制最大值设置
    9,                                                      // 话筒延迟时间KCM_MIC_DELAY控制最大值
    9 << 4 | 9,                                             // 话筒1及2音量比例KCM_MIC_VOLUME控制最大值
    9,                                                      // 回声比例KCM_MIC_ECHO_EQ控制最大值
    9 << 4 | 9,                                             // 话筒重复及直达声比例KCM_MIC_REPEAT控制最大值
    9 << 4 | 9,                                             // 话筒混响1及混响2比例KCM_MIC_REVERB控制最大值
};  

void MEQMIC_Restore(){										// EQ及话筒恢复记忆
	BYTE value;
    BYTE counter;
    BYTE select;
    BYTE temp[10];

    MKCM_WriteXByte(KCM_MIC_ADJ_MAX, TabMicAdjMax, sizeof(TabMicAdjMax));   // 话筒各种参数最大值设置

	value = MKCM_ReadRegister(KCM_MIC_VOLUME);	            // 话筒1及话筒2音量比例
	gDIP_MicCtrl[0] = value & 0x0f;                         // B7:4为话筒1音量
	gDIP_MicCtrl[1] = (value >> 4) & 0x0f;                  // B3:0为话筒2音量
    if (gDIP_MicCtrl[0] > 9){                               // 超出范围的处理
        gDIP_MicCtrl[0] = 2;                                // 设置默认值
    }
    if (gDIP_MicCtrl[1] > 9){                               // 超出范围的处理
        gDIP_MicCtrl[1] = 2;                                // 设置默认值
    }

	value = MKCM_ReadRegister(KCM_MIC_ECHO_EQ);	            // 话筒回声及话筒多段EQ均衡音效处理选择比例
                                                            // B7:4为话筒多段EQ均衡音效处理选择，1打开EQ音效，0关闭音效
	gDIP_MicCtrl[2] = value & 0x0f;                         // B3:0为回声比例
    if (gDIP_MicCtrl[2] > 9){                               // 超出范围的处理
        gDIP_MicCtrl[2] = 2;                                // 设置默认值
    }	
	value = MKCM_ReadRegister(KCM_MIC_REPEAT);	            // 话筒重复及话筒直达声比例
                                                            // B7:4为话筒直达声合成的比例
	gDIP_MicCtrl[3] = value & 0x0f;                         // B3:0为话筒重复比例
	 if (gDIP_MicCtrl[3] > 9){                               // 超出范围的处理
        gDIP_MicCtrl[3] = 2;                                // 设置默认值
    }	
	value = MKCM_ReadRegister(KCM_MIC_DELAY);	            // 话筒延迟时间 记忆调整值
                                                            // B7:4为保留为0。
	gDIP_MicCtrl[4] = value;	                            // B3:0为话筒延迟时间。最大延迟时间为1000毫秒
	 if (gDIP_MicCtrl[4] > 9){                               // 超出范围的处理
        gDIP_MicCtrl[4] = 2;                                // 设置默认值
    }	

	value = MKCM_ReadRegister(KCM_MIC_REVERB);	            // 话筒混响比例 记忆调整值
	gDIP_MicCtrl[5] = MKCM_FromRegister(KCM_MIC_DELAY, value & 0x0f);   // 0-15转换到0-9
    if (gDIP_MicCtrl[5] > 9){                               // 超出范围的处理
        gDIP_MicCtrl[5] = 2;                                // 设置默认值
    }	

	for (select = 0; select < 4; select++){
        // 多路均衡EQ音效处理设置，一般写入一次用于初始化
        for (counter = 0; counter < sizeof(TabEqSetup); counter++){ 
            temp[counter + 1] = TabEqSetup[counter];
        }
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_SETUP, temp, 1 + sizeof(TabEqSetup));   // 多路均衡EQ音效处理设置

        // 多路均衡EQ音效频率，一般写入一次用于初始化
        for (counter = 0; counter < sizeof(TabEqFreq); counter++){ // 写入EQ频率
            temp[counter + 1] = TabEqFreq[counter];
        }
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
        if (++value > 4){                                   // 1至4分别为4组预置音效高低音音调或多段EQ均衡器。
            value = 0;                                      // 0为停止使用音效
        }
        MKCM_WriteRegister(KCM_EQ_SELECT, value);
    }
    MDIP_EqSelect(value);
}


void MEQMIC_KeyCtrl(){										// EQ或MIC按键CTRL入口
#ifdef MICROPHONE											// 定义就将遥控器的音调、音调+、音调-改变为话筒控制
	if (gDIP_MenuSelect < cMenu_MicVolume1){	    		// 还没有进入Mic菜单 
		MDIP_MenuNormal(cMenu_MicVolume1);
	}else {
		if (++gDIP_MenuSelect > cMenu_MicReverb){
			gDIP_MenuSelect = cMenu_MicVolume1;
		}
		MDIP_MenuNormal(gDIP_MenuSelect);
	}
#endif			
}
void MEQMIC_KeyUp(){										// EQ或MIC按键调节+入口
#ifdef MICROPHONE											// 定义就将遥控器的音调、音调+、音调-改变为话筒控制
	if (gDIP_MenuSelect >= cMenu_MicVolume1 && gDIP_MenuSelect <= cMenu_MicReverb){	// 已经进入Mic菜单 
		gRmKeyContinCanclTm = 1;
		MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_UP);
	}
#endif			
}
void MEQMIC_KeyDown(){										// EQ或MIC按键调节-入口
#ifdef MICROPHONE											// 定义就将遥控器的音调、音调+、音调-改变为话筒控制
	if (gDIP_MenuSelect >= cMenu_MicVolume1 && gDIP_MenuSelect <= cMenu_MicReverb){	// 已经进入Mic菜单 
		gRmKeyContinCanclTm = 1;
		MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_DOWN);
	}
#endif			
}


// 话筒1音量 话筒2音量 话筒回声比例 话筒重复比例 话筒延迟时间 话筒混响比例
CONST_CHAR Tab_DIP_MicShow[] = {
	"MIC1MIC2ECHOREPTDELYREVB"
//	 ++++----++++----++++----
};
void MEQMIC_MicDisplay(BYTE index, MENU_SET mode){          // 显示话筒各种参数调节
//if(mode!=MENU_SET_TWINKLE_ON&&mode!=MENU_SET_TWINKLE_OFF){MDEBUG(0xee);MDEBUG(index);MDEBUG(mode);}
	if (mode >= MENU_SET_ADJ_DOWN){							    // MENU_SET_ADJ_DOWN或MENU_SET_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // 上次已经进入当前菜单
			MEQMIC_MicKcm(index, (mode == MENU_SET_ADJ_UP) ? 1 : 0); 	// 话筒各种参数调节写入到KCM
			gDIP_MenuTimer = 80;
		}
	}
    index -= cMenu_MicVolume1;                              // 从话筒1音量开始
	MDIP_WriteString((char*)&Tab_DIP_MicShow[index * 4]);
    MDIP_SingleChar(4, ' ');

	if (mode == MENU_SET_TWINKLE_OFF){							// MENU_SET_TWINKLE_OFF闪烁熄灭
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_SET_NORMAL或MENU_SET_TWINKLE_ON闪烁点亮
    	BYTE gLocal_1 = gDIP_MicCtrl[index];
        MDIP_SingleChar(5, gLocal_1 + '0');
		if (mode == MENU_SET_NORMAL){                       // 正常的显示模式
			gDIP_MenuTimer = 80;
			FDIP_MenuTwinkle = 1;
		}
       
	}
}
void MEQMIC_MicKcm(BYTE index, BYTE directUp){              // 话筒各种参数调节写入到KCM
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
    switch (index){                                         // 0话筒1音量 1话筒2  2回声 3重复 4延迟 5混响
    case cMenu_MicVolume1:                                  // 话筒1音量
    case cMenu_MicVolume2:                                  // 话筒2音量
        MKCM_WriteRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[0] << 4 | gDIP_MicCtrl[1]);   // 话筒1及话筒2音量比例
        break;
    case cMenu_MicEcho:                                     // 话筒回声比例
        temp0 = 0;                                          // 话筒多段EQ均衡音效处理选择比例
        MKCM_WriteRegister(KCM_MIC_ECHO_EQ, temp0 << 4 | gDIP_MicCtrl[2]);   // 话筒回声及话筒多段EQ均衡音效处理选择比例
        break;
    case cMenu_MicRepeat:                                   // 话筒重复比例
        temp0 = 8;                                          // 直达声比例固定为8
        MKCM_WriteRegister(KCM_MIC_REPEAT, temp0 << 4 | gDIP_MicCtrl[3]);
        break;
    case cMenu_MicReverb:                                   // 话筒混响比例
        temp0 = 1;                                          // 话筒混响2固定为1
        MKCM_WriteRegister(KCM_MIC_REVERB, temp0 << 4 | gDIP_MicCtrl[5]);
        break;
    case cMenu_MicDelay:                                    // 话筒延迟时间
        MKCM_WriteRegister(KCM_MIC_DELAY, gDIP_MicCtrl[4]);
        break;
    }
}



/*
CONST_CHAR Tab_MicDelay[] = {                               // 0-255转换到0-9
    60/20,80/20,100/20,120/20,      160/20,200/20,300/20,500/20,    1000/20,2000/20,    // 单位毫秒    
};
CONST_CHAR Tab_Mic16To10[] = {                              // 0-15转换到0-9
    0,2,4,6,     8,10,12,13,    14,15,         
};
	case KCM_MIC_VOLUME :									// 话筒比例
	    return Tab_Mic16To10[counter];
	case KCM_MIC_DELAY :				                    // 话筒延迟时间
	    return Tab_MicDelay[counter];

	case KCM_MIC_VOLUME :								    // 0话筒1音量 1话筒2  2回声 3重复
		do {
			if (Tab_Mic16To10[gLocal_1] == value){          // 0-15转换到0-9
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_Mic16To10));
	    return 0;
    case KCM_MIC_DELAY:                                     // 话筒延迟时间
		do {
			if (Tab_MicDelay[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_MicDelay));
        return 0;

*/
