
#ifndef __KC3X_TYPE_H
#define __KC3X_TYPE_H


typedef enum {
	KCM_READ_IRQ = 0x01,				// ���ж�����Ĵ�����16λ�Ĵ���
	KCM_CLEAR_IRQ = 0x03,				// ����ж�����Ĵ�����16λ�Ĵ���
	KCM_POWER_ON = 0x05,				// �û������ϵ�	
	KCM_VOLUME_MUTE = 0x08,				// ��Ƶ�����������Ӽ�����
	KCM_TEST_TONE = 0x09,				// ��������
	KCM_SRC_DETECT = 0x0a,				// ���������Ч����Դһ��
	KCM_SRC_FORMAT = 0x18,				// �����ź������ʽ��ͨ����Ϣָʾ
	KCM_BPS_RATE = 0x1a,				// ����Ƶ�ʼ�������ָʾ
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
	KCM_MIC_DELAY = 0x38,				// ��Ͳ�ӳ�ʱ�䣬ÿ��20����
	KCM_MIC_VOLUME = 0x39,				// ��Ͳ1����Ͳ2��������
	KCM_MIC_ECHO_EQ = 0x3a,				// ��Ͳ������������Ͳ���EQ������Ч����ѡ��
	KCM_MIC_REPEAT = 0x3b,				// ��Ͳ�ظ���ֱ��������
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
	KCM_MIC_EQ_VALUE = 0x8d,			// ��Ͳ���EQ������Ч������ֵ
	KCM_WR_SPECTRUM = 0x8e,				// ����Ƶ��ģʽ
	KCM_RD_SPECTRUM = 0x8f,				// Ƶ����ֵ��ȡ
	KCM_WR_FLASH = 0x90,				// д��512�ֽ�FLASH�������ռ䣬�����Ե���д��
	KCM_RD_FLASH = 0x91,				// ��ȡ512�ֽ�FLASH�������ռ䣬�����Ե�����ȡ
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
	KCM_IRQ_SYSTEM_INIT = 0x0001,       // ģʽ��ʼ������жϣ���Ҫд��"KCM_POWER_ON"�Ĵ�����
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
	KCM_SRC_NOS = 0x00,					// ����û���ź�
	KCM_SRC_ANA = 0x01,					// ����ģ���ź�
	KCM_SRC_PCM = 0x02,					// ����PCM�ź�
	KCM_SRC_AC3 = 0x03,					// ����AC-3�ź�
	KCM_SRC_DTS = 0x04,					// ����DTS�ź�
	KCM_SRC_DTS_CD = 0x05,				// ����DTS CD�ź�
	KCM_SRC_AAC = 0x06,					// ����AAC�ź�
	KCM_SRC_LPCM = 0x07,				// ����LPCM�ź�
	KCM_SRC_HDCD = 0x08,				// ����HD-CD�ź�
	KCM_SRC_DSD = 0x09,					// ����DSD�ź�
	KCM_SRC_MP3 = 0x0a,					// ����MP3�ź�
	KCM_SRC_E_AC3 = 0x10,				// ����Enhanced AC-3�ź�
	KCM_SRC_AC3_HD = 0x11,				// ����ű�TRUE HD�ź�
	KCM_SRC_DTS_ES = 0x14,				// ����DTS Extended Surround�ź�
	KCM_SRC_DTS_HRA = 0x15,				// ����DTS HD High Resolution Audio�ź�
	KCM_SRC_DTS_MA = 0x16				// ����DTS HD Master Audio�ź�
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
	KCM_INPUT_BT = 0x33,				// ��Դѡ����������
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
	KCM_FREQ_8K = 0x01,					// �ź�ԴƵ��8KHz
	KCM_FREQ_11K = 0x02,				// �ź�ԴƵ��11.025KHz
	KCM_FREQ_12K = 0x03,				// �ź�ԴƵ��12KHz
	KCM_FREQ_16K = 0x04,				// �ź�ԴƵ��16KHz
	KCM_FREQ_22K = 0x05,				// �ź�ԴƵ��22.05KHz
	KCM_FREQ_24K = 0x06,				// �ź�ԴƵ��24KHz
	KCM_FREQ_32K = 0x07,				// �ź�ԴƵ��32KHz
	KCM_FREQ_44K = 0x08,				// �ź�ԴƵ��44.1KHz
	KCM_FREQ_48K = 0x09,				// �ź�ԴƵ��48KHz
	KCM_FREQ_64K = 0x0a,				// �ź�ԴƵ��64KHz
	KCM_FREQ_88K = 0x0b,				// �ź�ԴƵ��88.2KHz
	KCM_FREQ_96K = 0x0c,				// �ź�ԴƵ��96KHz
	KCM_FREQ_128K = 0x0d,				// �ź�ԴƵ��128KHz
	KCM_FREQ_176K = 0x0e,				// �ź�ԴƵ��176.4KHz
	KCM_FREQ_192K = 0x0f				// �ź�ԴƵ��192KHz
} KC3X_INPUT_FREQ;

typedef enum {
	KCM_MODEL_32C = 0x31,				// ģ���ͺ�KC32C
	KCM_MODEL_35H = 0x53,				// ģ���ͺ�KC35H
	KCM_MODEL_36H = 0x56				// ģ���ͺ�KC36H
} KC3X_MODEL_TYPE;

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
	KCM_VOL_CHIP_DSP = 0x01,			// ʹ��ģ���ڲ���DSP��������
	KCM_VOL_CHIP_DAC = 0x02,			// ʹ��ģ���ڲ���DAC��������
	KCM_VOL_CHIP_PT2258 = 0x03,			// Ϊʹ��PT2258 + PT2259���߼��ݵ�����оƬ
	KCM_VOL_CHIP_M62446 = 0x04			// ʹ��M62446���߼��ݵ�����оƬ
} KC3X_VOLUME_CHIP;

#define CUSTOM_SWAP_CHANNEL				0x00070000			// �����������
#define CUSTOM_6CH_TO_8CH				0x00080000			// �����Ϊ5.1��ϵͳ֮��ʹ��7.1����
#define CUSTOM_VOLUME_CHIP				0x00700000			// ����оƬ����
#define CUSTOM_IN_EACH_MODE				0x00800000			// Ϊÿ������ͨ��������������ģʽ�����EQ������Чѡ��
#define CUSTOM_MIC_LTRT_SWAP			0x08000000			// ��ͲMIC��ģ�����뽻��
#define CUSTOM_MIC_ENABLE				0x10000000			// ʹ�û�Ͳ������Ϲ���
#define CUSTOM_BCK_WCK_INPUT			0x80000000			// BCK��WCK�˿�Ϊ����


#endif		// __KC3X_TYPE_H