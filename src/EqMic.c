
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           


CONST_CHAR TabEqSetup[5] = {                                // ��·����EQ��Ч��������
    0x03, 0xc0 | 5, 0, 8,                                   // ǰ��ͨ��ʹ��EQ��5�μ�HSF&LSF��MICû����Ч��������Qֵ8
};     
CONST_CHAR TabEqFreq[9] = {                                 // ��·�ξ���EQ��Ч����Ƶ��
    0x00|6,  0x20|3,  0,                                    // 50Hz(50-20/5=6),             350Hz(300-150/50=3)
    0x20|17, 0x40|18, 0,                                    // 1000Hz(1000-150/50=17),      3300Hz(3300-1500/100=18)
    0x60|21, 0,       0,                                    // 15000Hz(15000-4500/500=21)
};     

CONST_CHAR TabEqLevel[4][9] = {                             // ��·�ξ���EQ��Ч�����ƽ
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

CONST_CHAR TabMicAdjMax[5] = {                              // ��Ͳ���ֲ����������ֵ����
    9,                                                      // ��Ͳ�ӳ�ʱ��KCM_MIC_DELAY�������ֵ
    9 << 4 | 9,                                             // ��Ͳ1��2��������KCM_MIC_VOLUME�������ֵ
    9,                                                      // ��������KCM_MIC_ECHO_EQ�������ֵ
    9 << 4 | 9,                                             // ��Ͳ�ظ���ֱ��������KCM_MIC_REPEAT�������ֵ
    9 << 4 | 9,                                             // ��Ͳ����1������2����KCM_MIC_REVERB�������ֵ
};  

void MEQMIC_Restore(){										// EQ����Ͳ�ָ�����
	BYTE value;
    BYTE counter;
    BYTE select;
    BYTE temp[10];

    MKCM_WriteXByte(KCM_MIC_ADJ_MAX, TabMicAdjMax, sizeof(TabMicAdjMax));   // ��Ͳ���ֲ������ֵ����

	value = MKCM_ReadRegister(KCM_MIC_VOLUME);	            // ��Ͳ1����Ͳ2��������
	gDIP_MicCtrl[0] = value & 0x0f;                         // B7:4Ϊ��Ͳ1����
	gDIP_MicCtrl[1] = (value >> 4) & 0x0f;                  // B3:0Ϊ��Ͳ2����
    if (gDIP_MicCtrl[0] > 9){                               // ������Χ�Ĵ���
        gDIP_MicCtrl[0] = 2;                                // ����Ĭ��ֵ
    }
    if (gDIP_MicCtrl[1] > 9){                               // ������Χ�Ĵ���
        gDIP_MicCtrl[1] = 2;                                // ����Ĭ��ֵ
    }

	value = MKCM_ReadRegister(KCM_MIC_ECHO_EQ);	            // ��Ͳ��������Ͳ���EQ������Ч����ѡ�����
                                                            // B7:4Ϊ��Ͳ���EQ������Ч����ѡ��1��EQ��Ч��0�ر���Ч
	gDIP_MicCtrl[2] = value & 0x0f;                         // B3:0Ϊ��������
    if (gDIP_MicCtrl[2] > 9){                               // ������Χ�Ĵ���
        gDIP_MicCtrl[2] = 2;                                // ����Ĭ��ֵ
    }	
	value = MKCM_ReadRegister(KCM_MIC_REPEAT);	            // ��Ͳ�ظ�����Ͳֱ��������
                                                            // B7:4Ϊ��Ͳֱ�����ϳɵı���
	gDIP_MicCtrl[3] = value & 0x0f;                         // B3:0Ϊ��Ͳ�ظ�����
	 if (gDIP_MicCtrl[3] > 9){                               // ������Χ�Ĵ���
        gDIP_MicCtrl[3] = 2;                                // ����Ĭ��ֵ
    }	
	value = MKCM_ReadRegister(KCM_MIC_DELAY);	            // ��Ͳ�ӳ�ʱ�� �������ֵ
                                                            // B7:4Ϊ����Ϊ0��
	gDIP_MicCtrl[4] = value;	                            // B3:0Ϊ��Ͳ�ӳ�ʱ�䡣����ӳ�ʱ��Ϊ1000����
	 if (gDIP_MicCtrl[4] > 9){                               // ������Χ�Ĵ���
        gDIP_MicCtrl[4] = 2;                                // ����Ĭ��ֵ
    }	

	value = MKCM_ReadRegister(KCM_MIC_REVERB);	            // ��Ͳ������� �������ֵ
	gDIP_MicCtrl[5] = MKCM_FromRegister(KCM_MIC_DELAY, value & 0x0f);   // 0-15ת����0-9
    if (gDIP_MicCtrl[5] > 9){                               // ������Χ�Ĵ���
        gDIP_MicCtrl[5] = 2;                                // ����Ĭ��ֵ
    }	

	for (select = 0; select < 4; select++){
        // ��·����EQ��Ч�������ã�һ��д��һ�����ڳ�ʼ��
        for (counter = 0; counter < sizeof(TabEqSetup); counter++){ 
            temp[counter + 1] = TabEqSetup[counter];
        }
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_SETUP, temp, 1 + sizeof(TabEqSetup));   // ��·����EQ��Ч��������

        // ��·����EQ��ЧƵ�ʣ�һ��д��һ�����ڳ�ʼ��
        for (counter = 0; counter < sizeof(TabEqFreq); counter++){ // д��EQƵ��
            temp[counter + 1] = TabEqFreq[counter];
        }
        for (counter = 0; counter < sizeof(TabEqFreq); counter++){ // д��EQƵ�ʵ�ƽ
            temp[counter + 1] |= TabEqLevel[select][counter];
        }
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_VALUE, temp, 1 + sizeof(TabEqFreq));   // ��·����EQ��Ч��������
    }

}

