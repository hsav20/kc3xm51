
#ifndef __KC3X_TYPE_H
#define __KC3X_TYPE_H


typedef enum {
	KCM_READ_IRQ = 0x01,				// ���ж�����Ĵ�����16λ�Ĵ���
	KCM_CLEAR_IRQ = 0x03,				// ����ж�����Ĵ�����16λ�Ĵ���
	KCM_POWER_ON = 0x05,				// ģ���ϵ翪��������ͬ���������������Ĵ���
	KCM_VOLUME_MUTE = 0x08,				// ��Ƶ�����������Ӽ�����
	KCM_TEST_TONE = 0x09,				// ��������
	KCM_SRC_DETECT = 0x0a,				// ���������Ч����Դһ��
	KCM_SRC_FORMAT = 0x18,				// �����ź������ʽָʾ
	KCM_SRC_CHANNEL = 0x19,				// �����ź�����ͨ����Ϣ��������ָʾ
	KCM_SRC_RATE = 0x1a,				// �����ź���������ʼ�ʵ�ʲ��Ų�����ָʾ
	KCM_SRC_BPS = 0x1b,					// �����ź�����������ָʾ
	KCM_SRC_VALID = 0x1c,				// ��Ч����Դ����ı䣬16λ�Ĵ���
	KCM_WORK_STATUS = 0x1f,				// ģ�鹤��/����״ָ̬ʾ

	// ����Ϊ���ϵ����ļĴ���
	KCM_INPUT_SOURCE = 0x20,			// ������Դѡ��
	KCM_INPUT_VIDEO = 0x21,				// ������ƵԴѡ��
	KCM_DYN_COMPRES = 0x23,				// �ű����붯̬ѹ��
	KCM_SPK_CONFIG = 0x24,				// ��������
	KCM_LPF_FREQ = 0x25,				// ������ͨ��LPF��ͨ�˲���Ƶ��
	KCM_HPF_FREQ = 0x26,				// ������С����HPF��ͨ�˲���Ƶ��
	KCM_LIP_SYNC_SET = 0x28,			// ����ͬ���ӳ�ʱ�䣬�����Ի�����������ͬ��
	KCM_LIP_SYNC_MAX = 0x29,			// ����ͬ�������ӳ�ʱ��
	KCM_LISTEN_MODE = 0x2b,				// ����ģʽѡ��
	KCM_EQ_SELECT = 0x2c,				// ��·������Ч����ѡ��
	KCM_VOLUME_MAX = 0x2e,				// �����������ֵ
	KCM_VOLUME_CTRL = 0x2f,				// ����ֵ����
	KCM_FL_TRIM = 0x30,					// ǰ��������΢�� B4=1Ϊ��dB B3-B0ΪdB��ֵ
	KCM_FR_TRIM = 0x31,					// ǰ��������΢�� B4=1Ϊ��dB B3-B0ΪdB��ֵ
	KCM_CE_TRIM = 0x32,					// ��������΢��	  B4=1Ϊ��dB B3-B0ΪdB��ֵ
	KCM_SW_TRIM = 0x33,					// ����������΢�� B4=1Ϊ��dB B3-B0ΪdB��ֵ
	KCM_SL_TRIM = 0x34,					// ����������΢�� B4=1Ϊ��dB B3-B0ΪdB��ֵ
	KCM_SR_TRIM = 0x35,					// ����������΢�� B4=1Ϊ��dB B3-B0ΪdB��ֵ
	KCM_BL_TRIM = 0x36,					// ����������΢�� B4=1Ϊ��dB B3-B0ΪdB��ֵ
	KCM_BR_TRIM = 0x37,					// ����������΢�� B4=1Ϊ��dB B3-B0ΪdB��ֵ
	KCM_MIC_MIXER = 0x38,				// ��Ͳ�������������ϳɱ���
	KCM_MIC_VOLUME = 0x39,				// ��Ͳ1����Ͳ2��������
	KCM_MIC_ECHO = 0x3a,				// ��Ͳֱ��������������
	KCM_MIC_DELAY = 0x3b,				// ��Ͳ�ӳ�ʱ�估�ظ�����
	KCM_MIC_REVERB = 0x3c,				// ��Ͳ����1������2����
	KCM_MIC_WHISTLE = 0x3d,				// ��ͲХ����������ģʽ
	KCM_EXTR_MEMORY = 0x40,				// ��չ���û������ĵ������ռ䣬0x40-0x7f��64�ֽڣ����ϵ���䣩

	// ����Ϊ���ֽڶ�д�Ĵ�������Щ�Ĵ��������Զ����ӼĴ���������ֵ
	KCM_CUSTOM_CODE = 0x80,				// �����û��Զ��幦�ܼĴ���
	KCM_RD_INFO = 0x81,					// ��ȡģ����Ϣ�Ĵ���
	KCM_FW_UPGRADE = 0x82,				// ����ģ��̼��Ĵ���
	KCM_RD_RAM = 0x83,					// ��ȡָ����ַ��RAM����
	KCM_MAX_DELAY = 0x86,				// ��ȡ�������������õ��ӳ�ʱ��
	KCM_DELAY_TIME = 0x87,				// ���������������ӳ�
	KCM_PROGUCE_SIGNAL = 0x88,			// ģ���ڲ��������ź�����
	KCM_EQ_SETUP = 0x8b,				// ���EQ������Ч��������
	KCM_EQ_VALUE = 0x8c,				// ���EQ������Ч������ֵ
	KCM_MIC_ADJ_MAX = 0x8d,				// ��Ͳ���ֲ������ֵ����
	KCM_WR_SPECTRUM = 0x90,				// ����Ƶ��ģʽ
	KCM_RD_SPECTRUM = 0x91,				// Ƶ����ֵ��ȡ
	KCM_WR_FLASH = 0x94,				// д��512�ֽ�FLASH�������ռ䣬��ƫ����������
	KCM_WR_GET_FLASH = 0x95,			// ׼����ȡ512�ֽ�FLASH�������ռ䣬д��ƫ����
	KCM_RD_FLASH = 0x96,				// ��ȡ512�ֽ�FLASH�������ռ�
	KCM_APP_COMMAND = 0x98,				// ��ȡ�ֻ�/Զ��APP����ָ����ֽ�

	KCM_PLAY_SD_QTY = 0xa0,				// ��ȡSD����ý���ļ�����������2�ֽ�
	KCM_PLAY_UDISK_QTY = 0xa1,			// ��ȡU�̶�ý���ļ�����������2�ֽ�
	KCM_PLAY_FILE_TIME = 0xa2,			// ��ȡ���ڲ����ļ�����ʱ�䣬��2�ֽڵ�λ��
	KCM_PLAY_TIME = 0xa3,				// ��ȡ���ڲ��ŵ�ʵ��ʱ�䣬��2�ֽڵ�λ��
	KCM_PLAY_INDEX = 0xa4,				// ��ȡ/д���ļ����ű�ţ���2�ֽ�
	KCM_PLAY_STATE = 0xa5,				// ��ȡ/д���ļ�����״̬����1�ֽ�
	KCM_PLAY_OPERATE = 0xa6,			// ��ȡ/д���ļ����ſ��ƣ���1�ֽ�
	KCM_PLAY_FILE_NAME = 0xa7,			// ��ȡ��ǰ��ý���ļ���/�����������32�ֽ�
	KCM_REGISTER_LAST = 0xaf
} KC3X_REGISTER;

