#pragma once
#include "afxdialogex.h"


// CMyResource 对话框

class CMyResource : public CDialogEx
{
	DECLARE_DYNAMIC(CMyResource)

public:
	CMyResource(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyResource();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESOURCE };
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