void MEQMIC_KeyEqSelect(){                                  // ����EQ������ѡ��
    BYTE value = MKCM_ReadRegister(KCM_EQ_SELECT);          // ��·������Ч����ѡ��
    if (gDIP_MenuSelect == MENU_EQ_SELECT){                 // ֻ�н����Ӧ�Ĳ˵��Ÿı�ģʽ
        if (++value > 4){                                   // 1��4�ֱ�Ϊ4��Ԥ����Ч�ߵ�����������EQ��������
            value = 0;                                      // 0Ϊֹͣʹ����Ч
        }
        MKCM_WriteRegister(KCM_EQ_SELECT, value);
    }
    MDIP_EqSelect(value);
}


void MEQMIC_KeyCtrl(){										// EQ��MIC����CTRL���
#ifdef MICROPHONE											// ����ͽ�ң����������������+������-�ı�Ϊ��Ͳ����
	if (gDIP_MenuSelect < cMenu_MicVolume1){	    		// ��û�н���Mic�˵� 
		MDIP_MenuNormal(cMenu_MicVolume1);
	}else {
		if (++gDIP_MenuSelect > cMenu_MicReverb){
			gDIP_MenuSelect = cMenu_MicVolume1;
		}
		MDIP_MenuNormal(gDIP_MenuSelect);
	}
#endif			
}
void MEQMIC_KeyUp(){										// EQ��MIC��������+���
#ifdef MICROPHONE											// ����ͽ�ң����������������+������-�ı�Ϊ��Ͳ����
	if (gDIP_MenuSelect >= cMenu_MicVolume1 && gDIP_MenuSelect <= cMenu_MicReverb){	// �Ѿ�����Mic�˵� 
		gRmKeyContinCanclTm = 1;
		MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_UP);
	}
#endif			
}
void MEQMIC_KeyDown(){										// EQ��MIC��������-���
#ifdef MICROPHONE											// ����ͽ�ң����������������+������-�ı�Ϊ��Ͳ����
	if (gDIP_MenuSelect >= cMenu_MicVolume1 && gDIP_MenuSelect <= cMenu_MicReverb){	// �Ѿ�����Mic�˵� 
		gRmKeyContinCanclTm = 1;
		MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_DOWN);
	}
#endif			
}