typedef enum {
	KCM_IRQ_SYSTEM_INIT = 0x0001,       // ģʽ��ʼ������жϣ����Զ�ȡһЩ�Ĵ����ָ����صļ���
	KCM_IRQ_FORMAT_INFO = 0x0002,       // �����źŸ�ʽ�ı��жϣ���Ҫ��ȡ"KCM_SRC_FORMAT"�Ĵ���
	KCM_IRQ_VOLUME = 0x0004,            // �������ڸı��жϣ���Ҫ��ȡ"KCM_VOLUME_CTRL"�Ĵ�����ȡ���µ�����ֵ
	KCM_IRQ_SRC_VALID = 0x0008,        	// ��Ч����Դ����ı��жϣ���Ҫ��ȡ"KCM_SRC_VALID"�Ĵ���

	KCM_IRQ_FIRMWARE = 0x10,        	// �̼����£���Ҫ��ȡ"KCM_RD_INFO"�Ĵ���
	KCM_IRQ_PLAY_STATE = 0x0020,       	// ��ý���ļ�����״̬�ı䣬��Ҫ��ȡ"KCM_PLAY_STATE"�Ĵ���
	KCM_IRQ_PLAY_TIME = 0x0040,        	// ��ý�岥��ʱ��ı䣬��Ҫ��ȡ"KCM_PLAY_TIME"�Ĵ���
	KCM_IRQ_APP_COMMAND = 0x0080,  		// �յ��ֻ�/Զ��APP����ָ���Ҫ��ȡ"KCM_APP_COMMAND"�Ĵ���

	KCM_IRQ_EX_FONT = 0x0400			// �ֿ��ȡ����ж�
} KC3X_IRQ_TYPE;
typedef enum {
	KCM_SET_STANDBY = 0x00,       		// ����ģ��������״̬���ɹ�������״̬�ı�ΪKCM_IS_STANDBY
	KCM_SET_POWER_ON = 0x01,       		// ����ģ�������������״̬���ɹ�������״̬�ı�ΪKCM_IS_POWER_ON
	KCM_SET_REBOOT = 0x55,            	// ����ģ���������ɹ�������״̬�ı�ΪKCM_SET_POWER_ON
	KCM_IS_STANDBY = 0x80,        		// ��ȡ��ģ��������״̬
	KCM_IS_POWER_ON = 0x81        		// ��ȡ��ģ�������������״̬
} KC3X_POWER_ON;

