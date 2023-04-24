// CMyTLS.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyTLS.h"


// CMyTLS 对话框

IMPLEMENT_DYNAMIC(CMyTLS, CDialogEx)

CMyTLS::CMyTLS(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent),
	m_File(NULL)
{

}

CMyTLS::~CMyTLS()
{

}

void CMyTLS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CMyTLS, CDialogEx)
END_MESSAGE_MAP()


// CMyTLS 消息处理程序


BOOL CMyTLS::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListCtrl.InsertColumn(0, _T("源数据起始位置"), 0, 200);
	m_ListCtrl.InsertColumn(1, _T("源数据终止位置"), 0, 200);
	m_ListCtrl.InsertColumn(2, _T("AddressOfCAllBacks"), 0, 300);

	// 先获取 dos 头
	PIMAGE_DOS_HEADER dos_head =
		(PIMAGE_DOS_HEADER)m_File;
	// 根据 dos 头获取 nt 头
	PIMAGE_NT_HEADERS nt_head =
		(PIMAGE_NT_HEADERS)(m_File + dos_head->e_lfanew);

	// 获取扩展头
	PIMAGE_OPTIONAL_HEADER option_head =
		(PIMAGE_OPTIONAL_HEADER)(&nt_head->OptionalHeader);

	// 获取TLS表，在数据目录表的下标为 9 的位置
	DWORD TLS_rva = option_head->DataDirectory[9].VirtualAddress;
	DWORD TLS_foa = RVA2FOA(TLS_rva);

	PIMAGE_TLS_DIRECTORY32 TLS = PIMAGE_TLS_DIRECTORY32(TLS_foa + (DWORD)m_File);
	if (TLS == NULL) {
		MessageBox(_T("空表"));
		return FALSE;
	}
	CString star_add;
	star_add.Format(_T("%08X"), TLS->StartAddressOfRawData);
	m_ListCtrl.InsertItem(0, star_add);

	CString end_add;
	end_add.Format(_T("%08X"), TLS->EndAddressOfRawData);
	m_ListCtrl.SetItemText(0, 1, end_add);

	CString add_call;
	add_call.Format(_T("%08X"), TLS->AddressOfCallBacks);
	m_ListCtrl.SetItemText(0, 2, add_call);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


DWORD CMyTLS::RVA2FOA(DWORD rva)
{

	if (rva == 0) {
		return 0;
	}
	// 先获取 dos 头
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)m_File;
	// 根据 dos 头获取 nt 头
	PIMAGE_NT_HEADERS nt_head = (PIMAGE_NT_HEADERS)(m_File + dos_head->e_lfanew);
	//获取区段头
	PIMAGE_SECTION_HEADER section_head = IMAGE_FIRST_SECTION(nt_head);
	//判断rva落在哪个区
	while (section_head != 0) {
		if (rva >= section_head->VirtualAddress && rva <
			section_head->VirtualAddress + section_head->SizeOfRawData) {
			return section_head->PointerToRawData + rva - section_head->VirtualAddress;
		}
		section_head++;
	}
	return -1;
}