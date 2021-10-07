
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#define ROOT     		  
#include "main.h"    


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
	if (channel == CHANNEL_SL){								// ����������
		MKCM_WriteRegister(KCM_SR_TRIM, value);				// ����������һ���޸�
	}else if (channel == CHANNEL_BL){						// ����������
		MKCM_WriteRegister(KCM_BR_TRIM, value);				// ����������һ���޸�
	}

//MLOG("MAUD_TrimAdjust B %d %d", MKCM_ReadRegister(KCM_CE_TRIM), MKCM_ReadRegister(KCM_SW_TRIM));	
}
void MDIP_TrimMenu(BYTE index, BYTE mode){					// mode 0һ��ģʽ 1��˸���� 2��˸Ϩ�� 3����- 4����+  
	BYTE temp0;
	BYTE value;
	temp0 = (index == MENU_TRIM_WOOFER) ? 2 : (index == MENU_TRIM_CENTER) ? 1 : (index == MENU_TRIM_SURROUND) ? 7 : 6;
	if (mode >= MENU_SET_ADJ_DOWN){							    // MENU_SET_ADJ_DOWN��MENU_SET_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // �ϴ��Ѿ����뵱ǰ�˵�
            MAUD_TrimAdjust(temp0, (mode == MENU_SET_ADJ_UP) ? 1 : 0); 	
			gDIP_MenuTimer = 80;
		}
	}
	MDIP_WriteString((char*)&Tab_TrimMenu[(index - MENU_TRIM_WOOFER) * 3]);
	MDIP_SingleChar(3, ' ');
	//  MLOG("MDIP_TrimMenu %d %d", index, mode);
	value = gDIP_TrimCtrl[temp0];			// 0=FL 1=CN 2=SW 3=FR 4=SR 5=BR 6=BL 7=SL
//MLOG("MDIP_TrimMenu A %d %d %d %d", index, temp0, value, gDIP_TrimCtrl[1]);
//if(mode!=MENU_SET_TWINKLE_ON&&mode!=MENU_SET_TWINKLE_OFF){MDEBUG(0xee);MDEBUG(index);MDEBUG(value);}
	if (mode == MENU_SET_TWINKLE_OFF){							// MENU_SET_TWINKLE_OFF��˸Ϩ��
		MDIP_SingleChar(4, ' ');
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_SET_NORMAL��MENU_SET_TWINKLE_ON��˸����

		if (value < 9){
			MDIP_SingleChar(4, '-');
			MDIP_SingleChar(5, (9 - value) + '0');
		}else {
			MDIP_SingleChar(4, ' ');
			MDIP_SingleChar(5, (value - 9) + '0');
		}
		if (mode == MENU_SET_NORMAL){                           // ��������ʾģʽ
			gDIP_MenuTimer = 80;
			FDIP_MenuTwinkle = 1;
		}
	}
}

void MDIP_MenuCustom(BYTE index, MENU_SET mode){
	switch (index){
    case MENU_TRIM_WOOFER :                                 // �˵�����΢������������
    case MENU_TRIM_CENTER :                                 // �˵�����΢����������
    case MENU_TRIM_SURROUND :                               // �˵�����΢����������
    case MENU_TRIM_BACK :                                   // �˵�����΢����������
        MAUD_AutoCanclTestTone();
		MDIP_TrimMenu(index, mode);
		break;
	}
}


void MA000_KEY_10msTimer(BYTE baseTimer){   				// B3=1000ms B2=500ms B1=100ms B0=10ms 
	MPKey_Scan();

	if (gDIP_MenuLock > 0){
		if (!FSYS_Standby){									// �����ڴ���ģʽ
			if ((baseTimer & 0x02) > 0){					// 100msʱ��
				--gDIP_MenuLock;
			}
		}
		return;	
	}
	if (FSYS_Standby){
		if (FRmDecodDone){
			FRmDecodDone = 0;
			if (GRmCodeData == cRmKey_Standby){
				MKCM_SetPowerOn();						// KCM����
			}
		}
	   	if (FPKeyDecodeDone){
			FPKeyDecodeDone = 0;
			FPKeyStep = 1;
			if (GPKeyData == cPanKey_InputSource){
				MKCM_SetPowerOn();						// KCM����
			}
		}
		return;
	}
   	if (FPKeyDecodeDone){
		FPKeyDecodeDone = 0;
		FPKeyStep = 1;

MLOG("FPKeyDecodeDone %02x", GPKeyData);		
		switch (GPKeyData){
  		case cPanKey_Standby:								// JOG
            break;
  		case cPanKey_JogMenu:								// JOG
			
			if (gDIP_MenuSelect < MENU_MASTER_VOL || gDIP_MenuSelect > MENU_MIC_VOL2){		// ��û�н���JOG�˵� 
				MDIP_MenuNormal(MENU_MASTER_VOL);
			}
			else {
				if (++gDIP_MenuSelect > MENU_MIC_VOL2){
					gDIP_MenuSelect = MENU_MASTER_VOL;
				}
				MDIP_MenuNormal(gDIP_MenuSelect);
			}
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
			FSYS_Standby = ~FSYS_Standby;
			if (FSYS_Standby){
				MDIP_MenuNormal(MENU_STANDBY);
			}
			else {
				MKCM_WriteRegister(KCM_POWER_ON, 1);
			    MDIP_MenuNormal(MENU_POWER_ON);             // �˵�״̬:��Դ��
				gDIP_MenuLock = 30;							// ��ʱ������ʾ3��
			}
			break;
		case cRmKey_FactorySet:
			MDIP_WriteString(" RESET");
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
			if (gDIP_MenuSelect >= MENU_TRIM_WOOFER && gDIP_MenuSelect <= MENU_TRIM_BACK){	// �Ѿ��������ò˵� 
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
			if (gDIP_MenuSelect >= MENU_TRIM_WOOFER && gDIP_MenuSelect <= MENU_TRIM_BACK){	// �Ѿ��������ò˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_DOWN);
			}
			break;
		case cRmKey_MeunRight:
			if (gDIP_MenuSelect >= MENU_TRIM_WOOFER && gDIP_MenuSelect <= MENU_TRIM_BACK){	// �Ѿ��������ò˵� 
				gRmKeyContinCanclTm = 1;
				MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_UP);
			}
			break;
		case cRmKey_PlayPause:								// ��������/��ͣ
			MKEY_PlayPause();
			break;
		case cRmKey_SkipDown:
			MDIP_PlaySkip(KCM_OPERATE_SKIP_DOWN);			// ��ý�岥�ź�һ��
			break;
		case cRmKey_SkipUp:
			MDIP_PlaySkip(KCM_OPERATE_SKIP_UP);			// ��ý�岥��ǰһ��
			break;
		case cRmKey_Surround:                               // ��廷��������
            MKEY_ListenMode(0);                             // ��������ģʽѡ��
			break;
		case cRmKey_InputOnKey:
			MAUD_InputOneKey();         					// ����������һ������ѡ��
			break;

		}
	}
	if (FKeyJogSwOK){
		FKeyJogSwOK = 0;
		if (gDIP_MenuSelect >= MENU_MASTER_VOL && gDIP_MenuSelect <= MENU_MIC_VOL2){		// �Ѿ�����JOG�˵� 
			MDIP_MenuSelect(gDIP_MenuSelect, FKeyJogUp ? MENU_SET_ADJ_UP : MENU_SET_ADJ_DOWN);
		}
		else {
			MDIP_MenuNormal(MENU_MASTER_VOL);
		}
	}
    return;
}	


