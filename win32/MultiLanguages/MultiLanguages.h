
// MultiLanguages.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#define LANG_CHINESE    0
#define LANG_ENGLISH    1

// CMultiLanguagesApp:
// 有关此类的实现，请参阅 MultiLanguages.cpp
//

class CMultiLanguagesApp : public CWinApp
{
public:
	CMultiLanguagesApp();

// 重写
public:
	virtual BOOL InitInstance();

private:
    int m_nLanguage;
    HINSTANCE m_hLangDLL;

// 实现

	DECLARE_MESSAGE_MAP()
public:
    virtual int ExitInstance();
};

extern CMultiLanguagesApp theApp;