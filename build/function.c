
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www,hsav.com/download/kc3xm51.zip


#include "main.h"           

void MKCM_Initialize(){										// KCM模块初始化  	
	gDIP_Brightness = 0;
	MKCM_SetPowerOn();										// KCM开机
}	  
void MKCM_SetPowerOn(){ 									// KCM开机
	MKCM_WriteRegister(KCM_POWER_ON, 1);
    MDIP_MenuSelect(cMenu_PowerOn, MENU_NORMAL);
	gDIP_MenuLock = 100;									// 暂时锁定显示10秒
	gAUD_SrcFormat = 0;
    gAUD_BpsRate = 0;
    g2SUB_SrcValid = 0;
    gSUB_SrcAuto = INPUT_SWITCH_NONE;                       // 自动选择失效
    MLOG("KCM_POWER_ON\r\n");
}	  



void MKCM_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
    if (!HAL_KCM_I2C_INT()){                           		// detect KCM interrupt 检测到I2C从机产生中断,INT为低
	    BYTE gLocal_1;
	    BYTE gLocal_2;

		gLocal_1 = MKCM_ReadRegister(KCM_READ_IRQ);				// Read interrupt 读取KCM中断
		MKCM_WriteRegister(KCM_CLEAR_IRQ, gLocal_1);  			// Clear interrupt 清除相应的中断
		if (FKCM_I2C_Error){
            MLOG("KCM_READ_IRQ Error\r\n");
			return;
		}
        MLOG("KCM_CLEAR_IRQ %02x\r\n", (u32)gLocal_1);
		if ((gLocal_1 & KCM_IRQ_SYSTEM_INIT) > 0){			// 模式初始化完成中断，需要写入"KCM_POWER_ON"寄存器，
            MKCM_RestoreMemory();
		}
		if ((gLocal_1 & KCM_IRQ_FORMAT_INFO) > 0){          // 数码信号输入格式改变中断，需要读取"KCM_SRC_FORMAT"寄存器
			gAUD_SrcFormat = MKCM_ReadRegister(KCM_SRC_FORMAT);
            gAUD_BpsRate = MKCM_ReadRegister(KCM_BPS_RATE);
			if (gDIP_MenuLock == 0){						// 
				if (!FSYS_TestTone){						// 没有打开噪音测试
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
		if ((gLocal_1 & KCM_IRQ_SRC_VALID) > 0){            // 有效的音源输入改变中断，需要读取"KCM_SRC_VALID"寄存器
            MKCM_ReadSrcValid();
		}
		if ((gLocal_1 & KCM_IRQ_VOLUME) > 0){               // 有效的音源输入改变中断，需要读取"KCM_SRC_VALID"寄存器
			gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);     // 读取当前音量值
			if (gDIP_MenuSelect == cMenu_MasterVolume){
				MDIP_MenuSelect(cMenu_MasterVolume, 0);
			}
		}
        if ((gLocal_1 & KCM_IRQ_FIRMWARE) > 0){             // 固件更新，需要读取"KCM_RD_INFO"寄存器
            MDIP_MenuSelect(cMenu_Fireware, MENU_NORMAL);
        }
        if ((gLocal_1 & KCM_IRQ_MEDIA_TIME) > 0){           // 多媒体播放时间改变
            g2PlayTime = MKCM_Read2Byte(KCM_PLAY_TIME);
            if (g2PlayTime){                                // 播放时间改变 
                MDIP_MenuSelect(cMenu_PlayTime, MENU_NORMAL);
            }else {                                         // 播放完成了
                WORD g2Local_1 = (mINPUT_SWITCH == INPUT_SWITCH_SD) ? g2SdQty : g2UDiskQty;
                if (++g2PlayIndex >= g2Local_1){
                    g2PlayIndex = 0;
                }
                MKCM_Write2Byte(KCM_PLAY_INDEX, g2PlayIndex);  // 播放g2PlayIndex
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
CONST_CHAR TabSpectrum[] = {
    0x01,                                                   // 字节0选择频谱方式，1为频率电平方式
    5, 8, 0,                                                // 字节1至字节3为组成两个12位的显示缓冲宽度及高度像素
    100 & 0xff,   (100 >> 8) & 0xff,                        // 100Hz
    350 & 0xff,   (350 >> 8) & 0xff,                        // 350Hz
    1000 & 0xff,  (1000 >> 8) & 0xff,                       // 1000Hz
    3300 & 0xff,  (3300 >> 8) & 0xff,                       // 3300Hz
    10000 & 0xff, (10000 >> 8) & 0xff,                      // 10000Hz
};


void MKCM_RestoreMemory(){ 									// 开机，从KCM之中恢复记忆
    BYTE counter;
    BYTE select;
	BYTE value;
	BYTE gLocal_1;
    BYTE temp[10];

	// 输入端口选择：使用KCM_INPUT寄存器的记忆值
	value = MKCM_ReadRegister(KCM_INPUT_SOURCE);			// 记忆的输入端口选择
	if (FKCM_I2C_Error){									// 读取KCM出错
		MDIP_MenuSelect(cMenu_Restore, 0);
		return;
	}
	gSYS_ModelType = MKCM_ReadRegister(KCM_RD_INFO);
	mINPUT_SWITCH = MKCM_FromRegister(KCM_INPUT_SOURCE, value);	// 通过寄存器反向将选择的数值恢复
//MDEBUG(0xf8);MDEBUG(value);MDEBUG(mINPUT_SWITCH);
	// 音量：使用KCM_VOLUME寄存器的记忆值
	gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);	// 记忆的音量值
	// 音效处理通道：使用KCM_EQ_SELECT及各自的寄存器的记忆值
	gDIP_SoundEffect = MKCM_ReadRegister(KCM_EQ_SELECT);			// 记忆的音效处理通道选择

	for (counter = 0; counter < sizeof(gDIP_TrimCtrl); counter++){ // 声道微调：使用KCM_FL_TRIM等寄存器的记忆值
		gLocal_1 = MKCM_FromRegister(KCM_TEST_TONE, counter);	// 测试噪音的顺序
		value = MKCM_ReadRegister(KCM_FL_TRIM + gLocal_1);	// 记忆微调值
		gDIP_TrimCtrl[gLocal_1] = MKCM_FromRegister(KCM_FL_TRIM, value);	// 微调 寄存器->调节值
//MDEBUG(0xf9);MDEBUG(gLocal_1);MDEBUG(gDIP_TrimCtrl[gLocal_1]);
	};

	value = MKCM_ReadRegister(KCM_MIC_VOLUME);	            // 话筒1及话筒2音量比例
	gDIP_MicCtrl[0] = MKCM_FromRegister(KCM_MIC_VOLUME, value & 0x0f);  // 0-15转换到0-9
	gDIP_MicCtrl[1] = MKCM_FromRegister(KCM_MIC_VOLUME, (value >> 4) & 0x0f);

	value = MKCM_ReadRegister(KCM_MIC_ECHO_EQ);	            // 话筒回声及话筒多段EQ均衡音效处理选择比例
	gDIP_MicCtrl[2] = MKCM_FromRegister(KCM_MIC_DELAY, value & 0x0f);   // 0-15转换到0-9
	
	value = MKCM_ReadRegister(KCM_MIC_REPEAT);	            // 话筒重复及话筒直达声比例
	gDIP_MicCtrl[3] = MKCM_FromRegister(KCM_MIC_DELAY, value & 0x0f);   // 0-15转换到0-9
	
	value = MKCM_ReadRegister(KCM_MIC_DELAY);	            // 话筒延迟时间 记忆调整值
	gDIP_MicCtrl[4] = MKCM_FromRegister(KCM_MIC_DELAY, value);	// 255步转换到0-9
	
	value = MKCM_ReadRegister(KCM_MIC_REVERB);	            // 话筒混响比例 记忆调整值
	gDIP_MicCtrl[5] = MKCM_FromRegister(KCM_MIC_DELAY, value & 0x0f);   // 0-15转换到0-9

//MDEBUG(0xf6);MDEBUG(gDIP_MicCtrl[0]);MDEBUG(gDIP_MicCtrl[1]);


	// 喇叭设置选择：使用KCM_SPK_CONFIG的记忆值
	gLocal_1 = MKCM_ReadRegister(KCM_SPK_CONFIG);			// 记忆的喇叭设置
	MKCM_FromRegister(KCM_SPK_CONFIG, gLocal_1);

	// KCM扩展记忆：使用KCM_MEMORYRD寄存器的记忆值
	MKCM_ReadXByte(KCM_EXTR_MEMORY, 4, temp);               // 用户记忆
	gDIP_Surround[0] = temp[MEM_SURROUND_2CH];              // 环绕声模式立体声
	gDIP_Surround[1] = temp[MEM_SURROUND_8CH];              // 环绕声模式多声道
	gDIP_Select2Ch = temp[MEM_SELECT_2CH];                  // 选择为立体声
	gDIP_Brightness = temp[MEM_BRIGHTNESS];                 // 显示屏亮度
    MDIP_Brightness(0, gDIP_Brightness);
//MDEBUG(0xf9);MDEBUG(gDIP_Surround[0]);MDEBUG(gDIP_Surround[1]);MDEBUG(gDIP_Select2Ch);

	MKCM_WriteRegister(KCM_VOLUME_MAX, 80);                 // 80 设置音量最大值
	MKCM_WriteRegister(KCM_LPF_FREQ, 70);                   // 70Hz 超低音通道LPF低通滤波器频率
	MKCM_WriteRegister(KCM_HPF_FREQ, 70);                   // 70Hz 主声道小喇叭HPF高通滤波器频率
//KCM_DYN_COMPRES           KCM_DYN_COMPRES
//KCM_LIP_SYNC_SET  齿音同步延迟时间

    temp[0] = (BYTE)(CUSTOM_CODE >> 24);
    temp[1] = (BYTE)(CUSTOM_CODE >> 16);
    temp[2] = (BYTE)(CUSTOM_CODE >> 8);
    temp[3] = (BYTE)(CUSTOM_CODE >> 0);
    MKCM_WriteXByte(KCM_CUSTOM_CODE, 4, temp);              // 设置用户自定义功能代码及升级模块固件寄存器
                      
    for (select = 0; select < 4; select++){
        // 多路均衡EQ音效处理设置，一般写入一次用于初始化
        for (counter = 0; counter < sizeof(TabEqSetup); counter++){ 
            temp[counter + 1] = TabEqSetup[counter];
        }
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_SETUP, 1 + sizeof(TabEqSetup), temp);   // 多路均衡EQ音效处理设置

        // 多路均衡EQ音效频率，一般写入一次用于初始化
        for (counter = 0; counter < sizeof(TabEqFreq); counter++){ // 写入EQ频率
            temp[counter + 1] = TabEqFreq[counter];
        }
        for (counter = 0; counter < sizeof(TabEqFreq); counter++){ // 写入EQ频率电平
            temp[counter + 1] |= TabEqLevel[select][counter];
        }
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_VALUE, 1 + sizeof(TabEqFreq), temp);   // 多路均衡EQ音效处理设置
    }
    MKCM_WriteXByte(KCM_WR_SPECTRUM, sizeof(TabSpectrum), TabSpectrum);   // 设置频谱模式

    g2SUB_SrcValid = 0;                                     // 重新开始检测
	FSYS_Standby = 0;
	FSYS_TestTone = 0;
	FSYS_MuteEnable = 0;
    MDIP_MenuSelect(cMenu_PowerOn, 0);
	gDIP_MenuLock = 10;										// 可以退出暂时锁定显示了
    MDIP_SurroundSymbol();
    MDIP_SrcFormatSymbol();
	MDIP_WifiSymbol(0xff);	    	 
    return;
}

void MKCM_FactorySet(){										// 出厂设置
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

	// KCM扩展记忆：使用KCM_MEMORYRD寄存器的记忆值
	gLocal_Buffer[MEM_SURROUND_2CH] = 0;                    // 环绕声模式立体声
	gLocal_Buffer[MEM_SURROUND_8CH] = 0;                    // 环绕声模式多声道
	gLocal_Buffer[MEM_SELECT_2CH] = 0;                      // 选择为立体声
	gLocal_Buffer[MEM_BRIGHTNESS] = 2;                      // 显示屏亮度
	MKCM_WriteXByte(KCM_EXTR_MEMORY, 4, gLocal_Buffer);

	MKCM_WriteRegister(KCM_VOLUME_CTRL, 39); 				// 音量值

	// 声道微调：使用KCM_FL_TRIM等寄存器的记忆值
	gLocal_1 = 0;
	do {
		MKCM_WriteRegister(KCM_FL_TRIM + gLocal_1, 0);
	} while (++gLocal_1 < 8);

	MKCM_WriteRegister(KCM_SPK_CONFIG, 0xab);				// 后置大、环绕大、中置大、前置大、有超低音
	MKCM_WriteRegister(KCM_POWER_ON, 1);
    MDIP_MenuSelect(cMenu_PowerOn, 0);
	gDIP_MenuLock = 30;										// 暂时锁定显示3秒
    return;
}

char MKCM_HdmiInsert(WORD flag, WORD g2Local_1){            // 检查HDMI插入，有插入返回1
    if (g2Local_1 & flag){                                  // 本次有本次的HDMI
        if (!(g2SUB_SrcValid & flag)){                      // 上次没有本次的HDMI
            if (mINPUT_SWITCH < INPUT_SWITCH_HDMI1 && mINPUT_SWITCH > INPUT_SWITCH_H_ARC){ // 如果原来不是选择HDMI
                if (mINPUT_SWITCH != INPUT_SWITCH_SD && mINPUT_SWITCH != INPUT_SWITCH_UDISK){ // 如果原来不是选择SD/U盘
                    gSUB_SrcAuto = mINPUT_SWITCH;           // 保存原来的选择
                }
            }
            if (flag == KCM_SRC_VALID_HDMI1){
                mINPUT_SWITCH = INPUT_SWITCH_HDMI1;         // 自动换到HDMI1
            }else if (flag == KCM_SRC_VALID_HDMI2){
                mINPUT_SWITCH = INPUT_SWITCH_HDMI2;         // 自动换到HDMI2
            }else {
                mINPUT_SWITCH = INPUT_SWITCH_HDMI3;         // 自动换到HDMI3
            }
            //MKCM_WriteRegister(KCM_INPUT_SOURCE, MKCM_ToRegister(KCM_INPUT_SOURCE, mINPUT_SWITCH));  
			gDIP_MenuSelect = cMenu_Restore;				// 不管菜单在哪里，都需要马上动作
            MAUD_MixInputSource(mINPUT_SWITCH);
//            MLOG("HdmiInsertB:%d %02x\r\n", (u32)mINPUT_SWITCH, (u32)MKCM_ToRegister(KCM_INPUT_SOURCE, mINPUT_SWITCH));
            return 1;
        }
    }
    return 0;
}
char MKCM_HdmiRemove(WORD flag, WORD g2Local_1){            // 检查HDMI拔出，有拔出返回1
    if (!(g2Local_1 & flag)){                               // 本次没有HDMI
        if (g2SUB_SrcValid & flag){                         // 上次有HDMI
            if (gSUB_SrcAuto != INPUT_SWITCH_NONE){         // 不是自动选择失效
                MKCM_WriteRegister(KCM_INPUT_SOURCE, MKCM_ToRegister(KCM_INPUT_SOURCE, gSUB_SrcAuto));  // 恢复原来的输入
                MLOG("MKCM_HdmiRemove:%d\r\n", gSUB_SrcAuto);
            }
            return 1;
        }
    }
    return 0;
}

char MKCM_SdUdiskInsert(WORD flag, WORD g2Local_1){         // 检查SD/U盘插入，有插入返回1
    if (g2Local_1 & flag){                                  // 本次有SD/U盘
        if (!(g2SUB_SrcValid & flag)){                      // 上次没有SD/U盘
            WORD track = MKCM_Read2Byte((flag == KCM_SRC_VALID_SD) ? KCM_RD_SD_QTY : KCM_RD_UDISK_QTY);
    //MDEBUG(0xe8);MDEBUG(total);
            if (track){                                     // 找到文件
                if (mINPUT_SWITCH != INPUT_SWITCH_SD && mINPUT_SWITCH != INPUT_SWITCH_UDISK){ // 如果原来不是选择SD/U盘
                    gSUB_SrcAuto = mINPUT_SWITCH;           // 保存原来的选择
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
                MKCM_Write2Byte(KCM_PLAY_INDEX, g2PlayIndex);  // 播放第0首
            }else {
                MDIP_MenuSelect((flag == KCM_SRC_VALID_SD) ? cMenu_SdInsert : cMenu_UDiskInsert, 0);     // 显示SD/U盘插入
            }
            return 1;
        }
    }
    return 0;
}
char MKCM_SdUdiskRemove(WORD flag, WORD g2Local_1){         // 检查SD/U盘拔出，有拔出返回1
    if (!(g2Local_1 & flag)){                               // 本次没有SD/U盘
        if (g2SUB_SrcValid & flag){                         // 上次有SD/U盘
            if (flag == KCM_SRC_VALID_SD){
                g2SdQty = 0;
            }else {
                g2UDiskQty = 0;
            }
            if (gSUB_SrcAuto != INPUT_SWITCH_NONE){         // 不是自动选择失效
                MKCM_WriteRegister(KCM_INPUT_SOURCE, MKCM_ToRegister(KCM_INPUT_SOURCE, gSUB_SrcAuto));  // 恢复原来的输入
            }else {
                MDIP_MenuSelect((flag == KCM_SRC_VALID_SD) ? cMenu_UDiskRemove : cMenu_UDiskRemove, 0);         // 显示SD/U盘插出
            }
            return 1;
        }
    }
    return 0;
}
void MKCM_ReadSrcValid(){
	WORD g2Local_1 = MKCM_Read2Byte(KCM_SRC_VALID);         // 本次的有效音源
//if(g2Local_1==KCM_SRC_VALID_UDISK){g2Local_1=KCM_SRC_VALID_HDMI1|KCM_SRC_VALID_HDMI2;}
    if (g2SUB_SrcValid != g2Local_1){                       // 本次与上次的有效音源改变
        MLOG("KCM_SRC_VALID:%04x Last:%04x\r\n", (u32)g2Local_1, (u32)g2SUB_SrcValid);
        if (!MKCM_HdmiInsert(KCM_SRC_VALID_HDMI1, g2Local_1)){         // 检查HDMI1插入，没有才检查HDMI2插入
            if (!MKCM_HdmiInsert(KCM_SRC_VALID_HDMI2, g2Local_1)){     // 检查HDMI2插入，没有才检查HDMI3插入
                MKCM_HdmiInsert(KCM_SRC_VALID_HDMI3, g2Local_1);       // 检查HDMI3插入
            }
        }
        MKCM_HdmiRemove(KCM_SRC_VALID_HDMI1, g2Local_1);
        MKCM_HdmiRemove(KCM_SRC_VALID_HDMI2, g2Local_1);
        MKCM_HdmiRemove(KCM_SRC_VALID_HDMI3, g2Local_1);

        if (!MKCM_SdUdiskInsert(KCM_SRC_VALID_UDISK, g2Local_1)){   // 检查U盘插入，没有才检查SD插入
            MKCM_SdUdiskInsert(KCM_SRC_VALID_SD, g2Local_1);    // 检查SD插入
        }
        MKCM_SdUdiskRemove(KCM_SRC_VALID_SD, g2Local_1);
        MKCM_SdUdiskRemove(KCM_SRC_VALID_UDISK, g2Local_1);
        g2SUB_SrcValid = g2Local_1;
    }else {
        MLOG("KCM_SRC_VALID = Last:%04x\r\n", (u32)g2Local_1);
    }
}

void MKCM_WifiCommand(BYTE regNumber, BYTE value){		// 收到远程APP的指令
//MDEBUG(0xa9);MDEBUG(regNumber);MDEBUG(value);
	switch (regNumber){
	case KCM_VOLUME_MUTE:									// 音频静音及音量加减控制
		if (FSYS_MuteEnable != (value & 0x01)){				// MUTE改变
			MKEY_AudioMute();
		}
		else if (value & 0x04){								// 音量加或减
			gDIP_MenuSelect = cMenu_MasterVolume;
			MKCM_WriteRegister(KCM_VOLUME_MUTE, value);
		}
		break;
	case KCM_INPUT_SOURCE:										// 输入音源选择
		mINPUT_SWITCH = MKCM_FromRegister(KCM_INPUT_SOURCE, value);	// 通过寄存器反向将选择的数值恢复
		MAUD_AutoCanclMute();
		MAUD_AutoCanclTestTone();
		MDIP_MenuSelect(cMenu_Restore, 0);
		break;
	}
}


// //////////////////////////////////////////////////
// KCM寄存器的值与本机调节的值相互转换
// //////////////////////////////////////////////////
CONST_CHAR Tab_InputSwitch[] = {							// KCM_INPUT_SOURCE
	KCM_INPUT_ANALOG,                                       // 0=模拟输入
	KCM_INPUT_DIGITAL | 0,                                  // 1=数码1
    KCM_INPUT_DIGITAL | 1,                                  // 2=数码2
    KCM_INPUT_DIGITAL | 2, 			                        // 3=数码3
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
// 规范的通道顺序：FL FR CN SW SL SR BL BR
// 测试噪音的顺序：FL CN SW FR SR BR BL SL 					// 属于声音从喇叭分布的顺时针
	0,2,3,1, 5,7, 6,4,
};

CONST_CHAR Tab_ChannelTrim[] = {
	0x10 | 9, 0x10 | 8, 0x10 | 7, 0x10 | 6, 0x10 | 5,       // -9 -8 -7 -6 -5 dB     
	0x10 | 4, 0x10 | 3, 0x10 | 2, 0x10 | 1, 0x00,		    // -4 -3 -2 -1 0  dB
	1,2,3,4,5,6,7,8,9, 								        // +1 ~ +9 dB
};  						 
CONST_CHAR Tab_MicDelay[] = {                               // 0-255转换到0-9
    60/20,80/20,100/20,120/20,      160/20,200/20,300/20,500/20,    1000/20,2000/20,    // 单位毫秒    
};
CONST_CHAR Tab_Mic16To10[] = {                              // 0-15转换到0-9
    0,2,4,6,     8,10,12,13,    14,15,         
};

BYTE MKCM_ToRegister(BYTE index, BYTE counter){				// 从本机处理的值，转换到KCM寄存器的值
	switch (index){
	case KCM_INPUT_SOURCE :									// 输入端口选择
    	return Tab_InputSwitch[counter];
	case KCM_LISTEN_MODE :									// 环绕声模式
    	return Tab_SurroundMode[counter];
	case KCM_TEST_TONE :									// 噪音测试
    	return Tab_TestToneChannel[counter];
	case KCM_FL_TRIM :										// 声道微调
	    return Tab_ChannelTrim[counter];
	case KCM_SPK_CONFIG :									// 喇叭设置
		// B7-B6后置B5-B4环绕B3-B2中置(0=没有安装 1=小喇叭 2=大喇叭)B1前置(0=小喇叭)B0超低音(0=没有安装)
		// 0前置 1中置 2超低音 3环绕 4后置
		if (1){
			BYTE gLocal_1 = 0;
			if (gDIP_SpeakSetup[2] > 0){					// 超低音
				gLocal_1 |= 0x01;
			}
			if (gDIP_SpeakSetup[0] > 0){					// 前置大喇叭
				gLocal_1 |= 0x02;
			}
			gLocal_1 |= gDIP_SpeakSetup[1] << 2;			// 中置
			gLocal_1 |= gDIP_SpeakSetup[3] << 4;			// 环绕
			gLocal_1 |= gDIP_SpeakSetup[4] << 6;			// 后置
			return gLocal_1;
		}
	case KCM_MIC_VOLUME :									// 话筒比例
	    return Tab_Mic16To10[counter];
	case KCM_MIC_DELAY :				                    // 话筒延迟时间
	    return Tab_MicDelay[counter];
	}
	return 0;
}
BYTE MKCM_FromRegister(BYTE index, BYTE value){				// 从KCM来的寄存器，转换到本机处理的值
	BYTE gLocal_1 = 0;
	switch (index){
	case KCM_INPUT_SOURCE :									// 输入端口选择
		do {
			if (Tab_InputSwitch[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_InputSwitch));
		return INPUT_SWITCH_AUX;								// 超出范围，返回默认值AUX
	case KCM_LISTEN_MODE :									// 环绕声模式
		do {
			if (Tab_SurroundMode[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_SurroundMode));
		return 0x01;										// 超出范围，返回默认值
	case KCM_TEST_TONE :									// 噪音测试
		do {
			if (Tab_TestToneChannel[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_TestToneChannel));
		return 0;											// 超出范围，返回默认值0
	case KCM_FL_TRIM :										// 声道微调
		do {
			if (Tab_ChannelTrim[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_ChannelTrim));
		return 0;											// 超出范围，返回默认值0dB
    case KCM_SPK_CONFIG :								// 喇叭设置
		// B7:6后置B5:4环绕B3:2中置B1前置B0超低音 超低音：0=没有安装；前置：0=小喇叭，其他：0=没有安装 1=小喇叭 2=大喇叭
		// 0前置 1中置 2超低音 3环绕 4后置
		gDIP_SpeakSetup[0] = (value & 0x02) ? 1 : 0;		// 前置：小(0)大(1)选择
		gDIP_SpeakSetup[1] = (value >> 2) & 0x03;
		gDIP_SpeakSetup[2] = (value & 0x01) ? 1 : 0;		// 超低音：没有安装(0)/安装(1)
	    gDIP_SpeakSetup[3] = (value >> 4) & 0x03;
	    gDIP_SpeakSetup[4] = (value >> 6) & 0x03;
		break;
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
	}
	return 0;												// 超出范围，返回默认值0dB
}
void MAPI_COPY_BUFF8(BYTE length, BYTE* in_data, BYTE* out_data){
	for (; length != 0; length--){
        *out_data++ = *in_data++;
    };
    return;
}
