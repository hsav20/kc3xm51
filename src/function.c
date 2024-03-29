
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
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
	MKCM_WriteRegister(KCM_POWER_ON, KCM_SET_POWER_ON);		// 设置模块进入正常工作状态
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
 MLOG("KCM_READ_IRQ %02x", gLocal_1);
		}
		if ((gLocal_1 & KCM_IRQ_SYSTEM_INIT) > 0){			// KCM模式初始化完成中断
            MKCM_RestoreMemory();							// 从KCM恢复本地的记忆
			MAUD_InputWrite(mINPUT_SWITCH, mINPUT_SWITCH);
		}
		if (FSYS_Standby){									// 待机状态下不处理后面的中断了
			return;
		}
		if ((gLocal_1 & KCM_IRQ_FORMAT_INFO) > 0){          // 数码信号输入格式改变中断，需要读取"KCM_SRC_FORMAT"寄存器
			MKCM_ReadSrcInfo();								// 收到中断读取格式、通道、采样率、码流率等信息
		}
		if ((gLocal_1 & KCM_IRQ_SRC_VALID) > 0){            // 有效的音源输入改变中断，需要读取"KCM_SRC_VALID"寄存器
            MKCM_ReadSrcValid();							// 收到中断读取有效的音源输入改变
		}
		if ((gLocal_1 & KCM_IRQ_VOLUME) > 0){               	// 音量调节改变中断，需要读取"KCM_VOLUME_CTRL"寄存器获取更新的音量值
			gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);     // 读取当前音量值
			if (gDIP_MenuSelect == MENU_MASTER_VOL){
				MDIP_MenuNormal(MENU_MASTER_VOL);
			}
		}
        if ((gLocal_1 & KCM_IRQ_FIRMWARE) > 0){             // 固件更新，需要读取"KCM_RD_INFO"寄存器
            MDIP_FirewareInfo();                            // 显示固件更新
        }
        if ((gLocal_1 & KCM_IRQ_PLAY_TIME) > 0){           	// 多媒体播放时间改变
            if (gDIP_MenuSelect == MENU_RESTORE || gDIP_MenuSelect == MENU_INPUT_SOURCE){	// 只有在菜单恢复或输入音源选择状态
				 g2PlayTime = MKCM_Read2Byte(KCM_PLAY_TIME);
				 MDIP_InputSource();
			}
		//	MLOG("PLAY_TIME %d %02x", gDIP_MenuSelect, g2PlayTime);
        }
        if ((gLocal_1 & KCM_IRQ_PLAY_STATE) > 0){           // 多媒体文件播放状态改变
        	gPlayStatus = MKCM_ReadRegister(KCM_PLAY_STATE);     // 读取多媒体文件播放状态
			MLOG("KCM_IRQ_PLAY_STATE %02x", gPlayStatus);
			if ((gPlayStatus & KC3X_STATE_PLAY_FLAG) == KC3X_STATE_PLAY_END){	// 文件播放完成，已经停止
#ifndef NO_PLAY_NEXT	
				// 文件播放完成，需要继续播放后一首			
				MKCM_WriteRegister(KCM_PLAY_OPERATE, KCM_OPERATE_SKIP_UP);  // 多媒体播放后一首
#endif				
			}
        	MDIP_PlaySymbol(gPlayStatus);
        }
		if ((gLocal_1 & KCM_IRQ_APP_COMMAND) > 0){          // 收到手机/远程APP控制指令，需要读取"KCM_APP_COMMAND"寄存器
            MKCM_AppCommand();
        }
    }
    
}
	  
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
	BYTE value;
	BYTE gLocal_1;
    BYTE temp[4];

	// KCM扩展记忆：使用KCM_MEMORYRD寄存器的记忆值
	MKCM_ReadXByte(KCM_EXTR_MEMORY, temp, 4);               // 读取用户记忆区的记忆到temp
	if (FKCM_I2C_Error){									// 读取KCM出错
		MDIP_MenuNormal(MENU_RESTORE);						// 已经出错，就不做 
		return;
	}
	gDIP_Brightness = temp[MEM_BRIGHTNESS] & 0x03;          // 从记忆之中恢复显示屏亮度
	gDIP_MicTone[0] = temp[MEM_MIC_BASS];					// 从记忆之中恢复话筒低音
	gDIP_MicTone[1] = temp[MEM_MIC_TREBLE];					// 从记忆之中恢复话筒高音
    
	gLocal_1 = MKCM_ReadRegister(KCM_INPUT_SOURCE);			// 记忆的输入端口选择
	value = MKCM_FromRegister(KCM_INPUT_SOURCE, gLocal_1);  // 自动输入的恢复
	MLOG("INPUT_A %d %02x", value, gLocal_1); 
	if (value >= INPUT_SWITCH_SD && value <= INPUT_SWITCH_BT){	// 抢占式输入 SD/UDISK/USB声卡/蓝牙音频
		value = MKCM_FromRegister(KCM_INPUT_SOURCE, temp[MEM_SOURCE_AUTO]);	// 通过寄存器反向将选择的数值恢复
		MLOG("INPUT_B %d %02x", value, temp[MEM_SOURCE_AUTO]);
	}
	mINPUT_SWITCH = value;
	
