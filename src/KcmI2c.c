
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Media Hardware interface ////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MKCM_WriteRegister(					  				// 写DA32Ux的I2C寄存器, 成功返回1
					BYTE address, 							// 地址索引值
					BYTE vaule){ 							// 数据
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	MI2C_Bus_Write(address);							// Index 写入I2C从机的寄存器索引值
	MI2C_Bus_Write(vaule);								// Data 写入I2C从机的数据
	MI2C_Bus_Stop();										// I2C Stop
    return;
}
void MKCM_Write2Byte(										// Write 2 bytes to DA32C. 写入2个字节
					BYTE address,    						// Index. 寄存器索引值
					WORD vaule){      						// 16 bit Data. 16位数据
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	MI2C_Bus_Write(address);							// Index. 写入I2C从机的寄存器索引值
	MI2C_Bus_Write(vaule);								// Low byte data. 写入I2C从机的低字节数据
	MI2C_Bus_Write(vaule >> 8);							// High byte data. 写入I2C从机的高字节数据
	MI2C_Bus_Stop();										// I2C Stop
	return;
}
void MKCM_WriteXByte(										// Write buffer to DA32C. 写入多个字节
					BYTE address,   						// Index. 寄存器索引值
					WORD length,    						// Length. 长度
					BYTE* in_data){ 						// Buffer address. 数据缓冲
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	MI2C_Bus_Write(address);							// Index. 写入I2C从机的寄存器索引值
	do { 
		MI2C_Bus_Write(*in_data++);						// Write byte data. 写入I2C从机的数据
	} while (--length != 0);
	MI2C_Bus_Stop();										// I2C Stop
	return;
}

BYTE MKCM_ReadRegister(										// Read byte from DA32C. 读取8位的寄存器
					BYTE address){     						// Index. 寄存器索引值
	BYTE gLocal_1 = 0;
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	if (!FKCM_I2C_Error){
		MI2C_Bus_Write(address);							// Index. 写入I2C从机的寄存器索引值
	//	MI2C_Bus_Stop();										// I2C Stop
	//	MUSDELAY(10);											// delay 10us
		MI2C_Bus_Start();									// I2C Start
		MI2C_Bus_Write(cI2C_ADDRESS+1);						// KCM I2C read address
	//gLocal_1 = MI2C_Bus_Read(0);							// Read 1 byte 
	//MDEBUG(gLocal_1);
	    gLocal_1 = MI2C_Bus_Read(1);							// Read 1 byte 
//MDEBUG(gLocal_1);
	}
	MI2C_Bus_Stop();										// I2C Stop
	return gLocal_1;
}
WORD MKCM_Read2Byte(										// Read word from DA32C. 读取16位的寄存器
					BYTE address){     						// Index. 寄存器索引值
    WORD g2Local_1 = 0;
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	if (!FKCM_I2C_Error){
		MI2C_Bus_Write(address);								// Index. 写入I2C从机的寄存器索引值
	//	MI2C_Bus_Stop();										// I2C Stop
	//	MUSDELAY(10);											// delay 10us
  
		MI2C_Bus_Start();									// I2C Start
		MI2C_Bus_Write(cI2C_ADDRESS+1);						// KCM I2C read address
	    g2Local_1 = MI2C_Bus_Read(0);						// Read 1 byte 
	    g2Local_1 = g2Local_1 | (MI2C_Bus_Read(1) << 8);	// Read last byte 
	}
	MI2C_Bus_Stop();										// I2C Stop
	return g2Local_1;
}


void MKCM_MutilRead(WORD length, BYTE* outData){
	if (length > 1){									// 防止只有一次时g2Local_1变为0产生跨界错误
		length = length - 1;							// Counter. 做少一次，最后的字节使用MI2C_Bus_Read(1);
	}
	do {													
	    *outData++ = MI2C_Bus_Read(0);					// Read 1 byte 
	} while (--length != 0);
    *outData = MI2C_Bus_Read(1);						// Read last byte 
	MI2C_Bus_Stop();										// I2C Stop
}

