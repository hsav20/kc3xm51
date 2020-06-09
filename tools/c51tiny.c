
#include <stdarg.h>
#include "c51tiny.h"

xdata char gLogBuffer[32];

// 从8位的HEX转换到ASCII码
// lowercase 字母小写
// value 	输入的数据
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

// 将数字格式化为字符串输出，返回字符串的长度
// mode 	'd'为有符号十进制，'u'为无符号十进制，'x'为16进制小写字母，'X'为16进制大写字母
// zeroPad 	不管前面是否非0，共需要显示的位数
// value 	输入的数据
// outData 	输出的串（定义缓冲区必需大于10）
static u8 formatValue(char mode, char zeroPad, u32 value, char* outData){
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
	if (mode == 'd' || mode == 'u'){                        // 'd'为有符号十进制，'u'为无符号十进制
	    u32 g4Local_1 = 1000000000;
		if (mode == 'd' && value & 0x80000000){				// 负数
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
	}
	else {                                                  // 'x'为16进制小写字母，'X'为16进制大写字母
		gLocal_3 = 32;
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
	u32 value;
	while (1){
		if (fmt[inCoun] != '%') {
			if (outCoun > 30){
//OWN_LOG("return A %d %d\r\n",inCoun, outCoun);							
				return outCoun;
			}
			outData[outCoun++] = fmt[inCoun++];
			if (!fmt[inCoun]){
//OWN_LOG("return B %d %d\r\n",inCoun, outCoun);											
				return outCoun;
			}
			continue;
		}
		nowStr = fmt[inCoun+1];
//OWN_LOG("nowStr %02x %02x\r\n", inCoun+1, nowStr);		
		if (nowStr == 's'){									// 字符串
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
		if (nowStr == '%'){									// 需要输出字符%
			inCoun += 2;
			outData[outCoun++] = '%';
			continue;
		}
		if (nowStr == 'c'){									// 需要输出字符
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

//OWN_LOG("EE %d %d %02x %02x\r\n", zeroPad, strCoun, inCoun+1, outCoun);// 补零
		strData = (char*)&fmt[(1 + inCoun + strCoun)];
		inCoun = 1 + 1 + inCoun + strCoun;
		switch (strData[0]){
		case 'd':										// 有符号十进制
		case 'u':										// 无符号十进制
		case 'x': 										// 小写的十六进制
		case 'X': 										// 大写的十六进制 
			value = va_arg(args,char);
            value <<= 8;
			value |= va_arg(args,char);
            value <<= 8;
			value |= va_arg(args,char);
            value <<= 8;
			value |= va_arg(args,char);
			outCoun += formatValue(strData[0], zeroPad, value, &outData[outCoun]);
			break;
		}
	}
//OWN_LOG("return C %d %d\r\n",inCoun, outCoun);								
	return outCoun;
}


void MLOG(char *fmt, ...){
	char counter;
	char length;
	va_list args; 
	va_start(args, fmt);  
    length = VsPrintf(gLogBuffer, fmt, args);
	va_end(args); 
    for (counter = 0; counter < length; counter++){
        TI = 0;
        SBUF = gLogBuffer[counter];
        while (!TI);
    }
}
