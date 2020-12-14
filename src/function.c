
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip


#include "main.h"           

void MKCM_Initialize(){										// KCM模块初始化  	
	gDIP_Brightness = 0;
	MKCM_SetPowerOn();										// KCM开机
}	  
void MKCM_SetPowerOn(){ 									// KCM开机
	MKCM_WriteRegister(KCM_POWER_ON, 1);
    MDIP_MenuNormal(MENU_POWER_ON);                         // 菜单状态:电源打开
	gDIP_MenuLock = 100;									// 暂时锁定显示10秒
    MLOG("KCM_POWER_ON");
}	  



void MKCM_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
    if (!HAL_KCM_I2C_INT()){                           		// detect KCM interrupt 检测到I2C从机产生中断,INT为低
	    BYTE gLocal_1;
	    BYTE gLocal_2;

		gLocal_1 = MKCM_ReadRegister(KCM_READ_IRQ);			// Read interrupt 读取KCM中断
		MKCM_WriteRegister(KCM_CLEAR_IRQ, gLocal_1);  		// Clear interrupt 清除相应的中断
		if (FKCM_I2C_Error){
            MLOG("KCM_READ_IRQ Error");
			return;
		}
		if (gLocal_1 != KCM_IRQ_PLAY_TIME){					// 不是多媒体播放时间改变
// MLOG("KCM_READ_IRQ %02x", gLocal_1);
		}
		if ((gLocal_1 & KCM_IRQ_SYSTEM_INIT) > 0){			// 模式初始化完成中断，需要写入"KCM_POWER_ON"寄存器，
            MKCM_RestoreMemory();
		}
		if ((gLocal_1 & KCM_IRQ_FORMAT_INFO) > 0){          // 数码信号输入格式改变中断，需要读取"KCM_SRC_FORMAT"寄存器
			gAUD_SrcFormat = MKCM_ReadRegister(KCM_SRC_FORMAT);
			//MLOG("gAUD_SrcFormat %02x", gAUD_SrcFormat);
            gAUD_BpsRate = MKCM_ReadRegister(KCM_BPS_RATE);
			if (gDIP_MenuLock == 0){						// 
				if (!FSYS_TestTone){						// 没有打开噪音测试
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
		if ((gLocal_1 & KCM_IRQ_SRC_VALID) > 0){            	// 有效的音源输入改变中断，需要读取"KCM_SRC_VALID"寄存器
            MKCM_ReadSrcValid();
		}
		if ((gLocal_1 & KCM_IRQ_VOLUME) > 0){               	// 音量调节改变中断，需要读取"KCM_VOLUME_CTRL"寄存器获取更新的音量值
			gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);     // 读取当前音量值
			if (gDIP_MenuSelect == cMenu_MasterVolume){
				MDIP_MenuNormal(cMenu_MasterVolume);
			}
		}
        if ((gLocal_1 & KCM_IRQ_FIRMWARE) > 0){             // 固件更新，需要读取"KCM_RD_INFO"寄存器
            MDIP_FirewareInfo();                            // 显示固件更新
//            MDIP_MenuNormal(cMenu_Fireware);
        }
        if ((gLocal_1 & KCM_IRQ_PLAY_TIME) > 0){           		// 多媒体播放时间改变
            g2PlayTime = MKCM_Read2Byte(KCM_PLAY_TIME);
            if (g2PlayTime){                                	// 播放时间改变 
				if (gDIP_MenuSelect != MENU_INPUT_SOURCE){
	                MDIP_MenuNormal(cMenu_PlayTime);
				}
            }else {                                         	// 播放完成了
                MDIP_MenuNormal(cMenu_PlayTrack);
            }
        }
        if ((gLocal_1 & KCM_IRQ_PLAY_STATE) > 0){           // 多媒体文件播放状态改变
        	gPlayStatus = MKCM_ReadRegister(KCM_PLAY_STATE);     // 读取多媒体文件播放状态
			MLOG("KCM_IRQ_PLAY_STATE %02x", gPlayStatus);
        	MDIP_PlaySymbol(gPlayStatus);
        }
		if ((gLocal_1 & KCM_IRQ_APP_COMMAND) > 0){          // 收到手机/远程APP控制指令，需要读取"KCM_APP_COMMAND"寄存器
            MKCM_AppCommand();
        }
    }
    
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

	// KCM扩展记忆：使用KCM_MEMORYRD寄存器的记忆值
	MKCM_ReadXByte(KCM_EXTR_MEMORY, temp, 4);               // 读取用户记忆区的记忆到temp
	if (FKCM_I2C_Error){									// 读取KCM出错
		MDIP_MenuNormal(MENU_RESTORE);						// 已经出错，就不做 
		return;
	}
    value = temp[MEM_SOURCE_AUTO];
//	gDIP_Surround[0] = temp[MEM_SURROUND_2CH] & 0x01;       // 聆听模式立体声
//	gDIP_Surround[1] = temp[MEM_SURROUND_8CH] & 0x03;       // 聆听模式多声道
	//gDIP_Select2Ch = temp[MEM_SELECT_2CH] & 0x01;           // 选择为立体声
	gDIP_Brightness = temp[MEM_BRIGHTNESS] & 0x03;          // 显示屏亮度
//MLOG("BrightA_%02x_%02x_", temp[MEM_SOURCE_AUTO], temp[MEM_BRIGHTNESS]);

	gLocal_1 = MKCM_FromRegister(KCM_INPUT_SOURCE, value);	// 通过寄存器反向将选择的数值恢复
	MLOG("RestoreA %02x %02x %02x", value, gLocal_1, temp[MEM_SOURCE_AUTO]);
	if (gLocal_1 >= INPUT_SWITCH_SD && gLocal_1 <= INPUT_SWITCH_BT){	// SD/UDISK/USB声卡/蓝牙音频
		mINPUT_SWITCH = temp[MEM_SOURCE_AUTO];  			// 自动输入的恢复 
	}else{
		mINPUT_SWITCH = gLocal_1;							// 使用KCM记忆的输入端口选择
	}
//MLOG("mINPUT_B %02x %02x", mINPUT_SWITCH, gLocal_1);
	
	// 输入端口选择：使用KCM_INPUT寄存器的记忆值
	value = MKCM_ReadRegister(KCM_INPUT_SOURCE);			// 记忆的输入端口选择
	MKCM_ReadXByte(KCM_RD_INFO, temp, 2);                   // 读取模块信息寄存器
    gSYS_ModelType = temp[1];                               // [0]文件升级百分比[1]模块型号[2-n]每8字节为各种固件的日期版本


	// 音量：使用KCM_VOLUME寄存器的记忆值
	gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);	// 记忆的音量值
	// 音效处理通道：使用KCM_EQ_SELECT及各自的寄存器的记忆值
//	gDIP_SoundEffect = MKCM_ReadRegister(KCM_EQ_SELECT);			// 记忆的音效处理通道选择

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
    MKCM_WriteXByte(KCM_CUSTOM_CODE, temp, 4);              // 设置用户自定义功能代码及升级模块固件寄存器
                      
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
    MKCM_WriteXByte(KCM_WR_SPECTRUM, TabSpectrum, sizeof(TabSpectrum));   // 设置频谱模式

	// 清除一些上电需要确定值的变量及标志
	gAUD_SrcFormat = 0;
    gAUD_BpsRate = 0;
    gPlayStatus = 0;
	FSYS_Standby = 0;
	FSYS_TestTone = 0;
	FSYS_MuteEnable = 0;
	gRemoveTimer = 0;
	MAUD_MakePreemptible(KCM_SRC_VALID_E8CH);				// 生成抢占式输入选择，模块版本是KC35H，需要加入外置7.1声道 

    MDIP_MenuNormal(MENU_POWER_ON);                         // 菜单状态:电源打开
	gDIP_MenuLock = 10;										// 可以退出暂时锁定显示了
    MDIP_SurroundSymbol();
    MDIP_SrcFormatSymbol();
	MDIP_WifiSymbol(0xff);	    	 
	MDIP_PlaySymbol(gPlayStatus);
	MDIP_Brightness(0, gDIP_Brightness);
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
	gLocal_Buffer[MEM_SOURCE_AUTO] = 0;                    	// 自动输入的恢复
	gLocal_Buffer[MEM_SURROUND_2CH] = 0;                    // 环绕声模式立体声
	gLocal_Buffer[MEM_SURROUND_8CH] = 0;                    // 环绕声模式多声道
	gLocal_Buffer[MEM_SELECT_2CH] = 0;                      // 选择为立体声
	gLocal_Buffer[MEM_BRIGHTNESS] = 2;                      // 显示屏亮度
	MKCM_WriteXByte(KCM_EXTR_MEMORY, gLocal_Buffer, 5);

	MKCM_WriteRegister(KCM_VOLUME_CTRL, 39); 				// 音量值

	// 声道微调：使用KCM_FL_TRIM等寄存器的记忆值
	gLocal_1 = 0;
	do {
		MKCM_WriteRegister(KCM_FL_TRIM + gLocal_1, 0);
	} while (++gLocal_1 < 8);

	MKCM_WriteRegister(KCM_SPK_CONFIG, 0xab);				// 后置大、环绕大、中置大、前置大、有超低音
	MKCM_WriteRegister(KCM_POWER_ON, 1);
    MDIP_MenuNormal(MENU_POWER_ON);                         // 菜单状态:电源打开
	gDIP_MenuLock = 30;										// 暂时锁定显示3秒
    return;
}
void MKCM_AppCommand(){
    BYTE inData[8];
    BYTE length = MKCM_ReadAutoByte(KCM_APP_COMMAND, inData, 8);
    if (length == 2){
//        MKCM_WriteRegister(inData[0], inData[1]);
    	switch (inData[0]){
    	case KCM_VOLUME_CTRL:									// 音频静音及音量加减控制
            MLOG("KCM_VOLUME_CTRL %d", inData[1]);
            gAUD_MasterVolume = inData[1];
            MDIP_MenuNormal(cMenu_MasterVolume);
    		break;
    	case KCM_INPUT_SOURCE:									// 输入音源选择
            MLOG("KCM_INPUT_SOURCE %02x", inData[1]);
    		mINPUT_SWITCH = MKCM_FromRegister(KCM_INPUT_SOURCE, inData[1]);	// 通过寄存器反向将选择的数值恢复
    		MAUD_AutoCanclMute();
    		MAUD_AutoCanclTestTone();
    		MDIP_MenuNormal(MENU_RESTORE);
    		break;
	    case KCM_LISTEN_MODE:				                    // 聆听模式选择
            MLOG("KCM_LISTEN_MODE %02x", inData[1]);
            MDIP_ListenMode(inData[1]);
            break;
	    case KCM_EQ_SELECT:				                        // 多路均衡音效处理选择
            MLOG("KCM_LISTEN_MODE %02x", inData[1]);
            MDIP_EqSelect(inData[1]);
            break;

    	case KCM_VOLUME_MUTE:									// 音频静音及音量加减控制
    		if (FSYS_MuteEnable != (inData[1] & 0x01)){			// MUTE改变
    			//MKEY_AudioMute();
    		}
    		else if (inData[1] & 0x04){							// 音量加或减
    			gDIP_MenuSelect = cMenu_MasterVolume;
    			//MKCM_WriteRegister(KCM_VOLUME_MUTE, inData[1]);
    		}
    		break;
    	default :
            if (inData[0] >= KCM_FL_TRIM && inData[0] <= KCM_BR_TRIM){    // 各声道的微调
            }
        MLOG("AppCommand %02x %02x %02x", length, inData[0], inData[1]);
    	    break;
    	}
    }
    MDIP_WifiSymbol(1);
}