// ��Ͳ1���� ��Ͳ2���� ��Ͳ�������� ��Ͳ�ظ����� ��Ͳ�ӳ�ʱ�� ��Ͳ�������
CONST_CHAR Tab_DIP_MicShow[] = {
	"MIC1MIC2ECHOREPTDELYREVB"
//	 ++++----++++----++++----
};
void MEQMIC_MicDisplay(BYTE index, MENU_SET mode){          // ��ʾ��Ͳ���ֲ�������
//if(mode!=MENU_SET_TWINKLE_ON&&mode!=MENU_SET_TWINKLE_OFF){MDEBUG(0xee);MDEBUG(index);MDEBUG(mode);}
	if (mode >= MENU_SET_ADJ_DOWN){							    // MENU_SET_ADJ_DOWN��MENU_SET_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // �ϴ��Ѿ����뵱ǰ�˵�
			MEQMIC_MicKcm(index, (mode == MENU_SET_ADJ_UP) ? 1 : 0); 	// ��Ͳ���ֲ�������д�뵽KCM
			gDIP_MenuTimer = 80;
		}
	}
    index -= cMenu_MicVolume1;                              // �ӻ�Ͳ1������ʼ
	MDIP_WriteString((char*)&Tab_DIP_MicShow[index * 4]);
    MDIP_SingleChar(4, ' ');

	if (mode == MENU_SET_TWINKLE_OFF){							// MENU_SET_TWINKLE_OFF��˸Ϩ��
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_SET_NORMAL��MENU_SET_TWINKLE_ON��˸����
    	BYTE gLocal_1 = gDIP_MicCtrl[index];
        MDIP_SingleChar(5, gLocal_1 + '0');
		if (mode == MENU_SET_NORMAL){                       // ��������ʾģʽ
			gDIP_MenuTimer = 80;
			FDIP_MenuTwinkle = 1;
		}
       
	}
}
void MEQMIC_MicKcm(BYTE index, BYTE directUp){              // ��Ͳ���ֲ�������д�뵽KCM
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
    switch (index){                                         // 0��Ͳ1���� 1��Ͳ2  2���� 3�ظ� 4�ӳ� 5����
    case cMenu_MicVolume1:                                  // ��Ͳ1����
    case cMenu_MicVolume2:                                  // ��Ͳ2����
        MKCM_WriteRegister(KCM_MIC_VOLUME, gDIP_MicCtrl[0] << 4 | gDIP_MicCtrl[1]);   // ��Ͳ1����Ͳ2��������
        break;
    case cMenu_MicEcho:                                     // ��Ͳ��������
        temp0 = 0;                                          // ��Ͳ���EQ������Ч����ѡ�����
        MKCM_WriteRegister(KCM_MIC_ECHO_EQ, temp0 << 4 | gDIP_MicCtrl[2]);   // ��Ͳ��������Ͳ���EQ������Ч����ѡ�����
        break;
    case cMenu_MicRepeat:                                   // ��Ͳ�ظ�����
        temp0 = 8;                                          // ֱ���������̶�Ϊ8
        MKCM_WriteRegister(KCM_MIC_REPEAT, temp0 << 4 | gDIP_MicCtrl[3]);
        break;
    case cMenu_MicReverb:                                   // ��Ͳ�������
        temp0 = 1;                                          // ��Ͳ����2�̶�Ϊ1
        MKCM_WriteRegister(KCM_MIC_REVERB, temp0 << 4 | gDIP_MicCtrl[5]);
        break;
    case cMenu_MicDelay:                                    // ��Ͳ�ӳ�ʱ��
        MKCM_WriteRegister(KCM_MIC_DELAY, gDIP_MicCtrl[4]);
        break;
    }
}



/*
CONST_CHAR Tab_MicDelay[] = {                               // 0-255ת����0-9
    60/20,80/20,100/20,120/20,      160/20,200/20,300/20,500/20,    1000/20,2000/20,    // ��λ����    
};
CONST_CHAR Tab_Mic16To10[] = {                              // 0-15ת����0-9
    0,2,4,6,     8,10,12,13,    14,15,         
};
	case KCM_MIC_VOLUME :									// ��Ͳ����
	    return Tab_Mic16To10[counter];
	case KCM_MIC_DELAY :				                    // ��Ͳ�ӳ�ʱ��
	    return Tab_MicDelay[counter];

	case KCM_MIC_VOLUME :								    // 0��Ͳ1���� 1��Ͳ2  2���� 3�ظ�
		do {
			if (Tab_Mic16To10[gLocal_1] == value){          // 0-15ת����0-9
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_Mic16To10));
	    return 0;
    case KCM_MIC_DELAY:                                     // ��Ͳ�ӳ�ʱ��
		do {
			if (Tab_MicDelay[gLocal_1] == value){
				return gLocal_1;
			}
		} while (++gLocal_1 < sizeof(Tab_MicDelay));
        return 0;

*/
