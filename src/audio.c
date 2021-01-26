
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           

void MAUD_Initialize(){										// ����ģ���ʼ��  	
}	  
void MAUD_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
    if (gRemoveTimer && ++gRemoveTimer > 200){			    // ��Լ2��� 
		INPUT_SWITCH select = MKCM_ReadRegister(KCM_EXTR_MEMORY + MEM_SOURCE_AUTO);	// �Զ�����Ļָ�
    	MLOG("gRemoveTimer %02x", select);
		gDIP_MenuSelect = MENU_RESTORE;				        // �˵����̽�������Ļָ� 
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
            value |= 0x02;                                  // B1Ϊ������Ƶ�ľ��� B0=0�����ر� 
        }
	}
	else {
		value = 0x20;
	}
	MKCM_WriteRegister(KCM_VOLUME_MUTE, value);	            // B5Ϊ���������ļӼ���B4Ϊ0��ʾ����ֵ��
//MDEBUG(0xb1);MDEBUG(value);
}
void MAUD_TestToneChannel(BYTE channel){					// ��������
	if (channel < 0xff){
		MKCM_WriteRegister(KCM_TEST_TONE, 0x10 | MKCM_ToRegister(KCM_TEST_TONE, channel));	// ��������ͨ��
//MDEBUG(0xb1);MDEBUG(0x10 | MKCM_ToRegister(KCM_TEST_TONE, channel));
	}
	else {									 				// �ر�
		MKCM_WriteRegister(KCM_TEST_TONE, 0);				// �������Թر�
	}
}
/*
void MAUD_MixSoundEffect(){
	MAUD_AutoCanclMute();
	MAUD_AutoCanclTestTone();
	if (++gDIP_SoundEffect > 4){
		gDIP_SoundEffect = 0;
	}
	MKCM_WriteRegister(KCM_EQ_SELECT, gDIP_SoundEffect);	// ѡ����Ҫ����Ч����ͨ��
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
void MAUD_AutoCanclTestTone(){
//MDEBUG(0xa7);
	if (FSYS_TestTone){
		FSYS_TestTone = 0;
		MKCM_WriteRegister(KCM_TEST_TONE, 0);				// �������Թر�
	}

}
void MAUD_AutoCanclMute(){
	if (FSYS_MuteEnable){
		FSYS_MuteEnable = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��Դ����ѡ����ռʽ����  //////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MAUD_InputSelect(INPUT_SWITCH select){
    DIP_SURROUND_OFF();
    DIP_SRC_FORMAT_OFF();
	MAUD_AutoCanclMute();
	MAUD_AutoCanclTestTone();
	
	if (gDIP_MenuSelect == MENU_RESTORE || gDIP_MenuSelect == MENU_INPUT_SOURCE){	// ֻ�в˵��Ѿ�����MENU_RESTORE�󣬲ſ��Ըı䵱ǰ������
//MLOG("MenuSelectA %02x", select);
		mINPUT_SWITCH = select;
		MAUD_InputWrite(mINPUT_SWITCH, mINPUT_SWITCH);
		if (mINPUT_SWITCH == INPUT_SWITCH_SD){
			MKCM_AutoTrack(KCM_PLAY_SD_QTY);					// ����KCM_PLAY_SD_QTY��������Բ��ŷ���1
		}else if (mINPUT_SWITCH == INPUT_SWITCH_UDISK){
			MKCM_AutoTrack(KCM_PLAY_UDISK_QTY);				// ����KCM_PLAY_UDISK_QTY��������Բ��ŷ���1
		}
	}
	MDIP_MenuNormal(MENU_INPUT_SOURCE);                    	// �˵�״̬:������Դѡ��
}
void MAUD_Preemptible(){									// ��ռʽ����ѡ�� 
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
	INPUT_SWITCH_AUX,                                   // 0=ģ������
    INPUT_SWITCH_OPTIC,                                 // 1=����1   
    INPUT_SWITCH_COA1,                                  // 2=����2   
    INPUT_SWITCH_COA2,                                  // 3=����3   
};
void MAUD_InputOneKey(){								// ����������һ������ѡ�� 
	gDIP_MenuSelect = MENU_RESTORE;					// �˵����̽�������Ļָ� 
	if (gWithHdmiQty && gWithHdmiStep < gWithHdmiQty){	 // ѭ����ʽ
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
 //   MLOG("SourceW:%02x(%02x)%02x(%02x)", select, MKCM_ToRegister(KCM_INPUT_SOURCE, select), last, MKCM_ToRegister(KCM_INPUT_SOURCE, last));
	if (last <= INPUT_SWITCH_COA2){ 								// ���ԭ��������ռʽ����
	    MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_SOURCE_AUTO, MKCM_ToRegister(KCM_INPUT_SOURCE, last));	// �Զ�����Ļָ�
		// MLOG("SourceJ:%02x last:%02x", select, MKCM_ReadRegister(KCM_INPUT_SOURCE));
	}
}
CONST_WORD TabSrcValid[] = {
	KCM_SRC_VALID_SD, KCM_SRC_VALID_UDISK,
	KCM_SRC_VALID_USBA, KCM_SRC_VALID_BT,
	KCM_SRC_VALID_HDMI1, KCM_SRC_VALID_HDMI2, KCM_SRC_VALID_HDMI3
	
}; 
CONST_CHAR TabValidSwitch[] = {
	INPUT_SWITCH_SD, INPUT_SWITCH_UDISK,
	INPUT_SWITCH_USBA, INPUT_SWITCH_BT,
	INPUT_SWITCH_HDMI1, INPUT_SWITCH_HDMI2, INPUT_SWITCH_HDMI3
};
void MAUD_MakePreemptible(WORD g2Local_1){			        // ������ռʽ����ѡ�� 
	BYTE qty = 0;
    if (gSYS_ModelType == KCM_MODEL_35H){                   // ģ��汾��KC35H����Ҫ��������7.1����
        gPreemptibleIn[qty++] = INPUT_SWITCH_E8CH;
    }    
    if (g2Local_1){									// ����ռʽ���� 
    	BYTE index;
    	WORD flag;
    	for (index = 0; index < sizeof(TabSrcValid)/2; index++){
        	flag = TabSrcValid[index];
        	if (g2Local_1 & flag){    				// �з���ռʽ
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
		if (gWithHdmiQty != qty){						// ��HDMI����ռʽ����  
			gWithHdmiStep = 0;
			gWithHdmiQty = qty;
		}
		for (index = 0; index < gWithHdmiQty; index++){
			if (gPreemptibleIn[index] >= INPUT_SWITCH_HDMI1 && gPreemptibleIn[index] <= INPUT_SWITCH_H_ARC){
				break;
			}
		}
		if (gPreemptibleQty != index){						// ����HDMI����ռʽ����  
			gPreemptibleStep = 0;
			gPreemptibleQty = qty;
		}
  	  	MLOG("KCM_SRC_VALID E %d %d", gPreemptibleQty, gWithHdmiQty);  
    }else {												// û������ռʽ����
		gPreemptibleQty = qty;
		gWithHdmiQty = 0; 
		gPreemptibleStep = 0;
		gWithHdmiStep = 0;
		g2SdQty = 0;
		g2UDiskQty = 0;
    }
    g2AUD_SrcValid = g2Local_1;
}


BYTE MKCM_AutoTrack(BYTE value){							// ����KCM_PLAY_SD_QTY��KCM_PLAY_UDISK_QTY��������Բ��ŷ���1
	WORD qty = MKCM_Read2Byte(value);
	if (value == KCM_PLAY_SD_QTY){
		g2SdQty = qty;
	}else{
		g2UDiskQty = qty;
	}
	if (qty){												// �в����ļ� 
		MKCM_Write2Byte(KCM_PLAY_INDEX, 0xffff);  			// �����ϴμ������Ŀ
		return 1;
	}
	return 0;
}

void MKCM_ReadSrcValid(){
	WORD g2Local_1 = MKCM_Read2Byte(KCM_SRC_VALID);         // ���ε���Ч��Դ
//if(g2Local_1==KCM_SRC_VALID_UDISK){g2Local_1=KCM_SRC_VALID_HDMI1|KCM_SRC_VALID_HDMI2;}
    if (g2AUD_SrcValid != g2Local_1){                       // �������ϴε���Ч��Դ�ı�
		BYTE index;
    	WORD flag;
    	BOOL found = 0;

        MLOG("KCM_SRC_VALID:%02x%02x Last:%02x%02x", g2Local_1>>8, g2Local_1, g2AUD_SrcValid>>8, g2AUD_SrcValid);

        for (index = 0; index < sizeof(TabSrcValid)/2; index++){
        	flag = TabSrcValid[index];
        	if ((g2Local_1 & flag) && !(g2AUD_SrcValid & flag)){    // �����м��ϴ�û�з���ռʽ
		       	found = 1;
	        	switch (flag){
	            case KCM_SRC_VALID_SD:
					if (MKCM_AutoTrack(KCM_PLAY_SD_QTY) == 0){
						MDIP_MenuNormal(MENU_SD_INSERT); 
		            	found = 0;							// û�в����ļ�
					}
					break;	
	            case KCM_SRC_VALID_UDISK: 
					if (MKCM_AutoTrack(KCM_PLAY_UDISK_QTY) == 0){
						MDIP_MenuNormal(MENU_UD_INSERT); 
		            	found = 0;							// û�в����ļ�
					}
					break;	
	            case INPUT_SWITCH_E8CH: 
	            	found = 0;
					break;	
      			}
      			if (found){
					MAUD_InputWrite(TabValidSwitch[index], mINPUT_SWITCH);
					mINPUT_SWITCH = TabValidSwitch[index];
MLOG("mINPUT_A %02x", mINPUT_SWITCH);
					gDIP_MenuSelect = MENU_RESTORE;				// �˵����̽�������Ļָ� 
					MDIP_MenuNormal(MENU_INPUT_SOURCE);     // �˵�״̬:������Դѡ��
					gRemoveTimer = 0;					// ȡ���γ��ָ�������ռʽ����
			    }
	        	break;									// ֻ����һ����ռʽ����
        	}
        }
        if (!found){									// û�з���ռʽ����
			for (index = 0; index < sizeof(TabSrcValid)/2; index++){
	        	flag = TabSrcValid[index];
	    //MLOG("KCM_SRC_VALID C %d %02x%02x", index, flag>>8, flag);  
				if (!(g2Local_1 & flag) && (g2AUD_SrcValid & flag)){    // ����û�м��ϴ��з���ռʽ
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
		            case KCM_SRC_VALID_HDMI1: 
		            case KCM_SRC_VALID_HDMI2: 
		            case KCM_SRC_VALID_HDMI3: 
			            break;
		            }
					if (mINPUT_SWITCH == TabValidSwitch[index]){	// ����ռʽ�γ������ǵ�ǰ������
						gRemoveTimer = 1;					// ��Լ2��� 
			        }					
		            break;									// ֻ����һ����ռʽ�γ�
	        	}
	        }
        }
		MAUD_MakePreemptible(g2Local_1);					// ������ռʽ����ѡ�� 
    }else {
        MLOG("KCM_SRC_VALID = Last:%02x%02x\r\n", g2Local_1>>8, g2Local_1);
    }
}

