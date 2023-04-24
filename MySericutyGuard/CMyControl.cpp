// CMyControl.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyControl.h"
#include "resource.h"


// CMyControl 对话框

IMPLEMENT_DYNAMIC(CMyControl, CDialogEx)

CMyControl::CMyControl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONTROL, pParent)
{

}

CMyControl::~CMyControl()
{
}

void CMyControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CMyControl, CDialogEx)
	ON_COMMAND_RANGE(ID_32779,ID_32779, &CMyControl::OnClickMenu)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMyControl::OnRclickList1)
END_MESSAGE_MAP()


// CMyControl 消息处理程序


BOOL CMyControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_ListCtrl.InsertColumn(0, _T("服务名"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("服务状态"), 0, 200);
	m_ListCtrl.InsertColumn(2, _T("服务类型"), 0, 100);

	m_ListCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	getServerInfo();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMyControl::getServerInfo()
{
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	hh = hSCM;
	//首次调用，获取内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,
		NULL,
		0,
		&dwSize,
		&dwServiceNum,
		NULL, NULL);

	//申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerice = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	//第二次枚举
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx(hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,
		(PBYTE)pEnumSerice,
		dwSize,
		&dwSize,
		&dwServiceNum,
		NULL, NULL);
	//遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++) {
		//服务名
		CString name;
		name.Format(_T("%s"), pEnumSerice[i].lpServiceName);
		m_ListCtrl.InsertItem(i, name);
		//服务状态
		CString Status;
		DWORD num = pEnumSerice[i].ServiceStatusProcess.dwCurrentState;
		switch (num)
		{
		case SERVICE_CONTINUE_PENDING:
		{
			Status = _T("该服务即将继续");
			break;
		}
		case SERVICE_PAUSE_PENDING:
		{
			Status = _T("服务正在暂停");
			break;
		}
		case SERVICE_PAUSED:
		{
			Status = _T("该服务已暂停");
			break;
		}
		case SERVICE_RUNNING:
		{
			Status = _T("服务正在运行");
			break;
		}
		case SERVICE_START_PENDING:
		{
			Status = _T("服务正在启动");
			break;
		}
		case SERVICE_STOP_PENDING:
		{
			Status = _T("服务正在停止");
			break;
		}
		case SERVICE_STOPPED:
		{
			Status = _T("服务已停止");
			break;
		}
		default:
			Status.Format(_T("%08X"), num);
			break;
		}
		m_ListCtrl.SetItemText(i, 1, Status);
		//服务类型
		CString Type;
		DWORD num2 = pEnumSerice[i].ServiceStatusProcess.dwServiceType;
		Type.Format(_T("%08X"), num2);
		m_ListCtrl.SetItemText(i, 2, Type);

		

	}
}

void CMyControl::OnClickMenu(UINT nID)
{
	//根据鼠标点击位置，获取句柄
	switch (nID) {

	case 32779:


		break;
	}

}



void CMyControl::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//弹出一个菜单
	//获取鼠标位置
	CPoint pt = {};
	GetCursorPos(&pt);

	//创建一个菜单对象弹出子菜单
	CMenu menu;
	menu.LoadMenuW(IDR_MENU2);
	//创建弹出菜单指针
	CMenu* pop = menu.GetSubMenu(0);

	//在当前位置弹出菜单
	pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	//获取当前窗口的索引
	int row = pNMItemActivate->iItem;
	CString name = m_ListCtrl.GetItemText(row,0);
	WCHAR NAME[MAX_PATH] = {};
	_tcscpy_s(NAME, name);

	SC_HANDLE hService = OpenService(hh, NAME, GENERIC_ALL);
	DeleteService(hService);
	CloseServiceHandle(hService);
	CloseServiceHandle(hh);
	m_ListCtrl.DeleteAllItems();
	getServerInfo();
	*pResult = 0;
}
