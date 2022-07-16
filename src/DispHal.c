
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           

void MDIP_HalInit(){										// ��ʾģ��Ӳ���ײ��ʼ��									  
    BYTE gLocal_0;

	HAL_DIP_LED(0);                                         // Ϩ����ʾ���̶��ķ���
    HAL_DIP_STB(0);											// TA6932Ƭѡ
    MUSDELAY(1);
	MDIP_WriteByte(0x40);		  							// ��ʼ���ϵ�ʱ����ַ������1
    MUSDELAY(1);
    HAL_DIP_STB(1);    

    MUSDELAY(1);
    HAL_DIP_STB(0);
    MUSDELAY(1);
	MDIP_WriteByte(0xc0);		  							// �����ַ 
    MUSDELAY(1);
	gLocal_0 = 0;
	do{
		MDIP_WriteByte(0x00);		  						// ����1
	    MUSDELAY(1);
	}while(++gLocal_0 < 16);
	MUSDELAY(10);
    HAL_DIP_STB(1);    
}	  

CONST_CHAR Tab_DIP_Brightness[] = {
	0x00,0x02,0x07,  
};
void MDIP_Brightness(BYTE show, BYTE bright){
	if (show > 0){
		MDIP_WrString("BRIGH");
		MDIP_SingleChar(5, bright + '1');
	}

    HAL_DIP_STB(0);
    MUSDELAY(1);
	MDIP_WriteByte(0x88 | Tab_DIP_Brightness[bright]);		  					// ��ʾ������
    MUSDELAY(1);
    HAL_DIP_STB(1);    
}




////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MDIP_SurroundSymbol(){
}
void MDIP_SrcFormatSymbol(){
	DIP_SRC_FORMAT_OFF();
	//g2DIP_ShowBuffer[6] &= ~(0x0700 | 0x0004);	
	// ��[6] B15:D1 B14:D1 B13:D1 B12:D1 B11:WIFI B10:LPCM B9:HD    B8:DTS  B7:PROLOGIC B6:NEO6  B5:��  B4:DSP B3:ATMOS B2:DD  B1:AUTO B0:ST
	// ��[6] B15:D1 B14:D1 B13:D1 B12:D1 B11:WIFI B10:��   B9:PAUSE B8:PLAY B7:RPALL    B6:RP1   B5:DTS B4:DD  B3:AUTO  B2:DSP B1:PLII B0:ST
	
//MDEBUG(0xa9);MDEBUG(gSYS_ModelType);MDEBUG(gAUD_SrcFormat);
	switch (gAUD_SrcFormat){
	case KCM_SRC_PCM :
		if (gSYS_ModelType != KCM_MODEL_35H && gSYS_ModelType != KCM_MODEL_36H){
			g2DIP_ShowBuffer[6] |= 0x0400;	
		}
		break;
   	case KCM_SRC_TRUEHD :                               	// �ű�TRUE HD�ź�����
		g2DIP_ShowBuffer[6] |= 0x0200;						// B9:HD 
   	case KCM_SRC_EAC3 :                                		// Enhanced AC-3�ź�����
   	case KCM_SRC_AC3 :                                  	// ��׼��AC3�ź�����
		g2DIP_ShowBuffer[6] |= 0x0004;	
		break;
	case KCM_SRC_DTS_HRA:				                	// DTS HD High Resolution Audio�ź�����
	case KCM_SRC_DTS_MA:				                	// DTS HD Master Audio�ź�����
		g2DIP_ShowBuffer[6] |= 0x0200;						// B9:HD 
	case KCM_SRC_DTS_ES:				                	// DTS Extended Surround�ź�����
	case KCM_SRC_DTS :                                  	// ��׼��DTS/DTS-CD�ź�����
		g2DIP_ShowBuffer[6] |= 0x0100;	
		break;
	case KCM_SRC_LPCM :
		g2DIP_ShowBuffer[6] |= 0x0400;	
		break;
	}
	FDIP_ScreenUpdata = 1;
}
void MDIP_PlaySymbol(BYTE status){
	BYTE flag = status & 0x03;

	DIP_PLAY_OFF();
	if (flag == KC3X_STATE_PLAY_PAUSE){
		g2DIP_ShowBuffer[7] |= 0x08;
	}else if (status == KC3X_STATE_PLAY_PLAY){
		g2DIP_ShowBuffer[7] |= 0x04;
	}
	if (status & KC3X_STATE_REPEAT_ALL){						// �������ظ�
		if ((status & KC3X_STATE_REPEAT_ALL) == KC3X_STATE_REPEAT_ALL){	// �ظ������ļ�
			g2DIP_ShowBuffer[7] |= 0x01;
		}else {													// �ظ���ǰ�ļ�/��
			g2DIP_ShowBuffer[7] |= 0x02;
		}	
	}
	FDIP_ScreenUpdata = 1;
	// MLOG("PlaySymbol %02x", (WORD)g2DIP_ShowBuffer[7]);	
}
void MDIP_WifiSymbol(BYTE turnOn){
	if (turnOn == 0xff){
		turnOn = (MKCM_ReadRegister(KCM_WORK_STATUS) & 0x80) ? 1 : 0;
	}
	if (turnOn){
		g2DIP_ShowBuffer[6] |= 0x0800;	
	}
	else {
		g2DIP_ShowBuffer[6] &= ~0x0800;	
	}
	FDIP_ScreenUpdata = 1;
}
void MDIP_WrText(char* string, BYTE length){
	BYTE index = 0;
	for (; index < length; index++){
		if (string[index] != 0){
			MDIP_SingleChar(index, string[index]);
		}else{
			break;
		}
	}
}

