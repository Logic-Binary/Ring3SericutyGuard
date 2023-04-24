#pragma once
#include "afxdialogex.h"
#include <vector>
using std::vector;
#include <Psapi.h>

// CMyFiles 对话框

class CMyFiles : public CDialogEx
{
	DECLARE_DYNAMIC(CMyFiles)

public:
	CMyFiles(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyFiles();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFILES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_TreeCtrl;
	//afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	//获取双击节点的全路径
	CString GetFullPath(HTREEITEM hTreeItem);
	//获取指定路径下的所有子文件
	vector<CString> GetSubFiles(CString path);

	CString m_szFileName;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CString calMD5(CString path);

	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();

	ULARGE_INTEGER EnumFoldAndDeleteFile(
		WCHAR* lpFoldPath, 
		WCHAR* suffix1,
		WCHAR* suffix2,
		WCHAR* suffix3);

	static double FILETIME2Double(const _FILETIME& fileTime);
	static int getCPUInfomation();
	HANDLE hThread;
	BOOL m_bClose;
	static UINT MyThreadFunction(LPVOID lpParam);
	CString m_CPUNum;
	CString m_CPUNum2;
	afx_msg void OnBnClickedButton8();
	CString m_PATH;
	CString m_HouZhui;

	CString m_cpunum1;
	CString m_memnum1;
	CStatic m_cpu111;
	CStatic m_mem111;
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();

	static UINT MyThreadProc1(LPVOID lpParam);
	static UINT MyThreadProc2(LPVOID lpParam);
	ULARGE_INTEGER EnumFoldAndDeleteFile(WCHAR* lpFoldPath);
	ULARGE_INTEGER EnumFoldAndDeleteFile2(WCHAR* lpFoldPath);

	static PCHAR pMD5;
	afx_msg void OnBnClickedButton11();

	static CWinThread* h1;
	static CWinThread* h2;
	afx_msg void OnBnClickedButton12();
};

