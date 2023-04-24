// CMyExport.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyExport.h"


// CMyExport 对话框

IMPLEMENT_DYNAMIC(CMyExport, CDialogEx)

CMyExport::CMyExport(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXPORT, pParent),
	m_File(NULL)
	, m_edit1(_T(""))
	, m_edit2(_T(""))
	, m_edit4(_T(""))
	, m_edit3(_T(""))
	, m_edit5(_T(""))
	, m_edit6(_T(""))
	, m_edit7(_T(""))
{

}

CMyExport::~CMyExport()
{

}

void CMyExport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_edit1);
	DDX_Text(pDX, IDC_EDIT2, m_edit2);
	DDX_Text(pDX, IDC_EDIT4, m_edit4);
	DDX_Text(pDX, IDC_EDIT3, m_edit3);
	DDX_Text(pDX, IDC_EDIT5, m_edit5);
	DDX_Text(pDX, IDC_EDIT6, m_edit6);
	DDX_Text(pDX, IDC_EDIT7, m_edit7);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CMyExport, CDialogEx)
END_MESSAGE_MAP()


// CMyExport 消息处理程序


BOOL CMyExport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	PIMAGE_DOS_HEADER dos_header = PIMAGE_DOS_HEADER(m_File);
	PIMAGE_NT_HEADERS nt_header = PIMAGE_NT_HEADERS(dos_header->e_lfanew + m_File);
	DWORD export_rva = nt_header->OptionalHeader.DataDirectory[0].VirtualAddress;
	if (export_rva == 0) {
		MessageBox(_T("空表"));
		return FALSE;
	}
	DWORD FOA = RVA2FOA(export_rva);
	PIMAGE_EXPORT_DIRECTORY export_header = (PIMAGE_EXPORT_DIRECTORY)(FOA + m_File);
	//dll名字
	CString name;
	DWORD name_foa = RVA2FOA(export_header->Name);
	char* buff = (PCHAR)(name_foa + (DWORD)m_File);
	//char转wchar
	char temp[30] = {};
	char temp2[30] = {};
	strcpy_s(temp,strlen(buff)+1, buff);
	DWORD dwPathLen = MultiByteToWideChar(CP_ACP, 0, temp, -1, NULL, 0);
	wchar_t* lpcTheFile = new wchar_t[dwPathLen];
	MultiByteToWideChar(CP_ACP, 0, temp, -1, lpcTheFile, dwPathLen);
	m_edit1 = lpcTheFile;
	
	//序号基数
	CString base;
	base.Format(_T("%d"), export_header->Base);
	m_edit2 = base;

	//函数数量
	CString count;
	count.Format(_T("%d"), export_header->NumberOfFunctions);
	m_edit3 = count;

	//函数名称数量
	CString count_name;
	count_name.Format(_T("%d"), export_header->NumberOfNames);
	m_edit4 = count_name;

	//函数地址表地址
	CString add1;
	add1.Format(_T("%08X"), export_header->AddressOfFunctions);
	m_edit5 = add1;

	//函数名称表地址
	CString add2;
	add2.Format(_T("%08X"),export_header->AddressOfNames);
	m_edit6 = add2;

	//序号表地址
	CString add3;
	add3.Format(_T("%08X"), export_header->AddressOfNameOrdinals);
	m_edit7 = add3;

	UpdateData(FALSE);

	m_ListCtrl.InsertColumn(0, _T("ORIDINAL"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("RVA"), 0, 100);
	m_ListCtrl.InsertColumn(2, _T("FUNCTION NAME"), 0, 200);
	m_ListCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	//函数地址导出数组
	LPVOID AddOfFun = RVA2FOA(export_header->AddressOfFunctions)+m_File;
	//函数名称导出数组
	LPVOID AddOfName = RVA2FOA(export_header->AddressOfNames) + m_File;
	//函数序号导出数组
	LPVOID AddOfOrdinals = RVA2FOA(export_header->AddressOfNameOrdinals) + m_File;


	for (int dwOridinal = 0; dwOridinal < export_header->NumberOfFunctions; dwOridinal++) {
		if ((PDWORD)AddOfFun == 0) {
			continue;
		}
		for (int dwIndex = 0; dwIndex < export_header->NumberOfFunctions;dwIndex++) {
			if (((PWORD)AddOfOrdinals)[dwIndex] == dwOridinal) {
				//带函数名的导出函数信息
				CString Ori;
				WORD ori = ((PWORD)AddOfOrdinals)[dwOridinal];
				Ori.Format(_T("%d"), ori);
				m_ListCtrl.InsertItem(dwOridinal, Ori);

				CString fun_rva;
				fun_rva.Format(_T("%08X"), ((PDWORD)AddOfFun)[dwOridinal]);
				m_ListCtrl.SetItemText(dwOridinal,1,fun_rva);

				CString fun_name;
				DWORD fun_name_rva = ((PDWORD)AddOfName)[dwOridinal];
				fun_name.Format(_T("%S"), (PCHAR)(RVA2FOA(fun_name_rva) + (DWORD)m_File));
				m_ListCtrl.SetItemText(dwOridinal, 2, fun_name);
			}
		}
	}





	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


DWORD CMyExport::RVA2FOA(DWORD rva)
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

