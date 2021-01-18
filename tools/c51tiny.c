
#include <stdarg.h>
#include "c51tiny.h"

xdata char gLogBuffer[32];

// ��8λ��HEXת����ASCII��
// lowercase ��ĸСд
// value 	���������
static char Hex2Ascii(char lowercase, unsigned char value){	
    value &= 0x0f;
    if (value < 0x0a){
    	return value += '0';
	}
	if (lowercase){
		return (value - 10) + 'a';
	}
	return (value - 10) + 'A';
}

// �����ָ�ʽ��Ϊ�ַ�������������ַ����ĳ���
// mode 	'd'Ϊ�з���ʮ���ƣ�'u'Ϊ�޷���ʮ���ƣ�'x'Ϊ16����Сд��ĸ��'X'Ϊ16���ƴ�д��ĸ
// zeroPad 	����ǰ���Ƿ��0������Ҫ��ʾ��λ��
// value 	���������
// outData 	����Ĵ������建�����������10��
static u8 formatValue(char mode, char zeroPad, u8 value, char* outData){
    char FLocal_2;
    unsigned char gLocal_2;
    unsigned char gLocal_3;
    unsigned char gLocal_4;
    unsigned char gLocal_5;
	if (!zeroPad && !value){
		outData[0] = '0';
		outData[1] = 0x00;
	    return 1;
	}

    FLocal_2 = 0;
	gLocal_2 = 0;
	if (mode == 'd' || mode == 'u'){                        // 'd'Ϊ�з���ʮ���ƣ�'u'Ϊ�޷���ʮ����
	    /*
	    u32 g4Local_1 = 1000000000;
		if (mode == 'd' && value & 0x80000000){				// ����
	        outData[gLocal_2++] = '-';
			value = 0x80000000 - (value & ~0x80000000);
		}
		gLocal_3 = 11;
	    do {
	        gLocal_4 = (unsigned char)(value / g4Local_1);
	        if ((--gLocal_3 == zeroPad) || gLocal_4){
	            FLocal_2 = 1;
	        }
	        if (FLocal_2){
	            outData[gLocal_2++] = Hex2Ascii(0, gLocal_4);
	        }
	        value %= g4Local_1;
	        g4Local_1 /= 10;
	    } while(g4Local_1 != 0);
        */
        gLocal_3 = value / 100;
        if (gLocal_3){
            outData[gLocal_2++] = Hex2Ascii(0, gLocal_3);
        }else if (zeroPad == 3){
            outData[gLocal_2++] = '0';
        }
        gLocal_3 = value % 100;
        gLocal_4 = gLocal_3 / 10;
        if (gLocal_4){
            outData[gLocal_2++] = Hex2Ascii(0, gLocal_4);
        }else if (zeroPad == 2){
            outData[gLocal_2++] = '0';
        }
        outData[gLocal_2++] = Hex2Ascii(0, gLocal_3 % 10);
	}
	else {                                                  // 'x'Ϊ16����Сд��ĸ��'X'Ϊ16���ƴ�д��ĸ
//MDEBUG(0xa9);MDEBUG(zeroPad);
        if (value & 0xf0){
            outData[gLocal_2++] = Hex2Ascii((mode == 'X') ? 1 : 0, value >> 4);
        }else if (zeroPad >= 2){
            outData[gLocal_2++] = '0';
        }
        outData[gLocal_2++] = Hex2Ascii((mode == 'X') ? 1 : 0, value);
		/*gLocal_3 = 8;
		gLocal_4 = 9;
		do {
			gLocal_3 -= 4;
			gLocal_5 = (unsigned char)(value >> gLocal_3);
			if ((--gLocal_4 == zeroPad) || gLocal_5){
	            FLocal_2 = 1;
			}
	        if (FLocal_2){
				outData[gLocal_2++] = Hex2Ascii((mode == 'X') ? 1 : 0, gLocal_5);
			}
		} while (gLocal_3 != 0);
        */
	}
	outData[gLocal_2] = 0x00;
    return gLocal_2;
}
u8 VsPrintf(char* outData, const char* fmt, va_list args){
	char inCoun = 0;
	char outCoun = 0;
	char strCoun;
	char* strData;
	char nowStr;
	char zeroPad;
//	u32 value;
	u8 value;
	while (1){
//MDEBUG(0xa1);MDEBUG(inCoun);MDEBUG(fmt[inCoun]);
		if (fmt[inCoun] != '%') {
			if (outCoun > 30){
//OWN_LOG("return A %d %d\r\n",inCoun, outCoun);							

				return outCoun;
			}
			if (!fmt[inCoun]){
//OWN_LOG("return B %d %d\r\n",inCoun, outCoun);
//MDEBUG(0xa2);
				return outCoun;
			}
			outData[outCoun++] = fmt[inCoun++];
			continue;
		}
		nowStr = fmt[inCoun+1];
//OWN_LOG("nowStr %02x %02x\r\n", inCoun+1, nowStr);		
		if (nowStr == 's'){									// �ַ���
			strData = va_arg(args, char*);
			inCoun += 2;
			strCoun = 0;
//OWN_LOG("EE %d %d\r\n",inCoun, outCoun);			
			while (strData[strCoun] && outCoun < 30){
				outData[outCoun++] = strData[strCoun++];
			}
//OWN_LOG("FF %d %d %d\r\n",inCoun, outCoun, strCoun);
			continue;
		}
		if (nowStr == '%'){									// ��Ҫ����ַ�%
			inCoun += 2;
			outData[outCoun++] = '%';
			continue;
		}
		if (nowStr == 'c'){									// ��Ҫ����ַ�
			inCoun += 2;
			outData[outCoun++] = (char)va_arg(args,int);
			continue;
		}
		zeroPad = 0;
		for (strCoun = 0; strCoun < 3; strCoun++){
			nowStr = fmt[(1 + inCoun + strCoun)];
			if (nowStr >= '0' && nowStr <= '9'){
				zeroPad = (zeroPad * 10) + (nowStr - '0');
			}else {
				break;
			}
		}

//OWN_LOG("EE %d %d %02x %02x\r\n", zeroPad, strCoun, inCoun+1, outCoun);// ����
		strData = (char*)&fmt[(1 + inCoun + strCoun)];
//MDEBUG(0xa8);MDEBUG(strCoun);
		inCoun = 1 + 1 + inCoun + strCoun;              // % x 02
		switch (strData[0]){
		case 'd':										// �з���ʮ����
		case 'u':										// �޷���ʮ����
		case 'x': 										// Сд��ʮ������
		case 'X': 										// ��д��ʮ������ 
//			value = va_arg(args,char);
  //          value <<= 8;
	//		value |= va_arg(args,char);
      //      value <<= 8;
			value = va_arg(args,char);
//            value <<= 8;
//			value |= va_arg(args,char);
			outCoun += formatValue(strData[0], zeroPad, value, &outData[outCoun]);
			break;
		}
	}
//OWN_LOG("return C %d %d\r\n",inCoun, outCoun);								
	return outCoun;
}

void memset(u8* str, u8 value, u16 length){
    while (--length != 0){
        *str = value;
    }
}
void memcpy(u8* outData, u8* inData, u16 length){
    while (--length != 0){
        *outData = *outData;
    }
}
void MDEBUG(char value){
    /*
    while (TI);
    TI = 0;
    SBUF = value;
    */
	TI = 0;
	SBUF = value;
	while (!TI);

}

void MLOG(char *fmt, ...){
	char counter;
	char length;
	va_list args; 
	va_start(args, fmt);  
    
//    memset(gLogBuffer, 0, sizeof(gLogBuffer));
    length = VsPrintf(gLogBuffer, fmt, args);
//MDEBUG(length);
    gLogBuffer[length++] = '\r';                            // ����ʾ������س�
    gLogBuffer[length++] = '\n';                            // ����ʾ�����뻻��                       
	va_end(args); 
    
    for (counter = 0; counter < length; counter++){
        MDEBUG(gLogBuffer[counter]);
//        TI = 0;
  //      SBUF = gLogBuffer[counter];
    //    while (!TI);
    } 
}
