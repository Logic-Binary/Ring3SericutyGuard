// CMyDelayLoad.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyDelayLoad.h"


// CMyDelayLoad 对话框

IMPLEMENT_DYNAMIC(CMyDelayLoad, CDialogEx)

CMyDelayLoad::CMyDelayLoad(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CMyDelayLoad::~CMyDelayLoad()
{
}

void CMyDelayLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CMyDelayLoad, CDialogEx)
END_MESSAGE_MAP()


// CMyDelayLoad 消息处理程序


BOOL CMyDelayLoad::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListCtrl.InsertColumn(0, _T("DLLName"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("IAT的RVA"), 0, 100);
	m_ListCtrl.InsertColumn(2, _T("INT的RVA"), 0, 100);

	m_ListCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


	getInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMyDelayLoad::getInfo() {
	PIMAGE_DOS_HEADER dos_header = PIMAGE_DOS_HEADER(m_File);
	PIMAGE_NT_HEADERS nt_header = PIMAGE_NT_HEADERS(dos_header->e_lfanew + m_File);
	//拿到延迟表
	DWORD import_rva = nt_header->OptionalHeader.DataDirectory[13].VirtualAddress;
	PIMAGE_DELAYLOAD_DESCRIPTOR import_header = (PIMAGE_DELAYLOAD_DESCRIPTOR)(RVA2FOA(import_rva) + m_File);

	//插入数据
	CString name;
	DWORD Foa = RVA2FOA(import_header->DllNameRVA);
	name.Format(_T("%S"), Foa+m_File);
	m_ListCtrl.InsertItem(0, name);

	CString iat_rva;
	iat_rva.Format(_T("%08X"), import_header->ImportAddressTableRVA);
	m_ListCtrl.SetItemText(0, 1, iat_rva);

	CString int_rva;
	int_rva.Format(_T("%08X"), import_header->ImportNameTableRVA);
	m_ListCtrl.SetItemText(0, 2, int_rva);
}

DWORD CMyDelayLoad::RVA2FOA(DWORD rva)
{
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