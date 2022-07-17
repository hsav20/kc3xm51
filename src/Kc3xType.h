
#ifndef __KC3X_TYPE_H
#define __KC3X_TYPE_H


typedef enum {
	KCM_READ_IRQ = 0x01,				// 读中断请求寄存器，16位寄存器
	KCM_CLEAR_IRQ = 0x03,				// 清除中断请求寄存器，16位寄存器
	KCM_POWER_ON = 0x05,				// 模块上电开机、重新同步、待机及重启寄存器
	KCM_VOLUME_MUTE = 0x08,				// 音频静音及音量加减控制
	KCM_TEST_TONE = 0x09,				// 噪音测试
	KCM_SRC_DETECT = 0x0a,				// 检测所有有效的音源一次
	KCM_SRC_FORMAT = 0x18,				// 数码信号输入格式指示
	KCM_SRC_CHANNEL = 0x19,				// 数码信号输入通道信息及超低音指示
	KCM_SRC_RATE = 0x1a,				// 数码信号输入采样率及实际播放采样率指示
	KCM_SRC_BPS = 0x1b,					// 数码信号输入码流率指示
	KCM_SRC_VALID = 0x1c,				// 有效的音源输入改变，16位寄存器
	KCM_WORK_STATUS = 0x1f,				// 模块工作/运行状态指示

	// 以下为带上电记忆的寄存器
	KCM_INPUT_SOURCE = 0x20,			// 输入音源选择
	KCM_INPUT_VIDEO = 0x21,				// 输入视频源选择
	KCM_DYN_COMPRES = 0x23,				// 杜比数码动态压缩
	KCM_SPK_CONFIG = 0x24,				// 喇叭设置
	KCM_LPF_FREQ = 0x25,				// 超低音通道LPF低通滤波器频率
	KCM_HPF_FREQ = 0x26,				// 主声道小喇叭HPF高通滤波器频率
	KCM_LIP_SYNC_SET = 0x28,			// 齿音同步延迟时间，修正对画面与声音不同步
	KCM_LIP_SYNC_MAX = 0x29,			// 齿音同步最大的延迟时间
	KCM_LISTEN_MODE = 0x2b,				// 聆听模式选择
	KCM_EQ_SELECT = 0x2c,				// 多路均衡音效处理选择
	KCM_VOLUME_MAX = 0x2e,				// 设置音量最大值
	KCM_VOLUME_CTRL = 0x2f,				// 音量值设置
	KCM_FL_TRIM = 0x30,					// 前置左声道微调 B4=1为减dB B3-B0为dB数值
	KCM_FR_TRIM = 0x31,					// 前置右声道微调 B4=1为减dB B3-B0为dB数值
	KCM_CE_TRIM = 0x32,					// 中置声道微调	  B4=1为减dB B3-B0为dB数值
	KCM_SW_TRIM = 0x33,					// 超低音声道微调 B4=1为减dB B3-B0为dB数值
	KCM_SL_TRIM = 0x34,					// 环绕左声道微调 B4=1为减dB B3-B0为dB数值
	KCM_SR_TRIM = 0x35,					// 环绕右声道微调 B4=1为减dB B3-B0为dB数值
	KCM_BL_TRIM = 0x36,					// 后置左声道微调 B4=1为减dB B3-B0为dB数值
	KCM_BR_TRIM = 0x37,					// 后置右声道微调 B4=1为减dB B3-B0为dB数值
	KCM_MIC_MIXER = 0x38,				// 话筒声音与主声道合成比例
	KCM_MIC_VOLUME = 0x39,				// 话筒1及话筒2音量比例
	KCM_MIC_ECHO = 0x3a,				// 话筒直达声及回声比例
	KCM_MIC_DELAY = 0x3b,				// 话筒延迟时间及重复比例
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
	KCM_MIC_ADJ_MAX = 0x8d,				// 话筒各种参数最大值设置
	KCM_WR_SPECTRUM = 0x90,				// 设置频谱模式
	KCM_RD_SPECTRUM = 0x91,				// 频谱数值读取
	KCM_WR_FLASH = 0x94,				// 写入512字节FLASH掉电记忆空间，带偏移量及长度
	KCM_WR_GET_FLASH = 0x95,			// 准备读取512字节FLASH掉电记忆空间，写入偏移量
	KCM_RD_FLASH = 0x96,				// 读取512字节FLASH掉电记忆空间
	KCM_APP_COMMAND = 0x98,				// 读取手机/远程APP控制指令，多字节

	KCM_PLAY_SD_QTY = 0xa0,				// 读取SD卡多媒体文件总数量，共2字节
	KCM_PLAY_UDISK_QTY = 0xa1,			// 读取U盘多媒体文件总数量，共2字节
	KCM_PLAY_FILE_TIME = 0xa2,			// 读取正在播放文件的总时间，共2字节单位秒
	KCM_PLAY_TIME = 0xa3,				// 读取正在播放的实际时间，共2字节单位秒
	KCM_PLAY_INDEX = 0xa4,				// 读取/写入文件播放编号，共2字节
	KCM_PLAY_STATE = 0xa5,				// 读取/写入文件播放状态，共1字节
	KCM_PLAY_OPERATE = 0xa6,			// 读取/写入文件播放控制，共1字节
	KCM_PLAY_FILE_NAME = 0xa7,			// 读取当前多媒体文件名/歌曲名，最多32字节
	KCM_REGISTER_LAST = 0xaf
} KC3X_REGISTER;

