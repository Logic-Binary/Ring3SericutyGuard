// CMyRelocation.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyRelocation.h"

typedef struct _TYPEOFFSET {
	WORD Offset : 12;				// (1) 大小为12Bit的重定位偏移
	WORD Type : 4;					// (2) 大小为4Bit的重定位信息类型值
}TYPEOFFSET, * PTYPEOFFSET;			// 这个结构体是A1Pass总结的

// CMyRelocation 对话框

IMPLEMENT_DYNAMIC(CMyRelocation, CDialogEx)

CMyRelocation::CMyRelocation(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent),
	m_File(NULL)
{

}

CMyRelocation::~CMyRelocation()
{
}

void CMyRelocation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_LIST3, m_ListCtrl2);
}


BEGIN_MESSAGE_MAP(CMyRelocation, CDialogEx)
END_MESSAGE_MAP()


// CMyRelocation 消息处理程序


BOOL CMyRelocation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_ListCtrl.InsertColumn(0, _T("索引"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("RVA"), 0, 100);
	m_ListCtrl.InsertColumn(2, _T("个数"), 0, 100);

	m_ListCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


	m_ListCtrl2.InsertColumn(0, _T("索引"), 0, 100);
	m_ListCtrl2.InsertColumn(0, _T("RVA"), 0, 100);
	m_ListCtrl2.InsertColumn(0, _T("OFFSET"), 0, 100);
	m_ListCtrl2.InsertColumn(0, _T("DATA"), 0, 100);
	m_ListCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


	if (m_File == NULL) {
		return FALSE;
	}

	// 先获取 dos 头
	PIMAGE_DOS_HEADER dos_head =
		(PIMAGE_DOS_HEADER)m_File;
	// 根据 dos 头获取 nt 头
	PIMAGE_NT_HEADERS nt_head =
		(PIMAGE_NT_HEADERS)(m_File + dos_head->e_lfanew);

	// 获取扩展头
	PIMAGE_OPTIONAL_HEADER option_head =
		(PIMAGE_OPTIONAL_HEADER)(&nt_head->OptionalHeader);

	// 获取重定位表，在数据目录表的下标为 5 的位置
	DWORD reloc_rva = option_head->DataDirectory[5].VirtualAddress;
	DWORD reloc_foa = RVA2FOA(reloc_rva);

	// 找到重定位表的第一项
	PIMAGE_BASE_RELOCATION rel =
		(PIMAGE_BASE_RELOCATION)(reloc_foa + m_File);

	int index11 = 0;
	int index22 = 0;
	while (rel->SizeOfBlock != 0)
	{

		// 获取页内的需要重定位的项数
		DWORD count = (rel->SizeOfBlock - 8) / 2;
		DWORD base_rva = rel->VirtualAddress;
		PTYPEOFFSET table = PTYPEOFFSET(rel + 1);

		//区段信息
		CString index1;
		index1.Format(_T("%d"), index11);
		m_ListCtrl.InsertItem(index11, index1);

		CString section_rva;
		section_rva.Format(_T("%08X"), base_rva);
		m_ListCtrl.SetItemText(index11, 1, section_rva);

		CString items;
		items.Format(_T("%08X"), count);
		m_ListCtrl.SetItemText(index11, 2, items);

		index11++;

		for (int i = 0; i < count; i++)
		{

			if (table[i].Type == 3)
			{

				// 计算出真正要重定位的地址，并输出地址上的内容
				DWORD real_rva = base_rva + table[i].Offset;
				DWORD real_foa = RVA2FOA(real_rva);

				PDWORD data = (PDWORD)(real_foa + m_File);
				/*printf("  index : %d   RVA: %x  FOA:  %x  data: %x  \n",
					i, real_rva, real_foa, *data);*/

				CString index;
				index.Format(_T("%d"), index22);
				m_ListCtrl2.InsertItem(index22, index);

				CString RVA;
				RVA.Format(_T("%08X"), real_rva);
				m_ListCtrl2.SetItemText(index22, 1, RVA);

				CString FOA;
				FOA.Format(_T("%08X"), real_foa);
				m_ListCtrl2.SetItemText(index22, 2, FOA);

				CString DATA;
				DATA.Format(_T("%08X"), data);
				m_ListCtrl2.SetItemText(index22, 3, DATA);

				index22++;
			}
		}
		rel = (PIMAGE_BASE_RELOCATION)((DWORD)rel + rel->SizeOfBlock);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



DWORD CMyRelocation::RVA2FOA(DWORD rva)
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