void MDIP_WrString(char* string){
	BYTE gLocal_1;
	char counter = 0;
	do {
		gLocal_1 = *string++;
		if (gLocal_1 == 0){
			break;
		}
		MDIP_SingleChar(counter, gLocal_1);
	} while (++counter < 6);
}
BYTE MDIP_WriteDec(BYTE number, WORD value){                // ��ʾʮ��������������һ���ֿ�ʼ��λ��
    if (value / 10000){
        MDIP_SingleChar(number++, (value / 10000) + '0');
        value = value % 10000;
    }
    if (value / 1000){
        MDIP_SingleChar(number++, (value / 1000) + '0');
        value = value % 1000;
    }
    if (value / 100){
        MDIP_SingleChar(number++, (value / 100) + '0');
        value = value % 100;
    }
    if (value / 10){
        MDIP_SingleChar(number++, (value / 10) + '0');
        value = value % 10;
    }
    MDIP_SingleChar(number++, value + '0');
    return number;
}
void MDIP_Write2Digit(BYTE number, BYTE value){             // ��ʾ2������
	MDIP_SingleChar(number, (value / 10) + '0');
	MDIP_SingleChar(number+1, (value % 10) + '0');
}

CONST_CHAR TabDIPmiLED_BASE[] = { // ���ֹ�
    0x00,0x00,         // ' '
    0x24,0x3f,         // '0'
    0x00,0x06,         // '1'		   		____A____  
    0x00,0xdb,         //  2		       |\	|  	/| 	  A 0x0001 B 0x0002	C  0x0004 D  0x0008 
    0x00,0xcf,         //  3		       | H	J  K | 	  E 0x0010 F 0x0020 G1 0x0040 G2 0x0080
    0x00,0xe6,         //  4		       F  \	| /  B 	  H 0x0100 J 0x0200 K  0x0400 L  0x0800
    0x00,0xed,         //  5		       | 	|	 | 	  M 0x1000 N 0x2000
    0x00,0xfd,         //  6		       |_G1___G2_| 
    0x00,0x07,         //  7		       |		 | 
    0x00,0xff,         //  8		       |  	|    | 
    0x00,0xef,         //  9		       E  /	M \  C 
    0x00,0xf7,         //  'A'		       | N	|  L | 
    0x12,0x8f,         //  'B'		       |/___D___\| 
    0x00,0x39,         //  'C'		                  
    0x12,0x0f,         //  'D'
    0x00,0xf9,         //  'E'
    0x00,0xf1,         //  'F'
    0x00,0xbd,         //  'G'
    0x00,0xf6,         //  'H'
    0x12,0x09,         //  'I'
    0x00,0x1e,         //  'J' 
    0x0C,0x70,         //  'K'
    0x00,0x38,         //  'L'
    0x05,0x36,         //  'M'
    0x09,0x36,         //  'N'
    0x00,0x3f,         //  'O'
    0x00,0xf3,         //  'P'
    0x08,0x3f,         //  'Q'
    0x08,0xf3,         //  'R'
    0x00,0xed,         //  'S'
    0x12,0x01,         //  'T'
    0x00,0x3e,         //  'U'
    0x24,0x30,         //  'V'
    0x28,0x36,         //  'W'
    0x2d,0x00,         //  'X'
    0x00,0xee,         //  'y'
    0x24,0x09,         //  'Z'
    0x00,0xc0,         //  '-'
    0x24,0x00,         //  '/'
    0x12,0xc0,         //  '+'
};  						 