typedef enum {
	KCM_IRQ_SYSTEM_INIT = 0x0001,       // 模式初始化完成中断，可以读取一些寄存器恢复本地的记忆
	KCM_IRQ_FORMAT_INFO = 0x0002,       // 输入信号格式改变中断，需要读取"KCM_SRC_FORMAT"寄存器
	KCM_IRQ_VOLUME = 0x0004,            // 音量调节改变中断，需要读取"KCM_VOLUME_CTRL"寄存器获取更新的音量值
	KCM_IRQ_SRC_VALID = 0x0008,        	// 有效的音源输入改变中断，需要读取"KCM_SRC_VALID"寄存器

	KCM_IRQ_FIRMWARE = 0x10,        	// 固件更新，需要读取"KCM_RD_INFO"寄存器
	KCM_IRQ_PLAY_STATE = 0x0020,       	// 多媒体文件播放状态改变，需要读取"KCM_PLAY_STATE"寄存器
	KCM_IRQ_PLAY_TIME = 0x0040,        	// 多媒体播放时间改变，需要读取"KCM_PLAY_TIME"寄存器
	KCM_IRQ_APP_COMMAND = 0x0080,  		// 收到手机/远程APP控制指令，需要读取"KCM_APP_COMMAND"寄存器

	KCM_IRQ_EX_FONT = 0x0400			// 字库读取完成中断
} KC3X_IRQ_TYPE;
typedef enum {
	KCM_SET_STANDBY = 0x00,       		// 设置模块进入待机状态，成功待机后状态改变为KCM_IS_STANDBY
	KCM_SET_POWER_ON = 0x01,       		// 设置模块进入正常工作状态，成功待机后状态改变为KCM_IS_POWER_ON
	KCM_SET_REBOOT = 0x55,            	// 设置模块重启，成功待机后状态改变为KCM_SET_POWER_ON
	KCM_IS_STANDBY = 0x80,        		// 读取到模块进入待机状态
	KCM_IS_POWER_ON = 0x81        		// 读取到模块进入正常工作状态
} KC3X_POWER_ON;

typedef enum {
	KCM_SRC_NOS = 0x00,					// 输入没有信号
	KCM_SRC_PCM = 0x01,					// PCM信号输入
	KCM_SRC_AC3 = 0x02,					// 标准的AC3信号输入
	KCM_SRC_DTS = 0x03,					// 标准的DTS/DTS-CD信号输入
	KCM_SRC_AAC = 0x04,					// AAC信号输入
	KCM_SRC_MPEG2 = 0x05,				// MPEG2多声道信号输入
	KCM_SRC_DSD = 0x06,					// DSD信号输入
	KCM_SRC_MP3 = 0x07,					// MP3信号输入
	KCM_SRC_SBC = 0x08,					// SBC蓝牙信号输入
	KCM_SRC_LPCM = 0x11,				// LPCM信号输入
	KCM_SRC_HDCD = 0x21,				// HD-CD信号输入
	KCM_SRC_EAC3 = 0x12,				// Enhanced AC-3信号输入
	KCM_SRC_TRUEHD = 0x22,				// 杜比TRUE HD信号输入
	KCM_SRC_MLP = 0x32,					// DVD AUDIO MLP信号输入
	KCM_SRC_DTS_ES = 0x13,				// DTS Extended Surround信号输入
	KCM_SRC_DTS_MA = 0x23,				// DTS HD Master Audio信号输入
	KCM_SRC_DTS_HRA = 0x33				// DTS HD High Resolution Audio信号输入
} KC3X_SRC_TYPE;

