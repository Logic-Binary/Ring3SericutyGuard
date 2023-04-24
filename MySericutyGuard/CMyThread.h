#pragma once
#include "afxdialogex.h"


// CMyThread 对话框

class CMyThread : public CDialogEx
{
	DECLARE_DYNAMIC(CMyThread)

public:
	CMyThread(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyThread();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYTHREAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_dwProcessPid;
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
};