// 	value = temp[MEM_SOURCE_AUTO];							// 从记忆之中恢复自动输入
// //MLOG("BrightA_%02x_%02x_", temp[MEM_SOURCE_AUTO], temp[MEM_BRIGHTNESS]);
// 	if (gLocal_1 >= INPUT_SWITCH_SD && gLocal_1 <= INPUT_SWITCH_BT){	// 抢占式输入 SD/UDISK/USB声卡/蓝牙音频
// 		// 输入端口选择：使用KCM_INPUT寄存器的记忆值
// 		value = MKCM_ReadRegister(KCM_INPUT_SOURCE);			// 记忆的输入端口选择
// 		mINPUT_SWITCH = MKCM_FromRegister(KCM_INPUT_SOURCE, value);  			// 自动输入的恢复 
// 		MLOG("INPUT_B %02x %02x", mINPUT_SWITCH, value);
// 	}else{													// 不是抢占式输入
// 		mINPUT_SWITCH = gLocal_1;							// 使用KCM记忆的输入端口选择
// 		MLOG("INPUT_C %02x %02x", mINPUT_SWITCH, value);
// 	}	
	
	MKCM_ReadXByte(KCM_RD_INFO, temp, 2);                   // 读取模块信息寄存器
    gSYS_ModelType = temp[1];                               // [0]文件升级百分比[1]模块型号[2-n]每8字节为各种固件的日期版本

	// 音量：使用KCM_VOLUME寄存器的记忆值
	gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);	// 记忆的音量值
	// 音效处理通道：使用KCM_EQ_SELECT及各自的寄存器的记忆值
//	gDIP_SoundEffect = MKCM_ReadRegister(KCM_EQ_SELECT);			// 记忆的音效处理通道选择
	for (counter = 0; counter < sizeof(gDIP_TrimCtrl); counter++){ // 声道微调：使用KCM_FL_TRIM等寄存器的记忆值
		gLocal_1 = MKCM_ToRegister(KCM_TEST_TONE, counter);	// 测试噪音的顺序
		value = MKCM_ReadRegister(KCM_FL_TRIM + gLocal_1);	// 记忆微调值
		gDIP_TrimCtrl[counter] = MKCM_FromRegister(KCM_FL_TRIM, value);	// 微调 寄存器->调节值
//MLOG("gDIP_TrimCtrl A %d %02x %d %d", counter, KCM_FL_TRIM+gLocal_1, gDIP_TrimCtrl[gLocal_1], value);
	};