typedef enum {
	KCM_INPUT_ANALOG = 0x00,			// 音源选择模拟输入
	KCM_INPUT_DIGITAL = 0x10,			// 音源选择数码输入
	KCM_INPUT_RX1 = 0x10,				// 音源选择RX1输入
	KCM_INPUT_RX2 = 0x11,				// 音源选择RX2输入
	KCM_INPUT_RX3 = 0x12,				// 音源选择RX3输入
	KCM_INPUT_HDMI = 0x20,				// 音源选择HDMI输入
	KCM_INPUT_HDMI1 = 0x20,				// 音源选择HDMI1输入
	KCM_INPUT_HDMI2 = 0x21,				// 音源选择HDMI2输入
	KCM_INPUT_HDMI3 = 0x22,				// 音源选择HDMI3输入
	KCM_INPUT_ARC = 0x24,				// 音源选择HDMI ARC输入
	KCM_INPUT_MEDIA = 0x30,				// 音源选择U盘、TF卡、蓝牙、WIFI、USB声卡、网络等输入
	KCM_INPUT_SD = 0x30,				// 音源选择SD/TF卡输入
	KCM_INPUT_UDISK = 0x31,				// 音源选择U盘输入
	KCM_INPUT_PC = 0x32,				// 音源选择USB声卡输入
	KCM_INPUT_E8CH = 0x33,				// 音源选择外置7.1声道输入
	KCM_INPUT_BT = 0x34,				// 音源选择蓝牙输入
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
	KCM_SRC_VALID_HDMIS = 0x0f00,		// HDMI位掩码

	KCM_SRC_VALID_USBA = 0x1000,		// 有信号的音源输入：USB声卡
	KCM_SRC_VALID_E8CH = 0x2000,		// 有信号的音源输入：外置7.1声道
	KCM_SRC_VALID_BT = 0x4000,			// 有信号的音源输入：蓝牙音频
	KCM_SRC_VALID_WIFI = 0x8000			// 有信号的音源输入：WIFI音频
} KC3X_SRC_VALID;


typedef enum {
	KCM_ERROR_EQ = 0x80,				// B7多段EQ均衡音效初始化不正常，可能超出最大的预设计段数;
	KCM_ERROR_MIC = 0x20,				// B5话筒音量芯片硬件初始化不正常;
	KCM_ERROR_VOLUME = 0x10,			// B4模块控制音量芯片硬件初始化不正常;
	KCM_ERROR_CODEC = 0x08,				// B3模块ADC/DAC硬件初始化不正常;
	KCM_ERROR_SPDIF = 0x04,				// B2模块SPDIF接口芯片、时钟不正常;
	KCM_ERROR_TEMP1 = 0x02,				// B1:0模块温度范围
	KCM_ERROR_TEMP0 = 0x01				// B1:0模块温度范围
} KC3X_ERROR_FLAG;

typedef enum {
	KCM_MODEL_32C = 0x31,				// 模块型号KC32C
	KCM_MODEL_33A = 0x37,				// 模块型号KC33A
	KCM_MODEL_35H = 0x53,				// 模块型号KC35H
	KCM_MODEL_36H = 0x56				// 模块型号KC36H
} KC3X_MODEL_TYPE;