// //////////////////////////////////////////////////
// KCM寄存器的值与本机调节的值相互转换
// //////////////////////////////////////////////////
CONST_CHAR Tab_InputSwitch[] = {							// KCM_INPUT_SOURCE     KC3X_INPUT_TYPE
	KCM_INPUT_ANALOG,			                            // 音源选择模拟输入
	KCM_INPUT_RX1,				                            // 音源选择RX1输入
	KCM_INPUT_RX2,				                            // 音源选择RX2输入
	KCM_INPUT_RX3,				                            // 音源选择RX3输入
	KCM_INPUT_SD,				                            // 音源选择SD/TF卡输入
	KCM_INPUT_UDISK,				                        // 音源选择U盘输入
	KCM_INPUT_PC,				                            // 音源选择USB声卡输入
	KCM_INPUT_E8CH,				                            // 音源选择外置7.1声道输入
	KCM_INPUT_BT,				                            // 音源选择蓝牙输入
	KCM_INPUT_HDMI1,				                        // 音源选择HDMI1输入
	KCM_INPUT_HDMI2,				                        // 音源选择HDMI2输入
	KCM_INPUT_HDMI3,				                        // 音源选择HDMI3输入
	KCM_INPUT_ARC,				                            // 音源选择HDMI ARC输入
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
