#pragma once

#include "iostream"
#include "string"

#include <ObjSafe.h>

#include "atlimage.h"
using namespace ATL;

#include "image_decrypt_show.h"

// test_activexCtrl.h : Ctest_activexCtrl ActiveX �ؼ����������


// Ctest_activexCtrl : �й�ʵ�ֵ���Ϣ������� test_activexCtrl.cpp��
class CDuiFrameWnd;
class Ctest_activexCtrl : public COleControl
{
	DECLARE_DYNCREATE(Ctest_activexCtrl)

// ���캯��
public:
	Ctest_activexCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~Ctest_activexCtrl();

	DECLARE_OLECREATE_EX(Ctest_activexCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(Ctest_activexCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(Ctest_activexCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(Ctest_activexCtrl)	   // �������ƺ�����״̬

	DECLARE_INTERFACE_MAP()
	BEGIN_INTERFACE_PART(ObjSafe, IObjectSafety)   
		STDMETHOD_(HRESULT,   GetInterfaceSafetyOptions)   (     
		/*   [in]   */   REFIID   riid,   
		/*   [out]   */   DWORD   __RPC_FAR   *pdwSupportedOptions,   
		/*   [out]   */   DWORD   __RPC_FAR   *pdwEnabledOptions   
		);  

		STDMETHOD_(HRESULT,   SetInterfaceSafetyOptions)   (     
			/*   [in]   */   REFIID   riid,   
			/*   [in]   */   DWORD   dwOptionSetMask,   
			/*   [in]   */   DWORD   dwEnabledOptions   
			);  

	END_INTERFACE_PART(ObjSafe);

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:
	enum {
	};

private:
	std::string file_path_;		// ͼƬ��Դ��·��
	std::string mplayer_path_;	// ��������·��
	std::string pic_decrypt_key_;//����ͼ���key
	CImage image_;				// ��չʾͼƬ���ڴ�
	
	IImageDecryptShow			*image_decrypt_show_;

	bool is_show_player_;		// �������Ƿ��Ѿ���ʼ������
	bool is_can_play_type_;		// �ǲ���������֧�ֵ���Ƶý������
	bool is_can_show_type_;		// �ǿ�֧�ֵ�ͼƬ�ļ�����

	bool is_chose_window_size_html_sign_; // ��Ӧҳ��html��־----�Ƿ��Զ��������ſ�͸�

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose(DWORD dwSaveOption);

private:
	void FileTypeJudge(std::string file_path);
};

