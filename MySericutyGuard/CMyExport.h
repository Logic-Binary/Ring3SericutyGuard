#pragma once
#include "afxdialogex.h"


// CMyExport 对话框

class CMyExport : public CDialogEx
{
	DECLARE_DYNAMIC(CMyExport)

public:
	CMyExport(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyExport();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CHAR* m_File;
	virtual BOOL OnInitDialog();
	DWORD RVA2FOA(DWORD rva);
	//CListCtrl m_ListCtrl;
	CString m_edit1;
	CString m_edit2;
	CString m_edit4;
	CString m_edit3;
	CString m_edit5;
	CString m_edit6;
	CString m_edit7;
	CListCtrl m_ListCtrl;
};
