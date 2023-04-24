// CMyResource.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyResource.h"


// CMyResource 对话框

IMPLEMENT_DYNAMIC(CMyResource, CDialogEx)

CMyResource::CMyResource(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESOURCE, pParent)
{

}

CMyResource::~CMyResource()
{

}

void CMyResource::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CMyResource, CDialogEx)
END_MESSAGE_MAP()


// CMyResource 消息处理程序


BOOL CMyResource::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_ListCtrl.InsertColumn(0, _T("资源属性"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("资源ID"), 0, 100);
	m_ListCtrl.InsertColumn(2, _T("资源RVA"), 0, 100);
	m_ListCtrl.InsertColumn(3, _T("资源大小"), 0, 100);

	m_ListCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// 先获取 dos 头
	PIMAGE_DOS_HEADER dos_head =
		(PIMAGE_DOS_HEADER)m_File;
	// 根据 dos 头获取 nt 头
	PIMAGE_NT_HEADERS nt_head =
		(PIMAGE_NT_HEADERS)(m_File + dos_head->e_lfanew);

	// 获取扩展头
	PIMAGE_OPTIONAL_HEADER option_head =
		(PIMAGE_OPTIONAL_HEADER)(&nt_head->OptionalHeader);

	// 获取资源表，在数据目录表的下标为 2 的位置
	DWORD resource_rva = option_head->DataDirectory[2].VirtualAddress;
	DWORD resource_foa = RVA2FOA(resource_rva);

	// 找到资源表的位置
	// 保存资源表的起始位置
	DWORD res_base = (resource_foa + (DWORD)m_File);

	PIMAGE_RESOURCE_DIRECTORY res_table =
		(PIMAGE_RESOURCE_DIRECTORY)res_base;

	// 获取资源种类的数量
	DWORD name_count = res_table->NumberOfNamedEntries;
	DWORD id_count = res_table->NumberOfIdEntries;
	DWORD count = name_count + id_count;

	// 得到第一层的directory_entry 结构
	PIMAGE_RESOURCE_DIRECTORY_ENTRY first_table =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(res_table + 1);
	int index = 0;
	for (int i = 0; i < count; i++)
	{
		// 判断名称的资源还是id资源
		if (first_table->NameIsString != 1)
		{
			switch (first_table->Id)
			{
			case 0x1:
				m_ListCtrl.InsertItem(i, L"CURSON");
				//printf("Cursor\n"); 
				break;

			case 0x2:
				m_ListCtrl.InsertItem(i, L"Bitmap");
				//printf("Bitmap\n"); 
				break;
			case 0x3:
				m_ListCtrl.InsertItem(i, L"Icon");
				//printf("Icon\n");
				break;
			case 0x4:
				m_ListCtrl.InsertItem(i, L"Menu");
				//printf("Menu\n"); 
				break;
			case 0x5:
				m_ListCtrl.InsertItem(i, L"Dialog");
				//printf("Dialog\n");
				break;
			case 0x6:
				m_ListCtrl.InsertItem(i, L"String Table");
				//printf("String Table\n");
				break;
			case 0x7:
				m_ListCtrl.InsertItem(i, L"Font Directory");
				//printf("Font Directory\n");
				break;
			case 0x8:
				m_ListCtrl.InsertItem(i, L"Font");
				//printf("Font\n"); 
				break;
			case 0x9:
				m_ListCtrl.InsertItem(i, L"Accelerators");
				//printf("Accelerators\n"); 
				break;
			case 0xA:
				m_ListCtrl.InsertItem(i, L"Unformatted");
				//printf("Unformatted\n"); 
				break;
			case 0xB:
				m_ListCtrl.InsertItem(i, L"Message Table");
				//printf("Message Table\n"); 
				break;
			case 0xC:
				m_ListCtrl.InsertItem(i, L"Group Cursor");
				//printf("Group Cursor\n"); 
				break;
			case 0xE:
				m_ListCtrl.InsertItem(i, L"Group Icon");
				//printf("Group Icon\n");
				break;
			case 0x10:
				m_ListCtrl.InsertItem(i, L"Version Information");
				//printf("Version Information\n"); 
				break;
			default:
				m_ListCtrl.InsertItem(i, L"Undefined");
				//printf("Undefined\n");
				break;
			}
		}
		// 这里是一个字符串资源
		else
		{
			PIMAGE_RESOURCE_DIR_STRING_U name_string =
				(PIMAGE_RESOURCE_DIR_STRING_U)(res_base + first_table->NameOffset);
			PWCHAR temp = new WCHAR[name_string->Length + 1]{};
			memcpy_s(temp, name_string->Length * 2,
				name_string->NameString, name_string->Length * 2);
			//printf("%S\n", temp);
			m_ListCtrl.InsertItem(i,temp);
		}

		// 找第二层的数据，每一种资源有多少个
		PIMAGE_RESOURCE_DIRECTORY second =
			(PIMAGE_RESOURCE_DIRECTORY)(first_table->OffsetToDirectory + res_base);

		// 数量
		DWORD res_count = second->NumberOfNamedEntries + second->NumberOfIdEntries;
		PIMAGE_RESOURCE_DIRECTORY_ENTRY second_table =
			(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(second + 1);

		for (int i = 0; i < res_count; i++)
		{
			if (second_table->NameIsString != 1)
			{
				//printf("资源ID ：%x", second_table->Name);
				CString id;
				id.Format(_T("%08X"), second_table->Name);
				m_ListCtrl.SetItemText(index, 1, id);
			}
			else
			{
				PIMAGE_RESOURCE_DIR_STRING_U name_string =
					(PIMAGE_RESOURCE_DIR_STRING_U)(res_base + second_table->NameOffset);
				PWCHAR temp = new WCHAR[name_string->Length + 1]{};
				memcpy_s(temp, name_string->Length * 2,
					name_string->NameString, name_string->Length * 2);
				//printf("%S\n", temp);
				m_ListCtrl.SetItemText(index, 1, temp);
			}

			// 找第三层
			if (second_table->DataIsDirectory == 1)
			{
				PIMAGE_RESOURCE_DIRECTORY third =
					(PIMAGE_RESOURCE_DIRECTORY)(second_table->OffsetToDirectory + res_base);
				PIMAGE_RESOURCE_DIRECTORY_ENTRY third_table =
					(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(third + 1);
				PIMAGE_RESOURCE_DATA_ENTRY data =
					(PIMAGE_RESOURCE_DATA_ENTRY)(third_table->OffsetToData + res_base);
				/*printf("资源的RVA：%X， 资源的大小 %X\n",
					data->OffsetToData, data->Size);*/
				CString RVA;
				RVA.Format(_T("%08X"), data->OffsetToData);
				m_ListCtrl.SetItemText(index, 2, RVA);

				CString size;
				size.Format(_T("%08X"), data->Size);
				m_ListCtrl.SetItemText(index, 3, size);
				index++;
			}
			second_table++;
		}

		first_table++;
	}




	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


DWORD CMyResource::RVA2FOA(DWORD rva)
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