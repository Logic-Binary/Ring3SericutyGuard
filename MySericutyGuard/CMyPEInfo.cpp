// CMyPEInfo.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyPEInfo.h"
#include "resource.h"
#include "MySection.h"
#include "CMyDirectory.h"

// CMyPEInfo 对话框

IMPLEMENT_DYNAMIC(CMyPEInfo, CDialogEx)

CMyPEInfo::CMyPEInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYPEINFO, pParent)
	, szFileName(_T(""))
	, m_edit2(_T(""))
	, m_File(NULL)
	, m_edit3(_T(""))
	, m_edit4(_T(""))
	, m_edit5(_T(""))
	, m_edit6(_T(""))
	, m_edit7(_T(""))
	, m_edit8(_T(""))
	, m_edit9(_T(""))
	, m_edit10(_T(""))
	, m_edit11(_T(""))
	, m_calc(_T(""))
	, m_VA(_T(""))
	, m_FOA(_T(""))
{

}

CMyPEInfo::~CMyPEInfo()
{
}

void CMyPEInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, szFileName);
	DDX_Text(pDX, IDC_EDIT2, m_edit2);
	DDX_Text(pDX, IDC_EDIT3, m_edit3);
	DDX_Text(pDX, IDC_EDIT4, m_edit4);
	DDX_Text(pDX, IDC_EDIT5, m_edit5);
	DDX_Text(pDX, IDC_EDIT6, m_edit6);
	DDX_Text(pDX, IDC_EDIT7, m_edit7);
	DDX_Text(pDX, IDC_EDIT8, m_edit8);
	DDX_Text(pDX, IDC_EDIT9, m_edit9);
	DDX_Text(pDX, IDC_EDIT10, m_edit10);
	DDX_Text(pDX, IDC_EDIT11, m_edit11);
	DDX_Text(pDX, IDC_EDIT12, m_calc);
	DDX_Text(pDX, IDC_EDIT13, m_VA);
	DDX_Text(pDX, IDC_EDIT14, m_FOA);
	DDX_Control(pDX, IDC_EDIT12, m_Rva);
}

BEGIN_MESSAGE_MAP(CMyPEInfo, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyPEInfo::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyPEInfo::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CALC, &CMyPEInfo::OnBnClickedCalc)
END_MESSAGE_MAP()


// CMyPEInfo 消息处理程序


void CMyPEInfo::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DWORD dwFileCount = DragQueryFile(hDropInfo,-1,NULL,0);

	if (dwFileCount == 1) {
		TCHAR szFileName1[MAX_PATH];
		DragQueryFile(hDropInfo, 0, szFileName1, MAX_PATH);
		szFileName = szFileName1;

		//打开文件，读取文件信息
		HANDLE hFile = CreateFile(
			szFileName, GENERIC_READ,
			FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, NULL, NULL
		);
		if (hFile == INVALID_HANDLE_VALUE) {
			MessageBox(_T("文件打开失败"));
			return;
		}
		DWORD dwSize = GetFileSize(hFile, NULL);
		m_File = new CHAR[dwSize]{};
		DWORD dwRealRead = 0;
		ReadFile(hFile, m_File, dwSize, &dwRealRead, NULL);
		//判断是否是PE文件
		if (!(is_pe(m_File))) {
			MessageBox(_T("不是PE文件"));
			CloseHandle(hFile);
			return;
		}
		//是pe文件，开始往控件中填写数据
		addInfo(m_File);

		UpdateData(FALSE);
		DragFinish(hDropInfo);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

BOOL CMyPEInfo::is_pe(CHAR* file)
{
	//获取DOS头
	PIMAGE_DOS_HEADER dos_header = PIMAGE_DOS_HEADER(file);
	if (dos_header->e_magic == 0x5a4d) {
		//获取NT头
		PIMAGE_NT_HEADERS nt_header = PIMAGE_NT_HEADERS(dos_header->e_lfanew + file);
		if (nt_header->Signature == 0x00004550) {
			return TRUE;
		}
	}
	else {
		return FALSE;
	}

	return 0;
}

void CMyPEInfo::addInfo(CHAR* file)
{
	PIMAGE_DOS_HEADER dos_header = PIMAGE_DOS_HEADER(m_File);
	PIMAGE_NT_HEADERS nt_header = PIMAGE_NT_HEADERS(dos_header->e_lfanew + m_File);
	PIMAGE_OPTIONAL_HEADER optional_header = PIMAGE_OPTIONAL_HEADER(&nt_header->OptionalHeader);
	CString entry;
	entry.Format(_T("%08x"), optional_header->AddressOfEntryPoint);
	m_edit2 = entry;

	CString base_OEP;
	base_OEP.Format(_T("%08x"), optional_header->ImageBase);
	m_edit3 = base_OEP;

	CString size;
	size.Format(_T("%08x"), optional_header->SizeOfImage);
	m_edit4 = size;

	CString base_code;
	base_code.Format(_T("%08X"), optional_header->BaseOfCode);
	m_edit5 = base_code;

	CString section_ali;
	section_ali.Format(_T("%08X"), optional_header->SectionAlignment);
	m_edit6 = section_ali;

	CString file_ali;
	file_ali.Format(_T("%08X"), optional_header->FileAlignment);
	m_edit7 = file_ali;

	CString section_count;
	section_count.Format(_T("%08X"), nt_header->FileHeader.NumberOfSections);
	m_edit8 = section_count;

	CString data;
	data.Format(_T("%08X"), nt_header->FileHeader.TimeDateStamp);
	m_edit9 = data;

	CString verson;
	verson.Format(_T("%08X"), optional_header->MajorSubsystemVersion);
	m_edit10 = verson;

	CString hSize;
	hSize.Format(_T("%08X"), nt_header->FileHeader.SizeOfOptionalHeader);
	m_edit11 = hSize;

}

void CMyPEInfo::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	MySection section;
	section.m_File = m_File;
	section.DoModal();
	
}


