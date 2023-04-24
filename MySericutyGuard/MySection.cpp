// MySection.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "MySection.h"


// MySection 对话框

IMPLEMENT_DYNAMIC(MySection, CDialog)

MySection::MySection(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MySection, pParent),
	m_File(NULL)
{

}

MySection::~MySection()
{

}

void MySection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}


BEGIN_MESSAGE_MAP(MySection, CDialog)
END_MESSAGE_MAP()


// MySection 消息处理程序


BOOL MySection::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	if (m_File == NULL) {
		return FALSE;
	}

	m_listCtrl.InsertColumn(0, _T("名称"), 0, 100);
	m_listCtrl.InsertColumn(1, _T("VOffset"), 0, 100);
	m_listCtrl.InsertColumn(2, _T("Vsize"), 0, 100);
	m_listCtrl.InsertColumn(3, _T("ROffset"), 0, 100);
	m_listCtrl.InsertColumn(4, _T("RSize"), 0, 100);

	m_listCtrl.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	
	//拿到区段头表的指针
	PIMAGE_DOS_HEADER dos_header = PIMAGE_DOS_HEADER(m_File);
	PIMAGE_NT_HEADERS nt_header = PIMAGE_NT_HEADERS(dos_header->e_lfanew + m_File);
	PIMAGE_SECTION_HEADER section_header = IMAGE_FIRST_SECTION(nt_header);

	for (int i = 0; i < nt_header->FileHeader.NumberOfSections; i++) {
		//开始插入数据
		CString name;
		PCHAR p1 = (PCHAR)section_header->Name;
		BYTE p2[2] = {};
		p2[0] = p1[8];
		p2[1] = p1[9];
		p1[8] = 0x00;
		p1[9] = 0x00;
		name.Format(_T("%S"), section_header->Name);
		m_listCtrl.InsertItem(i, name);
		p1[8] = p2[0];
		p1[9] = p2[1];

		CString RVA;
		RVA.Format(_T("%08x"), section_header->VirtualAddress);
		m_listCtrl.SetItemText(i,1, RVA);

		CString VSize;
		VSize.Format(_T("%08x"), section_header->Misc.VirtualSize);
		m_listCtrl.SetItemText(i, 2, VSize);

		CString ROffset;
		ROffset.Format(_T("%08x"), section_header->PointerToRawData);
		m_listCtrl.SetItemText(i,3,ROffset);

		CString RSize;
		RSize.Format(_T("%08x"), section_header->SizeOfRawData);
		m_listCtrl.SetItemText(i,4,RSize);
		section_header++;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
