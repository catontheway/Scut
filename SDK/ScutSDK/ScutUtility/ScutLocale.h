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

#ifndef LOCALE_H
#define LOCALE_H
#include <string>
namespace ScutUtility
{
	class CLocale
	{
	private:
		CLocale(void);
		~CLocale(void);
	public:		
		static void setLanguage(const char* pszLanguage);
		//"zh_CN"  ���� 
		//"zh_TW"  ����
		//"ja_JP"  ����
		//"en_US"  Ӣ�� �����������ֵĶ�ʶ��ΪӢ�� 
		static const char* getLanguage();

		//dst �����������64�ֽ�
		//iPhone Appstore ��Ҫ����SCUT_IPHONE_APPSTORE��
		//iPhone ģ������Ҫ����SCUT_IPHONE_SIMULATOR��
		static const char* getImsi();
		static const char* getImei();
		//���½ӿڽ��ʺ���Win32�����á�
		static void setImsi(const char* pszImsi);
		static bool isSDCardExist();
	};
}

#endif//LOCALE_H