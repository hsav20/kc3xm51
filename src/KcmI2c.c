
// Copyright (c) 2002-2020, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

#include "main.h"           

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Media Hardware interface ////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MKCM_WriteRegister(					  				// дDA32Ux��I2C�Ĵ���, �ɹ�����1
					BYTE address, 							// ��ַ����ֵ
					BYTE vaule){ 							// ����
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	MI2C_Bus_Write(address);							// Index д��I2C�ӻ��ļĴ�������ֵ
	MI2C_Bus_Write(vaule);								// Data д��I2C�ӻ�������
	MI2C_Bus_Stop();										// I2C Stop
    return;
}
void MKCM_Write2Byte(										// Write 2 bytes to DA32C. д��2���ֽ�
					BYTE address,    						// Index. �Ĵ�������ֵ
					WORD vaule){      						// 16 bit Data. 16λ����
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	MI2C_Bus_Write(address);							// Index. д��I2C�ӻ��ļĴ�������ֵ
	MI2C_Bus_Write(vaule);								// Low byte data. д��I2C�ӻ��ĵ��ֽ�����
	MI2C_Bus_Write(vaule >> 8);							// High byte data. д��I2C�ӻ��ĸ��ֽ�����
	MI2C_Bus_Stop();										// I2C Stop
	return;
}
void MKCM_WriteXByte(										// Write buffer to DA32C. д�����ֽ�
					BYTE address,   						// Index. �Ĵ�������ֵ
					WORD length,    						// Length. ����
					BYTE* in_data){ 						// Buffer address. ���ݻ���
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	MI2C_Bus_Write(address);							// Index. д��I2C�ӻ��ļĴ�������ֵ
	do { 
		MI2C_Bus_Write(*in_data++);						// Write byte data. д��I2C�ӻ�������
	} while (--length != 0);
	MI2C_Bus_Stop();										// I2C Stop
	return;
}

