
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#define ROOT     		  
#include "main.h"    

BYTE gDelayInput;
BYTE gA000Input;
//BOOL FStandbyBreak;
//BOOL FSetPowerOff;


CONST_CHAR Tab_InputTextA000[] = {
	"AUXOPTCO1CO2SD USBUSB   BT HD1HD2HD3"
//	 +++---+++---+++---+++---+++---+++---+++
};

void MDIP_CustomInput(){									// 用户定制音源输入显示
	if (FSYS_Standby){
		return;
	}
	g2DIP_ShowBuffer[6] &= ~(0x4000|0x0020);				// 熄灭云符号及:
	if (mINPUT_SWITCH >= INPUT_SWITCH_SD && mINPUT_SWITCH <= INPUT_SWITCH_BT){
		g2DIP_ShowBuffer[6] |= 0x0020;						// 点亮云符号
		if (mINPUT_SWITCH == INPUT_SWITCH_SD || mINPUT_SWITCH == INPUT_SWITCH_UDISK){
			if (g2PlayTime > 0){
				if (g2PlayTime < 3){
					MDIP_PlayTrack();
					return;
				}
				if (g2PlayTime < 5){
					 BYTE temp = gAUD_SrcFreq >> 3;
					if ((gAUD_SrcFormat & 0x0f) == KCM_SRC_MP3){
						gDIP_MenuTimer = 20;
						if (temp > 0 && temp < 16){
							BYTE gLocal_Buffer[6];
							MAPI_COPY_BUFF8(3, "MP3", gLocal_Buffer);
							MAPI_COPY_BUFF8(3, &Tab_DIP_Bps[(temp-1) * 3], &gLocal_Buffer[3]);
							MDIP_WrString((char*)gLocal_Buffer);
							return;
						}
					}
				}
				MDIP_WrString("  ");
				MDIP_Write2Digit(2, g2PlayTime/60);
				MDIP_Write2Digit(4, g2PlayTime%60);
				g2DIP_ShowBuffer[6] |= 0x4000;                          // 点亮:
				return;
			}
		}
	}
//MLOG("MDIP_CustomInput  %d %d", gA000Input, mINPUT_SWITCH);
	MDIP_WrText((char*)&Tab_InputTextA000[mINPUT_SWITCH * 3], 3);
	MDIP_SingleChar(3, ' ');
	MDIP_ShowVolume();										// 在4及5的位置显示音量的值
}

CONST_CHAR Tab_InputA000[] = {
    INPUT_SWITCH_HDMI1, INPUT_SWITCH_HDMI2, INPUT_SWITCH_HDMI3,
	INPUT_SWITCH_OPTIC, INPUT_SWITCH_COA1, INPUT_SWITCH_COA2, 
	INPUT_SWITCH_UDISK, INPUT_SWITCH_AUX, INPUT_SWITCH_BT
};
void MAUD_InputA000(){         								// 所有输入用一个按键选择
	gDIP_MenuSelect = MENU_RESTORE;							// 菜单即刻进入输入的恢复 
	MAUD_InputSelect(Tab_InputA000[gA000Input]);
}
void MAUD_InputSave(){         								// 所有输入用一个按键选择
	if (++gA000Input >= sizeof(Tab_InputA000)){
		gA000Input = 0;
	}
	MKCM_WriteRegister(KCM_EXTR_MEMORY + 4, gA000Input);
}

CONST_CHAR Tab_TrimMenu[] = {
	"SW CENSURBAC"
//	 +++---+++---
};

