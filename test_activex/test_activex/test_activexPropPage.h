#pragma once

// test_activexPropPage.h : Ctest_activexPropPage ����ҳ���������


// Ctest_activexPropPage : �й�ʵ�ֵ���Ϣ������� test_activexPropPage.cpp��

class Ctest_activexPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(Ctest_activexPropPage)
	DECLARE_OLECREATE_EX(Ctest_activexPropPage)

// ���캯��
public:
	Ctest_activexPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_TEST_ACTIVEX };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

