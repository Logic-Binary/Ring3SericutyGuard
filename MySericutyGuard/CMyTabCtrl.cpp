// CMyTabCtrl.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "CMyTabCtrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}

void CMyTabCtrl::InitTabCtrl(int count, ...)
{
	va_list vl;
	va_start(vl,count);
	for (int i = 0; i < count; i++) {
		TCHAR* name = va_arg(vl,TCHAR*);
		CDialogEx* dlg = va_arg(vl, CDialogEx*);
		DWORD id = va_arg(vl, DWORD);
		InsertItem(i,name);
		dlg->Create(id, this);
		CRect rc = {};
		GetClientRect(&rc);
		rc.DeflateRect(1, 23, 1, 1);
		dlg->MoveWindow(rc);
		dlg->ShowWindow(SW_HIDE);
		m_dlg.push_back(dlg);
	}
	m_dlg[0]->ShowWindow(SW_SHOW);
	va_end(vl);
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTabCtrl 消息处理程序

void CMyTabCtrl::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int select = GetCurSel();
	for (int i = 0; i < m_dlg.size(); i++) {
		if (i == select) {
			m_dlg[i]->ShowWindow(SW_SHOW);
		}
		else {
			m_dlg[i]->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}
