// CMyThread.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyThread.h"
#include <TlHelp32.h>


// CMyThread 对话框

IMPLEMENT_DYNAMIC(CMyThread, CDialogEx)

CMyThread::CMyThread(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYTHREAD, pParent)
{

}

CMyThread::~CMyThread()
{
}

void CMyThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CMyThread, CDialogEx)
END_MESSAGE_MAP()


// CMyThread 消息处理程序


BOOL CMyThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化

	//响应消息
	m_ListCtrl.InsertColumn(0, _T("线程ID"), 0, 260);
	m_ListCtrl.InsertColumn(1, _T("线程优先级"), 0, 260);

	m_ListCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//遍历线程添加到表中
	//创建进程快照
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	//遍历所有进程，插入到list控件
	THREADENTRY32 thread32 = { sizeof(THREADENTRY32) };
	Thread32First(hThreadSnap, &thread32);
	int i = 0;
	do
	{
		if (m_dwProcessPid == thread32.th32OwnerProcessID) {
			CString Pid;
			Pid.Format(_T("%d"), thread32.th32ThreadID);
			m_ListCtrl.InsertItem(i, Pid);
			CString pri;	//优先级
			pri.Format(_T("%d"), thread32.tpBasePri);
			m_ListCtrl.SetItemText(i, 1, pri);
			i++;
		}

	} while (Thread32Next(hThreadSnap, &thread32));


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
