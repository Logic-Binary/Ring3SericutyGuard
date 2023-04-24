// CMyWindows.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyWindows.h"
#include "resource.h"


// CMyWindows 对话框

IMPLEMENT_DYNAMIC(CMyWindows, CDialogEx)

CMyWindows::CMyWindows(CWnd* pParent /*=nullptr*/)
	:CDialogEx(IDD_MYWINDOWS, pParent)
{

}

CMyWindows::~CMyWindows()
{
}

void CMyWindows::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CMyWindows, CDialogEx)
	ON_WM_TIMER()
	ON_COMMAND_RANGE(ID_32771, ID_32773, &CMyWindows::OnClickMenu)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CMyWindows::OnRclickList2)
END_MESSAGE_MAP()


// CMyWindows 消息处理程序


BOOL CMyWindows::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//给list控件添加列
	m_ListCtrl.InsertColumn(0,_T("窗口"),0,1000);

	//设置拓展风格
	m_ListCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//给list控件添加内容
	FlashWindows();
	//设置定时器，刷新显示窗口
	SetTimer(1, 2345, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CMyWindows::MyEnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	//遍历窗口，添加到list控件中，需要拥有窗口类的指针
	//若return FALSE ，那么停止遍历窗口，否则一直遍历
	//获取窗口名
	TCHAR szWindName[MAX_PATH] = {};
	::GetWindowText(hWnd, szWindName, MAX_PATH);
	//将获取到的抽口添加到list控件中
	//判断哪些窗口需要添加到list中
	//不隐藏的，名称不为空的
	CString temp = szWindName;
	if (::IsWindowVisible(hWnd) && temp.GetLength() != NULL)
	{
		//先获取窗口类的this指针
		CMyWindows* pThis = (CMyWindows*)lParam;
		pThis->m_ListCtrl.InsertItem(0, szWindName);
	}

	return TRUE;
}


void CMyWindows::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent) {
	case 1:
	{
		//先获取一次所有窗口，然后添加到list中
		//先删除当前列表中所有内容
		m_ListCtrl.DeleteAllItems();
		FlashWindows();
		break;
	}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CMyWindows::FlashWindows()
{
	//EnumWindows这个函数遍历窗口
	//回调函数需要是全局的
	EnumWindows(MyEnumWindowsProc, (LPARAM)this);
}

void CMyWindows::OnClickMenu(UINT nID)
{

	//先根据鼠标点击位置获取窗口名
	CWnd* window = FindWindow(NULL, m_szWindowName);
	//判断一下窗口名是否为空
	if (m_szWindowName.GetLength() != 0) {
		switch (nID)
		{

		case ID_32771:
		{
			//最小化
			window->CloseWindow();
		}break;
		//case ID_32772:
		//{
		//	//还原
		//	
		//}break;
		case ID_32773:
		{
			//关闭
			window->SendMessage(WM_CLOSE);
			//刷新一下
			FlashWindows();
		}break;
		default:
			break;
		}
	}
	
}

void CMyWindows::OnRclickList2(NMHDR* pNMHDR, LRESULT* pResult)
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
	//0是指获取菜单的第几个菜单
	CMenu* pop = menu.GetSubMenu(0);
	//在当前位置弹出菜单
	pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTALIGN, pt.x, pt.y,this);
	//获取当前点击的窗口名
	int row = pNMItemActivate->iItem;
	m_szWindowName = m_ListCtrl.GetItemText(row, 0);

	*pResult = 0;
}
