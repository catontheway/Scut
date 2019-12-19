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
#ifndef _STREAM_H_
#define _STREAM_H_

#include "Defines.h"

namespace ScutSystem
{
	typedef enum tagStreamOrigin
	{
		soBegin = 0,
		soCurrent,
		soEnd,
	} EStreamOrigin;

	/**
	* @brief ������
	*    
	* \remarks
	* 
	*/
	class CStream
	{
	public:
		CStream(void);
		CStream(const CStream* pSource);
		virtual ~CStream(void);
	public:
		int GetPosition();
		/**
		* @brief ���������α�ĵ�ǰλ��
		* 
		* 
		* @n<b>��������</b>					: SetPosition
		* @n@param int		nPos			: �����õ���λ��
		* @return
		* @remarks �˺������ڵ��������α��λ�ã�Ҳ����ʹ��Seek����
		* @see	Seek����
		*/
		void SetPosition(const int nPos);
		/**
		* @brief ��ȡ���Ĵ�С
		* 
		* 
		* @n<b>��������</b>					: GetSize
		* @return ���Ĵ�С
		* @see	
		*/
		virtual int GetSize();
		/**
		* @brief �������Ĵ�С
		* 
		* 
		* @n<b>��������</b>					: SetSize
		* @n@param int		nSize			: �����õ�����С
		* @return 
		* @remarks ������õ�����С��֮ǰ��С����ô����ü����ݣ����򽫻��������
		* @see	
		*/
		virtual void SetSize(const int nSize) = 0;

		/**
		* @brief �����ж�ȡ����
		* 
		* 
		* @n<b>��������</b>					: Read
		* @n@param char*	pszBuffer		: ����ȡ���ݵĻ���
		* @param   int		nSize			: Ҫ��ȡ�����ݴ�С
		* @return  ���سɹ���ȡ�����ݴ�С��byte��                     
		* @remarks 
		* @see 
		*/
		virtual int Read(char* pszBuffer, int nSize) = 0;
		/**
		* @brief ������д�뵽����
		* 
		* 
		* @n<b>��������</b>					: Write
		* @n@param char*	pszBuffer		: ��д�����ݵĻ���
		* @param   int		nSize			: Ҫд������ݴ�С
		* @return  ���سɹ�д������ݴ�С��byte��                     
		* @remarks 
		* @see 
		*/
		virtual int Write(const char* pszBuffer, int nSize) = 0;

		/**
		* @brief ��������Ѱ���ݣ��˺���������������ǰ���α�λ��
		* 
		* 
		* @n<b>��������</b>						: Seek
		* @n@param int					nOffset	: ��Ѱ��ƫ��ֵ
		* @param   EStreamOrigin		origin	: ��Ѱʹ�õĿ�ʼλ��
		* @return  ���ص�ǰ���α�λ��                     
		* @remarks �˺������ڵ��������α��λ��,EStreamOrigin��������Ѱʹ�õĿ�ʼλ�ã��ֱ��Ǵ��ʼ����ǰ�Լ�����濪ʼ��Ѱ
		* @see 
		*/
		virtual int Seek(int nOffset, EStreamOrigin origin) = 0;
		/**
		* @brief �����ݴ����ж�ȡ������
		* 
		* 
		* @n<b>��������</b>					: ReadBuffer
		* @n@param char*	pszBuffer		: ����ȡ���ݵĻ���
		* @param   int		nSize			: Ҫ��ȡ�����ݴ�С
		* @return  �ɹ�����true�����򷵻�false
		* @remarks �˺�����Read���������ڣ�Read�����ȡ�����ݴ�СС������Ĵ�С�����ú�����Ҫ������ȡnSizeָ����С������
		* @see 
		*/
		bool ReadBuffer(char* pszBuffer, int nSize);
		/**
		* @brief ������д�뵽����
		* 
		* 
		* @n<b>��������</b>					: WriteBuffer
		* @n@param char*	pszBuffer		: ��д�����ݵĻ���
		* @param   int		nSize			: Ҫд������ݴ�С
		* @return  �ɹ�����true�����򷵻�false
		* @remarks �˺�����Write���������ڣ�Write����д������ݴ�СС������Ĵ�С�����ú�����Ҫ�����д��nSizeָ����С������
		* @see 
		*/
		bool WriteBuffer(const char* pszBuffer, int nSize);

		/**
		* @brief ����Դ���п���ָ����С������
		* 
		* 
		* @n<b>��������</b>					: CopyFrom
		* @n@param CStream*	pSource			: ���������ݵ�ԭʼ��ָ��
		* @param   int		nSize			: �����������ݴ�С
		* @return  ���سɹ����������ݴ�С��byte��                     
		* @remarks 
		* @see 
		*/
		int CopyFrom(CStream* pSource, int nSize = 0);
		
		/**
		* @brief ������ȡ������
		* ��ע�⡿����ȡ̫�������
		* 
		* @n<b>��������</b>					: GetBuffer
		* @param   int		nSize			: Ҫ��ȡ�����ݴ�С
		* @return  ���سɹ���ȡ�����ݿ�                     
		* @remarks 
		* @see 
		*/
		virtual char* GetBuffer(int nSize);