void MAUD_TrimAdjust(SPKER_CHANNEL channel, BYTE directUp){
    BYTE address;
    BYTE value;

	MAUD_AutoCanclMute();
	if (directUp){
		if (gDIP_TrimCtrl[channel] < 18){
			++gDIP_TrimCtrl[channel];
		}
	}
	else {
		if (gDIP_TrimCtrl[channel] > 0){
			--gDIP_TrimCtrl[channel];
		}
	}
    
    address = MKCM_ToRegister(KCM_TEST_TONE, channel);
    value = MKCM_ToRegister(KCM_FL_TRIM, gDIP_TrimCtrl[channel]);
//MLOG("MAUD_TrimAdjust A %d %02x %d", channel, KCM_FL_TRIM + address, value);
    MKCM_WriteRegister(KCM_FL_TRIM + address, value);
	if (channel == CHANNEL_SL){								// 环绕左声道
		MKCM_WriteRegister(KCM_SR_TRIM, value);				// 环绕右声道一起修改
	}else if (channel == CHANNEL_BL){						// 后置左声道
		MKCM_WriteRegister(KCM_BR_TRIM, value);				// 后置右声道一起修改
	}

//MLOG("MAUD_TrimAdjust B %d %d", MKCM_ReadRegister(KCM_CE_TRIM), MKCM_ReadRegister(KCM_SW_TRIM));	
}
void MDIP_TrimMenu(BYTE index, BYTE mode){					// mode 0一般模式 1闪烁点亮 2闪烁熄灭 3调整- 4调整+  
	BYTE temp0;
	BYTE value;
	temp0 = (index == MENU_TRIM_WOOFER) ? 2 : (index == MENU_TRIM_CENTER) ? 1 : (index == MENU_TRIM_SURROUND) ? 7 : 6;
	if (mode >= MENU_SET_ADJ_DOWN){							    // MENU_SET_ADJ_DOWN或MENU_SET_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // 上次已经进入当前菜单
            MAUD_TrimAdjust(temp0, (mode == MENU_SET_ADJ_UP) ? 1 : 0); 	
			gDIP_MenuTimer = 80;
		}
	}
	MDIP_WrString((char*)&Tab_TrimMenu[(index - MENU_TRIM_WOOFER) * 3]);
	MDIP_SingleChar(3, ' ');
	//  MLOG("MDIP_TrimMenu %d %d", index, mode);
	value = gDIP_TrimCtrl[temp0];			// 0=FL 1=CN 2=SW 3=FR 4=SR 5=BR 6=BL 7=SL
//MLOG("MDIP_TrimMenu A %d %d %d %d", index, temp0, value, gDIP_TrimCtrl[1]);
//if(mode!=MENU_SET_TWINKLE_ON&&mode!=MENU_SET_TWINKLE_OFF){MDEBUG(0xee);MDEBUG(index);MDEBUG(value);}
	if (mode == MENU_SET_TWINKLE_OFF){							// MENU_SET_TWINKLE_OFF闪烁熄灭
		MDIP_SingleChar(4, ' ');
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_SET_NORMAL或MENU_SET_TWINKLE_ON闪烁点亮

		if (value < 9){
			MDIP_SingleChar(4, '-');
			MDIP_SingleChar(5, (9 - value) + '0');
		}else {
			MDIP_SingleChar(4, ' ');
			MDIP_SingleChar(5, (value - 9) + '0');
		}
		if (mode == MENU_SET_NORMAL){                           // 正常的显示模式
			gDIP_MenuTimer = 80;
			FDIP_MenuTwinkle = 1;
		}
	}
}

