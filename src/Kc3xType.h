
#ifndef __KC3X_TYPE_H
#define __KC3X_TYPE_H


typedef enum {
	KCM_READ_IRQ = 0x01,				// 读中断请求寄存器，16位寄存器
	KCM_CLEAR_IRQ = 0x03,				// 清除中断请求寄存器，16位寄存器
	KCM_POWER_ON = 0x05,				// 用户主机上电	
	KCM_SRC_FORMAT = 0x06,				// 数码信号输入格式及通道信息指示
	KCM_BPS_RATE = 0x07,				// 采样频率及码流率指示
	KCM_VOLUME_MUTE = 0x08,				// 音频静音及音量加减控制
	KCM_TEST_TONE = 0x09,				// 噪音测试
	KCM_WIFI_STATUS = 0x0e,				// WIFI状态指示
	KCM_PLAY_OPERATE = 0x0f,			// 控制多媒体文件播放模式
	KCM_PLAY_INDEX = 0x10,				// 控制多媒体文件播放编号，16位寄存器
	KCM_PLAY_TIME = 0x12,				// 读取多媒体文件正在播放的时间，16位寄存器
	KCM_SRC_VALID = 0x1c,				// 有效的音源输入改变，16位寄存器
	KCM_SRC_DETECT = 0x1f,				// 检测所有有效的音源一次

	KCM_INPUT_SOURCE = 0x20,			// 输入音源选择（带上电记忆）
	KCM_INPUT_VIDEO = 0x21,				// 输入视频源选择（带上电记忆）
	KCM_DYN_COMPRES = 0x23,				// 杜比数码动态压缩（带上电记忆）
	KCM_SPK_CONFIG = 0x24,				// 喇叭设置（带上电记忆）
	KCM_LPF_FREQ = 0x25,				// 超低音通道LPF低通滤波器频率（带上电记忆）
	KCM_HPF_FREQ = 0x26,				// 主声道小喇叭HPF高通滤波器频率（带上电记忆）
	KCM_LIP_SYNC_SET = 0x28,			// 齿音同步延迟时间，修正对画面与声音不同步（带上电记忆）
	KCM_LIP_SYNC_MAX = 0x29,			// 齿音同步最大的延迟时间（带上电记忆）
	KCM_LISTEN_MODE = 0x2b,				// 聆听模式选择（带上电记忆）
	KCM_EQ_SELECT = 0x2c,				// 多路均衡音效处理选择（带上电记忆）
	KCM_VOLUME_MAX = 0x2e,				// 设置音量最大值（带上电记忆）
	KCM_VOLUME_CTRL = 0x2f,				// 音量值设置（带上电记忆）
	KCM_FL_TRIM = 0x30,					// 前置左声道微调 B4=1为减dB B3-B0为dB数值（带上电记忆）
	KCM_FR_TRIM = 0x31,					// 前置右声道微调 B4=1为减dB B3-B0为dB数值（带上电记忆）
	KCM_CE_TRIM = 0x32,					// 中置声道微调	  B4=1为减dB B3-B0为dB数值（带上电记忆）
	KCM_SW_TRIM = 0x33,					// 超低音声道微调 B4=1为减dB B3-B0为dB数值（带上电记忆）
	KCM_SL_TRIM = 0x34,					// 环绕左声道微调 B4=1为减dB B3-B0为dB数值（带上电记忆）
	KCM_SR_TRIM = 0x35,					// 环绕右声道微调 B4=1为减dB B3-B0为dB数值（带上电记忆）
	KCM_BL_TRIM = 0x36,					// 后置左声道微调 B4=1为减dB B3-B0为dB数值（带上电记忆）
	KCM_BR_TRIM = 0x37,					// 后置右声道微调 B4=1为减dB B3-B0为dB数值（带上电记忆）
	KCM_MIC_DELAY = 0x38,				// 话筒延迟时间，每步20毫秒
	KCM_MIC_VOLUME = 0x39,				// 话筒1及话筒2音量比例
	KCM_MIC_ECHO_EQ = 0x3a,				// 话筒回声比例及话筒多段EQ均衡音效处理选择
	KCM_MIC_REPEAT = 0x3b,				// 话筒重复及直达声比例
	KCM_MIC_REVERB = 0x3c,				// 话筒混响1及混响2比例
	KCM_MIC_WHISTLE = 0x3d,				// 话筒啸叫声音反馈模式
	KCM_EXTR_MEMORY = 0x40,				// 扩展给用户主机的掉电记忆空间，0x40-0x7f共64字节（带上电记忆）

	// 以下为多字节读写寄存器，这些寄存器不会自动增加寄存器的索引值
	KCM_CUSTOM_CODE = 0x80,				// 设置用户自定义功能寄存器
	KCM_RD_INFO = 0x81,					// 读取模块信息寄存器
	KCM_FW_UPGRADE = 0x82,				// 升级模块固件寄存器
	KCM_RD_RAM = 0x83,					// 读取指定地址的RAM内容
	KCM_MAX_DELAY = 0x86,				// 读取所有声道最大可用的延迟时间
	KCM_DELAY_TIME = 0x87,				// 设置所有声道的延迟
	KCM_PROGUCE_SIGNAL = 0x88,			// 模块内部产生的信号配置
	KCM_EQ_SETUP = 0x8b,				// 多段EQ均衡音效处理设置
	KCM_EQ_VALUE = 0x8c,				// 多段EQ均衡音效处理数值
	KCM_MIC_EQ_VALUE = 0x8d,			// 话筒多段EQ均衡音效处理数值
	KCM_WR_SPECTRUM = 0x8e,				// 设置频谱模式
	KCM_RD_SPECTRUM = 0x8f,				// 频谱数值读取
	KCM_WR_FLASH = 0x90,				// 写入512字节FLASH掉电记忆空间，不可以单独写入
	KCM_RD_FLASH = 0x91,				// 读取512字节FLASH掉电记忆空间，不可以单独读取
	KCM_RD_SD_QTY = 0x96,				// 读取多媒体文件正在播放的时间，共2字节单位秒
	KCM_RD_UDISK_QTY = 0x97,			// 读取多媒体文件正在播放的时间，共2字节单位秒
	KCM_RD_FILE_TIME = 0x99,			// 读取多媒体文件正在播放的总时间，共2字节单位秒
	KCM_COMMAND_RCV = 0xa0,				// 云通讯指令接收，0xa8-0xaf共8字节
	KCM_COMMAND_SEND = 0xa1				// 云通讯指令发送，0xb0-0xbf共16字节
} KC3X_REGISTER;

