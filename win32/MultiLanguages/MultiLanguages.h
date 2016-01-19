
// MultiLanguages.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#define LANG_CHINESE    0
#define LANG_ENGLISH    1

// CMultiLanguagesApp:
// �йش����ʵ�֣������ MultiLanguages.cpp
//

class CMultiLanguagesApp : public CWinApp
{
public:
	CMultiLanguagesApp();

// ��д
public:
	virtual BOOL InitInstance();

private:
    int m_nLanguage;
    HINSTANCE m_hLangDLL;

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
    virtual int ExitInstance();
};

extern CMultiLanguagesApp theApp;