#pragma once
#include "afxdialogex.h"


// CMyDirectory 对话框

class CMyDirectory : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDirectory)

public:
	CMyDirectory(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyDirectory();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYDIRECTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	char* m_File;
	virtual BOOL OnInitDialog();
	CString m_edit1;
	CString m_edit2;
	CString m_edit3;
	CString m_edit4;
	CString m_edit5;
	CString m_edit6;
	CString m_edit7;
	CString m_edit8;
	CString m_edit9;
	CString m_edit10;
	CString m_edit11;
	CString m_edit12;
	afx_msg void OnBnClickedImportbutton();
	afx_msg void OnBnClickedExportbutton();
	afx_msg void OnBnClickedRelbutton();
	afx_msg void OnBnClickedResbutton();
	afx_msg void OnBnClickedTlsbutton();
	afx_msg void OnBnClickedButton1();
};