void MDIP_SingleChar(
					BYTE gLocal_1,		  			// 0-2 (���ֹ�) 3-5 (8�ֹ�) 6 (STEREO-WIFI+3��DOT) 7-12 (Ƶ�׵�1-6��)
					BYTE gLocal_2){			 
	BYTE gLocal_Tab;

	if (gLocal_1 < 3){ 								// 0-2 (���ֹ�)
	    if ((gLocal_2 >= '0') && (gLocal_2 <= '9')) {
	       gLocal_2 -= '0';
	       gLocal_Tab = gLocal_2+1;
	    }
	    else if ((gLocal_2 >= 'a') && (gLocal_2 <= 'z')){
	        gLocal_Tab = gLocal_2 - 'a' + 0x0b;            // ���д
	    }
	    else if ((gLocal_2 >= 'A') && (gLocal_2 <= 'Z')){
	        gLocal_Tab = (gLocal_2 - 'A') + 0x0b; // 0x14
	    }
	    else {
			switch (gLocal_2){
			case '-': gLocal_Tab = 37; break;
			case '/': gLocal_Tab = 38; break;
			case '+': gLocal_Tab = 39; break;
	    	default :
	    		gLocal_Tab = 0; 
				break;
			}
		}
		g2DIP_ShowBuffer[gLocal_1] = ((WORD)TabDIPmiLED_BASE[gLocal_Tab*2])<<8;
		g2DIP_ShowBuffer[gLocal_1] |= TabDIPmiLED_BASE[gLocal_Tab*2+1];
	}
	else if (gLocal_1 < 6){ 								// 3-5 (8�ֹ�) 
	    if ((gLocal_2 >= '0') && (gLocal_2 <= '9')) {
	       gLocal_2 -= '0';
	       gLocal_Tab = gLocal_2+1;
			g2DIP_ShowBuffer[gLocal_1] = ((WORD)TabDIPmiLED_BASE[gLocal_Tab*2])<<8;
			g2DIP_ShowBuffer[gLocal_1] |= TabDIPmiLED_BASE[gLocal_Tab*2+1];
	    }
	    else {
			switch (gLocal_2){
			case '-': gLocal_2 = 0xc0; break;
			case 'A': gLocal_2 = 0xf7; break;
			case 'B': gLocal_2 = 0x7c; break;
			case 'C': gLocal_2 = 0x39; break;
			case 'D': gLocal_2 = 0xde; break;
			case 'E': gLocal_2 = 0xf9; break;
			case 'F': gLocal_2 = 0xf1; break;
			case 'G': gLocal_2 = 0xef; break;
			case 'I': gLocal_2 = 0x06; break;
			case 'H': gLocal_2 = 0xf6; break;
			case 'L': gLocal_2 = 0x38; break;
			case 'N': gLocal_2 = 0x37; break;
			case 'O': gLocal_2 = 0x3f; break;
			case 'P': gLocal_2 = 0xf3; break;
			case 'R':
			case 'r': gLocal_2 = 0x50; break;
			case 'S': gLocal_2 = 0xed; break;
			case 'T':
			case 't': gLocal_2 = 0xf8; break;
			case 'U': gLocal_2 = 0x3e; break;
			case 'Y': gLocal_2 = 0x6e; break;
	    	default :
				gLocal_2 = 0x0000;
				break;
			}
			g2DIP_ShowBuffer[gLocal_1] = gLocal_2;

		}
	}
	else {
		// if(gLocal_1 > 6 && gLocal_1 < 13){ // ����
		//     if ((gLocal_2 >= 1) && (gLocal_2 <= 5)) {
		//        gLocal_Tab = gLocal_2+10+3+26-1;
		//     }
		// }
		// else {
		// }

		// g2DIP_ShowBuffer[gLocal_1] = ((WORD)TabDIPmiLED_BASE[gLocal_Tab*2])<<8;
		// g2DIP_ShowBuffer[gLocal_1] |= TabDIPmiLED_BASE[gLocal_Tab*2+1];
		
	//	g2DIP_ShowBuffer[gLocal_1]=0xffff;
//MDEBUG(0xbc);MDEBUG(gLocal_Tab);MDEBUG(g2DIP_ShowBuffer[gLocal_1]>>8);MDEBUG(g2DIP_ShowBuffer[gLocal_1]);
	}
	FDIP_ScreenUpdata = 1;
    return;
}
CONST_CHAR Tab_DIP_Spectrum1[] = {
	// 			   �ظ�		 �ظ�
	0x00,0x10,0x18,0x1c,0x1c,0x1e,0x1e,0x1f,
};

