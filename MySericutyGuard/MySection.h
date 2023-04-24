#pragma once
#include "afxdialogex.h"


// MySection 对话框

class MySection : public CDialog
{
	DECLARE_DYNAMIC(MySection)

public:
	MySection(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MySection();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MySection };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	CHAR* m_File;
	virtual BOOL OnInitDialog();
	CListCtrl m_listCtrl;
};
