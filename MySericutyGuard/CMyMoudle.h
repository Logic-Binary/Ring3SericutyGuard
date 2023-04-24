#pragma once
#include "afxdialogex.h"


// CMyMoudle 对话框

class CMyMoudle : public CDialogEx
{
	DECLARE_DYNAMIC(CMyMoudle)

public:
	CMyMoudle(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyMoudle();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYMODULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwProcessPid;
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
};
