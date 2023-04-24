#pragma once
#include "afxdialogex.h"
#include <winsvc.h>



// CMyControl 对话框

class CMyControl : public CDialogEx
{
	DECLARE_DYNAMIC(CMyControl)

public:
	CMyControl(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyControl();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
	void getServerInfo();

	//菜单响应消息
	void OnClickMenu(UINT nID);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);

	SC_HANDLE hh;



};
