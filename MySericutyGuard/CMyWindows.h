#pragma once
#include "afxdialogex.h"


// CMyWindows 对话框

class CMyWindows : public CDialogEx
{
	DECLARE_DYNAMIC(CMyWindows)

public:
	CMyWindows(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyWindows();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYWINDOWS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
	//遍历窗口的回调函数

	static BOOL CALLBACK MyEnumWindowsProc(HWND hWnd, LPARAM lParam);
	//响应定时器消息
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//刷新界面
	void FlashWindows();
	//菜单响应消息
	void OnClickMenu(UINT nID);
	//list右键点击消息
	afx_msg void OnRclickList2(NMHDR* pNMHDR, LRESULT* pResult);
	//保存获取到的窗口名
	CString m_szWindowName;
};
