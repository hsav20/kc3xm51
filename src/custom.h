
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

// ����������汾ѡ��

// ��׼�汾 ʹ��PT2258����IC��֧�ֻ�Ͳ���ֽ�2(B6:4ΪPT2258)�ֽ�3(B4��MIC����)
#define CUSTOM_CODE                     0x00ff3010

// ������� ʹ��PT2258����IC��֧�ֻ�ͲMIC���ֽ�2(B6:4ΪPT2258)�ֽ�3(B4��MIC����)
//#define CUSTOM_CODE                     0x00ff3010          

// ������� ʹ��PT2258����IC��I2S�����ʽΪ����룬�ֽ�2(B6:4ΪPT2258)�ֽ�3(B7:6Ϊ1)
//#define CUSTOM_CODE                     0x00ff3040          

// ������� ʹ��PT2258����IC��I2S�����ʽΪ�Ҷ��룬�ֽ�2(B6:4ΪPT2258)�ֽ�3(B7:6Ϊ2)
//#define CUSTOM_CODE                     0x00ff3080          

// ������� ʹ��PT2258����IC��ѡ��BCK��WCKΪ���룬�ֽ�2(B6:4ΪPT2258)�ֽ�3(B5Ϊ1)
//#define CUSTOM_CODE                     0x00ff3020          

// ������� ÿ��1dB��DSP����������֧�ֻ�ͲMIC���ֽ�2(B6:4ΪDSPÿ��1dB����)�ֽ�3(B4��MIC����)
//#define CUSTOM_CODE                     0x00ff2010 

// ������� ÿ��0.5dB��DSP����������֧�ֻ�ͲMIC���ֽ�2(B6:4ΪDSPÿ��0.5dB����)�ֽ�3(B4��MIC����)
//#define CUSTOM_CODE                     0x00ff1010 

// ������� ʹ��PT2258����IC��֧�ֻ�ͲMIC(��ģ�����뽻��)���ֽ�2(B6:4ΪPT2258)�ֽ�3(B4��MIC���� B3��ͲMIC��ģ�����뽻��)
//#define CUSTOM_CODE                     0x00ff3018          

 // ��׼�汾��
//#define CUSTOM_CODE                     0x00ff0000 

// ����Ƶ�壬���ó������뻷���� �Ե�  ʹ��PT2258����IC
//#define CUSTOM_CODE                     0x44443000          

// ����������������ѡ��һ���ʺ��Լ��ı��룬���ұ���ѡ��һ��(ֻ��һ��û��//ע��)


// #define DISPLAY_OLD                                      // ����ͱ��� �ɰ汾��ʾ��(P2.5ȡ��)


#define TONE_ENABLE                                       	// �����ʹ��ң����������������+������-
#define SPECTRUM_ENABLE                                    	// �������ʾƵ��
// #define NO_PLAY_NEXT                                    	// ����Ͳ��������ź�һ�׵��ļ���������ֹͣ

#if (CUSTOM_CODE & 0x00000010) != 0							// ֧�ֻ�Ͳ
#define MICROPHONE                                         	// ����ͽ�ң����������������+������-�ı�Ϊ��Ͳ����
#endif

