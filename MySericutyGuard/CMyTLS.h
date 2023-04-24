#pragma once
#include "afxdialogex.h"


// CMyTLS 对话框

class CMyTLS : public CDialogEx
{
	DECLARE_DYNAMIC(CMyTLS)

public:
	CMyTLS(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyTLS();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	CHAR* m_File;
	virtual BOOL OnInitDialog();
	DWORD RVA2FOA(DWORD rva);
};
