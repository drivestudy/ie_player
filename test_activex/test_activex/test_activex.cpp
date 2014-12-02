// test_activex.cpp : Ctest_activexApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "test_activex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Ctest_activexApp theApp;

const GUID CDECL _tlid = { 0xD73D0B09, 0xDAA6, 0x4076, { 0x95, 0xA9, 0xF0, 0x99, 0x5B, 0xA3, 0xBD, 0xB1 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// Ctest_activexApp::InitInstance - DLL 初始化

BOOL Ctest_activexApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// Ctest_activexApp::ExitInstance - DLL 终止

int Ctest_activexApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
