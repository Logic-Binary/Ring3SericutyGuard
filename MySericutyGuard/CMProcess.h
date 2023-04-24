#pragma once
#include "afxdialogex.h"


// CMProgress 对话框

class CMProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CMProcess)

public:
	CMProcess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMProcess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYPROCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtrl;

	//获取当前进程信息
	void GetProcessInfo();
	//右键响应消息
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	//响应菜单消息
	void OnClickMenu(UINT nID);
	//保存要操作的进程pid
	CString m_szPid;
	//保存一下任务管理器的id
	DWORD taskID;
};
