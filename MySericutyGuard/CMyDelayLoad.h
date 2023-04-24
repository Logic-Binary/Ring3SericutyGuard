#pragma once
#include "afxdialogex.h"


// CMyDelayLoad 对话框

class CMyDelayLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDelayLoad)

public:
	CMyDelayLoad(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyDelayLoad();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
	char* m_File;
	void getInfo();
	DWORD RVA2FOA(DWORD rva);
};