typedef enum {
	KCM_SRC_NOS = 0x00,					// ����û���ź�
	KCM_SRC_PCM = 0x01,					// PCM�ź�����
	KCM_SRC_AC3 = 0x02,					// ��׼��AC3�ź�����
	KCM_SRC_DTS = 0x03,					// ��׼��DTS/DTS-CD�ź�����
	KCM_SRC_AAC = 0x04,					// AAC�ź�����
	KCM_SRC_MPEG2 = 0x05,				// MPEG2�������ź�����
	KCM_SRC_DSD = 0x06,					// DSD�ź�����
	KCM_SRC_MP3 = 0x07,					// MP3�ź�����
	KCM_SRC_SBC = 0x08,					// SBC�����ź�����
	KCM_SRC_LPCM = 0x11,				// LPCM�ź�����
	KCM_SRC_HDCD = 0x21,				// HD-CD�ź�����
	KCM_SRC_EAC3 = 0x12,				// Enhanced AC-3�ź�����
	KCM_SRC_TRUEHD = 0x22,				// �ű�TRUE HD�ź�����
	KCM_SRC_MLP = 0x32,					// DVD AUDIO MLP�ź�����
	KCM_SRC_DTS_ES = 0x13,				// DTS Extended Surround�ź�����
	KCM_SRC_DTS_MA = 0x23,				// DTS HD Master Audio�ź�����
	KCM_SRC_DTS_HRA = 0x33				// DTS HD High Resolution Audio�ź�����
} KC3X_SRC_TYPE;

typedef enum {
	KCM_INPUT_ANALOG = 0x00,			// ��Դѡ��ģ������
	KCM_INPUT_DIGITAL = 0x10,			// ��Դѡ����������
	KCM_INPUT_RX1 = 0x10,				// ��Դѡ��RX1����
	KCM_INPUT_RX2 = 0x11,				// ��Դѡ��RX2����
	KCM_INPUT_RX3 = 0x12,				// ��Դѡ��RX3����
	KCM_INPUT_HDMI = 0x20,				// ��Դѡ��HDMI����
	KCM_INPUT_HDMI1 = 0x20,				// ��Դѡ��HDMI1����
	KCM_INPUT_HDMI2 = 0x21,				// ��Դѡ��HDMI2����
	KCM_INPUT_HDMI3 = 0x22,				// ��Դѡ��HDMI3����
	KCM_INPUT_ARC = 0x24,				// ��Դѡ��HDMI ARC����
	KCM_INPUT_MEDIA = 0x30,				// ��Դѡ��U�̡�TF����������WIFI��USB���������������
	KCM_INPUT_SD = 0x30,				// ��Դѡ��SD/TF������
	KCM_INPUT_UDISK = 0x31,				// ��Դѡ��U������
	KCM_INPUT_PC = 0x32,				// ��Դѡ��USB��������
	KCM_INPUT_E8CH = 0x33,				// ��Դѡ������7.1��������
	KCM_INPUT_BT = 0x34,				// ��Դѡ����������
	KCM_INPUT_SIGNAL = 0x40				// ��Դѡ���ڲ������ź�
} KC3X_INPUT_TYPE;