void MKCM_ReadXByte(										// Read buffer from DA32C. 读取多个字节
					BYTE address,      						// Index. 寄存器索引值
					WORD length,      						// Length. 长度
					BYTE* outData){   						// Buffer address. 数据缓冲
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	MI2C_Bus_Write(address);								// Index. 写入I2C从机的寄存器索引值

	MI2C_Bus_Start();									    // I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS+1);						    // KCM I2C read address
    MKCM_MutilRead(length, outData);
	return;
}
WORD MKCM_ReadAutoByte(										// 读取由字节指示长度的多字节
					BYTE address,      						// Index. 寄存器索引值
					WORD limit,      						// limit. 最大输出长度
					BYTE* outData){   						// Buffer address. 数据缓冲
	WORD length;
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	MI2C_Bus_Write(address);								// Index. 写入I2C从机的寄存器索引值

	MI2C_Bus_Start();									    // I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS+1);						    // KCM I2C read address
	length = MI2C_Bus_Read(0);
    if (length > limit){
        length = limit;
    }
    MKCM_MutilRead(length, outData);
	return length;
}

    
void MI2C_Bus_Write(BYTE gLocal_1){      					// 往I2C总线写入一个字节，gLocal为待写数据
    BOOL FLocal_1;											// 返回成功/失败标志
    BYTE gLocal_2;											// 位计数器
      
    gLocal_2 = 8;											// 8位，高位先出
    do { 
        HAL_KCM_I2C_SCL(0);									// 置低I2C时钟线，以改变数据
        if (gLocal_1 & 0x80){								// 如果数据位为1
            HAL_KCM_I2C_SDA(1);								// 置高I2C数据线
        }
        else {												// 如果数据位为0
            HAL_KCM_I2C_SDA(0);								// 置低I2C数据线
        }
	    MI2C_400K_DELAY();									// 按照400Kbps的标准延时，可以有效地提高通讯的速度
        gLocal_1 <<= 1;										// 准备下一位数据
        HAL_KCM_I2C_SCL(1);									// 置高I2C时钟线，保持数据稳定
	    MI2C_400K_DELAY();									// 按照400Kbps的标准延时，可以有效地提高通讯的速度
    } while (--gLocal_2 != 0);								// 完成8位数据
    HAL_KCM_I2C_SCL(0);										// 置低I2C时钟线，准备ACK位
    HAL_KCM_I2C_SDA(1);										// 将SDA设置为输入
    MI2C_100K_DELAY();										// 按照100Kbps的标准延时，以确保状态稳定
    HAL_KCM_I2C_SCL(1);										// 置高I2C时钟线
    MI2C_100K_DELAY();										// 按照100Kbps的标准延时，以确保状态稳定
    if ((HAL_KCM_I2C_IN_SDA())){							// 高为NAK
        FKCM_I2C_Error = 1;                                 // NAK标志表示从机没有应答，可能是硬件故障
    }
    HAL_KCM_I2C_SCL(0);                                            
    MI2C_100K_DELAY();										// 按照100Kbps的标准延时，以确保状态稳定
    return;                                        			// 返回cI2C_NCK/cI2C_ACK标志
}
BYTE MI2C_Bus_Read(BOOL FLocal_NAK){						// 从I2C总线读一个字节，在最后一次读时，FLocal_NAK必须为1
    BYTE gLocal_1;											// 数据暂存器 
    BYTE gLocal_2;											// 位计数器

    HAL_KCM_I2C_SDA(1);                                     // 将SDA设置为输入
    MI2C_100K_DELAY();										// 按照100Kbps的标准延时，以确保状态稳定
    gLocal_2 = 8;											// 8位
    do{														
        HAL_KCM_I2C_SCL(1);									// 置高I2C时钟线，时钟线为高电平时数据方有效
        gLocal_1 <<= 1;										// 向左移一位，第0位为0
        if ((HAL_KCM_I2C_IN_SDA())){
        	gLocal_1 |= 0x01;                				// 如果数据线为高电平，则数据暂存器的第0位赋1
	    }
	    MI2C_400K_DELAY();									// 按照400Kbps的标准延时，可以有效地提高通讯的速度
        HAL_KCM_I2C_SCL(0);									// 置低I2C时钟线，接收下一位
	    MI2C_400K_DELAY();									// 按照400Kbps的标准延时，可以有效地提高通讯的速度
    } while (--gLocal_2 != 0);								// 循环8次，完成一个字节的接收
      
    if (!FLocal_NAK){										// 如果ACK标志为0
        HAL_KCM_I2C_SDA(0);									// 置低数据线，ACK为0，继续读取
    }
    else {													// 如果ACK标志为1
        HAL_KCM_I2C_SDA(1); 								// 置高数据线，ACK为1，读取结束
    }
    MI2C_100K_DELAY();										// 按照100Kbps的标准延时，以确保状态稳定
    HAL_KCM_I2C_SCL(1);
    MI2C_100K_DELAY();										// 按照100Kbps的标准延时，以确保状态稳定
    HAL_KCM_I2C_SCL(0);
    return gLocal_1;
} 
void MI2C_Bus_Start(){  									// I2C总线开始
    HAL_KCM_I2C_SDA(1);                   					// 置高数据线，数据线空闲
    HAL_KCM_I2C_SCL(1);                   					// 置高时钟线，时钟线空闲
    MI2C_100K_DELAY();										// 按照100Kbps的标准延时，以确保状态稳定
	FKCM_I2C_Error = 0;                                     // 假定是正确的
    if (!(HAL_KCM_I2C_IN_SDA())){							// 如果为低，可能硬件出了问题
	    FKCM_I2C_Error = 1;                                 // 出错了
    }
    HAL_KCM_I2C_SDA(0);										// 数据线在时钟为高电平时由高电平向低电平切换表示开始
    MI2C_100K_DELAY();										// 按照100Kbps的标准延时，以确保状态稳定
    HAL_KCM_I2C_SCL(0);										// 置低时钟线，准备接收或发送数据
    return;
}
void MI2C_Bus_Stop(){										// I2C总线停止
    HAL_KCM_I2C_SDA(0);                   					// 置低数据线，准备停止
    MI2C_100K_DELAY();										// 按照100Kbps的标准延时，以确保状态稳定
    HAL_KCM_I2C_SCL(1);										// 置高时钟线
    MI2C_100K_DELAY();										// 按照100Kbps的标准延时，以确保状态稳定
    HAL_KCM_I2C_SDA(1); 									// 数据线在时钟为高电平时由低电平向高电平切换表示结束
    return;
}
void MI2C_100K_DELAY(){										// 按照100Kbps的标准延时，以确保状态稳定
	BYTE gLocal_1;
	gLocal_1 = 10;
	while (--gLocal_1 != 0);
}
void MI2C_400K_DELAY(){										// 按照400Kbps的标准延时，可以有效地提高通讯的速度
}