typedef enum {
	KCM_IRQ_SYSTEM_INIT = 0x0001,       // 模式初始化完成中断，需要写入"KCM_POWER_ON"寄存器，
	KCM_IRQ_FORMAT_INFO = 0x0002,       // 输入信号格式改变中断，需要读取"KCM_SRC_FORMAT"寄存器
	KCM_IRQ_VOLUME = 0x0004,            // 音量调节改变中断，需要读取"VOLUME_CTRL"寄存器获取更新的音量值
	KCM_IRQ_SRC_VALID = 0x0008,        	// 有效的音源输入改变中断，需要读取"KCM_SRC_VALID"寄存器

	KCM_IRQ_FIRMWARE = 0x0010,        	// 固件更新，需要读取"KCM_RD_INFO"寄存器
	KCM_IRQ_MEDIA_TIME = 0x0020,        // 多媒体播放时间改变，需要读取"KCM_PLAY_TIME"寄存器

	KCM_IRQ_WIFI_RCV = 0x0100,          // 接收到WIFI指令中断，需要读取"KCM_COMMAND_RCV"寄存器获取更新的音量值
	KCM_IRQ_WIFI_STATUS = 0x0200,		// WIFI状态指示变化扩展中断，需要读取"KCM_WIFI_STATUS"寄存器
	KCM_IRQ_EX_FONT = 0x0400			// 字库读取完成中断
} KC3X_IRQ_TYPE;