#define KC3X_STATE_PLAY_END				0x00				// 不能写入，读取为文件播放完成，已经停止
#define KC3X_STATE_PLAY_PAUSE			0x01				// 写入暂停，读取为已经暂停
#define KC3X_STATE_PLAY_PLAY			0x02				// 写入播放，读取为已经播放
#define KC3X_STATE_PLAY_STOP			0x03				// 写入停止，读取为已经停止
#define KC3X_STATE_PLAY_FLAG			0x03				// 修改播放标志
#define KC3X_STATE_RANDOM_NONE  		0x00				// 随机播放关闭，多媒体文件播放状态
#define KC3X_STATE_RANDOM_PATH			0x10				// 随机播放当前文件夹，多媒体文件播放状态
#define KC3X_STATE_RANDOM_ALL 			0x20				// 随机播放全部，多媒体文件播放状态
#define KC3X_STATE_RANDOM_FLAG			0x30				// 修改RANDOM标志
#define KC3X_STATE_REPEAT_NONE  		0x00				// 多媒体文件播放没有重复
#define KC3X_STATE_REPEAT_FILE  		0x40				// 多媒体文件播放重复当前文件
#define KC3X_STATE_REPEAT_PATH  		0x80				// 多媒体文件播放重复当前文件夹
#define KC3X_STATE_REPEAT_ALL  			0xc0				// 多媒体文件播放重复所有文件
#define KC3X_STATE_REPEAT_FLAG			0xc0				// 修改REPEAT标志

typedef enum {
	KCM_OPERATE_SKIP_UP = 0x01,			// 播放前一个文件
	KCM_OPERATE_SKIP_DOWN = 0x02,		// 播放后一个文件
	KCM_OPERATE_FAST_FORW = 0x03,		// 多媒体播放快进
	KCM_OPERATE_FAST_BACK = 0x04		// 多媒体播放快退
} KC3X_PLAY_OPERATE;

typedef enum {
	KCM_VOL_CHIP_NONE = 0x00,			// 不使用模块内部的音量
	KCM_VOL_CHIP_DSP_05 = 0x01,			// 使用DSP数码音量，每步0.5dB
	KCM_VOL_CHIP_DSP_10 = 0x02,			// 使用DSP数码音量，每步1dB
	KCM_VOL_CHIP_PT2258 = 0x03,			// 为使用PT2258 + PT2259或者兼容的音量芯片
	KCM_VOL_CHIP_M62446 = 0x04,	 		// 使用M62446或者兼容的音量芯片
	KCM_VOL_CHIP_CS3318 = 0x05			// 使用CS3318或者兼容的音量芯片
} KC3X_VOLUME_CHIP;

#define CUSTOM_ENA_DOWNMIX				0x00000100			// 允许解码输出后的声道下混模式及各种聆听模式
#define CUSTOM_ENA_BASSMANAGE			0x00000200 			// 允许喇叭设置及低音管理
#define CUSTOM_ENA_DELAY_LIN			0x00000400 			// 允许各声道延迟时间及齿音同步
#define CUSTOM_ENA_VOL_TRIM				0x00000800 			// 允许各声道音量及声道微调
#define CUSTOM_ENA_TONE_EQ				0x00001000 			// 允许音调及多段EQ均衡音效
#define CUSTOM_ENA_SPECTRUM				0x00008000 			// 允许电平取样及频谱输出

#define CUSTOM_SWAP_CHANNEL				0x00070000			// 互换输出声道
#define CUSTOM_6CH_TO_8CH				0x00080000			// 在设计为5.1的系统之中使用7.1功能
#define CUSTOM_VOLUME_CHIP				0x00700000			// 音量芯片类型
#define CUSTOM_IN_EACH_MODE				0x00800000			// 为每个输入通道单独记忆聆听模式及多段EQ均衡音效选择
#define CUSTOM_TRACK_MODE				0x00800000			// 为每个输入通道单独记忆聆听模式及多段EQ均衡音效选择
#define CUSTOM_MIC_LTRT_SWAP			0x08000000			// 话筒MIC与模拟输入交换
#define CUSTOM_MIC_ENABLE				0x10000000			// 话筒声音混合功能
#define CUSTOM_BCK_WCK_INPUT			0x20000000			// 数码输入输出时钟输入输出选择
#define CUSTOM_I2C_DATA					0xc0000000			// 数码输入输出格式数据选择


#endif		// __KC3X_TYPE_H

