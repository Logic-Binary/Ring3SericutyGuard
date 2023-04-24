// CMyDirectory.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyDirectory.h"
#include "CMyImport.h"
#include "CMyExport.h"
#include "CMyRelocation.h"
#include "CMyResource.h"
#include "CMyTLS.h"
#include "CMyDelayLoad.h"


// CMyDirectory 对话框

IMPLEMENT_DYNAMIC(CMyDirectory, CDialogEx)

CMyDirectory::CMyDirectory(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYDIRECTORY, pParent),
	m_File(NULL)
	, m_edit1(_T(""))
{

}

CMyDirectory::~CMyDirectory()
{
}

void CMyDirectory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_edit1);
	DDX_Text(pDX, IDC_EDIT2, m_edit2);
	DDX_Text(pDX, IDC_EDIT3, m_edit3);
	DDX_Text(pDX, IDC_EDIT4, m_edit4);
	DDX_Text(pDX, IDC_EDIT5, m_edit5);
	DDX_Text(pDX, IDC_EDIT7, m_edit6);
	DDX_Text(pDX, IDC_EDIT9, m_edit7);
	DDX_Text(pDX, IDC_EDIT6, m_edit8);
	DDX_Text(pDX, IDC_EDIT10, m_edit9);
	DDX_Text(pDX, IDC_EDIT8, m_edit10);
	DDX_Text(pDX, IDC_EDIT11, m_edit11);
	DDX_Text(pDX, IDC_EDIT12, m_edit12);
}


BEGIN_MESSAGE_MAP(CMyDirectory, CDialogEx)
	ON_BN_CLICKED(IDC_IMPORTBUTTON, &CMyDirectory::OnBnClickedImportbutton)
	ON_BN_CLICKED(IDC_EXPORTBUTTON, &CMyDirectory::OnBnClickedExportbutton)
	ON_BN_CLICKED(IDC_RELBUTTON, &CMyDirectory::OnBnClickedRelbutton)
	ON_BN_CLICKED(IDC_RESBUTTON, &CMyDirectory::OnBnClickedResbutton)
	ON_BN_CLICKED(IDC_TLSBUTTON, &CMyDirectory::OnBnClickedTlsbutton)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDirectory::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMyDirectory 消息处理程序


BOOL CMyDirectory::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_File == NULL) {
		return false;
	}
	//拿到拓展头指针
	PIMAGE_DOS_HEADER dos_header = PIMAGE_DOS_HEADER(m_File);
	PIMAGE_NT_HEADERS nt_header = PIMAGE_NT_HEADERS(dos_header->e_lfanew + m_File);
	PIMAGE_OPTIONAL_HEADER optional_header = PIMAGE_OPTIONAL_HEADER(&nt_header->OptionalHeader);

	//导出表RVA
	CString export_RVA;
	export_RVA.Format(_T("%08X"), optional_header->DataDirectory[0].VirtualAddress);
	m_edit1 = export_RVA;
	//导出表大小
	CString export_size;
	export_size.Format(_T("%08X"), optional_header->DataDirectory[0].Size);
	m_edit2 = export_size;
	//导入表
	CString import_RVA;
	import_RVA.Format(_T("%08X"), optional_header->DataDirectory[1].VirtualAddress);
	m_edit3 = import_RVA;
	//导入表大小
	CString import_Size;
	import_Size.Format(_T("%08X"), optional_header->DataDirectory[1].Size);
	m_edit4 = import_Size;
	//资源表
	CString resource_RVA;
	resource_RVA.Format(_T("%08X"), optional_header->DataDirectory[2].VirtualAddress);
	m_edit5 = resource_RVA;
	CString resource_size;
	resource_size.Format(_T("%08X"), optional_header->DataDirectory[2].Size);
	m_edit6 = resource_size;
	//重定位表
	CString basereloc_RVA;
	basereloc_RVA.Format(_T("%08X"), optional_header->DataDirectory[5].VirtualAddress);
	m_edit7 = basereloc_RVA;
	CString basereloc_size;
	basereloc_size.Format(_T("%08X"), optional_header->DataDirectory[5].Size);
	m_edit8 = basereloc_size;
	//延迟加载表
	CString delay_RAV;
	delay_RAV.Format(_T("%08X"), optional_header->DataDirectory[13].VirtualAddress);
	m_edit9 = delay_RAV;
	CString delay_size;
	delay_size.Format(_T("%08X"), optional_header->DataDirectory[13].Size);
	m_edit10 = delay_RAV;
	//TLS表
	CString TLS_RVA;
	TLS_RVA.Format(_T("%08X"), optional_header->DataDirectory[9].VirtualAddress);
	m_edit11 = TLS_RVA;
	CString TLS_size;
	TLS_size.Format(_T("%08x"), optional_header->DataDirectory[9].Size);
	m_edit12 = TLS_size;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMyDirectory::OnBnClickedExportbutton()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyExport export_table = {};
	export_table.m_File = m_File;
	export_table.DoModal();

}

void CMyDirectory::OnBnClickedImportbutton()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyImport import = {};
	import.m_File = m_File;
	import.DoModal();

}

void CMyDirectory::OnBnClickedRelbutton()
{
	// TODO: 在此添加控件通知处理程序代码

	CMyRelocation relocation = {};
	relocation.m_File = m_File;
	relocation.DoModal();

}


void CMyDirectory::OnBnClickedResbutton()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyResource resource = {};
	resource.m_File = m_File;
	resource.DoModal();

}


void CMyDirectory::OnBnClickedTlsbutton()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyTLS TLS = {};
	TLS.m_File = m_File;
	TLS.DoModal();

}


void CMyDirectory::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyDelayLoad dela = {};
	dela.m_File = m_File;
	dela.DoModal();

}
