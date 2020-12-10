
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           

void MAUD_Initialize(){										// ����ģ���ʼ��  	
}	  
void MAUD_10msTimer(BYTE baseTimer){   						// B3=1000ms B2=500ms B1=100ms B0=10ms 
    if (gRemoveTimer && ++gRemoveTimer > 200){			// ��Լ2��� 
		INPUT_SWITCH select = MKCM_ReadRegister(KCM_EXTR_MEMORY + MEM_SOURCE_AUTO);	// �Զ�����Ļָ�
    	MLOG("gRemoveTimer %d\r\n", (u32)select);
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
void MAUD_MixMicAdjust(BYTE index, BYTE directUp){
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
    switch (index){                                     // 0��Ͳ1���� 1��Ͳ2  2���� 3�ظ� 4�ӳ� 5����
    case cMenu_MicVolume1:                               // ��Ͳ1����
    case cMenu_MicVolume2:                               // ��Ͳ2����
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[0]);
        temp0 = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[1]);
        MKCM_WriteRegister(KCM_MIC_VOLUME, temp0 << 4 | value);   // ��Ͳ1����Ͳ2��������
        break;
    case cMenu_MicEcho:                                 // ��Ͳ��������
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[2]);
        temp0 = 0;                                  // ��Ͳ���EQ������Ч����ѡ�����
        MKCM_WriteRegister(KCM_MIC_ECHO_EQ, temp0 << 4 | value);   // ��Ͳ��������Ͳ���EQ������Ч����ѡ�����
        break;
    case cMenu_MicRepeat:                               // ��Ͳ�ظ�����
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[3]);
        temp0 = 8;                                  // ֱ���������̶�Ϊ8
        MKCM_WriteRegister(KCM_MIC_REPEAT, temp0 << 4 | value);
        break;
    case cMenu_MicReverb:                               // ��Ͳ�������
        value = MKCM_ToRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[5]);
        temp0 = 1;                                  // ��Ͳ����2�̶�Ϊ1
        MKCM_WriteRegister(KCM_MIC_REVERB, temp0 << 4 | value);
        break;
    case cMenu_MicDelay:                                // ��Ͳ�ӳ�ʱ�䣬ÿ��20����
        value = MKCM_ToRegister(KCM_MIC_DELAY, gDIP_MicCtrl[4]);    // 0-255ת����0-9
        MKCM_WriteRegister(KCM_MIC_DELAY, value);
        break;
    }
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
		mINPUT_SWITCH = select;
		MAUD_InputWrite(mINPUT_SWITCH, mINPUT_SWITCH);
	}
	MDIP_MenuNormal(MENU_INPUT_SOURCE);                    // �˵�״̬:������Դѡ��
}
void MAUD_Preemptible(){							// ��ռʽ����ѡ�� 
	MLOG("MKCM_Preemptible:%d %d/%d\r\n", (u32)gPreemptibleIn[gPreemptibleStep], (u32)gPreemptibleStep, (u32)gPreemptibleQty);
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
    MLOG("InputSource:%d last:%02x %02x\r\n", (u32)select, (u32)last, (u32)MKCM_ReadRegister(KCM_INPUT_SOURCE));
	if (last <= INPUT_SWITCH_COA2){ 								// ���ԭ��������ռʽ����
	    MKCM_WriteRegister(KCM_EXTR_MEMORY + MEM_SOURCE_AUTO, last);	// �Զ�����Ļָ�
		MLOG("InputSourceB:%d last:%02x\r\n", (u32)select, (u32)MKCM_ReadRegister(KCM_INPUT_SOURCE));
//		return;
	}
}
CONST_WORD TabSrcValid[] = {
	KCM_SRC_VALID_SD, KCM_SRC_VALID_UDISK,
	KCM_SRC_VALID_USBA, KCM_SRC_VALID_BT,
	KCM_SRC_VALID_HDMI1, KCM_SRC_VALID_HDMI2, KCM_SRC_VALID_HDMI3, 
	KCM_SRC_VALID_E8CH
}; 
CONST_CHAR TabValidSwitch[] = {
	INPUT_SWITCH_SD, INPUT_SWITCH_UDISK,
	INPUT_SWITCH_USBA, INPUT_SWITCH_BT,
	INPUT_SWITCH_HDMI1, INPUT_SWITCH_HDMI2, INPUT_SWITCH_HDMI3,
    INPUT_SWITCH_E8CH
};
void MAUD_MakePreemptible(WORD g2Local_1){			// ������ռʽ����ѡ�� 
    if (g2Local_1){									// ����ռʽ���� 
    	BYTE qty = 0;
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
  	  	MLOG("KCM_SRC_VALID E %d %d\r\n", (u32)gPreemptibleQty, (u32)gWithHdmiQty);  
    }else {												// û������ռʽ����
		gPreemptibleQty = 0;
		gWithHdmiQty = 0; 
		gPreemptibleStep = 0;
		gWithHdmiStep = 0;
		g2SdQty = 0;
		g2UDiskQty = 0;
    }
    g2AUD_SrcValid = g2Local_1;
}

