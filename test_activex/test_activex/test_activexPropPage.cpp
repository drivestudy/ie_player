// test_activexPropPage.cpp : Ctest_activexPropPage 属性页类的实现。

#include "stdafx.h"
#include "test_activex.h"
#include "test_activexPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(Ctest_activexPropPage, COlePropertyPage)

// 消息映射

BEGIN_MESSAGE_MAP(Ctest_activexPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(Ctest_activexPropPage, "TEST_ACTIVEX.test_activexPropPage.1",
	0xa93b7e6d, 0x1742, 0x420c, 0xa9, 0x3a, 0x68, 0xf7, 0x36, 0x2c, 0x9a, 0x20)

// Ctest_activexPropPage::Ctest_activexPropPageFactory::UpdateRegistry -
// 添加或移除 Ctest_activexPropPage 的系统注册表项

BOOL Ctest_activexPropPage::Ctest_activexPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_TEST_ACTIVEX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// Ctest_activexPropPage::Ctest_activexPropPage - 构造函数

Ctest_activexPropPage::Ctest_activexPropPage() :
	COlePropertyPage(IDD, IDS_TEST_ACTIVEX_PPG_CAPTION)
{
}

// Ctest_activexPropPage::DoDataExchange - 在页和属性间移动数据

void Ctest_activexPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// Ctest_activexPropPage 消息处理程序