typedef enum {
	KCM_SRC_VALID_ANALOG = 0x0001,		// ���źŵ���Դ���룺ģ������
	KCM_SRC_VALID_RX1 = 0x0002,			// ���źŵ���Դ���룺����1
	KCM_SRC_VALID_RX2 = 0x0004,			// ���źŵ���Դ���룺����2
	KCM_SRC_VALID_RX3 = 0x0008,			// ���źŵ���Դ���룺����3

	KCM_SRC_VALID_SD = 0x0020,			// ���ļ�����Դ���룺SD����
	KCM_SRC_VALID_UDISK = 0x0040,		// ���ļ�����Դ���룺U�̲���
	KCM_SRC_VALID_MIC = 0x0080,			// ���źŵ���Դ���룺��Ͳ����
	
	KCM_SRC_VALID_HDMI1 = 0x0100,		// ���źŵ���Դ���룺HDMI1
	KCM_SRC_VALID_HDMI2 = 0x0200,		// ���źŵ���Դ���룺HDMI2
	KCM_SRC_VALID_HDMI3 = 0x0400,		// ���źŵ���Դ���룺HDMI3
	KCM_SRC_VALID_HDMIS = 0x0f00,		// HDMIλ����

	KCM_SRC_VALID_USBA = 0x1000,		// ���źŵ���Դ���룺USB����
	KCM_SRC_VALID_E8CH = 0x2000,		// ���źŵ���Դ���룺����7.1����
	KCM_SRC_VALID_BT = 0x4000,			// ���źŵ���Դ���룺������Ƶ
	KCM_SRC_VALID_WIFI = 0x8000			// ���źŵ���Դ���룺WIFI��Ƶ
} KC3X_SRC_VALID;


typedef enum {
	KCM_ERROR_EQ = 0x80,				// B7���EQ������Ч��ʼ�������������ܳ�������Ԥ��ƶ���;
	KCM_ERROR_MIC = 0x20,				// B5��Ͳ����оƬӲ����ʼ��������;
	KCM_ERROR_VOLUME = 0x10,			// B4ģ���������оƬӲ����ʼ��������;
	KCM_ERROR_CODEC = 0x08,				// B3ģ��ADC/DACӲ����ʼ��������;
	KCM_ERROR_SPDIF = 0x04,				// B2ģ��SPDIF�ӿ�оƬ��ʱ�Ӳ�����;
	KCM_ERROR_TEMP1 = 0x02,				// B1:0ģ���¶ȷ�Χ
	KCM_ERROR_TEMP0 = 0x01				// B1:0ģ���¶ȷ�Χ
} KC3X_ERROR_FLAG;

typedef enum {
	KCM_MODEL_32C = 0x31,				// ģ���ͺ�KC32C
	KCM_MODEL_33A = 0x37,				// ģ���ͺ�KC33A
	KCM_MODEL_35H = 0x53,				// ģ���ͺ�KC35H
	KCM_MODEL_36H = 0x56				// ģ���ͺ�KC36H
} KC3X_MODEL_TYPE;

#define KC3X_STATE_PLAY_END				0x00				// ����д�룬��ȡΪ�ļ�������ɣ��Ѿ�ֹͣ
#define KC3X_STATE_PLAY_PAUSE			0x01				// д����ͣ����ȡΪ�Ѿ���ͣ
#define KC3X_STATE_PLAY_PLAY			0x02				// д�벥�ţ���ȡΪ�Ѿ�����
#define KC3X_STATE_PLAY_STOP			0x03				// д��ֹͣ����ȡΪ�Ѿ�ֹͣ
#define KC3X_STATE_PLAY_FLAG			0x03				// �޸Ĳ��ű�־
#define KC3X_STATE_RANDOM_NONE  		0x00				// ������Źرգ���ý���ļ�����״̬
#define KC3X_STATE_RANDOM_PATH			0x10				// ������ŵ�ǰ�ļ��У���ý���ļ�����״̬
#define KC3X_STATE_RANDOM_ALL 			0x20				// �������ȫ������ý���ļ�����״̬
#define KC3X_STATE_RANDOM_FLAG			0x30				// �޸�RANDOM��־
#define KC3X_STATE_REPEAT_NONE  		0x00				// ��ý���ļ�����û���ظ�
#define KC3X_STATE_REPEAT_FILE  		0x40				// ��ý���ļ������ظ���ǰ�ļ�
#define KC3X_STATE_REPEAT_PATH  		0x80				// ��ý���ļ������ظ���ǰ�ļ���
#define KC3X_STATE_REPEAT_ALL  			0xc0				// ��ý���ļ������ظ������ļ�
#define KC3X_STATE_REPEAT_FLAG			0xc0				// �޸�REPEAT��־

typedef enum {
	KCM_OPERATE_SKIP_UP = 0x01,			// ����ǰһ���ļ�
	KCM_OPERATE_SKIP_DOWN = 0x02,		// ���ź�һ���ļ�
	KCM_OPERATE_FAST_FORW = 0x03,		// ��ý�岥�ſ��
	KCM_OPERATE_FAST_BACK = 0x04		// ��ý�岥�ſ���
} KC3X_PLAY_OPERATE;

typedef enum {
	KCM_VOL_CHIP_NONE = 0x00,			// ��ʹ��ģ���ڲ�������
	KCM_VOL_CHIP_DSP_05 = 0x01,			// ʹ��DSP����������ÿ��0.5dB
	KCM_VOL_CHIP_DSP_10 = 0x02,			// ʹ��DSP����������ÿ��1dB
	KCM_VOL_CHIP_PT2258 = 0x03,			// Ϊʹ��PT2258 + PT2259���߼��ݵ�����оƬ
	KCM_VOL_CHIP_M62446 = 0x04,	 		// ʹ��M62446���߼��ݵ�����оƬ
	KCM_VOL_CHIP_CS3318 = 0x05			// ʹ��CS3318���߼��ݵ�����оƬ
} KC3X_VOLUME_CHIP;

#define CUSTOM_ENA_DOWNMIX				0x00000100			// ������������������»�ģʽ����������ģʽ
#define CUSTOM_ENA_BASSMANAGE			0x00000200 			// �����������ü���������
#define CUSTOM_ENA_DELAY_LIN			0x00000400 			// ����������ӳ�ʱ�估����ͬ��
#define CUSTOM_ENA_VOL_TRIM				0x00000800 			// �������������������΢��
#define CUSTOM_ENA_TONE_EQ				0x00001000 			// �������������EQ������Ч
#define CUSTOM_ENA_SPECTRUM				0x00008000 			// �����ƽȡ����Ƶ�����

#define CUSTOM_SWAP_CHANNEL				0x00070000			// �����������
#define CUSTOM_6CH_TO_8CH				0x00080000			// �����Ϊ5.1��ϵͳ֮��ʹ��7.1����
#define CUSTOM_VOLUME_CHIP				0x00700000			// ����оƬ����
#define CUSTOM_IN_EACH_MODE				0x00800000			// Ϊÿ������ͨ��������������ģʽ�����EQ������Чѡ��
#define CUSTOM_TRACK_MODE				0x00800000			// Ϊÿ������ͨ��������������ģʽ�����EQ������Чѡ��
#define CUSTOM_MIC_LTRT_SWAP			0x08000000			// ��ͲMIC��ģ�����뽻��
#define CUSTOM_MIC_ENABLE				0x10000000			// ��Ͳ������Ϲ���
#define CUSTOM_BCK_WCK_INPUT			0x20000000			// �����������ʱ���������ѡ��
#define CUSTOM_I2C_DATA					0xc0000000			// �������������ʽ����ѡ��


#endif		// __KC3X_TYPE_H