void MDIP_ClearSpectrum(){
	BYTE gLocal_1 = 0;
    do {
		g2DIP_ShowBuffer[8 + gLocal_1] = 0;
    } while (++gLocal_1 < 5);
}

BYTE MDIP_GetLevel(BYTE level){
//MDEBUG(0xaa);MDEBUG(level& 0x07);
    return Tab_DIP_Spectrum1[level & 0x07];
}
void MDIP_ReadSpectrum(){
	BYTE counter;
	BYTE outData[3];

	// 5��Ƶ�ף��ֽ�0��B2:0Ϊ��1�У�B5:3Ϊ��2�У��ֽ�1�� B2:0Ϊ��3�У�B5:3Ϊ��4�У��ֽ�2�� B2:0Ϊ��5��
P35=~P35;  
	MKCM_ReadXByte(KCM_RD_SPECTRUM, outData, 3);
//MDEBUG(0xaa);/*MDEBUG(outData[0]);MDEBUG(outData[1]);*/MDEBUG(outData[2]);
	if (FKCM_I2C_Error){
		MDIP_ClearSpectrum();
		return;
	}
    g2DIP_ShowBuffer[8 + 0] = MDIP_GetLevel(outData[0]);
    g2DIP_ShowBuffer[8 + 1] = MDIP_GetLevel(outData[0]>>3);
    g2DIP_ShowBuffer[8 + 2] = MDIP_GetLevel(outData[1]);
    g2DIP_ShowBuffer[8 + 3] = MDIP_GetLevel(outData[1]>>3);
    g2DIP_ShowBuffer[8 + 4] = MDIP_GetLevel(outData[2]);
    FDIP_ScreenUpdata = 1;
//MLOG("S_");            
}
void MDIP_CleanSpectrum(){
	BYTE gLocal_1;
    gLocal_1 = 0;
    do {
		g2DIP_ShowBuffer[8] = 0;
    } while (++gLocal_1 < 5);
	FDIP_ScreenUpdata = 1;
}