		/**
		* @brief �ͷŴ�����ȡ������
		* 
		* 
		* @n<b>��������</b>					: ReleaseBuffer
		* @param   char* pBuffer			: ҪҪ�ͷŵ�����ָ��
		* @return                      
		* @remarks 
		* @see 
		*/
		//virtual void ReleaseBuffer(char* pBuffer);

		private:
			char* m_Buffer;
	};

	/**
	* @brief ���������
	*    
	* \remarks
	* 
	*/
	class CHandleStream: public CStream
	{
	public:
		CHandleStream();
		/**
		* @brief �������Ĵ�С
		* 
		* 
		* @n<b>��������</b>					: SetSize
		* @see	�μ�����˵��
		*/
		virtual void SetSize(const int nSize);
		/**
		* @brief �����ж�ȡ����
		* 
		* 
		* @n<b>��������</b>					: Read
		* @see �μ�����˵��
		*/
		virtual int Read(char* pszBuffer, int nSize);
		/**
		* @brief ������д�뵽����
		* 
		* 
		* @n<b>��������</b>					: Write
		* @see	�μ�����˵��
		*/
		virtual int Write(const char* pszBuffer, int nSize);
		/**
		* @brief ��������Ѱ���ݣ��˺���������������ǰ���α�λ��
		* 
		* 
		* @n<b>��������</b>					: Seek
		* @see	�μ�����˵��
		*/
		virtual int Seek(int nOffset, EStreamOrigin origin);
		/**
		* @brief ��ȡ���
		* 
		* 
		* @n<b>��������</b>					: GetHandle
		* @return  �������ľ��                     
		* @remarks 
		* @see 
		*/
		intptr_t GetHandle();
		
		void SetHandle(intptr_t hHandle);
	protected:
	private:
		intptr_t m_hHandle;
	};

	/**
	* @brief �ļ��������ڴ�ȡһ���ļ�
	*    
	* \remarks
	* 
	*/
	class CFileStream: public CHandleStream
	{
	public:
		CFileStream();
		//����һ���ļ���chMode��c������ʹ�ã���IPhone
		int Open(const char* lpszFileName, DWORD dwFlag, DWORD dwMode, char* chMode = NULL);
		virtual ~CFileStream();
	private:
	};

	/**
	* @brief �ڴ�������
	*    
	* \remarks
	* 
	*/
	class CBaseMemoryStream: public CStream
	{
	public:
		CBaseMemoryStream();
		/**
		* @brief �����ж�ȡ����
		* 
		* 
		* @n<b>��������</b>					: Read
		* @see �μ�����˵��
		*/
		virtual int Read(char* pszBuffer, int nSize);
		/**
		* @brief ��������Ѱ���ݣ��˺���������������ǰ���α�λ��
		* 
		* 
		* @n<b>��������</b>					: Seek
		* @see	�μ�����˵��
		*/
		virtual int Seek(int nOffset, EStreamOrigin origin);
		/**
		* @brief ����ǰ���е����ݱ��浽Ŀ������
		* 
		* 
		* @n<b>��������</b>					: SaveTo
		* @n@param CStream*	pDest			: Ŀ����ָ��
		* @return  
		* @remarks 
		* @see 
		*/
		void SaveTo(CStream* pDest);
		/**
		* @brief ����ǰ���е����ݱ��浽Ŀ���ļ���
		* 
		* 
		* @n<b>��������</b>					: SaveTo
		* @n@param LPCTSTR	lpszFileName	: ��������ļ�����
		* @return  
		* @remarks 
		* @see 
		*/
		void SaveTo(const char* lpszFileName);
		void* GetMemory();
	protected:
		void SetPointer(void* pNewPtr, int nSize);
	protected:
		void* m_pMemory;
		int m_nSize;
		int m_nPosition;
	};

	/**
	* @brief �ڴ��������ڴ�ȡ�ڴ�
	*    
	* \remarks
	* 
	*/
	class CMemoryStream: public CBaseMemoryStream
	{
	public:
		CMemoryStream();
		virtual ~CMemoryStream();
		/**
		* @brief ���������������
		* 
		* 
		* @n<b>��������</b>					: Clear
		* @return  
		* @remarks 
		* @see 
		*/
		void Clear();
		/**
		* @brief ����Դ���п�������
		* 
		* 
		* @n<b>��������</b>					: LoadFrom
		* @n@param CStream*	pSource			: ����������Դ��ָ��
		* @return  
		* @remarks 
		* @see 
		*/
		void LoadFrom(CStream* pSource);
		/**
		* @brief ��ָ�����ļ�����������
		* 
		* 
		* @n<b>��������</b>					: LoadFrom
		* @n@param LPCTSTR	lpszFileName	: ��������ļ�����
		* @return  
		* @remarks 
		* @see 
		*/
		void LoadFrom(const char* lpszFileName);
		/**
		* @brief �������Ĵ�С
		* 
		* 
		* @n<b>��������</b>					: SetSize
		* @see	�μ�����˵��
		*/
		virtual void SetSize(const int nSize);
		/**
		* @brief ������д�뵽����
		* 
		* 
		* @n<b>��������</b>					: Write
		* @see	�μ�����˵��
		*/
		virtual int Write(const char* pszBuffer, int nSize);
	protected:
		virtual void* Realloc(int& nNewCapacity);
		void SetCapacity(int nNewCapacity);
		int GetCapacity();
	private:	
	private:
		int m_nCapacity;
	};

}

#endif
