#pragma once

// test_activexPropPage.h : Ctest_activexPropPage 属性页类的声明。


// Ctest_activexPropPage : 有关实现的信息，请参阅 test_activexPropPage.cpp。

class Ctest_activexPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(Ctest_activexPropPage)
	DECLARE_OLECREATE_EX(Ctest_activexPropPage)

// 构造函数
public:
	Ctest_activexPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_TEST_ACTIVEX };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