void MKCM_ReadSrcValid(){
	WORD g2Local_1 = MKCM_Read2Byte(KCM_SRC_VALID);         // ���ε���Ч��Դ
//if(g2Local_1==KCM_SRC_VALID_UDISK){g2Local_1=KCM_SRC_VALID_HDMI1|KCM_SRC_VALID_HDMI2;}
    if (g2AUD_SrcValid != g2Local_1){                       // �������ϴε���Ч��Դ�ı�
		BYTE index;
    	WORD flag;
    	BOOL found = 0;

        MLOG("KCM_SRC_VALID:%04x Last:%04x\r\n", (u32)g2Local_1, (u32)g2AUD_SrcValid);

        for (index = 0; index < sizeof(TabSrcValid)/2; index++){
        	flag = TabSrcValid[index];
        	if ((g2Local_1 & flag) && !(g2AUD_SrcValid & flag)){    // �����м��ϴ�û�з���ռʽ
		       	found = 1;
	        	switch (flag){
	            case KCM_SRC_VALID_SD:
		            g2SdQty = MKCM_Read2Byte(KCM_PLAY_SD_QTY);
		            if (g2SdQty){								// �в����ļ� 
			            g2PlayIndex = 0;
	                	MKCM_Write2Byte(KCM_PLAY_INDEX, g2PlayIndex);  // ���ŵ�0��	         
            		}else {
		            	MDIP_MenuNormal(cMenu_SdInsert); 
		            	found = 0;								// û�в����ļ� 
		            }
					break;	
	            case KCM_SRC_VALID_UDISK: 
					g2UDiskQty = MKCM_Read2Byte(KCM_PLAY_UDISK_QTY);
					if (g2UDiskQty){								// �в����ļ� 
			            g2PlayIndex = 0;
	                	MKCM_Write2Byte(KCM_PLAY_INDEX, g2PlayIndex);  // ���ŵ�0��	         
            		}else {										// û�в����ļ� 
		            	MDIP_MenuNormal(cMenu_UDiskInsert); 
		            	found = 0;
		            }
					break;	
      			}
      			if (found){
					MAUD_InputWrite(TabValidSwitch[index], mINPUT_SWITCH);
					mINPUT_SWITCH = TabValidSwitch[index];
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
	    //MLOG("KCM_SRC_VALID C %d %04x\r\n", (u32)index, (u32)flag);  
				if (!(g2Local_1 & flag) && (g2AUD_SrcValid & flag)){    // ����û�м��ϴ��з���ռʽ
		            switch (flag){
		            case KCM_SRC_VALID_SD:
						g2SdQty = 0; 
						MDIP_PlaySymbol(0);
						MDIP_MenuNormal(cMenu_SdRemove); 
						break;	
		            case KCM_SRC_VALID_UDISK: 
						g2UDiskQty = 0;
						MDIP_PlaySymbol(0);
						MDIP_MenuNormal(cMenu_UDiskRemove); 
						break;	
		            case KCM_SRC_VALID_USBA: 
						MDIP_MenuNormal(cMenu_UsbaRemove); 
						break;	
		            case KCM_SRC_VALID_BT: 
						MDIP_MenuNormal(cMenu_BtRemove); 
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
        MLOG("KCM_SRC_VALID = Last:%04x\r\n", (u32)g2Local_1);
    }
}

