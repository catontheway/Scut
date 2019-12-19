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

#ifndef NetStreamExport_Header_yay
#define NetStreamExport_Header_yay

#include "LuaString.h"
#include <stack>
#include <list>
///////////////////////////////////////////////////////////////////////////
///// �������ݰ��ṹ: ǰ��λΪ����С������RECORD ������Ȼ��ṹSIZE���ṹ���ݡ�
/////////////////////////////////////////////////////////////////////////////

namespace ScutDataLogic 
{
	struct RECORDINFO
	{
		int nRecordSize;
		int nRecordReadSize;
		RECORDINFO(int _RecordSize, int _RecordReadSize)
		{
			nRecordSize		= _RecordSize;
			nRecordReadSize	= _RecordReadSize; 
		}
		
	};


		/**
		* @brief  ������Ϣ����������
		* @remarks 
		* @see		
		*/
	class CNetStreamExport{

	public:
		CNetStreamExport();
		~CNetStreamExport();
	

		/**
		* @brief  ����������Ϣ����������С
		* @n<b>��������</b>					: pushNetStream
		* @return  bool ���NETWORK ����İ���С���ǰ�����ֽڵ�ֵ��һ��������FALSE��
		* @n@param  char* ������Ϣ������
		* @param    int ������size��С
		* @remarks  �յ�������Ϣ��ʱ���ô˺���������������
		* @see		
		*/
		virtual bool pushNetStream(char* pdataStream,int wSize);

		 
		/**
		* @brief  ��ʼȡ��¼����
		* @n<b>��������</b>					: recordBegin
		* @return  bool �������ݽṹ����TRUE�� ��񷵻�FALSE�� 
		* @remarks  ��ʼȡ��¼����
		* @see	
		* 
		*/
		bool recordBegin();
		void recordEnd();
		
		 
		/**
		* @brief  ȡ���ݼ�¼��
		* @remarks   
		* @see	
		* 
		*/
		unsigned char  getRecordNumber(void);

		/**
		* @brief  ����������ȡһ��BYTE����
		* @remarks   
		* @see	
		*/
		unsigned char	getBYTE(void);
		/**
		* @brief  ����������ȡһ��CHAR����
		* @remarks   
		* @see	
		*/
		char			getCHAR(void);
		/**
		* @brief  ����������ȡһ��WORD����
		* @remarks   
		* @see	
		*/
		unsigned short	getWORD(void);
		/**
		* @brief  ����������ȡһ��SHORT����
		* @remarks   
		* @see	
		*/
		short			getSHORT(void);
		/**
		* @brief  ����������ȡһ��DWORD����
		* @remarks   
		* @see	
		*/
		unsigned int	getDWORD(void);
		/**
		* @brief  ����������ȡһ��Int����
		* @remarks   
		* @see	
		*/
		int				getInt(void);
		/**
		* @brief  ����������ȡһ��Float����
		* @remarks   
		* @see	
		*/
		float			getFloat(void);
		/**
		* @brief  ����������ȡһ��Double����
		* @remarks   
		* @see	
		*/
		double			getDouble(void);

		/**
		* @brief  ����������ȡһ��Int64����
		* @remarks   
		* @see	
		*/
		unsigned long long getInt64();

		 
		/**
		* @brief  ��LUA�з����ַ��ڴ棬����C����������
		* @n<b>��������</b>					: getString
		* @return  bool �ڴ����ʧ�ܻ����ݴ�С�Ƿ�������FALSE��
		* @n@param  char* Ҫ��ȡ������ָ��
		* @param    int Ҫ��ȡ���ݴ�С
		* @remarks   
		* @see		
		*/
		bool			getString(char* psz, int nLength);

		/**
		* @brief  ��C�з����ڴ棬����LUA ,����ӿ���freeStringBuffer() �ӿ�����ʹ��
		* @n<b>��������</b>					: getString
		* @return  bool �ڴ����ʧ�ܻ����ݴ�С�Ƿ�������FALSE��
		* @n@param  int Ҫ��ȡ���ݴ�С
		* @remarks   
		* @see		
		*/
		const char*		getString(/*const  char* pData,*/int nLength);
		/**
		* @brief  �ͷ���C�з�����ڴ�,����ӿ���getString() �ӿ�����ʹ��
		* @n<b>��������</b>					: freeStringBuffer
		* @remarks   
		* @see		
		*/
		void			freeStringBuffer();

		bool IsStatusReady();
		
	protected:

		std::stack< RECORDINFO*, std::list<RECORDINFO*> > m_RecordStack;
		char*   m_pDataStream;
		int 	m_nStreamPos;
		int		m_nSize;
		char*	m_szRet;

	};


}

//extern "C" int  tolua_NetStreamExport_open(lua_State* tolua_S);

#endif