BYTE MKCM_ReadRegister(										// Read byte from DA32C. ��ȡ8λ�ļĴ���
					BYTE address){     						// Index. �Ĵ�������ֵ
	BYTE gLocal_1 = 0;
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	if (!FKCM_I2C_Error){
		MI2C_Bus_Write(address);							// Index. д��I2C�ӻ��ļĴ�������ֵ
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
WORD MKCM_Read2Byte(										// Read word from DA32C. ��ȡ16λ�ļĴ���
					BYTE address){     						// Index. �Ĵ�������ֵ
    WORD g2Local_1 = 0;
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	if (!FKCM_I2C_Error){
		MI2C_Bus_Write(address);								// Index. д��I2C�ӻ��ļĴ�������ֵ
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
	if (length > 1){									// ��ֹֻ��һ��ʱg2Local_1��Ϊ0����������
		length = length - 1;							// Counter. ����һ�Σ������ֽ�ʹ��MI2C_Bus_Read(1);
	}
	do {													
	    *outData++ = MI2C_Bus_Read(0);					// Read 1 byte 
	} while (--length != 0);
    *outData = MI2C_Bus_Read(1);						// Read last byte 
	MI2C_Bus_Stop();										// I2C Stop
}

void MKCM_ReadXByte(										// Read buffer from DA32C. ��ȡ����ֽ�
					BYTE address,      						// Index. �Ĵ�������ֵ
					WORD length,      						// Length. ����
					BYTE* outData){   						// Buffer address. ���ݻ���
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	MI2C_Bus_Write(address);								// Index. д��I2C�ӻ��ļĴ�������ֵ

	MI2C_Bus_Start();									    // I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS+1);						    // KCM I2C read address
    MKCM_MutilRead(length, outData);
	return;
}
WORD MKCM_ReadAutoByte(										// ��ȡ���ֽ�ָʾ���ȵĶ��ֽ�
					BYTE address,      						// Index. �Ĵ�������ֵ
					WORD limit,      						// limit. ����������
					BYTE* outData){   						// Buffer address. ���ݻ���
	WORD length;
	MI2C_Bus_Start();										// I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS);							// KCM I2C address
	MI2C_Bus_Write(address);								// Index. д��I2C�ӻ��ļĴ�������ֵ

	MI2C_Bus_Start();									    // I2C Start
	MI2C_Bus_Write(cI2C_ADDRESS+1);						    // KCM I2C read address
	length = MI2C_Bus_Read(0);
    if (length > limit){
        length = limit;
    }
    MKCM_MutilRead(length, outData);
	return length;
}

    
void MI2C_Bus_Write(BYTE gLocal_1){      					// ��I2C����д��һ���ֽڣ�gLocalΪ��д����
    BOOL FLocal_1;											// ���سɹ�/ʧ�ܱ�־
    BYTE gLocal_2;											// λ������
      
    gLocal_2 = 8;											// 8λ����λ�ȳ�
    do { 
        HAL_KCM_I2C_SCL(0);									// �õ�I2Cʱ���ߣ��Ըı�����
        if (gLocal_1 & 0x80){								// �������λΪ1
            HAL_KCM_I2C_SDA(1);								// �ø�I2C������
        }
        else {												// �������λΪ0
            HAL_KCM_I2C_SDA(0);								// �õ�I2C������
        }
	    MI2C_400K_DELAY();									// ����400Kbps�ı�׼��ʱ��������Ч�����ͨѶ���ٶ�
        gLocal_1 <<= 1;										// ׼����һλ����
        HAL_KCM_I2C_SCL(1);									// �ø�I2Cʱ���ߣ����������ȶ�
	    MI2C_400K_DELAY();									// ����400Kbps�ı�׼��ʱ��������Ч�����ͨѶ���ٶ�
    } while (--gLocal_2 != 0);								// ���8λ����
    HAL_KCM_I2C_SCL(0);										// �õ�I2Cʱ���ߣ�׼��ACKλ
    HAL_KCM_I2C_SDA(1);										// ��SDA����Ϊ����
    MI2C_100K_DELAY();										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
    HAL_KCM_I2C_SCL(1);										// �ø�I2Cʱ����
    MI2C_100K_DELAY();										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
    if ((HAL_KCM_I2C_IN_SDA())){							// ��ΪNAK
        FKCM_I2C_Error = 1;                                 // NAK��־��ʾ�ӻ�û��Ӧ�𣬿�����Ӳ������
    }
    HAL_KCM_I2C_SCL(0);                                            
    MI2C_100K_DELAY();										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
    return;                                        			// ����cI2C_NCK/cI2C_ACK��־
}
BYTE MI2C_Bus_Read(BOOL FLocal_NAK){						// ��I2C���߶�һ���ֽڣ������һ�ζ�ʱ��FLocal_NAK����Ϊ1
    BYTE gLocal_1;											// �����ݴ��� 
    BYTE gLocal_2;											// λ������

    HAL_KCM_I2C_SDA(1);                                     // ��SDA����Ϊ����
    MI2C_100K_DELAY();										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
    gLocal_2 = 8;											// 8λ
    do{														
        HAL_KCM_I2C_SCL(1);									// �ø�I2Cʱ���ߣ�ʱ����Ϊ�ߵ�ƽʱ���ݷ���Ч
        gLocal_1 <<= 1;										// ������һλ����0λΪ0
        if ((HAL_KCM_I2C_IN_SDA())){
        	gLocal_1 |= 0x01;                				// ���������Ϊ�ߵ�ƽ���������ݴ����ĵ�0λ��1
	    }
	    MI2C_400K_DELAY();									// ����400Kbps�ı�׼��ʱ��������Ч�����ͨѶ���ٶ�
        HAL_KCM_I2C_SCL(0);									// �õ�I2Cʱ���ߣ�������һλ
	    MI2C_400K_DELAY();									// ����400Kbps�ı�׼��ʱ��������Ч�����ͨѶ���ٶ�
    } while (--gLocal_2 != 0);								// ѭ��8�Σ����һ���ֽڵĽ���
      
    if (!FLocal_NAK){										// ���ACK��־Ϊ0
        HAL_KCM_I2C_SDA(0);									// �õ������ߣ�ACKΪ0��������ȡ
    }
    else {													// ���ACK��־Ϊ1
        HAL_KCM_I2C_SDA(1); 								// �ø������ߣ�ACKΪ1����ȡ����
    }
    MI2C_100K_DELAY();										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
    HAL_KCM_I2C_SCL(1);
    MI2C_100K_DELAY();										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
    HAL_KCM_I2C_SCL(0);
    return gLocal_1;
} 
void MI2C_Bus_Start(){  									// I2C���߿�ʼ
    HAL_KCM_I2C_SDA(1);                   					// �ø������ߣ������߿���
    HAL_KCM_I2C_SCL(1);                   					// �ø�ʱ���ߣ�ʱ���߿���
    MI2C_100K_DELAY();										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
	FKCM_I2C_Error = 0;                                     // �ٶ�����ȷ��
    if (!(HAL_KCM_I2C_IN_SDA())){							// ���Ϊ�ͣ�����Ӳ����������
	    FKCM_I2C_Error = 1;                                 // ������
    }
    HAL_KCM_I2C_SDA(0);										// ��������ʱ��Ϊ�ߵ�ƽʱ�ɸߵ�ƽ��͵�ƽ�л���ʾ��ʼ
    MI2C_100K_DELAY();										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
    HAL_KCM_I2C_SCL(0);										// �õ�ʱ���ߣ�׼�����ջ�������
    return;
}
void MI2C_Bus_Stop(){										// I2C����ֹͣ
    HAL_KCM_I2C_SDA(0);                   					// �õ������ߣ�׼��ֹͣ
    MI2C_100K_DELAY();										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
    HAL_KCM_I2C_SCL(1);										// �ø�ʱ����
    MI2C_100K_DELAY();										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
    HAL_KCM_I2C_SDA(1); 									// ��������ʱ��Ϊ�ߵ�ƽʱ�ɵ͵�ƽ��ߵ�ƽ�л���ʾ����
    return;
}
void MI2C_100K_DELAY(){										// ����100Kbps�ı�׼��ʱ����ȷ��״̬�ȶ�
	BYTE gLocal_1;
	gLocal_1 = 10;
	while (--gLocal_1 != 0);
}
void MI2C_400K_DELAY(){										// ����400Kbps�ı�׼��ʱ��������Ч�����ͨѶ���ٶ�
}