typedef enum {
	KCM_SRC_NOS = 0x00,					// 输入没有信号
	KCM_SRC_ANA = 0x01,					// 输入模拟信号
	KCM_SRC_PCM = 0x02,					// 输入PCM信号
	KCM_SRC_AC3 = 0x03,					// 输入AC-3信号
	KCM_SRC_DTS = 0x04,					// 输入DTS信号
	KCM_SRC_DTS_CD = 0x05,				// 输入DTS CD信号
	KCM_SRC_AAC = 0x06,					// 输入AAC信号
	KCM_SRC_LPCM = 0x07,				// 输入LPCM信号
	KCM_SRC_HDCD = 0x08,				// 输入HD-CD信号
	KCM_SRC_DSD = 0x09,					// 输入DSD信号
	KCM_SRC_MP3 = 0x0a,					// 输入MP3信号
	KCM_SRC_E_AC3 = 0x10,				// 输入Enhanced AC-3信号
	KCM_SRC_AC3_HD = 0x11,				// 输入杜比TRUE HD信号
	KCM_SRC_DTS_ES = 0x14,				// 输入DTS Extended Surround信号
	KCM_SRC_DTS_HRA = 0x15,				// 输入DTS HD High Resolution Audio信号
	KCM_SRC_DTS_MA = 0x16				// 输入DTS HD Master Audio信号
} KC3X_SRC_TYPE;

typedef enum {
	KCM_INPUT_ANALOG = 0x00,			// 音源选择模拟输入
	KCM_INPUT_DIGITAL = 0x10,			// 音源选择数码输入
	KCM_INPUT_HDMI = 0x20,				// 音源选择HDMI输入
	KCM_INPUT_MEDIA = 0x30,				// 音源选择U盘、TF卡、蓝牙、WIFI、USB声卡、网络等输入
	KCM_INPUT_SIGNAL = 0x40				// 音源选择内部产生信号
} KC3X_INPUT_TYPE;

typedef enum {
	KCM_SRC_VALID_ANALOG = 0x0001,		// 有信号的音源输入：模拟输入
	KCM_SRC_VALID_RX1 = 0x0002,			// 有信号的音源输入：数码1
	KCM_SRC_VALID_RX2 = 0x0004,			// 有信号的音源输入：数码2
	KCM_SRC_VALID_RX3 = 0x0008,			// 有信号的音源输入：数码3

	KCM_SRC_VALID_SD = 0x0020,			// 有文件的音源输入：SD插入
	KCM_SRC_VALID_UDISK = 0x0040,		// 有文件的音源输入：U盘插入
	KCM_SRC_VALID_MIC = 0x0080,			// 有信号的音源输入：话筒插入
	
	KCM_SRC_VALID_HDMI1 = 0x0100,		// 有信号的音源输入：HDMI1
	KCM_SRC_VALID_HDMI2 = 0x0200,		// 有信号的音源输入：HDMI2
	KCM_SRC_VALID_HDMI3 = 0x0400,		// 有信号的音源输入：HDMI3

	KCM_SRC_VALID_UCARD = 0x1000,		// 有信号的音源输入：USB声卡
	KCM_SRC_VALID_E8CH = 0x2000,		// 有信号的音源输入：外置7.1声道
	KCM_SRC_VALID_BT = 0x4000,			// 有信号的音源输入：蓝牙音频
	KCM_SRC_VALID_WIFI = 0x8000			// 有信号的音源输入：WIFI音频
} KC3X_SRC_VALID;


typedef enum {
	KCM_FREQ_8K = 0x01,					// 信号源频率8KHz
	KCM_FREQ_11K = 0x02,				// 信号源频率11.025KHz
	KCM_FREQ_12K = 0x03,				// 信号源频率12KHz
	KCM_FREQ_16K = 0x04,				// 信号源频率16KHz
	KCM_FREQ_22K = 0x05,				// 信号源频率22.05KHz
	KCM_FREQ_24K = 0x06,				// 信号源频率24KHz
	KCM_FREQ_32K = 0x07,				// 信号源频率32KHz
	KCM_FREQ_44K = 0x08,				// 信号源频率44.1KHz
	KCM_FREQ_48K = 0x09,				// 信号源频率48KHz
	KCM_FREQ_64K = 0x0a,				// 信号源频率64KHz
	KCM_FREQ_88K = 0x0b,				// 信号源频率88.2KHz
	KCM_FREQ_96K = 0x0c,				// 信号源频率96KHz
	KCM_FREQ_128K = 0x0d,				// 信号源频率128KHz
	KCM_FREQ_176K = 0x0e,				// 信号源频率176.4KHz
	KCM_FREQ_192K = 0x0f				// 信号源频率192KHz
} KC3X_INPUT_FREQ;


