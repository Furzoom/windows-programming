// English.h : English DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEnglishApp
// �йش���ʵ�ֵ���Ϣ������� English.cpp
//

class CEnglishApp : public CWinApp
{
public:
	CEnglishApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
