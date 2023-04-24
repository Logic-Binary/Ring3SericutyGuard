#pragma once
#include "afxdialogex.h"


// CMyRelocation 对话框

class CMyRelocation : public CDialogEx
{
	DECLARE_DYNAMIC(CMyRelocation)

public:
	CMyRelocation(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyRelocation();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	char* m_File;
	virtual BOOL OnInitDialog();
	DWORD RVA2FOA(DWORD rva);
	CListCtrl m_ListCtrl2;
};
