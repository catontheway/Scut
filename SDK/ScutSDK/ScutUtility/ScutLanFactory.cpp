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
#include <assert.h>
#include "ScutLanFactory.h"
#include "ScutLocale.h"

namespace ScutUtility
{
	CScutLanFactory* CScutLanFactory::m_instance = NULL;
	CScutLan* CScutLanFactory::m_lanInstance = NULL;

	CScutLan* CScutLanFactory::GetLanInstance()
	{
		if (NULL == m_instance)
		{
			m_instance = new CScutLanFactory();
		}

		if (NULL == m_lanInstance)
		{
			m_lanInstance = m_instance->Create(CLocale::getLanguage());
		}

		assert(NULL != m_lanInstance);

		return m_lanInstance;
	}

	CScutLan* CScutLanFactory::Create(const char* pszLan)
	{
		if (0 == strcmp(pszLan, "zh_CN"))		//��������
		{
			return new CScutLanCn(); 
		}else if (0 == strcmp(pszLan, "en_US"))	//Ӣ��
		{
			return new CScutLanUs(); 
		}else if (0 == strcmp(pszLan, "zh_TW"))	//��������
		{
			return new CScutLanTw(); 
		}else if (0 == strcmp(pszLan, "ja_JP"))	//����
		{
			return new CScutLanJp(); 
		}

		return new CScutLanUs();	//Ĭ��Ӣ��
	}

	CScutLanCn::CScutLanCn()
	{
		m_szOK = "ȷ��";
		m_szCancel = "ȡ��";
		m_szKnown = "֪����";
		m_szTimeOut = "����ʱ����ȷ�������������";
		m_szFalseConnect = "��������ʧ�ܣ���ȷ�����Ŀ���";
		m_szUpdateError = "���¹����У�����δ֪����";
		m_szDownload = "����";
		m_szExit = "�˳�";
		m_szSDNotExist = "SD���������޷���ɸ��£������SD����";
		m_szSDReadError = "��ȡSD������ʧ�ܣ������SD������";
		m_szResPackageLoadingTip = "����������Դ�����Ƿ��е���̨���أ�";
		m_szResPackageFinishTip = "��Դ���Ѿ�������ɣ��Ƿ������˳���Ϸ��";
	}

	CScutLanUs::CScutLanUs()
	{
		m_szOK = "OK";
		m_szCancel = "Cancel";
		m_szKnown = "OK";
		m_szTimeOut = "Request timeout, please make sure your network is available";
		m_szFalseConnect = "Network connection failed, please make sure your network is available";
		m_szUpdateError = "An unknown error occurred when updating";
		m_szDownload = "Download";
		m_szExit = "Exit";
		m_szSDNotExist = "SD card is not exist, please insert the SD card and try again";
		m_szSDReadError = "SD card read error, please insert the SD card and try again";
		m_szResPackageLoadingTip = "Downloading resource package, need to switch to download in background?";
		m_szResPackageFinishTip = "Has finished downloading the resource package, restart the game now?";
	}

	CScutLanTw::CScutLanTw()
	{
		m_szOK = "�_��";
		m_szCancel = "ȡ��";
		m_szKnown = "֪����";
		m_szTimeOut = "Ո�󳬕r��Ո�_�J���ľW�j����";
		m_szFalseConnect = "�W�j�B��ʧ����Ո�_�J���ľW�j����";
		m_szUpdateError = "�����^���У��l��δ֪�e�`";
		m_szDownload = "���d";
		m_szExit = "�˳�";
		m_szSDNotExist = "SD�������ڟo����ɸ��£�Ո����SD����ԇ";
		m_szSDReadError = "�xȡSD������ʧ����Ո����SD����ԇ";
		m_szResPackageLoadingTip = "�������d�YԴ�����Ƿ��е����_���d��";
		m_szResPackageFinishTip = "�YԴ���ѽ����d��ɣ��Ƿ������˳��[��";
	}

	CScutLanJp::CScutLanJp()
	{
		m_szOK = "�Ϥ�";
		m_szCancel = "������";
		m_szKnown = "�Ϥ�";
		m_szTimeOut = "Request timeout, please make sure your network is available";
		m_szFalseConnect = "Network connection failed, please make sure your network is available";
		m_szUpdateError = "An unknown error occurred when updating";
		m_szDownload = "Download";
		m_szExit = "Exit";
		m_szSDNotExist = "SD card is not exist, please insert the SD card and try again";
		m_szSDReadError = "SD card read error, please insert the SD card and try again";
		m_szResPackageLoadingTip = "Downloading resource package, need to switch to download in background?";
		m_szResPackageFinishTip = "Has finished downloading the resource package, restart the game now?";
	}
}

