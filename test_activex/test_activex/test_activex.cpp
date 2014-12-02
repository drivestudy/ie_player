// test_activex.cpp : Ctest_activexApp �� DLL ע���ʵ�֡�

#include "stdafx.h"
#include "test_activex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Ctest_activexApp theApp;

const GUID CDECL _tlid = { 0xD73D0B09, 0xDAA6, 0x4076, { 0x95, 0xA9, 0xF0, 0x99, 0x5B, 0xA3, 0xBD, 0xB1 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// Ctest_activexApp::InitInstance - DLL ��ʼ��

BOOL Ctest_activexApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣
	}

	return bInit;
}



// Ctest_activexApp::ExitInstance - DLL ��ֹ

int Ctest_activexApp::ExitInstance()
{
	// TODO: �ڴ�������Լ���ģ����ֹ���롣

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - ������ӵ�ϵͳע���

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - �����ϵͳע������Ƴ�

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