void MDIP_ScreenFill(BYTE value){							// ���������ʾԪ�ص���ֵ��0x00Ϊȫ��Ϩ��
	BYTE gLocal_1;
    gLocal_1 = 0;
    do{
		g2DIP_ShowBuffer[gLocal_1] = value;    
    } while (++gLocal_1 < 13);
}
void MDIP_ScreenUpdata(){    
	BYTE gLocal_0;
	BYTE gLocal_1;

    gLocal_1 = 0;
    do{
		gDIP_Buffer[gLocal_1] = 0;    
    } while (++gLocal_1 < 16);    						  

	// ���ֹ�1
	if (g2DIP_ShowBuffer[0] & 0x0001) gDIP_Buffer[14] |= 0x08;
	if (g2DIP_ShowBuffer[0] & 0x0002) gDIP_Buffer[7]  |= 0x08;
	if (g2DIP_ShowBuffer[0] & 0x0004) gDIP_Buffer[0]  |= 0x08;
	if (g2DIP_ShowBuffer[0] & 0x0008) gDIP_Buffer[3]  |= 0x08;
	if (g2DIP_ShowBuffer[0] & 0x0010) gDIP_Buffer[11] |= 0x08;
	if (g2DIP_ShowBuffer[0] & 0x0020) gDIP_Buffer[15] |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0040) gDIP_Buffer[9]  |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0080) gDIP_Buffer[6]  |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0100) gDIP_Buffer[13] |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0200) gDIP_Buffer[12] |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0400) gDIP_Buffer[5]  |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x0800) gDIP_Buffer[1]  |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x1000) gDIP_Buffer[2]  |= 0x08;
  	if (g2DIP_ShowBuffer[0] & 0x2000) gDIP_Buffer[8]  |= 0x08;

	// ���ֹ�2
	if (g2DIP_ShowBuffer[1] & 0x0001) gDIP_Buffer[14] |= 0x20;
	if (g2DIP_ShowBuffer[1] & 0x0002) gDIP_Buffer[7]  |= 0x20;
	if (g2DIP_ShowBuffer[1] & 0x0004) gDIP_Buffer[0]  |= 0x20;
	if (g2DIP_ShowBuffer[1] & 0x0008) gDIP_Buffer[3]  |= 0x20;
	if (g2DIP_ShowBuffer[1] & 0x0010) gDIP_Buffer[11] |= 0x20;
	if (g2DIP_ShowBuffer[1] & 0x0020) gDIP_Buffer[15] |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0040) gDIP_Buffer[9]  |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0080) gDIP_Buffer[6]  |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0100) gDIP_Buffer[13] |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0200) gDIP_Buffer[12] |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0400) gDIP_Buffer[5]  |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x0800) gDIP_Buffer[1]  |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x1000) gDIP_Buffer[2]  |= 0x20;
  	if (g2DIP_ShowBuffer[1] & 0x2000) gDIP_Buffer[8]  |= 0x20;

	// ���ֹ�3
	if (g2DIP_ShowBuffer[2] & 0x0001) gDIP_Buffer[14] |= 0x10;
	if (g2DIP_ShowBuffer[2] & 0x0002) gDIP_Buffer[7]  |= 0x10;
	if (g2DIP_ShowBuffer[2] & 0x0004) gDIP_Buffer[0]  |= 0x10;
	if (g2DIP_ShowBuffer[2] & 0x0008) gDIP_Buffer[3]  |= 0x10;
	if (g2DIP_ShowBuffer[2] & 0x0010) gDIP_Buffer[11] |= 0x10;
	if (g2DIP_ShowBuffer[2] & 0x0020) gDIP_Buffer[15] |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0040) gDIP_Buffer[9]  |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0080) gDIP_Buffer[6]  |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0100) gDIP_Buffer[13] |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0200) gDIP_Buffer[12] |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0400) gDIP_Buffer[5]  |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x0800) gDIP_Buffer[1]  |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x1000) gDIP_Buffer[2]  |= 0x10;
  	if (g2DIP_ShowBuffer[2] & 0x2000) gDIP_Buffer[8]  |= 0x10;

	// ���ֹ�1
	if (g2DIP_ShowBuffer[3] & 0x0001) gDIP_Buffer[14] |= 0x04;
	if (g2DIP_ShowBuffer[3] & 0x0002) gDIP_Buffer[7]  |= 0x04;
	if (g2DIP_ShowBuffer[3] & 0x0004) gDIP_Buffer[0]  |= 0x04;
	if (g2DIP_ShowBuffer[3] & 0x0008) gDIP_Buffer[3]  |= 0x04;
	if (g2DIP_ShowBuffer[3] & 0x0010) gDIP_Buffer[11] |= 0x04;
	if (g2DIP_ShowBuffer[3] & 0x0020) gDIP_Buffer[15] |= 0x04;
  	if (g2DIP_ShowBuffer[3] & 0x0040) gDIP_Buffer[9]  |= 0x04;

	// ���ֹ�2
	if (g2DIP_ShowBuffer[4] & 0x0001) gDIP_Buffer[14] |= 0x80;
	if (g2DIP_ShowBuffer[4] & 0x0002) gDIP_Buffer[7]  |= 0x80;
	if (g2DIP_ShowBuffer[4] & 0x0004) gDIP_Buffer[0]  |= 0x80;
	if (g2DIP_ShowBuffer[4] & 0x0008) gDIP_Buffer[3]  |= 0x80;
	if (g2DIP_ShowBuffer[4] & 0x0010) gDIP_Buffer[11] |= 0x80;
	if (g2DIP_ShowBuffer[4] & 0x0020) gDIP_Buffer[15] |= 0x80;
  	if (g2DIP_ShowBuffer[4] & 0x0040) gDIP_Buffer[9]  |= 0x80;

	// ���ֹ�3
	if (g2DIP_ShowBuffer[5] & 0x0001) gDIP_Buffer[14] |= 0x02;
	if (g2DIP_ShowBuffer[5] & 0x0002) gDIP_Buffer[7]  |= 0x02;
	if (g2DIP_ShowBuffer[5] & 0x0004) gDIP_Buffer[0]  |= 0x02;
	if (g2DIP_ShowBuffer[5] & 0x0008) gDIP_Buffer[3]  |= 0x02;
	if (g2DIP_ShowBuffer[5] & 0x0010) gDIP_Buffer[11] |= 0x02;
	if (g2DIP_ShowBuffer[5] & 0x0020) gDIP_Buffer[15] |= 0x02;
  	if (g2DIP_ShowBuffer[5] & 0x0040) gDIP_Buffer[9]  |= 0x02;

	// STEREO -- WIFI ��
	if (g2DIP_ShowBuffer[6] & 0x0001) gDIP_Buffer[14] |= 0x01;
	if (g2DIP_ShowBuffer[6] & 0x0002) gDIP_Buffer[7]  |= 0x01;
	if (g2DIP_ShowBuffer[6] & 0x0004) gDIP_Buffer[0]  |= 0x01;
	if (g2DIP_ShowBuffer[6] & 0x0008) gDIP_Buffer[3]  |= 0x01;
	if (g2DIP_ShowBuffer[6] & 0x0010) gDIP_Buffer[11] |= 0x01;
	if (g2DIP_ShowBuffer[6] & 0x0020) gDIP_Buffer[15] |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0040) gDIP_Buffer[9]  |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0080) gDIP_Buffer[6]  |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0100) gDIP_Buffer[13] |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0200) gDIP_Buffer[12] |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0400) gDIP_Buffer[5]  |= 0x01;
  	if (g2DIP_ShowBuffer[6] & 0x0800) gDIP_Buffer[1]  |= 0x01;
  	// dot ':''.' ':'
  	if (g2DIP_ShowBuffer[6] & 0x1000) gDIP_Buffer[10]  |= 0x20;
  	if (g2DIP_ShowBuffer[6] & 0x2000) gDIP_Buffer[10]  |= 0x10;
  	if (g2DIP_ShowBuffer[6] & 0x4000) gDIP_Buffer[10]  |= 0x04;

	// Ƶ����ʾ
	// P5 P15 P25 
	// P31	(60 330 1K 3K3 10K 20K)
	// �ӵ�Դ ��������

	// p1 ���ϵ��� ��������
	if (g2DIP_ShowBuffer[7] & 0x01) gDIP_Buffer[5]  |= 0x04;
	if (g2DIP_ShowBuffer[7] & 0x02) gDIP_Buffer[12] |= 0x04;
	if (g2DIP_ShowBuffer[7] & 0x04) gDIP_Buffer[13] |= 0x04;
	if (g2DIP_ShowBuffer[7] & 0x08) gDIP_Buffer[6]  |= 0x04;

	if (g2DIP_ShowBuffer[8] & 0x01) gDIP_Buffer[1]  |= 0x80;
  	if (g2DIP_ShowBuffer[8] & 0x02) gDIP_Buffer[5]  |= 0x80;
  	if (g2DIP_ShowBuffer[8] & 0x04) gDIP_Buffer[12] |= 0x80;
  	if (g2DIP_ShowBuffer[8] & 0x08) gDIP_Buffer[13] |= 0x80;
  	if (g2DIP_ShowBuffer[8] & 0x10) gDIP_Buffer[6]  |= 0x80;

	//p11
  	if (g2DIP_ShowBuffer[9] & 0x01) gDIP_Buffer[5]  |= 0x02;
  	if (g2DIP_ShowBuffer[9] & 0x02) gDIP_Buffer[12] |= 0x02;
  	if (g2DIP_ShowBuffer[9] & 0x04) gDIP_Buffer[13] |= 0x02;
  	if (g2DIP_ShowBuffer[9] & 0x08) gDIP_Buffer[6]  |= 0x02;

	// p16
  	if (g2DIP_ShowBuffer[10] & 0x01) gDIP_Buffer[11] |= 0x40;
	if (g2DIP_ShowBuffer[10] & 0x02) gDIP_Buffer[3]  |= 0x40;
	if (g2DIP_ShowBuffer[10] & 0x04) gDIP_Buffer[0]  |= 0x40;
	if (g2DIP_ShowBuffer[10] & 0x08) gDIP_Buffer[7]  |= 0x40;
	if (g2DIP_ShowBuffer[10] & 0x10) gDIP_Buffer[14] |= 0x40;

	// p21
	if (g2DIP_ShowBuffer[11] & 0x01) gDIP_Buffer[13] |= 0x40;
	if (g2DIP_ShowBuffer[11] & 0x02) gDIP_Buffer[6]  |= 0x40;
  	if (g2DIP_ShowBuffer[11] & 0x04) gDIP_Buffer[9]  |= 0x40;
  	if (g2DIP_ShowBuffer[11] & 0x08) gDIP_Buffer[15] |= 0x40;

	// p26
  	if (g2DIP_ShowBuffer[12] & 0x01) gDIP_Buffer[8]  |= 0x40;
  	if (g2DIP_ShowBuffer[12] & 0x02) gDIP_Buffer[2]  |= 0x40;
  	if (g2DIP_ShowBuffer[12] & 0x04) gDIP_Buffer[1]  |= 0x40;
  	if (g2DIP_ShowBuffer[12] & 0x08) gDIP_Buffer[5]  |= 0x40;
  	if (g2DIP_ShowBuffer[12] & 0x10) gDIP_Buffer[12] |= 0x40;

    HAL_DIP_STB(0);
    MUSDELAY(1);
	MDIP_WriteByte(0x40);		  // ��ʼ��	�ϵ�ʱ����ַ��Ϊ00H(00H-0DH) //  �ϵ�ʱ��Ĭ������Ϊ������Ϊ1/16����ʾ�أ���ַ������1
    MUSDELAY(1);
    HAL_DIP_STB(1);    
    MUSDELAY(1);
    HAL_DIP_STB(0);
    MUSDELAY(1);
	MDIP_WriteByte(0xc0);		  // �����ַ 
	gLocal_0 = 0;

	do{
		MDIP_WriteByte(gDIP_Buffer[gLocal_0]);		  // ����
	}while(++gLocal_0 < 16);	  // TA6932 һ�����16��BYTE����
    HAL_DIP_STB(1);    
    return;
}

void MDIP_WriteByte(BYTE gLocal_1){
    BYTE gLocal_0;

    gLocal_0 = 8;
    do{
        HAL_DIP_CLK(0);
        HAL_DIP_DAT(gLocal_1 & 0x01);
        gLocal_1 >>= 1;
        HAL_DIP_CLK(1);
    } while (--gLocal_0 > 0); 
    return;
}