//MLOG("gDIP_TrimCtrl B %d %d", gDIP_TrimCtrl[1], gDIP_TrimCtrl[2]);		
//MLOG("gDIP_TrimCtrl C %d %d", MKCM_ReadRegister(KCM_CE_TRIM), MKCM_ReadRegister(KCM_SW_TRIM));		

	value = MKCM_ReadRegister(KCM_MIC_VOLUME);	            // 话筒1及话筒2音量比例
	gDIP_MicCtrl[0] = MKCM_FromRegister(KCM_MIC_VOLUME, value & 0x0f);  // 0-15转换到0-9
	gDIP_MicCtrl[1] = MKCM_FromRegister(KCM_MIC_VOLUME, (value >> 4) & 0x0f);

	// 喇叭设置选择：使用KCM_SPK_CONFIG的记忆值
	value = MKCM_ReadRegister(KCM_SPK_CONFIG);			// 记忆的喇叭设置
	MKCM_FromRegister(KCM_SPK_CONFIG, value);
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
// temp[0]=0x00;temp[1]=0x1f;temp[2]=0x38;temp[3]=0x00;	
    MKCM_WriteXByte(KCM_CUSTOM_CODE, temp, 4);              // 设置用户自定义功能代码及升级模块固件寄存器
	MEQMIC_EqRestore();										// EQ恢复记忆
	MEQMIC_MicRestore();									// 话筒恢复记忆
    MKCM_WriteXByte(KCM_WR_SPECTRUM, TabSpectrum, sizeof(TabSpectrum));   // 设置频谱模式

	// 清除一些上电需要确定值的变量及标志
	gAUD_SrcFormat = 0;
    gAUD_SrcRate = 0;
	g2PlayTime = 0;
    gPlayStatus = 0;
	FSYS_Standby = 0;
	FSYS_TestTone = 0;
	FSYS_MuteEnable = 0;
	gRemoveTimer = 0;
	MAUD_MakePreemptible(0);								// 没有了抢占式输入，初始化所有变量

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
	BYTE temp[4];

	MDIP_WrString("SET ");
	gLocal_1 = 0;
	do {
		MDIP_Write2Digit(4, gLocal_1);
		MDIP_ScreenUpdata();
		MUSDELAY(255);
		MUSDELAY(255);
	} while (++gLocal_1 < 100);

	// KCM扩展记忆：使用KCM_MEMORYRD寄存器的记忆值
	temp[MEM_BRIGHTNESS] = 2;                      			// 显示屏亮度
	temp[MEM_SOURCE_AUTO] = 0;                    			// 自动输入的恢复
	temp[MEM_MIC_BASS] = 9;                      			// 记忆话筒音调低音调节
	temp[MEM_MIC_TREBLE] = 9;                      			// 从记忆之中恢复话筒高音
	MKCM_WriteXByte(KCM_EXTR_MEMORY, temp, 4);

	MKCM_WriteRegister(KCM_VOLUME_CTRL, 39); 				// 音量值

	// 声道微调：使用KCM_FL_TRIM等寄存器的记忆值
	gLocal_1 = 0;
	do {
		MKCM_WriteRegister(KCM_FL_TRIM + gLocal_1, 0);
	} while (++gLocal_1 < 8);

	MKCM_WriteRegister(KCM_SPK_CONFIG, 0xab);				// 后置大、环绕大、中置大、前置大、有超低音
	MKCM_WriteRegister(KCM_POWER_ON, KCM_SET_POWER_ON);		// 设置模块进入正常工作状态
    MDIP_MenuNormal(MENU_POWER_ON);                         // 菜单状态:电源打开
	gDIP_MenuLock = 30;										// 暂时锁定显示3秒
    return;
}
void MKCM_AppCommand(){
    BYTE inData[8];
    BYTE length = MKCM_ReadAutoByte(KCM_APP_COMMAND, inData, 8);
//	 MLOG("AppCommand %d", length);
    if (length == 2){
//        MKCM_WriteRegister(inData[0], inData[1]);
    	switch (inData[0]){
    	case KCM_VOLUME_CTRL:									// 音频静音及音量加减控制
            MLOG("KCM_VOLUME_CTRL %d", inData[1]);
            gAUD_MasterVolume = inData[1];
            MDIP_MenuNormal(MENU_MASTER_VOL);
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
    			gDIP_MenuSelect = MENU_MASTER_VOL;
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
CONST_CHAR Tab_ListenMode[] = {
    KCM_LISTEN_STEREO, 										// 双声道立体声，对应 LISTEN_MODE_HIFI
	KCM_LISTEN_STEREO | KCM_LISTEN_ENA_SW, 					// 双声道立体声+超低音，对应 LISTEN_MODE_2_1CH
	KCM_LISTEN_MULTI | KCM_LISTEN_ENA_SW, 					// 选择多声道源码模式，没有任何多声道算法，对应 LISTEN_MODE_SURROUND1
	KCM_LISTEN_SURROUND | KCM_LISTEN_ENA_SW | 0, 			// 选择多声道模式，算法0，对应 LISTEN_MODE_SURROUND2 
	KCM_LISTEN_SURROUND | 1, 								// 选择多声道模式，算法1，对应 LISTEN_MODE_SURROUND3
	KCM_LISTEN_SURROUND | KCM_LISTEN_ENA_SW | 1, 			// 选择多声道模式，算法1，对应 LISTEN_MODE_SURROUND3
	KCM_LISTEN_EFFECT | KCM_LISTEN_ENA_SW | 0, 				// 选择多声道音效，算法0，对应 LISTEN_MODE_SURROUND4
};						 
CONST_CHAR Tab_TestToneChannel[] = {
// 规范的通道顺序：FL FR CN SW SL SR BL BR
// 测试噪音的顺序：FL CN SW FR SR BR BL SL 					// 属于声音从喇叭分布的顺时针
	CHANNEL_FL, CHANNEL_CE, CHANNEL_SW, CHANNEL_FR, 		//	0,2,3,1, 5,7, 6,4,
	CHANNEL_SR, CHANNEL_BR, CHANNEL_BL, CHANNEL_SL, 
};

CONST_CHAR Tab_ChannelTrim[] = {
	0x10 | 9, 0x10 | 8, 0x10 | 7, 0x10 | 6, 0x10 | 5,       // -9 -8 -7 -6 -5 dB     
	0x10 | 4, 0x10 | 3, 0x10 | 2, 0x10 | 1, 0x00,		    // -4 -3 -2 -1 0  dB
	1,2,3,4,5,6,7,8,9, 								        // +1 ~ +9 dB
};  						 

BYTE MKCM_ToRegister(BYTE index, BYTE counter){				// 从本机处理的值，转换到KCM寄存器的值
	switch (index){
	case KCM_INPUT_SOURCE :									// 输入端口选择
    	return Tab_InputSwitch[counter];
	case KCM_LISTEN_MODE :									// 聆听模式选择
    	return Tab_ListenMode[counter];
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
		return INPUT_SWITCH_AUX;							// 超出范围，返回默认值AUX
	case KCM_LISTEN_MODE :									// 聆听模式选择
    	do {
			if (Tab_ListenMode[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_ListenMode));
		return KCM_LISTEN_STEREO;							// 超出范围，返回默认值双声道立体声
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
	}
	return 0;												// 超出范围，返回默认值0dB
}
void MAPI_COPY_BUFF8(BYTE length, BYTE* in_data, BYTE* out_data){
	for (; length != 0; length--){
        *out_data++ = *in_data++;
    };
    return;
}
