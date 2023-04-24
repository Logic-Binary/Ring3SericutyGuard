// CMyImport.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyImport.h"


// CMyImport 对话框

IMPLEMENT_DYNAMIC(CMyImport, CDialogEx)

CMyImport::CMyImport(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMPORT, pParent),
	m_File(NULL)
{

}

CMyImport::~CMyImport()
{

}

void CMyImport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CMyImport, CDialogEx)
END_MESSAGE_MAP()


// CMyImport 消息处理程序

BOOL CMyImport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListCtrl.InsertColumn(0, _T("DLLName"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("OriginalFirstThunk"), 0, 200);
	m_ListCtrl.InsertColumn(2, _T("FirstThunk"), 0, 100);

	m_ListCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	PIMAGE_DOS_HEADER dos_header = PIMAGE_DOS_HEADER(m_File);
	PIMAGE_NT_HEADERS nt_header = PIMAGE_NT_HEADERS(dos_header->e_lfanew + m_File);
	//拿到导入表
	DWORD import_rva = nt_header->OptionalHeader.DataDirectory[1].VirtualAddress;
	PIMAGE_IMPORT_DESCRIPTOR import_header = (PIMAGE_IMPORT_DESCRIPTOR)(RVA2FOA(import_rva)+m_File);
	//插入信息
	int i = 0;
	while (import_header->Name != 0) {
		CString name;
		DWORD FOA = RVA2FOA(import_header->Name);
		name.Format(_T("%S"), FOA + m_File);
		m_ListCtrl.InsertItem(i, name);

		CString OriginalFirstThunk;
		OriginalFirstThunk.Format(_T("%08X"), import_header->OriginalFirstThunk);
		m_ListCtrl.SetItemText(i,1, OriginalFirstThunk);

		CString FirstThunk;
		FirstThunk.Format(_T("%08X"), import_header->FirstThunk);
		m_ListCtrl.SetItemText(i, 2, FirstThunk);

		import_header++;
		i++;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

DWORD CMyImport::RVA2FOA(DWORD rva)
{
	// 先获取 dos 头
	PIMAGE_DOS_HEADER dos_head =(PIMAGE_DOS_HEADER)m_File;
	// 根据 dos 头获取 nt 头
	PIMAGE_NT_HEADERS nt_head =(PIMAGE_NT_HEADERS)(m_File + dos_head->e_lfanew);
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
