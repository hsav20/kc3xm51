
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#define ROOT     		  
#include "main.h"    

void main(){
	MSYS_Initialize(); 										// CPU基本部件初始化
	MDIP_Initialize();										// 显示模块初始化
	MKEY_Initialize();       								// 按键模块初始化
	MAUD_Initialize();       								// 音频模块初始化
    MKCM_Initialize();                                      // KCM模块初始化
    while (1){ 						   
        if ((gTBascTimer & 0x01) > 0){						// 10ms
	        MKCM_10msTimer(gTBascTimer);                	// 子处理模块10ms时间处理 		
			MKEY_10msTimer(gTBascTimer);					// 按键模块10ms时间处理
            MDIP_10msTimer(gTBascTimer);  					// 显示模块10ms时间处理
            MAUD_10msTimer(gTBascTimer);                	// 音频模块10ms时间处理
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
void MDEBUG(BYTE gLocal_1){									// 调试输出
//	TI = 0;
//	SBUF = gLocal_1;
//	while (!TI);
	return;
} 
void MUSDELAY(BYTE gLocal_1){      
    if (gLocal_1 > 3){
        while (--gLocal_1 != 0);
    }
    return;
}
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
				HAL_DIP_LED(1);
			}
			else {
				if ((gDIP_Brightness == 0) && (gBrightnessTimer == 1)){
					HAL_DIP_LED(0);
				}
				else if ((gDIP_Brightness == 1) && (gBrightnessTimer == 4)){
					HAL_DIP_LED(0);
				}
				if ((gDIP_Brightness == 2) && (gBrightnessTimer == 8)){
					HAL_DIP_LED(0);
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

// ----------------------------------------------------------------
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
            }
            else if (gRmStatus == 2+32){ 
                FRmDecodDone = 1;
				gRmKeyContinSpeed = 0;
                if ((gRmBuffer ^ 0xff) != GRmCodeData){
                    FRmDecodDone = 0; 
                    FRmError = 1;
                }
            }
        }
    }
    gRmTimer = 1; 
    return;
}		 

