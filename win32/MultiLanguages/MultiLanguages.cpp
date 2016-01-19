
// MultiLanguages.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MultiLanguages.h"
#include "MultiLanguagesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiLanguagesApp

BEGIN_MESSAGE_MAP(CMultiLanguagesApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMultiLanguagesApp ����

CMultiLanguagesApp::CMultiLanguagesApp() : m_nLanguage(0), m_hLangDLL(NULL)
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMultiLanguagesApp ����

CMultiLanguagesApp theApp;


// CMultiLanguagesApp ��ʼ��

BOOL CMultiLanguagesApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

    // Multiple Languages
    CString strIniFile("Config.ini");
    m_nLanguage = ::GetPrivateProfileInt(TEXT("Language"), TEXT("Current"), 0, strIniFile);

    switch (m_nLanguage)
    {
    case LANG_CHINESE:
        m_hLangDLL = ::LoadLibrary(TEXT("English.dll"));
        break;
    case LANG_ENGLISH:
        m_hLangDLL = ::LoadLibrary(TEXT("English.dll"));
        break;
    default:
        m_hLangDLL = ::LoadLibrary(TEXT("Chinese.dll"));
        break;
    }

    if (m_hLangDLL != NULL)
    {
        AfxSetResourceHandle(m_hLangDLL);
    }
    else
    {
        AfxMessageBox(TEXT("Load language package failure!"));
        exit(1);
    }

	CMultiLanguagesDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



int CMultiLanguagesApp::ExitInstance()
{
    // TODO: �ڴ����ר�ô����/����û���
    if (m_hLangDLL)
    {
        AfxFreeLibrary(m_hLangDLL);
        m_hLangDLL = NULL;
    }
    return CWinApp::ExitInstance();
}
