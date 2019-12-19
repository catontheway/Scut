/****************************************************************************
Copyright (c) 2013-2015 Scutgame.com

http://www.scutgame.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#pragma once
#include <string>

namespace ScutUtility
{
	enum EPlatformType
	{
		//Win32
		ptWin32,
		//Ipod
		ptiPod,
		//Ipad
		ptiPad,
		//�ƽ��iPhone��iPad
		ptiPhone,
		//���ƽ��iPhone
		ptiPhone_AppStore,
		//ANDROID
		ptANDROID,
		// mac
		ptMac,
		// WP7
		ptwindowsPhone7,
		//δ֪
		ptUnknow,
	};

	//��ǰ��Ծ��������
	enum EActiveNetworkType
	{
		antNone,
		antWIFI,
		ant2G,
		ant3G,
	};

	class ScutUtils
	{
	public:
		//��ȡƽ̨����
		static EPlatformType GetPlatformType();
		//��ȡIMSI
		static const char* getImsi();
		//��ȡIMEI
		static const char* getImei();

		/*
		//ע��iphone����֪ͨ
		//pszSoundName:������
		//pszAlertBody:������Ϣ�����û�����ã�Notification������ʱ������ʾ������Ϣ
		//pszAlertAction:alertAction�����ݽ���Ϊ�����ж�����ť�ϵ����֣����δ���õĻ���������Ϣ�еĶ�����ť����ʾΪ��View�����������ʽ
		//pszLaunchImage:alertLaunchImage�����û�������ѿ��ж�����ť����View����ʱ���ȴ�Ӧ�ü���ʱ��ʾ��ͼƬ����������Ӧ��ԭ�����õļ���ͼ Ƭ
		//timeIntervalSince1970:����ʱ��
		//repeatInterval:�ظ������ļ��ʱ��
		//hasAction:hasAction��һ�������Ƿ������ѿ�����ʾ������ť�Ĳ���ֵ
		*
		*		ע��ANDROID����֪ͨ
		*	pszSoundName��	�����ļ�·����mp3�ļ���,�����Ҫ������������ַ���""
		*	pszAlertBody��	��Ϣ����
		*	pszAlertAction���޶��壬������NULL
		*	pszLaunchImage���޶��壬������NULL
		*	timeIntervalSince1970��	��ʾ����Ϣĩβ��ʱ�������System.currentTimeMillis()
		*	hasAction��		�޶���
		*	repeatInterval���ظ������ļ��ʱ�䣬0��ʾ���ظ�����
		*	pszAlertTitle��	��Ϣ����
		*	hasVibration��	�Ƿ���
		*	iconResId��		��ʾ����Ϣͷ��ͼ����Դ��id����R.drawable.icon���������ʾͼ�꣬��������������0�����֣���1
		*/
		static int scheduleLocalNotification(const char* pszSoundName, const char* pszAlertBody, const char* pszAlertAction, const char* pszLaunchImage, 
											double timeIntervalSince1970, int repeatInterval, bool hasAction, const char* pszAlertTitle = NULL, bool hasVibration = true, int iconResId = 1);
		//ȡ��ĳ���ض���֪ͨ
		static void cancelLocalNotification(int nNotificationID);
		//ȡ������֪ͨ
		static void cancelLocalNotifications();

		//�����ı���������
		static void setTextToClipBoard(std::string content);
		//�Ӽ������ȡ�ı�
		static const char* getTextFromClipBoard();

		//������������
		static void launchApp(std::string packageName, std::string data = "");

		//��װ��������
		static void installPackage(std::string packageFilePath);

		//�ж�ĳ�������Ƿ��Ѱ�װ
		static bool checkAppInstalled(std::string packageName, bool bForceUpdate);

		//��ȡ�������Ѱ�װ��apps����ʽΪ������_����_������
		static const char* getInstalledApps();

		//ע��������Ļص�
		static void registerWebviewCallback(std::string strFun);

		//��ȡMac��ַ
		static const char* getMacAddress();
		//��ȡutf-8�ַ����и����ַ����ֽڳ���
		//pszStr Դ�ַ���
		//outLengthes ���ÿ���ַ����ֽڳ�������
		//nSize ���ص��ַ�����
		//���outLengthesΪNULL����ôֻ�����ַ�����
		//nSize������ΪNULL
		static bool GetUtf8StringLengthes(const char* pszStr, unsigned char** outLengthes, int* nSize);

		//���ص����������appȥ
		static void GoBack();
		//��ȡ����ʱ������������
		static const char* getOpenUrlData();

		//�Ƿ���Խ��
		static bool isJailBroken();

		//��ȡ��ǰapp�İ���(id)
		static const char* getCurrentAppId();

		//��ȡ��ǰ��������
		static EActiveNetworkType getActiveNetworkInfo();
	};

}