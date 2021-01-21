
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           


#ifdef MICROPHONE
#define MAIN_EQ                         3                   // �����MIC����EQ��Ч���4��Ϊ��Ͳ����Ч
#else
#define MAIN_EQ                         4                   // û�д�MIC����EQ��Ч�����4��Ϊ��������Ч
#endif

CONST_CHAR TabEqSetup[5] = {                                // ��·����EQ��Ч�������ã�ע����ֽ�1��ʼ��
    0x03,                                                   // KCM_EQ_SETUP�ֽ�1 ǰ��ͨ��ʹ��EQ
    (0<<6) | (0<<5) | (5&0x1f),                             // ������ʹ��5��EQ �Զ���˥��ֵ�������оƬ
    (3<<6) | (0<<5) | (2&0x1f),                             // ��Ͳ����HSF LSF��2�� �Զ���˥��ֵ�������оƬ
    (6<<4) | 8,                                             // �˲���Qֵ ��Ͳ=6 ������=8 
    (9<<4) | 15,                                            // Ԥ˥��ֵ ��Ͳ�ߵ������Ϊ+9dB�����������Ϊ+15dB
}; 
CONST_CHAR TabEqFreq[9] = {                                 // ��·�ξ���EQ��Ч����Ƶ�ʣ�ע����ֽ�1��ʼ��
    0x00|6,  0x20|3,  0,                                    // 50Hz(50-20/5=6),             350Hz(300-150/50=3)
    0x20|17, 0x40|18, 0,                                    // 1000Hz(1000-150/50=17),      3300Hz(3300-1500/100=18)
    0x60|21, 0,       0,                                    // 15000Hz(15000-4500/500=21)
};     

