#pragma once
#include "afxdialogex.h"

// CMyImport 对话框

class CMyImport : public CDialogEx
{
	DECLARE_DYNAMIC(CMyImport)

public:
	CMyImport(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyImport();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORT };
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