////////////////////////////////////////////////////////////////////////////////////////////////////
void main(){
	MSYS_Initialize(); 										// CPU����������ʼ��
	MDIP_HalInit();											// ��ʾģ��Ӳ���ײ��ʼ��
	MDIP_BaseInit();										// ��ʾģ��������ʼ��
	MKEY_Initialize();       								// ����ģ���ʼ��
	MAUD_Initialize();       								// ��Ƶģ���ʼ��
    MKCM_Initialize();                                      // KCMģ���ʼ��
    while (1){ 					   
        if ((gTBascTimer & 0x01) > 0){						// 10ms (B3=1000ms B2=500ms B1=100ms B0=10ms) 
	        MKCM_10msTimer(gTBascTimer);                	// �Ӵ���ģ��10msʱ�䴦�� 		
			MA000_KEY_10msTimer(gTBascTimer);				// ����ģ��10msʱ�䴦��
            MDIP_10msTimer(gTBascTimer);  					// ��ʾģ��10msʱ�䴦��
            MAUD_10msTimer(gTBascTimer);                	// ��Ƶģ��10msʱ�䴦��
			if ((gTBascTimer & 0x08) > 0){					// 1000ms
				//   MLOG("gDIP_MenuSelect %02x %02x", gTemp0, gTemp1);
			}
			gTBascTimer = 0;
		}
    }
} 

void MSYS_Initialize(){
    TMOD = 0x21; 											// 8bit Auto Timer1 / 16bit Timer0
    TCON = 0x40 | 0x10 | 0x01;								// TR1 + TR0 + EX0�½��ش���
    SCON = 0x50;											// 8bit UART
	TH1 = 0xff;												// 115200bps UARTʹ��115200bpsͨѶ
	PCON  |= 0x80;											// UART����
    IE = 0x80 | 0x02 | 0x01;								// EA + ET0 + EX0
}
/*
void MDEBUG(BYTE gLocal_1){									// �������
//	TI = 0;
//	SBUF = gLocal_1;
//	while (!TI);
	return;
} 
*/
void MUSDELAY(BYTE gLocal_1){      
    if (gLocal_1 > 3){
        while (--gLocal_1 != 0);
    }
    return;
}



void MPKey_Scan(){
    BYTE gLocal_1;
    BYTE gLocal_2;

	gLocal_1 = 0;
	if(!HAL_IN_PKEY_0()) gLocal_1 |= 0x01;		

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
        if (++gKeyScan > 0){ 
            gKeyScan = 0;  
        }           
		FPKeyStep = 0; 
		gPKeyConfirmTm = 0; 
    }
    return;
}
void MKEY_CheckJop(){										// ��ת��ť ��ת
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

// ʱ���ж� ----------------------------------------------------------------
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
				HAL_DIP_LED(1);                             // ������ʾ���̶��ķ���
			}
			else {
				if ((gDIP_Brightness == 0) && (gBrightnessTimer == 1)){
					HAL_DIP_LED(0);                         // ����Ϊ0ʱϨ����ʾ���̶��ķ���
				}
				else if ((gDIP_Brightness == 1) && (gBrightnessTimer == 4)){
					HAL_DIP_LED(0);                         // ����Ϊ1ʱϨ����ʾ���̶��ķ���
				}else if ((gBrightnessTimer == 8)){
					HAL_DIP_LED(0);                         // ����Ϊ2ʱϨ����ʾ���̶��ķ���
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

// �����ж�0 ---------------------------------------------------------------
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

