// CMyMoudle.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyMoudle.h"
#include <TlHelp32.h>


// CMyMoudle 对话框

IMPLEMENT_DYNAMIC(CMyMoudle, CDialogEx)

CMyMoudle::CMyMoudle(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYMODULE, pParent)
{

}

CMyMoudle::~CMyMoudle()
{
}

void CMyMoudle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CMyMoudle, CDialogEx)
END_MESSAGE_MAP()


// CMyMoudle 消息处理程序


BOOL CMyMoudle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 
	//将moudle数据插入list中
	m_ListCtrl.InsertColumn(0, _T("模块ID"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("基址"),0,100);
	m_ListCtrl.InsertColumn(2, _T("模块大小/KB"), 0, 100);
	m_ListCtrl.InsertColumn(3, _T("模块名"), 0, 100);
	m_ListCtrl.InsertColumn(4, _T("路径"), 0, 500);
	m_ListCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


	HANDLE hMoudleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessPid);
	if (hMoudleSnap == INVALID_HANDLE_VALUE) {
		::MessageBox(0, _T("拒绝访问"), 0, 0);
		return FALSE;
	}

	//遍历所有进程，插入到list控件
	MODULEENTRY32 hMoudle32 = { sizeof(MODULEENTRY32) };
	Module32First(hMoudleSnap, &hMoudle32);
	int i = 0;
	do
	{
		CString Pid;
		Pid.Format(_T("%d"), hMoudle32.th32ModuleID);
		m_ListCtrl.InsertItem(i,Pid);

		CString address;
		address.Format(_T("%p"), hMoudle32.modBaseAddr);
		m_ListCtrl.SetItemText(i,1, address);

		CString size;
		size.Format(_T("%d"), hMoudle32.modBaseSize);
		m_ListCtrl.SetItemText(i, 2, size);

		m_ListCtrl.SetItemText(i, 3, hMoudle32.szModule);

		CString path;
		m_ListCtrl.SetItemText(i, 4, hMoudle32.szExePath);

		i++;
	} while (Module32Next(hMoudleSnap, &hMoudle32));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