CONST_CHAR TabEqLevel[4][9] = {                             // ��·�ξ���EQ��Ч�����ƽ��ע����ֽ�1��ʼ��
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

void MEQMIC_EqRestore(){									// EQ�ָ�����
	BYTE value;
    BYTE counter;
    BYTE select;
    BYTE temp[10];
    
    // ��·����EQ��Ч�����ʼ��
	for (select = 0; select < MAIN_EQ; select++){           // ��·����EQ��Ч����
        // ��·����EQ��Ч�������ã�һ��д��һ�����ڳ�ʼ��
        memcpy(&temp[1], TabEqSetup, sizeof(TabEqSetup));
        temp[0] = select + 1;
        MKCM_WriteXByte(KCM_EQ_SETUP, temp, 1 + sizeof(TabEqSetup));   // ��·����EQ��Ч��������

        // ��·����EQ��ЧƵ�ʣ�һ��д��һ�����ڳ�ʼ��
        memcpy(&temp[1], TabEqFreq, sizeof(TabEqFreq));
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
        if (++value > MAIN_EQ){                             // 1��3�ֱ�Ϊ3����EQ�����������4�����ڻ�Ͳ�ĸߵ�������
            value = 0;                                      // 0Ϊֹͣʹ����Ч
        }
        MKCM_WriteRegister(KCM_EQ_SELECT, value);
    }
    MDIP_EqSelect(value);
}
CONST_CHAR Tab_DIP_EqMode[] = {
	" FLAT  EQ   "
//	 ++++++------
};
void MDIP_EqSelect(BYTE value){                               // ��ʾEQ������ѡ��
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

CONST_CHAR TabMicAdjMax[5] = {                              // ��Ͳ���ֲ����������ֵ����KCM_MIC_ADJ_MAX
    // B7Ϊ�ӱ��ӳ�ʱ��ѡ�� B5:4��Ͳ�ϳɵ���ͨ��ѡ�� B3��Ͳ����֧��EQ ��B1:0Ϊ��Ͳ1��2�����ϳ�оƬ����
    (0<<7) | (0<<4) | (1<<3) | MIC_CHIP,                    // �ֽ�0Ϊ�ϳɼ�������ʽ����
    (9<<4) | 9,                                             // ��Ͳ1��2��������KCM_MIC_VOLUME�������ֵ
    (9<<4) | 0x0f,                                          // ��Ͳ�ӳ�ʱ�估���������������Ҫ100%��
    (0x0f<<4) | 0x0f,                                       // ��Ͳ�ظ���ֱ�������ֵ�������Ҫ100%��
    (0<<4) | 0,                                             // ��Ͳ����1������2����KCM_MIC_REVERB�������ֵ
};  

void MEQMIC_MicRestore(){									// ��Ͳ�ָ�����
	BYTE value;
    BYTE temp[5];

#ifdef MICROPHONE											// ����ͼ��뻰Ͳ����
    // MKCM_RestoreMemory ��KCM_EXTR_MEMORY����֮�лָ���Ͳ�͡�����	
	if (gDIP_MicTone[0] > 18){								// ��Ͳ����	-9��+9 ������Χ�Ĵ���
        gDIP_MicTone[0] = 9;                                // ����Ĭ��ֵ
	}
	if (gDIP_MicTone[1] > 18){								// ��Ͳ����	-9��+9 ������Χ�Ĵ���
        gDIP_MicTone[1] = 9;                                // ����Ĭ��ֵ
	}

	value = MKCM_ReadRegister(KCM_MIC_VOLUME);	            // ��Ͳ1����Ͳ2��������
	FMicTurnOn = value > 0 ? 1 : 0;                         // ��Ͳ�򿪱�־    
    gDIP_MicCtrl[0] = (value >> 4) & 0x0f;                  // B7:4Ϊ��Ͳ1����
	gDIP_MicCtrl[1] = value & 0x0f;                         // B3:0Ϊ��Ͳ2����
    if (gDIP_MicCtrl[0] > 9){                               // ������Χ�Ĵ���
        gDIP_MicCtrl[0] = 2;                                // ����Ĭ��ֵ
    }
    if (gDIP_MicCtrl[1] > 9){                               // ������Χ�Ĵ���
        gDIP_MicCtrl[1] = 2;                                // ����Ĭ��ֵ
    }
    MEQMIC_MicAutoMixer();                                  // ���ݻ�Ͳ1��2�������Զ����ڻ�Ͳ�������������ϳɱ���

    value = MKCM_ReadRegister(KCM_MIC_ECHO);	            // ��Ͳֱ��������������
                                                            // B7:4Ϊ��Ͳֱ�����ϳɵı���
	gDIP_MicCtrl[2] = value & 0x0f;                         // B3:0Ϊ��Ͳ��������
    if (gDIP_MicCtrl[2] > 9){                               // ������Χ�Ĵ���
        gDIP_MicCtrl[2] = 2;                                // ����Ĭ��ֵ
    }	

    value = MKCM_ReadRegister(KCM_MIC_DELAY);	            // ��Ͳ�ӳ�ʱ�估�ظ�����
	gDIP_MicCtrl[4] = (value >> 4) & 0x0f;	                // B7:4Ϊ��Ͳ�ӳ�ʱ�䣬��KCM_MIC_ADJ_MAX��Ӧ�����ֵ����
    if (gDIP_MicCtrl[4] > 9){                               // ������Χ�Ĵ���
        gDIP_MicCtrl[4] = 2;                                // ����Ĭ��ֵ
    }
	gDIP_MicCtrl[3] = value & 0x0f;                         // B3:0Ϊ��Ͳ�ظ�����
	 if (gDIP_MicCtrl[3] > 9){                              // ������Χ�Ĵ���
        gDIP_MicCtrl[3] = 2;                                // ����Ĭ��ֵ
    }

    MKCM_WriteXByte(KCM_MIC_ADJ_MAX, TabMicAdjMax, sizeof(TabMicAdjMax));   // ��Ͳ���ֲ������ֵ����
 
    temp[0] = 4;                                            // ��4��̶�Ϊ��Ͳ��EQԤ����Ч
    memcpy(&temp[1], TabEqSetup, sizeof(TabEqSetup));
    MKCM_WriteXByte(KCM_EQ_SETUP, temp, 1 + sizeof(TabEqSetup));   // ��·����EQ��Ч��������    
    MEQMIC_MicSetTone();                                    // д�뻰Ͳ��������KCM
#endif      // MICROPHONE ����ͼ��뻰Ͳ����
}
void MEQMIC_KeyCtrl(){										// EQ��MIC����CTRL���
#ifdef MICROPHONE											// ����ͽ�ң����������������+������-�ı�Ϊ��Ͳ����
	if (gDIP_MenuSelect < MENU_MIC_VOL1){	    		// ��û�н���Mic�˵� 
		MDIP_MenuNormal(MENU_MIC_VOL1);
	}else {
		if (++gDIP_MenuSelect > MENU_MIC_TREBLE){
			gDIP_MenuSelect = MENU_MIC_VOL1;
		}
		MDIP_MenuNormal(gDIP_MenuSelect);
	}
#endif			
}
void MEQMIC_KeyUp(){										// EQ��MIC��������+���
#ifdef MICROPHONE											// ����ͽ�ң����������������+������-�ı�Ϊ��Ͳ����
	if (gDIP_MenuSelect >= MENU_MIC_VOL1 && gDIP_MenuSelect <= MENU_MIC_TREBLE){	// �Ѿ�����Mic�˵� 
		gRmKeyContinCanclTm = 1;
		MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_UP);
	}
#endif			
}
void MEQMIC_KeyDown(){										// EQ��MIC��������-���
#ifdef MICROPHONE											// ����ͽ�ң����������������+������-�ı�Ϊ��Ͳ����
	if (gDIP_MenuSelect >= MENU_MIC_VOL1 && gDIP_MenuSelect <= MENU_MIC_TREBLE){	// �Ѿ�����Mic�˵� 
		gRmKeyContinCanclTm = 1;
		MDIP_MenuSelect(gDIP_MenuSelect, MENU_SET_ADJ_DOWN);
	}
#endif			
}


// ��Ͳ1���� ��Ͳ2���� ��Ͳ�������� ��Ͳ�ظ����� ��Ͳ�ӳ�ʱ�� ��Ͳ���� ��Ͳ����
CONST_CHAR Tab_DIP_MicShow[] = {
	"MIC1MIC2ECHOREPTDELYMBASMTRE"
//	 ++++----++++----++++----++++
};
void MEQMIC_MicDisplay(BYTE index, MENU_SET mode){          // ��ʾ��Ͳ���ֲ�������
    BYTE value;
    BYTE temp0;
	if (mode >= MENU_SET_ADJ_DOWN){							    // MENU_SET_ADJ_DOWN��MENU_SET_ADJ_UP 
		if (index == gDIP_MenuSelect){                      // �ϴ��Ѿ����뵱ǰ�˵�
			MEQMIC_MicKcm(index, (mode == MENU_SET_ADJ_UP) ? 1 : 0); 	// ��Ͳ���ֲ�������д�뵽KCM
			gDIP_MenuTimer = 80;
		}
	}
    temp0 = index - MENU_MIC_VOL1;                          // �ӻ�Ͳ1������ʼ
	MDIP_WriteString((char*)&Tab_DIP_MicShow[temp0 * 4]);
    MDIP_SingleChar(4, ' ');

	if (mode == MENU_SET_TWINKLE_OFF){							// MENU_SET_TWINKLE_OFF��˸Ϩ��
		MDIP_SingleChar(5, ' ');
	}
	else {											        // MENU_SET_NORMAL��MENU_SET_TWINKLE_ON��˸����
    	if (index < MENU_MIC_BASS){
            value = gDIP_MicCtrl[temp0];
            MDIP_SingleChar(5, value + '0');
        }else {
            MDIP_ShowNegative9(gDIP_MicTone[index - MENU_MIC_BASS]);    
        }
		if (mode == MENU_SET_NORMAL){                       // ��������ʾģʽ
			gDIP_MenuTimer = 80;
			FDIP_MenuTwinkle = 1;
		}
	}
}

#define MIC_BASS_300HZ                  (0x20 | 3)          // B6:5=1  B4:0=3*50+150=300Hz
#define MIC_TREBL_5000HZ                (0x60 | 1)          // B6:5=3  B4:0=1*500+4500=5000Hz
WORD makeEqValue(BYTE level, WORD freq){
    if (level < 9){                                         // ����
        freq |= 0x80;                                       // B7Ϊ1ʱB11:8��ֵΪ˥��-0��-15dB�� 
        freq |= (9 - level) << 8;                           // B11:8ΪdB����ֵ����������
    }else{                                                  // 0������
        freq |= (level - 9) << 8;                           // B11:8ΪdB����ֵ����������
    }
    return freq;
}
void MEQMIC_MicKcm(BYTE index, BYTE directUp){              // ��Ͳ���ֲ�������д�뵽KCM
	BYTE value;

	MAUD_AutoCanclMute();
    if (index < MENU_MIC_BASS){                             // 0��Ͳ1���� 1��Ͳ2  2���� 3�ظ� 4�ӳ�
        MDIP_SetUpDown(directUp, 9, &gDIP_MicCtrl[index - MENU_MIC_VOL1]);
        switch (index){                                     // 0��Ͳ1���� 1��Ͳ2  2���� 3�ظ� 4�ӳ� 5����
        case MENU_MIC_VOL1:                                 // ��Ͳ1����
        case MENU_MIC_VOL2:                                 // ��Ͳ2����
            MKCM_WriteRegister(KCM_MIC_VOLUME, (gDIP_MicCtrl[0]<<4) | gDIP_MicCtrl[1]);   // ��Ͳ1����Ͳ2��������
            MEQMIC_MicAutoMixer();
            break;
        case MENU_MIC_ECHO:                                 // ��Ͳ��������
            value = 8;                                      // ֱ���������̶�Ϊ8
            MKCM_WriteRegister(KCM_MIC_ECHO, (value<<4) | gDIP_MicCtrl[2]);   // ��Ͳ��������Ͳ���EQ������Ч����ѡ�����
            break;
        case MENU_MIC_DELAY:                                // ��Ͳ�ӳ�ʱ��
        case MENU_MIC_REPEAT:                               // ��Ͳ�ظ�����
            MKCM_WriteRegister(KCM_MIC_DELAY, (gDIP_MicCtrl[4]<<4) | gDIP_MicCtrl[3]);
        break;
            default:
        break;
        }
    }else{                                                  // ��Ͳ�����͡���������
        MDIP_SetUpDown(directUp, 18, &gDIP_MicTone[index - MENU_MIC_BASS]);
        MEQMIC_MicSetTone();
        if (index == MENU_MIC_BASS){                        // ���ı�ĵ���ֵд�����
            MKCM_WriteRegister(KCM_EXTR_MEMORY+MEM_MIC_BASS, gDIP_MicTone[0]);
        }else{                                              // ���ı�ĸ���ֵд�����
            MKCM_WriteRegister(KCM_EXTR_MEMORY+MEM_MIC_TREBLE, gDIP_MicTone[1]);      
        }
    }
}
void MEQMIC_MicSetTone(){                                   // д�뻰Ͳ��������KCM
    BYTE temp[4];
    WORD bass;
    WORD treble;
    bass = makeEqValue(gDIP_MicTone[0], MIC_BASS_300HZ);
    treble = makeEqValue(gDIP_MicTone[1], MIC_TREBL_5000HZ);
    
    temp[0] = 4;                                        // ��4��Ϊ��Ͳ����Ч
    // �ֽ�1��ʼÿ3���ֽ�Ϊ����12λ��EQ����ֵ
    temp[1] = bass & 0xff;                              // ��һ�ֽ�Ϊ��1�ε�8λ
    temp[2] = treble & 0xff;                            // �ڶ��ֽ�Ϊ��2�ε�8λ
    temp[3] = ((bass >> 8) & 0x0f) | ((treble >> 4) & 0xf0);  // �����ֽڵ�B3:0Ϊ��1�θ�4λ��B7:4Ϊ��2�θ�4λ
//MLOG("EQ_VALUE %02x %02x %02x", temp[1], temp[2], temp[3]);    
    MKCM_WriteXByte(KCM_EQ_VALUE, temp, 1 + 3);         // ��·����EQ��Ч��������
}

void MEQMIC_MicAutoMixer(){                                 // ���ݻ�Ͳ1��2�������Զ����ڻ�Ͳ�������������ϳɱ���
    if (gDIP_MicCtrl[0] < 7 && gDIP_MicCtrl[1] < 7){        // ��Ͳ1��2��������С��7
        MKCM_WriteRegister(KCM_MIC_MIXER, (8<<4) | 8);      // ��Ͳ�������������ϳɱ�����Ϊ50% (8��16)
    }else {
        BYTE maxVol = (gDIP_MicCtrl[0] > gDIP_MicCtrl[1]) ? gDIP_MicCtrl[0] : gDIP_MicCtrl[1];
        if (maxVol == 7){                                   // ���Ļ�ͲΪ7
            MKCM_WriteRegister(KCM_MIC_MIXER, (10<<4) | 6);  // ��Ͳ����������������
        }else if (maxVol == 8){                             // ���Ļ�ͲΪ8
            MKCM_WriteRegister(KCM_MIC_MIXER, (12<<4) | 4);  // ��Ͳ����������������
        }else {                                             // ���Ļ�ͲΪ9
            MKCM_WriteRegister(KCM_MIC_MIXER, (14<<4) | 2);  // ��Ͳ����������������
        }
    }
}
