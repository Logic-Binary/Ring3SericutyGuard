// CMProgress.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMProcess.h"
#include <TlHelp32.h>
#include "CMyThread.h"
#include "CMyMoudle.h"

#define PATH L"C:\\Users\\罗辑\\Desktop\\安全卫士\\MySericutyGuard\\Debug\\Dll1.dll"


// CMProgress 对话框

IMPLEMENT_DYNAMIC(CMProcess, CDialogEx)

CMProcess::CMProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYPROCESS, pParent),
	taskID(0)
{

}

CMProcess::~CMProcess()
{

}

void CMProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CMProcess, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMProcess::OnRclickList1)
	ON_COMMAND_RANGE(ID_32774,ID_32778,&CMProcess::OnClickMenu)
END_MESSAGE_MAP()

// CMProgress 消息处理程序

BOOL CMProcess::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	//使用进程快照，遍历所有进程
	//序号，进程名，pid，PPID，线程数量
	m_ListCtrl.InsertColumn(0, _T("序号"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("进程名"), 0, 300);
	m_ListCtrl.InsertColumn(2, _T("pid"), 0, 100);
	m_ListCtrl.InsertColumn(3, _T("PPID"), 0, 100);
	m_ListCtrl.InsertColumn(4, _T("线程数量"), 0, 100);
	m_ListCtrl.InsertColumn(5, _T("说明"), 0, 300);

	m_ListCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	//获取进程信息
	GetProcessInfo();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMProcess::GetProcessInfo()
{
	//创建进程快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		return;
	}
	//遍历所有进程，插入到list控件
	PROCESSENTRY32 stcPe32 = { sizeof(PROCESSENTRY32) };
	Process32First(hProcessSnap, &stcPe32);
	int i = 0;
	do
	{
		//添加序号
		CString Index;
		Index.Format(_T("%d"), i);
		m_ListCtrl.InsertItem(i, Index);
		//添加进程名
		CString szProcessName = stcPe32.szExeFile;
		USES_CONVERSION;
		char* temp = W2A(stcPe32.szExeFile);
		if (!(strcmp(temp, "Taskmgr.exe"))) 
		{
			 taskID = stcPe32.th32ProcessID;
		}

		m_ListCtrl.SetItemText(i,1,szProcessName);
		//添加PID
		CString Pid ;
		Pid.Format(_T("%d"), stcPe32.th32ProcessID);
		m_ListCtrl.SetItemText(i,2,Pid);
		//添加PPID
		CString Ppid;
		Ppid.Format(_T("%d"), stcPe32.th32ParentProcessID);
		m_ListCtrl.SetItemText(i,3, Ppid);
		//添加线程数量
		CString ThreadCount;
		ThreadCount.Format(_T("%d"), stcPe32.cntThreads);
		m_ListCtrl.SetItemText(i, 4, ThreadCount);
		i++;
	} while (Process32Next(hProcessSnap, &stcPe32));
	CloseHandle(hProcessSnap);
}

void CMProcess::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	//弹出一个菜单
	//获取鼠标位置
	CPoint pt = {};
	GetCursorPos(&pt);
	//创建一个菜单对象，弹出子菜单
	CMenu menu;
	menu.LoadMenuW(IDR_MENU1);
	//创建一个弹出菜单的指针
	//1是指获取菜单的第几个菜单
	CMenu* pop = menu.GetSubMenu(1);

	//在当前位置弹出菜单
	pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTALIGN, pt.x, pt.y, this);

	int row = pNMItemActivate->iItem;
	m_szPid = m_ListCtrl.GetItemText(row, 2);

	*pResult = 0;
}

void CMProcess::OnClickMenu(UINT nID)
{
	if (m_szPid.GetLength() == NULL) {
		return;
	}
	switch (nID)
	{
	case ID_32774:
	{
		m_ListCtrl.DeleteAllItems();
		GetProcessInfo();
		break;
	}
	case ID_32775:
	{
		CMyThread* thread = new CMyThread;

		thread->m_dwProcessPid = _ttoi(m_szPid);
		//创建对话框
		thread->DoModal();
		break;
	}
	case ID_32776: 
	{
		CMyMoudle* moudle = new CMyMoudle;
		moudle->m_dwProcessPid = _ttoi(m_szPid);
		//创建对话框
		moudle->DoModal();

	}break;
	case ID_32777: 
	{
		DWORD pid = _ttoi(m_szPid);
		HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		TerminateProcess(hproc, -1);
		CloseHandle(hproc);
		m_ListCtrl.DeleteAllItems();
		GetProcessInfo();
	}break;

	//保护
	case ID_32778: 
	{
		if (taskID == 0) {
			MessageBox(_T("请打开32位的任务管理器"));
			break;
		}

		DWORD size = (wcslen(PATH) + 1) * 2;
		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, taskID);
		LPVOID buff = VirtualAllocEx(hProc, NULL, size, MEM_COMMIT, PAGE_READWRITE);
		WriteProcessMemory(hProc, buff, PATH, size, NULL);
		HANDLE hTheread = CreateRemoteThread(
			hProc, NULL, NULL,
			(LPTHREAD_START_ROUTINE)LoadLibrary,
			buff, NULL, NULL);
		WaitForSingleObject(hTheread, -1);

		if (buff != 0) {
			VirtualFreeEx(hProc, buff, 0, MEM_RELEASE);
			buff = NULL;
		}

		// 1. 创建命名的文件映射
		HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,
			NULL, PAGE_READWRITE, 0,
			4, L"PID");
		if (NULL == hMapFile || INVALID_HANDLE_VALUE == hMapFile)
			return;
		// 2. 创建View
		PVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 4);
		if (NULL == pBuf)
			return;
		// 3. 将共享数据复制到文件映射中
		DWORD id = _ttoi(m_szPid);

		memcpy_s(pBuf, 4, &id, 4);
		char text[10] = {};
		int temp = *((PDWORD)pBuf);
		sprintf_s(text, 10, "%d", id);


		// 4.等待
		Sleep(300);

		//MessageBox(0, L"即将关闭", 0, 0);
		// 5. 取消Mapping，关闭句柄
		//UnmapViewOfFile(pBuf);
		//CloseHandle(hMapFile);
		CloseHandle(hProc);
		CloseHandle(hTheread);

	}break;
	default:
		break;
	}
}
