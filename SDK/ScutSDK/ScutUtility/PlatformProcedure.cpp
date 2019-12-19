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
#include "PlatformProcedure.h"
#include "FileHelper.h"
#include "PathUtility.h"
#include "Trace.h"
#include "cocos2d.h"

#ifdef SCUT_WIN32
#define  FILE_SEP '\\'

#else
#define  FILE_SEP '/'
#endif
using namespace cocos2d;

namespace ScutUtility
{

	std::string CPlatformProcedure::ProcessDir(const char* pszSrcDir, const char* pszProcessFile, const char* pszDesDir/* = NULL*/, bool bOverwrite/* = false*/)
	{
#ifdef SCUT_ANDROID
		return ProcessAndroidDir(pszSrcDir, pszProcessFile, pszDesDir, bOverwrite);
#endif

#ifdef SCUT_IPHONE
		return ProcessIosDir(pszSrcDir, pszProcessFile);
#endif
		return "";
	}

#ifdef SCUT_ANDROID
	std::string CPlatformProcedure::ProcessAndroidDir(const char* pszSrcDir, const char* pszProcessFile, const char* pszDesDir, bool bOverwrite)
	{
		std::string strDirPath = CCFileUtils::sharedFileUtils()->getWritablePath();///data/app/apkname/
		strDirPath += (pszDesDir != NULL ? pszDesDir : pszSrcDir);
		strDirPath += FILE_SEP;
		strDirPath += pszProcessFile;

		if (!bOverwrite && ScutSystem::CPathUtility::IsFileExists(strDirPath.c_str()))
		{
			return strDirPath;
		}

		//�Ƚ���Apk Ŀ¼��pszSrcDir/pszProcessFile�ļ� �Ƚ�ѹ���浽
		std::string strApkFullPath = ScutDataLogic::CFileHelper::getAndroidRelativeDir();
		strApkFullPath += pszSrcDir;
		strApkFullPath += FILE_SEP;
		strApkFullPath += pszProcessFile;

		unsigned long nSize = 0;
		unsigned char* pApkData = NULL;/*ScutDataLogic::CFileHelper::getFileDataFromZip(
			ScutDataLogic::CFileHelper::getAndroidResourcePath(), strApkFullPath.c_str(), &nSize);*/
		int nApkVersion = 0;
		//std::string strSDPath;
		if (pApkData)
		{
			//����pszDesDirĿ¼�� 
			//���浽data/app/apkname/pszDesDir/pszProcessFile
			if (!ScutDataLogic::CFileHelper::createDirs(strDirPath.c_str()))
			{
				ScutLog("Create Dir Error %s", strDirPath.c_str());
				return "";
			}
			FILE* pFile = fopen(strDirPath.c_str(), "wb");
			if (pFile)
			{

				fwrite(pApkData, nSize, 1, pFile);
				fclose(pFile);
				return strDirPath;
			}
			else
			{
				ScutLog("fopen Error %s", strDirPath.c_str());
				return "";
			}

			delete []pApkData;
		}
		else
		{
			ScutLog("read file error in isProcedureUpgrade %s, %s",ScutDataLogic::CFileHelper::getAndroidResourcePath(), strApkFullPath.c_str());
			return "";
		}

		return "";
	}
#endif

#ifdef SCUT_IPHONE
	std::string CPlatformProcedure::ProcessIosDir(const char* pszSrcDir, const char* pszProcessFile)
	{
		//��ȡApp ��ԴĿ¼�µ��ļ�
		std::string strRelative = pszSrcDir;
		strRelative += FILE_SEP;
		strRelative += pszProcessFile;

		return ScutDataLogic::appFullPathFromRelativePath(strRelative.c_str());
	}
#endif
}