typedef enum {
	KCM_MODEL_32C = 0x31,				// 模块型号KC32C
	KCM_MODEL_35H = 0x53,				// 模块型号KC35H
	KCM_MODEL_36H = 0x56				// 模块型号KC36H
} KC3X_MODEL_TYPE;

typedef enum {
	KCM_PLAY_STOP = 0x10,				// 多媒体播放停止
	KCM_PLAY_PAUSE = 0x20,				// 多媒体播放暂停/播放
	KCM_PLAY_ONLY = 0x30,				// 多媒体播放只暂停
	KCM_PAUSE_ONLY = 0x40,				// 多媒体播放只播放
	KCM_PLAY_SKIP_UP = 0x50,			// 多媒体播放前一首
	KCM_PLAY_SKIP_DOWN = 0x60,			// 多媒体播放后一首
	KCM_PLAY_FAST_FORW0 = 0x80,			// 多媒体播放快进，速度0
	KCM_PLAY_FAST_FORW1 = 0x81,			// 多媒体播放快进，速度1
	KCM_PLAY_FAST_FORW2 = 0x82,			// 多媒体播放快进，速度2
	KCM_PLAY_FAST_FORW3 = 0x83,			// 多媒体播放快进，速度3
	KCM_PLAY_FAST_BACK0 = 0x90,			// 多媒体播放快退，速度0
	KCM_PLAY_FAST_BACK1 = 0x91,			// 多媒体播放快退，速度1
	KCM_PLAY_FAST_BACK2 = 0x92,			// 多媒体播放快退，速度2
	KCM_PLAY_FAST_BACK3 = 0x93,			// 多媒体播放快退，速度3
	KCM_PLAY_RANDOM0 = 0xa0,			// 多媒体随机播放，时间0
	KCM_PLAY_RANDOM1 = 0xa1,			// 多媒体随机播放，时间1
	KCM_PLAY_RANDOM2 = 0xa2,			// 多媒体随机播放，时间2
	KCM_PLAY_RANDOM3 = 0xa3,			// 多媒体随机播放，时间2
	KCM_PLAY_REPEAT0 = 0xb4,			// 多媒体重复播放，类型0
	KCM_PLAY_REPEAT1 = 0xb5,			// 多媒体重复播放，类型1
	KCM_PLAY_REPEAT2 = 0xb6,			// 多媒体重复播放，类型2
	KCM_PLAY_REPEAT3 = 0xb7				// 多媒体重复播放，类型3
} KC3X_PLAY_OPERATE;


typedef enum {
	KCM_VOL_CHIP_NONE = 0x00,			// 不使用模块内部的音量
	KCM_VOL_CHIP_DSP = 0x01,			// 使用模块内部的DSP数码音量
	KCM_VOL_CHIP_DAC = 0x02,			// 使用模块内部的DAC数码音量
	KCM_VOL_CHIP_PT2258 = 0x03,			// 为使用PT2258 + PT2259或者兼容的音量芯片
	KCM_VOL_CHIP_M62446 = 0x04			// 使用M62446或者兼容的音量芯片
} KC3X_VOLUME_CHIP;

#define CUSTOM_SWAP_CHANNEL				0x00070000			// 互换输出声道
#define CUSTOM_6CH_TO_8CH				0x00080000			// 在设计为5.1的系统之中使用7.1功能
#define CUSTOM_VOLUME_CHIP				0x00700000			// 音量芯片类型
#define CUSTOM_IN_EACH_MODE				0x00800000			// 为每个输入通道单独记忆聆听模式及多段EQ均衡音效选择
#define CUSTOM_MIC_LTRT_SWAP			0x08000000			// 话筒MIC与模拟输入交换
#define CUSTOM_MIC_ENABLE				0x10000000			// 使用话筒声音混合功能
#define CUSTOM_BCK_WCK_INPUT			0x80000000			// BCK与WCK端口为输入


#endif		// __KC3X_TYPE_H