void CMyPEInfo::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyDirectory directory;
	directory.m_File = m_File;
	directory.DoModal();

}


void CMyPEInfo::OnBnClickedCalc()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_File == NULL) {
		return;
	}
	PIMAGE_DOS_HEADER dos_header = PIMAGE_DOS_HEADER(m_File);
	PIMAGE_NT_HEADERS nt_header = PIMAGE_NT_HEADERS(dos_header->e_lfanew + m_File);
	DWORD base = nt_header->OptionalHeader.ImageBase;

	CString RVA;
	m_Rva.GetWindowText(RVA);
	DWORD rva = _tcstoul(RVA, NULL, 16);

	DWORD foa = RVA2FOA(rva);
	if (foa == -1) {
		MessageBox(_T("请输入正确的RVA"));
		return;
	}
	CString FOA;
	FOA.Format(_T("%08X"), foa);
	m_FOA = FOA;

	DWORD va = base + rva;
	CString VA;
	VA.Format(_T("%08X"), va);
	m_VA = VA;

	UpdateData(FALSE);
	UpdateData(TRUE);

}

DWORD CMyPEInfo::RVA2FOA(DWORD rva)
{

	if (rva == 0) {
		return -1;
	}

	// 先获取 dos 头
	PIMAGE_DOS_HEADER dos_head =
		(PIMAGE_DOS_HEADER)m_File;
	// 根据 dos 头获取 nt 头
	PIMAGE_NT_HEADERS nt_head =
		(PIMAGE_NT_HEADERS)(m_File + dos_head->e_lfanew);

	// 先要获取区段头的位置
	// 从起始位置 + dos头+ dos_stub + nt头大小（文件头，扩展头）
	// 还可以通过宏定义获取
	PIMAGE_SECTION_HEADER section_head = IMAGE_FIRST_SECTION(nt_head);

	// 遍历区段，判断所给定的 rva 在哪一个区段中
	DWORD count = nt_head->FileHeader.NumberOfSections;
	for (int i = 0; i < count; i++)
	{
		// 有些内容在文件中没有记录
		// 所以用文件大小更合理
		if (section_head->VirtualAddress <= rva &&
			rva <= section_head->VirtualAddress +
			// section_head->Misc.VirtualSize)
			section_head->SizeOfRawData)
		{
			DWORD foa = 0;
			// 如果 rva 落在这个区段中，rva - 区段起始rva + 文件起始偏移
			foa = rva - section_head->VirtualAddress
				+ section_head->PointerToRawData;
			return foa;
		}
		// 如果在文件中没有找到，那么就返回 0 代表错误

		section_head++;
	}
	return -1;
}

