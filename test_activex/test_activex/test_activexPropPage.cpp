// test_activexPropPage.cpp : Ctest_activexPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "test_activex.h"
#include "test_activexPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(Ctest_activexPropPage, COlePropertyPage)

// ��Ϣӳ��

BEGIN_MESSAGE_MAP(Ctest_activexPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(Ctest_activexPropPage, "TEST_ACTIVEX.test_activexPropPage.1",
	0xa93b7e6d, 0x1742, 0x420c, 0xa9, 0x3a, 0x68, 0xf7, 0x36, 0x2c, 0x9a, 0x20)

// Ctest_activexPropPage::Ctest_activexPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� Ctest_activexPropPage ��ϵͳע�����

BOOL Ctest_activexPropPage::Ctest_activexPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_TEST_ACTIVEX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// Ctest_activexPropPage::Ctest_activexPropPage - ���캯��

Ctest_activexPropPage::Ctest_activexPropPage() :
	COlePropertyPage(IDD, IDS_TEST_ACTIVEX_PPG_CAPTION)
{
}

// Ctest_activexPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void Ctest_activexPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// Ctest_activexPropPage ��Ϣ�������
