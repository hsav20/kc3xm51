
// Copyright (c) 2002-2021, Hard & Soft Technology Co.,LTD.
// SPDX-License-Identifier: Apache-2.0
// https://gitee.com/hsav20/kc3xm51.git
// https://github.com/hsav20/kc3xm51.git
// http://www.hsav.com/download/kc3xm51.zip

// ����������汾ѡ��

#define CUSTOM_CODE                     0x12123010          // ��׼�汾 ʹ��PT2258����IC��֧�ֻ�Ͳ
//#define CUSTOM_CODE                     0x12123018          // ������� ʹ��PT2258����IC���ֽ�1(B3:PT2258)�ֽ�0(B3:ģ���MIC����)
//#define CUSTOM_CODE                     0x12120000          // ��׼�汾
//#define CUSTOM_CODE                     0x44443000          // ����Ƶ�壬���ó������뻷���� �Ե�  ʹ��PT2258����IC

// ����������������ѡ��һ���ʺ��Լ��ı��룬���ұ���ѡ��һ��(ֻ��һ��û��//ע��)

#define DISPLAY_OLD                                         // ����ͱ��� �ɰ汾��ʾ��(P2.5ȡ��)


#define TONE_ENABLE                                       	// �����ʹ��ң����������������+������-
#define SPECTRUM_ENABLE                                    	// �������ʾƵ��

#if (CUSTOM_CODE & 0x00000010) != 0							// ֧�ֻ�Ͳ
#define MICROPHONE                                         	// ����ͽ�ң����������������+������-�ı�Ϊ��Ͳ����
#endif

