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
#ifndef _NET_CLIENT_BASE_H_
#define _NET_CLIENT_BASE_H_

#include "curl.h"
#include "easy.h"

#include "HttpClientResponse.h"
#include "HttpSession.h"
#include "ScutString.h"	

#ifdef SCUT_UPHONE
#include "ssGlobal.h"
#elif defined(SCUT_IPHONE) || defined(SCUT_MAC)
#include <iostream>
#include <pthread.h> 
#endif

using namespace ScutSystem;

#define ERROR_UNSUPPORTED_PROTOCOL -1
#define ERROR_SOCK_CREATION_FAILED -2
#define ERROR_CONNECT_FAILED       -3
#define HTTP_MAX_RETRIES            1

namespace ScutNetwork
{
	class INetStatusNotify;

	enum EAsyncInfoStatus
	{
		aisNone,
		aisProgress,
		aisSucceed,	
		aisTimeOut,
		aisFailed,
	};

	enum EAsyncMode
	{
		amGet,
		amPost,
	};

	enum ENetType
	{
		ntNone,
		ntWIFI,
		ntCMWAP,
		ntCMNET,
	};

	class CNetClientBase;
	struct AsyncInfo
	{
		CNetClientBase* Sender;			//���������HttpClient
		CHttpClientResponse* Response;	//��Ӧ����		
		CScutString Url;						//URL
		const void* PostData;			//�ύ������
		int PostDataSize;				//�ύ�����ݴ�С
		bool FormFlag;					//�Ƿ��Ǳ��ύ
		EAsyncInfoStatus Status;		//���������첽״̬
		EAsyncMode Mode;				//�첽����ģʽ
		int ProtocalType;               //0:HTTP�� 1:TCP
		void* pScene;
		int Data1;						//����1
		int Data2;						//����2
		AsyncInfo()
		{
			Reset();
		}
		~AsyncInfo()
		{
		}
		void Reset();
	};
	typedef AsyncInfo* PAsyncInfo;

	class CNetClientBase
	{
	public:
		CNetClientBase();
		virtual ~CNetClientBase();		

		virtual int AsyncNetGet(const char* url, CHttpClientResponse* resp) = 0;
	
		//�첽����£���Ҫ�趨����״̬֪ͨ�ӿ�
		virtual void SetNetStautsNotify(INetStatusNotify* pNetNotify);
		virtual INetStatusNotify* GetNetStautsNotify();
		//ͷ��Ϣ��������������
		virtual bool AddHeader(const char *name, const char *value);

		//��ȡ���趨��ʱʱ�䣬����Ϊ��λ��0��INFINTE��ʾ�޳�ʱʱ��
		int GetTimeOut();
		void SetTimeOut(int nTimeOut);

		//�Ƿ�ʹ�ý���֪ͨ
		virtual bool GetUseProgressReport();
		virtual void SetUseProgressReport(bool bReport);		

		//����״̬
		virtual void FullReset();
		virtual void Reset();
		CScutString	GetHost();	

		//�Ƿ��ڷ�æ״̬���������ô�����ٵ���Get��Post
		//iPhoneƽ̨Ҳ��ͨ�����ж��첽�����Ƿ��Ѿ����
		virtual bool IsBusy();
		
		AsyncInfo* GetAsyncInfo();

		CURL* GetCurlHandle();

		//ע��lua����ص�
		virtual void RegisterCustomLuaHandle(const char* szHandle);
	protected:
		virtual void Initialize();
		virtual int DoProgressReport(double dlTotal, double dlNow, double ulTotal, double ulNow);
		virtual bool isLuaHandleExist();
	
	protected:
		CURL			*curl_handle;
		char			host[256];
		unsigned int	m_sgThreadID;
		AsyncInfo		m_sAsyncInfo;

		bool			m_bIsBusy;
		bool			m_bUseProgressReport;
		bool			m_bAsyncProcessing;

		int				m_nTimeOut;

		INetStatusNotify*	m_pNetNotify;	//����״̬֪ͨ�ӿ�

		std::string		m_strLuaHandle;
	};
}
#endif

