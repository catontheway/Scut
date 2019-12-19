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
#ifndef _SCUT_DATAREQUEST_H_
#define _SCUT_DATAREQUEST_H_

#include <map>
#include <list>
#include "NetClientBase.h"
#include "HttpSession.h"
#include "HttpClientResponse.h"
#include "AutoGuard.h"
#include "INetStatusNotify.h"
#include "TcpClient.h"

using namespace std;
using namespace ScutSystem;
using namespace ScutNetwork;

namespace ScutDataLogic
{	
	class CDataHandler;

	//TODO:֮������HttpClient��
	typedef struct RequestInfo 
	{
		CDataHandler* Handler;
		int HandlerTag;
		CNetClientBase* NetClient;
		CHttpClientResponse* HttpResponse;
		CHttpSession* HttpSession;
		DWORD StartTime;
		string Url;
		LPVOID LPData;
		int Status;

		//add by cd 
		void* pScene;
		RequestInfo();
		~RequestInfo();
	} *PRequestInfo;

	//�������
	enum ERequestError
	{
		reNetFailed,		//ʧ��
		reNetTimeOut,		//��ʱ
	};

	typedef void(*LUA_DATAHANDLE_CALLBACK)(void *pScene, int nTag, int nNet, CStream* lpData, LPVOID lpExternal);

	//���������࣬������ݵ�ͬ���첽����
	class CDataRequest: public INetStatusNotify
	{
	private:
		CDataRequest(void);
		virtual ~CDataRequest(void);
	public:
		static CDataRequest* Instance();
		
		///////////////////////////C++ ģʽ��غ���//////////////////////

		//ͬ��ִ������
		bool ExecRequest(CDataHandler* pHandler, const char* lpszUrl);
		//�첽ִ������
		DWORD AsyncExecRequest(CDataHandler* pHandler, int nTag, const char* lpszUrl, LPVOID lpData, void* pScene = NULL);
		//ȡ���첽�����ݲ�֧��
		bool CancelAsyncRequest(DWORD dwRequestID);

		//ͬ��ִ������
		bool ExecTcpRequest(CDataHandler* pHandler, const char* lpszUrl, const char* lpSendData, unsigned int nDateLen = 0);
		//�첽ִ������
		DWORD AsyncExecTcpRequest(CDataHandler* pHandler, int nTag, const char* lpszUrl, LPVOID lpData, void* pScene, const char* lpSendData, unsigned int nDateLen = 0);

		///////////////////////////LUA ģʽ��غ���//////////////////////

		//ͬ��ִ������
		bool ExecRequest(void* pScene);
		//������ݵĴ����� ĿǰĬ����ScutScene
		DWORD AsyncExecRequest(void* pScene, const char* szUrl, int nTag, void* lpData);

		//��socket��ʽ�������ݣ�lpSendDataΪҪ���͵����ݣ�nDateLenΪҪ���͵����ݳ���
		DWORD AsyncExecTcpRequest(void* pScene, const char* szUrl, int nTag, void* lpData, const char* lpSendData, unsigned int nDateLen = 0);

		//ע�����ݴ���ص�����
		void RegisterLUACallBack(LUA_DATAHANDLE_CALLBACK pCallBack);
		//ȡһ��LUA���ݲ����д���
		void PeekLUAData();

		void SetUseKeepAliveMode(bool bKeepAlive);
		bool GetUseKeepAliveMode();

		virtual void OnNotify(PAsyncInfo pAi);
	private:
		void ProcAsyncInfo(PAsyncInfo pAi);
		void FreeRequestInfo(PRequestInfo pRi);
		//�ж��Ƿ���Ҫ������Ҫ���ڿ����û�����Ƶ�����
		bool ShouldRequest(CDataHandler* pHandler, int nTag, const char* lpszUrl);

		//LUA
		void LuaHandleData(void* pScene, int nTag, int nNetRet, CStream* lpData, LPVOID lpExternal);
		bool LuaHandlePushData(CStream* lpData);
		void LuaHandleErrorData();
	public:
		bool LuaHandlePushDataWithInt(int p);
	private:
		void Initialize();
		void Finitialize();
	private:
		unsigned int m_sgAsyncThreadID;
		CThreadMutex m_ThreadMutex;
		map<CNetClientBase*, PRequestInfo>* m_pMapRequestInfos;		
		list<PRequestInfo> m_RequestInfos;
		list<CHttpClientResponse*> m_PushResponseInfos;
		list<int> m_ErrorInfos;
		CHttpClient* m_pOnlyOneClient; 
		CHttpSession* m_pOnlyOneSession;
		CTcpClient* m_pTcpClient;
		LUA_DATAHANDLE_CALLBACK m_pLuaDataHandleCallBack;
	};
}

#endif //_SCUT_DATAREQUEST_H_