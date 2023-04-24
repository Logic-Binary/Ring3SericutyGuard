#pragma once
#include "afxdialogex.h"


// CMyPEInfo 对话框

class CMyPEInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CMyPEInfo)

public:
	CMyPEInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyPEInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYPEINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString szFileName;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_edit2;

	BOOL is_pe(CHAR* file);
	void addInfo(CHAR* file);
	CHAR* m_File;

	CString m_edit3;
	CString m_edit4;
	CString m_edit5;
	CString m_edit6;
	CString m_edit7;
	CString m_edit8;
	CString m_edit9;
	CString m_edit10;
	CString m_edit11;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CString m_calc;
	CString m_VA;
	CString m_FOA;
	afx_msg void OnBnClickedCalc();

	DWORD RVA2FOA(DWORD rva);

	CEdit m_Rva;
};