void MDIP_MenuCustom(BYTE index, MENU_SET mode){
	switch (index){
    case MENU_TRIM_WOOFER :                                 // 菜单声道微调超低音声道
    case MENU_TRIM_CENTER :                                 // 菜单声道微调中置声道
    case MENU_TRIM_SURROUND :                               // 菜单声道微调环绕声道
    case MENU_TRIM_BACK :                                   // 菜单声道微调后置声道
        MAUD_AutoCanclTestTone();
		MDIP_TrimMenu(index, mode);
		break;
	}
}
void MA000_Standby(){
	FSYS_Standby = 1;
	MDIP_MenuNormal(MENU_STANDBY);
	MDIP_WrString("      ");
	P00 = 1;									// 单片机39脚(P00)输出高电平驱动LED红色灯点亮
	P01 = 0;									// 单片机38脚(P01)输出高电平驱动LED红色灯灭
	DIP_SURROUND_OFF();
	DIP_SRC_FORMAT_OFF();
	DIP_DOT_OFF();
	DIP_PLAY_OFF();

	MKCM_WriteRegister(KCM_POWER_ON, KCM_SET_STANDBY);	// 设置模块进入待机状态
}
void MA000_SetPowerOn(){
	P00 = 0;									// 单片机39脚(P00)输出高电平驱动LED红色灯灭
	P01 = 1;									// 单片机38脚(P01)输出高电平驱动LED红色灯点亮
	FSYS_Standby = 0;
	MKCM_SetPowerOn();							// KCM开机
//	gDelayInput = gDIP_MenuTimer;
// MLOG("SetPowerOn %d", gA000Input);
}
void MA000_KEY_10msTimer(BYTE baseTimer){   				// B3=1000ms B2=500ms B1=100ms B0=10ms 
	MPKey_Scan();
	if (FSYS_Standby){
		if (FRmDecodDone){
			FRmDecodDone = 0;
			if (GRmCodeData == cRmKey_Standby){
				MA000_SetPowerOn();
			}
		}
	   	if (FPKeyDecodeDone){
			FPKeyDecodeDone = 0;
			FPKeyStep = 1;
			if (GPKeyData == cPanKey_InputSource){
				MA000_SetPowerOn();	
			}
		}
		return;
	}
   	if (FPKeyDecodeDone){
		FPKeyDecodeDone = 0;
		FPKeyStep = 1;

//MLOG("FPKeyDecodeDone %02x", GPKeyData);		
		switch (GPKeyData){
  		case cPanKey_Standby:								// JOG
		 	MA000_Standby();	
            break;
  		case cPanKey_JogMenu:								// JOG			
			MAUD_InputSave();
			MAUD_InputA000();         						// 所有输入用一个按键选择
			break;
		}
	}
    
    if ((gRmKeyContinCanclTm != 0) && (++gRmKeyContinCanclTm > 60)){	
        gRmKeyContinCanclTm = 0;
		gRmKeyContinSpeed = 0;
    }
	if (FRmDecodDone){
		BYTE gLocal_1;
		FRmDecodDone = 0;
//MLOG("FRmDecodDone %02x ", GRmCodeData);	
		switch (GRmCodeData){
		case cRmKey_Standby:
			MA000_Standby();
			break;
		case cRmKey_FactorySet:
			MDIP_WrString(" RESET");
			MDIP_ScreenUpdata();
			MKCM_FactorySet();
			break;
		case cRmKey_VolumeUp:
			gRmKeyContinCanclTm = 1;
			gLocal_1 = (gDIP_MenuSelect == MENU_MASTER_VOL) ? MENU_SET_ADJ_UP : MENU_SET_NORMAL;
			MDIP_MenuSelect(MENU_MASTER_VOL, gLocal_1);
			break;
		case cRmKey_VolumeDown:
			if (!FSYS_MuteEnable){
				gRmKeyContinCanclTm = 1;
				gLocal_1 = (gDIP_MenuSelect == MENU_MASTER_VOL) ? MENU_SET_ADJ_DOWN : MENU_SET_NORMAL;
				MDIP_MenuSelect(MENU_MASTER_VOL, gLocal_1);
			}
			break;
		case cRmKey_AudioMute:
    		MKEY_AudioMute();
			break;
		case cRmKey_MeunCtrl:
			if (gDIP_MenuSelect >= MENU_TRIM_WOOFER && gDIP_MenuSelect <= MENU_TRIM_BACK){	// 已经进入设置菜单 
			//	MDIP_MenuNormal(MENU_RESTORE);
				++gDIP_MenuSelect;
				if (gDIP_MenuSelect > MENU_TRIM_BACK){
					gDIP_MenuSelect = MENU_TRIM_WOOFER;
				}
				MDIP_MenuNormal(gDIP_MenuSelect);
				gDIP_MenuTimer = 50;
			}
			else {
				MDIP_MenuNormal(MENU_TRIM_WOOFER);
			}
			break;
		case cRmKey_MeunLeft:
			if (gDIP_MenuSelect >= MENU_TRIM_WOOFER && gDIP_MenuSelect <= MENU_TRIM_BACK){	// 已经进入设置菜单 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_DOWN);
			}
			break;
		case cRmKey_MeunRight:
			if (gDIP_MenuSelect >= MENU_TRIM_WOOFER && gDIP_MenuSelect <= MENU_TRIM_BACK){	// 已经进入设置菜单 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_UP);
			}
			break;
		case cRmKey_PlayPause:								// 按键播放/暂停
			MKEY_PlayPause();
			break;
		case cRmKey_SkipDown:
			MDIP_PlaySkip(KCM_OPERATE_SKIP_DOWN);			// 多媒体播放后一首
			break;
		case cRmKey_SkipUp:
			MDIP_PlaySkip(KCM_OPERATE_SKIP_UP);				// 多媒体播放前一首
			break;
		case cRmKey_Surround:                               // 面板环绕声按键
            MKEY_ListenMode(0);                             // 按键聆听模式选择
			break;
		case cRmKey_InputOnKey:
			MAUD_InputSave();
			MAUD_InputA000();         						// 所有输入用一个按键选择
			break;

		}
	}
	if (FKeyJogSwOK){
		FKeyJogSwOK = 0;
		if (gDIP_MenuSelect >= MENU_MASTER_VOL && gDIP_MenuSelect <= MENU_MIC_VOL2){		// 已经进入JOG菜单 
			MDIP_MenuSelect(gDIP_MenuSelect, FKeyJogUp ? MENU_SET_ADJ_UP : MENU_SET_ADJ_DOWN);
		}
		else {
			MDIP_MenuNormal(MENU_MASTER_VOL);
		}
	}
    return;
}	
void MA000_KCM_10msTimer(BYTE baseTimer){   				// B3=1000ms B2=500ms B1=100ms B0=10ms 
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
// MLOG("FSYS_Standby %D", FSYS_Standby?1:0);
		}
		if ((gLocal_1 & KCM_IRQ_SYSTEM_INIT) > 0){			// KCM模式初始化完成中断
            if (!FSYS_Standby){
				MKCM_RestoreMemory();						// 从KCM恢复本地的记忆
				gA000Input = MKCM_ReadRegister(KCM_EXTR_MEMORY + 4);	// 恢复输入选择
				MAUD_InputA000();
			}else{
				MKCM_WriteRegister(KCM_POWER_ON, KCM_SET_STANDBY);	// 设置模块进入待机状态
				gLocal_1 = 0;								//后面的都不再处理
			}
		}
		if ((gLocal_1 & KCM_IRQ_FORMAT_INFO) > 0){          // 数码信号输入格式改变中断，需要读取"KCM_SRC_FORMAT"寄存器
			gAUD_SrcFormat = MKCM_ReadRegister(KCM_SRC_FORMAT);
			//MLOG("gAUD_SrcFormat %02x", gAUD_SrcFormat);
            gAUD_SrcFreq = MKCM_ReadRegister(KCM_SRC_FREQ);
			if (mINPUT_SWITCH == INPUT_SWITCH_SD || mINPUT_SWITCH == INPUT_SWITCH_UDISK){
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
		// if ((gLocal_1 & KCM_IRQ_SRC_VALID) > 0){            	// 有效的音源输入改变中断，需要读取"KCM_SRC_VALID"寄存器
        //     MKCM_ReadSrcValid();
		// }
		if ((gLocal_1 & KCM_IRQ_VOLUME) > 0){               	// 音量调节改变中断，需要读取"KCM_VOLUME_CTRL"寄存器获取更新的音量值
			gAUD_MasterVolume = MKCM_ReadRegister(KCM_VOLUME_CTRL);     // 读取当前音量值
			if (gDIP_MenuSelect == MENU_MASTER_VOL){
				MDIP_MenuNormal(MENU_MASTER_VOL);
			}
		}
        if ((gLocal_1 & KCM_IRQ_FIRMWARE) > 0){             // 固件更新，需要读取"KCM_RD_INFO"寄存器
            MDIP_FirewareInfo();                            // 显示固件更新
//            MDIP_MenuNormal(cMenu_Fireware);
        }
        if ((gLocal_1 & KCM_IRQ_PLAY_TIME) > 0){           	// 多媒体播放时间改变
            if (gDIP_MenuSelect == MENU_RESTORE || gDIP_MenuSelect == MENU_INPUT_SOURCE){	// 只有在菜单恢复或输入音源选择状态
				g2PlayTime = MKCM_Read2Byte(KCM_PLAY_TIME);
				MDIP_InputSource();
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


////////////////////////////////////////////////////////////////////////////////////////////////////
void main(){
	MSYS_Initialize(); 										// CPU基本部件初始化
	MDIP_HalInit();											// 显示模块硬件底层初始化	
	MDIP_BaseInit();										// 显示模块基础层初始化
	MAUD_Initialize();       								// 音频模块初始化
    // MKCM_Initialize();                                      // KCM模块初始化
	gDIP_Brightness = 0;
	MA000_Standby();
	
	while (1){ 					   
        if ((gTBascTimer & 0x01) > 0){						// 10ms (B3=1000ms B2=500ms B1=100ms B0=10ms) 
	        MA000_KCM_10msTimer(gTBascTimer);               // 子处理模块10ms时间处理 		
			MA000_KEY_10msTimer(gTBascTimer);				// 按键模块10ms时间处理           
			MDIP_10msTimer(gTBascTimer);  					// 显示模块10ms时间处理
           	MAUD_10msTimer(gTBascTimer);                	// 音频模块10ms时间处理
			if ((gTBascTimer & 0x08) > 0){					// 1000ms
			//	 MLOG("gDIP_MenuSelect %d %d", FSYS_Standby?1:0, gDIP_MenuLock);
			}
			if ((gTBascTimer & 0x02) > 0){					// 100ms时间
			//	if (gDelayInput > 0 && --gDelayInput == 0){
					// MLOG("gDelayInput B %d", gA000Input);
		//			MAUD_InputA000();
			//	}
			}			
			gTBascTimer = 0;
		}
    }
} 

void MSYS_Initialize(){
    TMOD = 0x21; 											// 8bit Auto Timer1 / 16bit Timer0
    TCON = 0x40 | 0x10 | 0x01;								// TR1 + TR0 + EX0下降沿触发
    SCON = 0x50;											// 8bit UART
	TH1 = 0xff;												// 115200bps UART使用115200bps通讯
	PCON  |= 0x80;											// UART倍速
    IE = 0x80 | 0x02 | 0x01;								// EA + ET0 + EX0
}


void MUSDELAY(BYTE gLocal_1){      
    if (gLocal_1 > 3){
        while (--gLocal_1 != 0);
    }
    return;
}

void MPKey_Scan(){
    BYTE gLocal_1;
    BYTE gLocal_2;

	if(gKeyScan == 0){
		HAL_PKEY_0(0);
		HAL_PKEY_1(1);
		HAL_PKEY_2(1);
	}		
	else if(gKeyScan == 1){
		HAL_PKEY_0(1);
		HAL_PKEY_1(0);
		HAL_PKEY_2(1);
	}		
	else {
		HAL_PKEY_0(1);
		HAL_PKEY_1(1);
		HAL_PKEY_2(0);
	}

	gLocal_1 = 0;
	// if(!HAL_IN_PKEY_0()) gLocal_1 |= 0x01;		
	// if(!HAL_IN_PKEY_1()) gLocal_1 |= 0x02;		
	// if(!HAL_IN_PKEY_2()) gLocal_1 |= 0x04;		
	if(!HAL_IN_PKEY_3()) gLocal_1 |= 0x08;					// 只用到一个按键，接在P05及P21

    if (gLocal_1){
        gLocal_1 = (gKeyScan << 4) + gLocal_1;    
        if (GPKeyData == gLocal_1){
            if (++gPKeyConfirmTm > 8){ 
		        if (!FPKeyStep){
	                FPKeyDecodeDone = 1;
	            }
				gPKeyConfirmTm = 0;
            }
        }
		else {
			FPKeyStep = 0; 
			gPKeyConfirmTm = 0; 
			GPKeyData = gLocal_1;
		}
    }
    else{ 
        if (++gKeyScan > 2){ 
            gKeyScan = 0;  
        }           
		FPKeyStep = 0; 
		gPKeyConfirmTm = 0; 
    }
    return;
}

void MKEY_CheckJop(){										// 旋转按钮 旋转
    if (!FKeyJopInputEnl){
        if (HAL_IN_JOP0() && HAL_IN_JOP1() && (++gKeyJopInputTm > 3)){
            gKeyJopInputTm = 0; 
            FKeyJopInputEnl = 1;
        }
        FKeyJopSta00 = 0;
        FKeyJopSta01 = 0;
    }                 
    else if (HAL_IN_JOP0() != HAL_IN_JOP1()){
        if (FKeyJopInputEnl){
            FKeyJogUp = HAL_IN_JOP1();
            if ((FKeyJogUp == FKeyJogUpSave) || (gKeyJopLockTm == 0)){
                FKeyJogSwOK = 1;
            }
            FKeyJogUpSave = FKeyJogUp;
            gKeyJopLockTm = 3;
            gKeyJopInputTm = 0;
            FKeyJopInputEnl = 0;
            FKeyJopSta01 = 0;
        }
        else {
            if (FKeyJopSta01){
                gKeyJopLockTm = 3;
                gKeyJopInputTm = 0;                          
                FKeyJopInputEnl = 0;
            }
            FKeyJopSta01 = 1;
        }                                
        FKeyJopSta00 = 0;
    }
    else if (!HAL_IN_JOP0() && !HAL_IN_JOP1()){ 
        if (FKeyJopSta00){
            gKeyJopLockTm = 3;
            gKeyJopInputTm = 0; 
            FKeyJopInputEnl = 0;
        }
        FKeyJopSta00 = 1;
        FKeyJopSta01 = 0;
    }
    return;
}

// 时间中断 ----------------------------------------------------------------
void TBascTimer(void) interrupt 1 {
    TL0 = (BYTE)(cSYS_TIMER0);                              // 100us
    TH0 = (BYTE)(cSYS_TIMER0 >> 8);

//P34=~P34;
	if ((++g10msTimer % 10) == 0){							// 1ms
		MKEY_CheckJop();
		if ((g10msTimer % 100) == 0){						// 10ms
			g10msTimer = 0;
		    gTBascTimer = 0x01;
			if ((++g1000msTimer % 10) == 0){				// 100ms
				gTBascTimer |= 0x02;
				if ((g1000msTimer % 50) == 0){				// 500ms
					gTBascTimer |= 0x04;
					if ((g1000msTimer % 100) == 0){			// 1000ms
						g1000msTimer = 0;	
						gTBascTimer |= 0x08;
					}
				}
			}
		}
		if (FDIP_FreqSymbol){
			if (++gBrightnessTimer > 19){
				gBrightnessTimer = 0;
				HAL_DIP_LED(1);                             // 点亮显示屏固定的符号
			}
			else {
				if ((gDIP_Brightness == 0) && (gBrightnessTimer == 1)){
					HAL_DIP_LED(0);                         // 亮度为0时熄灭显示屏固定的符号
				}
				else if ((gDIP_Brightness == 1) && (gBrightnessTimer == 4)){
					HAL_DIP_LED(0);                         // 亮度为1时熄灭显示屏固定的符号
				}else if ((gBrightnessTimer == 8)){
					HAL_DIP_LED(0);                         // 亮度为2时熄灭显示屏固定的符号
				}
			}
		}
	}

    if (gRmTimer != 0 && ++gRmTimer > 159){
        if(!FRmError && (gRmStatus == 2) && (gRmKeyContinCanclTm != 0)){
            if (gRmKeyContinSpeed < 8){
				if ((gRmKeyContinSpeed & 3) == 3){
					FRmDecodDone = 1;
				}
				++gRmKeyContinSpeed;
			}
			else {
	            FRmDecodDone = 1;
			}
        } 
        gRmStatus = 0;
        FRmError = 0; 
        gRmTimer = 0;
    }
    return;
}

// 外置中断0 ---------------------------------------------------------------
void EX0_int (void) interrupt 0 {		  
    if (!FRmError){
        ++gRmStatus;                                    
        if (gRmStatus == 2){    
            if (gRmTimer > 145) FRmError = 1; 
        }
        else{
            gRmBuffer >>= 1;
            if (gRmTimer > 16) gRmBuffer |= 0x80;   
            if ((gRmStatus == 2+8) || (gRmStatus == 2+24)){ 
                 GRmCodeData = gRmBuffer;
            }
            else if (gRmStatus == 2+16){
                if ((gRmBuffer != cRmNCustom) || (GRmCodeData != cRmCustom)){
                     gRmKeyContinCanclTm = 0;
                     FRmError = 1;
                } 
//                gTemp0=gRmBuffer;gTemp1=GRmCodeData;				
            }
            else if (gRmStatus == 2+32){ 
                 if ((gRmBuffer ^ 0xff) == GRmCodeData){
                    FRmDecodDone = 1;
                    gRmKeyContinSpeed = 0;
                 }else{
                    FRmDecodDone = 0; 
                    FRmError = 1;
                 }
            }
        }
    }
    gRmTimer = 1; 
    return;
}		 

