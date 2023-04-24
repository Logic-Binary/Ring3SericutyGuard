// CMyFiles.cpp: 实现文件
//

#include "pch.h"
#include "MySericutyGuard.h"
#include "afxdialogex.h"
#include "CMyFiles.h"
#include "resource.h"
#include "MD5.h"
#include <TlHelp32.h>
#include <strsafe.h>
#include <powrprof.h>
#include "CMyControl.h"

#define TXTPATH L"C:\\Users\\罗辑\\Desktop\\安全卫士\\MySericutyGuard\\MySericutyGuard\\md5.txt"
#define BLACKPATH L"C:\\Users\\罗辑\\Desktop\\安全卫士\\MySericutyGuard\\MySericutyGuard\\黑名单.txt"
#define TXTPATH2 L"C:\\Users\\罗辑\Desktop\\安全卫士\\MySericutyGuard\\MySericutyGuard\\md5-副本.txt"

PCHAR CMyFiles::pMD5 = NULL;
CWinThread* CMyFiles::h1 = NULL;
CWinThread* CMyFiles::h2 = NULL;
// CMyFiles 对话框

IMPLEMENT_DYNAMIC(CMyFiles, CDialogEx)

CMyFiles::CMyFiles(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYFILES, pParent)
	, m_szFileName(_T(""))
	, m_bClose(TRUE)
	, m_CPUNum(_T(""))
	, m_CPUNum2(_T(""))
	, m_PATH(_T(""))
	, m_HouZhui(_T(""))
	, m_cpunum1(_T(""))
	, m_memnum1(_T(""))
{

}

CMyFiles::~CMyFiles()
{
}

void CMyFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_szFileName);
	DDX_Text(pDX, IDC_EDIT2, m_CPUNum);
	DDX_Text(pDX, IDC_EDIT3, m_CPUNum2);
	DDX_Text(pDX, IDC_EDIT4, m_PATH);
	DDX_Text(pDX, IDC_EDIT5, m_HouZhui);
	DDX_Text(pDX, IDC_STATIC_1, m_cpunum1);
	DDX_Text(pDX, IDC_STATIC_2, m_memnum1);
	DDX_Control(pDX, IDC_STATIC_1, m_cpu111);
	DDX_Control(pDX, IDC_STATIC_2, m_mem111);
}


BEGIN_MESSAGE_MAP(CMyFiles, CDialogEx)
	//ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMyFiles::OnTvnSelchangedTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CMyFiles::OnDblclkTree1)
	ON_WM_DROPFILES()
	//Button1是读，Button2是写
	ON_BN_CLICKED(IDC_BUTTON1, &CMyFiles::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyFiles::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyFiles::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyFiles::OnBnClickedButton4)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMyFiles::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyFiles::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMyFiles::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyFiles::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMyFiles::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CMyFiles::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CMyFiles::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CMyFiles::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CMyFiles::OnBnClickedButton12)
END_MESSAGE_MAP()


// CMyFiles 消息处理程序


BOOL CMyFiles::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//如果程序是管理员权限运行，那么拖拽文件消息会被过滤
	//添加如下代码，解决以上问题
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	//0x0049 WM_COPYGLOBALDATA
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);

	//获取到当前电脑的所有盘符,作为根节点添加到树控件中
	TCHAR szDriverPath[MAX_PATH] = {};
	GetLogicalDriveStrings(MAX_PATH, szDriverPath);
	//循环插入到树控件
	TCHAR* drivers = szDriverPath;
	while (*drivers != NULL)
	{
		//把当前盘符添加到树控件中
		m_TreeCtrl.InsertItem(drivers);

		//指向下一个盘符
		drivers += _tcslen(drivers) + 1;
	}

	//创建CPU占用率线程
	hThread = AfxBeginThread(MyThreadFunction, this);

	//获取一下md5值
	HANDLE hTxtFile = CreateFile(TXTPATH, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (hTxtFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("文件打开失败"));
		return FALSE;
	}
	DWORD txt_size = GetFileSize(hTxtFile, NULL);
	PCHAR sztxt = new CHAR[txt_size+1]{};

	//写入
	DWORD RealRead = 0;
	ReadFile(hTxtFile, sztxt, txt_size, &RealRead, NULL);
	pMD5 = sztxt;

	CloseHandle(hTxtFile);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


//void CMyFiles::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


void CMyFiles::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	//获取当前点击的节点
	HTREEITEM hTreeItem = m_TreeCtrl.GetSelectedItem();

	//获取当前节点的完整路径
	CString szFullPath = GetFullPath(hTreeItem);
	//获取到当前路径下面的所有文件
	//添加到动态数组中
	vector<CString> vecFiles = GetSubFiles(szFullPath);
	//先要清空当前节点下的所有内容
	while (m_TreeCtrl.ItemHasChildren(hTreeItem)) {
		HTREEITEM temp = m_TreeCtrl.GetChildItem(hTreeItem);
		m_TreeCtrl.DeleteItem(temp);
	}

	//遍历动态数组，依次插入到双击的节点下
	for (int i = 0; i < vecFiles.size(); i++) {
		m_TreeCtrl.InsertItem(vecFiles[i], hTreeItem);
	}
	*pResult = 0;
}

CString CMyFiles::GetFullPath(HTREEITEM hTreeItem)
{
	//先获取父节点
	HTREEITEM hParentItem;
	hParentItem = m_TreeCtrl.GetParentItem(hTreeItem);
	//如果父节点不为空，就递归调用当前函数
	CString path;
	if (hParentItem != NULL)
	{
		//获取当前的节点名称
		path = m_TreeCtrl.GetItemText(hTreeItem);
		path = GetFullPath(hParentItem) + _T("//") + path;
		return path;
	}
	//否则直接返回路径
	else {
		path = m_TreeCtrl.GetItemText(hTreeItem);
		return path;
	}

	return CString();
}

vector<CString> CMyFiles::GetSubFiles(CString path)
{
	vector<CString> vecResult;

	//根据传入的路径，判断是一个文件还是一个文件夹
	//如果是文件，就可以获取文件的具体信息，创建时间，md5
	DWORD dwAttribute = GetFileAttributes(path);

	if ((dwAttribute & FILE_ATTRIBUTE_DIRECTORY) == 0) {
		//MessageBox(_T("这是一个文件"));
		WIN32_FIND_DATA stcFData = {};
		FindFirstFile(path, &stcFData);

		//计算大小
		CString size;
		CString out;
		size.Format(_T("%d"), stcFData.nFileSizeLow);
		out = _T("文件大小:") + size + _T("\kb\r\n");
		//计算时间
		SYSTEMTIME sysTime = {};
		PFILETIME pTime = &stcFData.ftCreationTime;
		FileTimeToSystemTime(pTime, &sysTime);
		CString yearTime;
		yearTime.Format(_T("%d"), sysTime.wYear);
		CString monthTime;
		monthTime.Format(_T("%d"), sysTime.wMonth);
		CString dayTime;
		dayTime.Format(_T("%d"), sysTime.wDay);
		out += _T("创建时间:") + yearTime + _T("/") + monthTime + _T("/") + dayTime + _T("\r\n");

		CString md5 = calMD5(path);
		out += _T("MD5:") + md5;

		MessageBox(out);
	}
	//否则就是一个文件夹，获取里面的内容
	else {
		//先拼接路径，然后通过 FindFirstFile() 遍历当前的所有子文件
		CString temp = path + _T("\\*");
		WIN32_FIND_DATA file = {};
		HANDLE hFile = FindFirstFile(temp, &file);
		do {
			if (_tcscmp(file.cFileName, _T(".")) == 0 ||
				_tcscmp(file.cFileName, _T("..")) == 0)
			{
				continue;
			}
			vecResult.push_back(file.cFileName);

		} while (FindNextFile(hFile, &file));
	}
	return vecResult;
}

CString CMyFiles::calMD5(CString path) {
	MD5 md5;
	HANDLE hFile = CreateFile(
		path,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	//读文件
	PCHAR szReadBuff = new CHAR[dwFileSize + 1]{};
	DWORD dwRealReadSize = 0;
	ReadFile(hFile, szReadBuff, dwFileSize, &dwRealReadSize, NULL);
	md5.update(szReadBuff);

	CString cs;
	string result = md5.toString();
	cs = result.c_str();

	if (szReadBuff != NULL) {
		delete[] szReadBuff;
		szReadBuff = NULL;
	}
	CloseHandle(hFile);
	return cs;
}

void CMyFiles::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//先修改对话框属性，接收文件
	DWORD dwFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);
	if (dwFileCount == 1) {
		TCHAR szFileName[MAX_PATH] = {};
		DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
		m_szFileName = szFileName;
		UpdateData(FALSE);
		DragFinish(hDropInfo);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//本地杀毒的功能的实现，往文件中添加特征值，读取文件中的的内容
//跟要查杀的文件进行对比
//读文件写文件，写入文件后，如何区分一条一条的数组(\r\n)


//写入文件
void CMyFiles::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 先判断文本框中的内容不为空
	UpdateData(TRUE);
	if (m_szFileName.IsEmpty()) {
		return;
	}
	//打开文件
	HANDLE hFile = CreateFile(
		_T(".\\test.txt"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE, NULL,
		OPEN_ALWAYS, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(_T("文件打开失败"));
		return;
	}
	//在文件末尾追加新的内容
	//把文件指针设置到文件末尾,通过读文件设置指针
	DWORD dwSize = GetFileSize(hFile, NULL);
	CHAR* szFileBuff = new CHAR[dwSize]{};
	DWORD dwRealRead = 0;
	ReadFile(hFile, szFileBuff, dwSize, &dwRealRead, NULL);
	//SetFilePointer()  //设置文件指针位置
	//int a = SetEndOfFile(hFile);    //将指定文件的物理文件大小设置为文件指针的当前位置

	//把编辑框中的内容写入文件,拼接一个\r\n
	//默认 CString是wchar，写入到文件中会有显示问题
	//先转char类型
	m_szFileName += (_T("\r\n"));

	USES_CONVERSION;
	CHAR* buff = W2A(m_szFileName.GetBuffer());
	DWORD dwRealWrite;
	WriteFile(hFile, buff, strlen(buff), &dwRealWrite, NULL);
	MessageBox(_T("添加成功"));
	//关闭文件
	CloseHandle(hFile);
}

//读数据
void CMyFiles::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//读文件
	HANDLE hFile = CreateFile(
		_T(".\\test.txt"), GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("文件打开失败"));
		return;
	}
	//读取文件内容
	DWORD dwSize = GetFileSize(hFile, NULL);
	//CString保存读取到的文件内容
	CString temp;
	if (dwSize != 0) {
		char* szFileBuff = new CHAR[dwSize]{};
		DWORD dwRealRead = 0;
		ReadFile(hFile, szFileBuff, dwSize, &dwRealRead, NULL);
		temp = szFileBuff;
	}

	//拆分文件内容
	CString temp1;
	vector<CString> vecFileContent;
	for (int i = 0; i < temp.GetLength(); i++) {
		//逐个字节对比内容，如果是\r\n == \x0d\x0a
		//证明读到了一条内容的结尾，添加到动态数组中
		if (memcmp(temp.GetBuffer() + i, _T("\x0d"), 1) == 0) {
			vecFileContent.push_back(temp1);
			//跳过\x0a
			temp1 = _T("");
			i++;
		}

		//拼接一个临时的CString
		temp1 += temp[i];
	}

	//动态数组中就是所有文件内容
	for (int i = 0; i < vecFileContent.size(); i++)
	{
		//具体功能,计算这个地址的md5值，跟数组里的md5比较。
		//如果相同，则删除该文件
		CString path = vecFileContent[i];
		MD5 md5;
		CString csmd5;
		//md5.reset();
		csmd5 = calMD5(path);

		HANDLE hTxtFile = CreateFile(TXTPATH, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
		if (hTxtFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(_T("文件打开失败"));
			return;
		}
		DWORD txt_size = GetFileSize(hTxtFile, NULL);
		PCHAR sztxt = new CHAR[txt_size]{};
		PCHAR old = sztxt;


		//写入
		DWORD RealRead = 0;
		ReadFile(hTxtFile, sztxt, txt_size, &RealRead, NULL);

		//循环比较md5值是否相同，相同则提示为病毒文件
		char* str = new char[33] {};
		for (int j = 0; j < 2; j++) {
			memcpy(str, sztxt, 32);
			CString tempstr;
			tempstr.Format(L"%S", str);
			if (csmd5 == tempstr) {
				CString out = _T("检测到病毒\r\n");
				out += path;
				MessageBox(out);
			}

			//txtmd5.push_back(str);
			//因为后面有\r\n所以多了2个字节
			sztxt += 34;
		}
		if (str != NULL) {
			delete[] str;
			str = NULL;
		}
		//释放
		sztxt = old;
		if (sztxt != NULL) {
			delete[] sztxt;
			sztxt = NULL;
		}

		CloseHandle(hTxtFile);
	}

	//关闭文件
	CloseHandle(hFile);
	
}

//黑名单
void CMyFiles::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		return;
	}
	PROCESSENTRY32 stcPe32 = { sizeof(PROCESSENTRY32) };
	Process32First(hProcessSnap, &stcPe32);

	HANDLE hProc = CreateFile(BLACKPATH, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	DWORD size = GetFileSize(hProc, NULL);
	CHAR* buff = new CHAR[size + 1]{};
	DWORD dwRealRead = 0;
	ReadFile(hProc, buff, size, &dwRealRead, NULL);

	BOOL is_Find = false;
	do
	{
		USES_CONVERSION;

		WCHAR* p1 = stcPe32.szExeFile;
		CHAR* p2 = new char[20] {};
		p2 = W2A(p1);
		if (strcmp(buff, p2) == 0) {
			MessageBox(_T("找到"));
			HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, stcPe32.th32ProcessID);
			TerminateProcess(h, NULL);

			is_Find = TRUE;
			CloseHandle(h);
		}
	} while (Process32Next(hProcessSnap, &stcPe32));

	if (!is_Find) {
		MessageBox(_T("没找到"));
	}

	CloseHandle(hProcessSnap);
	CloseHandle(hProc);
}

//清理垃圾按钮
void CMyFiles::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//清理指定路径的
	UpdateData(TRUE);
	WCHAR* path = m_PATH.AllocSysString();

	WCHAR* realSuffix = m_HouZhui.AllocSysString();
	WCHAR suffix1[10] = { _T(".zzz") };
	PWCHAR p1 = suffix1;
	WCHAR suffix2[10] = { _T(".zzz") };
	PWCHAR p2 = suffix2;
	WCHAR suffix3[10] = { _T(".zzz") };
	PWCHAR p3 = suffix3;

	if (realSuffix == 0) {
		return;
	}
	int i = 0;
	while (1) {
		while (!(realSuffix[0] == L'\\')) {
			memcpy(p1, realSuffix, 2);
			p1++;
			realSuffix++;
		}
		if (realSuffix[1] == 0) {
			break;
		}
		else {
			realSuffix++;
		}
		while (!(realSuffix[0] == L'\\')) {
			memcpy(p2, realSuffix, 2);
			p2++;
			realSuffix++;
		}
		if (realSuffix[1] == 0) {
			break;
		}
		else {
			realSuffix++;
		}
		while (!(realSuffix[0] == L'\\')) {
			memcpy(p3, realSuffix, 2);
			p3++;
			realSuffix++;
		}
		if (realSuffix[1] == 0) {
			break;
		}
	}

	EnumFoldAndDeleteFile(path, suffix1, suffix2, suffix3);
}

//清理垃圾
ULARGE_INTEGER CMyFiles::EnumFoldAndDeleteFile(
	WCHAR* lpFoldPath,
	WCHAR* suffix1,
	WCHAR* suffix2,
	WCHAR* suffix3

) {
	//初始化路径字符串(在路径后要拼接一个*)
	WCHAR szFoldPath[MAX_PATH] = {};
	DWORD size = wcslen(lpFoldPath) * 2;
	memcpy(szFoldPath, lpFoldPath, size);
	//拼接*
	StringCchCat(szFoldPath, MAX_PATH, L"\\* ");

	//获取第一个文件信息
	ULARGE_INTEGER qwFileTotalSize = {};
	WIN32_FIND_DATA w32FindData = {};
	HANDLE hFindFile = FindFirstFile(szFoldPath, &w32FindData);
	BOOL is_Find = FALSE;
	//循环遍历获取当前目录中的文件信息
	do {
		//去掉特殊目录
		if ((!wcscmp(w32FindData.cFileName, L".")) ||
			(!wcscmp(w32FindData.cFileName, L".."))) {
			continue;
		}
		if (w32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//文件夹，继续往下走
			WCHAR szSubfoldPath[MAX_PATH];
			//追加反斜杠
			StringCchPrintf(szSubfoldPath, MAX_PATH, L"%s\\", lpFoldPath);
			//StringCchPrintf(szSubfoldPath, MAX_PATH, L"%s\\", lpFoldPath);
			//子目录名
			StringCchCat(szSubfoldPath, MAX_PATH, w32FindData.cFileName);
			qwFileTotalSize = EnumFoldAndDeleteFile(szSubfoldPath, suffix1, suffix2, suffix3);
		}
		else {
			//获取文件后缀
			LPWSTR FileSuffix = PathFindExtension(w32FindData.cFileName);
			if (!lstrcmp(FileSuffix, suffix1) ||
				!lstrcmp(FileSuffix, suffix2) ||
				!lstrcmp(FileSuffix, suffix3)
				) {
				//拼接一下全路径
				WCHAR szFullPath[MAX_PATH] = {};
				DWORD size = wcslen(lpFoldPath) * 2;
				memcpy(szFullPath, lpFoldPath, size);
				//拼接
				StringCchCat(szFullPath, MAX_PATH, _T("\\"));
				StringCchCat(szFullPath, MAX_PATH, w32FindData.cFileName);
				DeleteFile(szFullPath);
				is_Find = TRUE;
			}
		}

	} while (FindNextFile(hFindFile, &w32FindData));
	if (is_Find) {
		MessageBox(_T("清理完毕"));
	}
	return qwFileTotalSize;
}


void CMyFiles::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


//锁屏
void CMyFiles::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	LockWorkStation();
}

//重启
void CMyFiles::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, NULL);
}

//注销
void CMyFiles::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, NULL);
}


double CMyFiles::FILETIME2Double(const _FILETIME& fileTime) {
	return double(fileTime.dwHighDateTime * 4.294967296e9) + double(fileTime.dwLowDateTime);
}

//CPU使用率
int CMyFiles::getCPUInfomation() {
	//空闲时间   内核时间  用户时间
	_FILETIME idleTime, kernelTime, userTime;
	//获取时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//等待1000毫秒使用内核对象会更精确
	WaitForSingleObject(hEvent, 1000);
	//获取新的时间
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	//将各个时间转换
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUserTime);
	//计算出使用率
	return int(100.0 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime) * 100.0);

}

UINT CMyFiles::MyThreadFunction(LPVOID lpParam) {
	CMyFiles* FileDlg = (CMyFiles*)lpParam;

	CString Buff1;
	CString Buff2;

	MEMORYSTATUS memStatus = {};
	memStatus.dwLength = sizeof(MEMORYSTATUS);

	while (FileDlg->m_bClose)
	{
		//CPU利用率
		Buff1.Format(L"利用率 %d %%", getCPUInfomation());
		FileDlg->m_cpu111.SetWindowText(Buff1);
		//FileDlg->m_CPUNum = Buff1;

		//内存占用率
		GlobalMemoryStatus(&memStatus);
		Buff2.Format(L"占用率 %d %%", (int)memStatus.dwMemoryLoad);
		FileDlg->m_mem111.SetWindowText(Buff2);
		//FileDlg->m_CPUNum2 = Buff2;
		//FileDlg->UpdateData(FALSE);
		Sleep(500);
	}

	return 0;
}


void CMyFiles::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyControl control = {};
	control.DoModal();

}


void CMyFiles::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	MEMORYSTATUSEX stcMem = {};
	stcMem.dwLength = sizeof(stcMem);
	GlobalMemoryStatusEx(&stcMem);
	DWORDLONG preUsedMem = stcMem.ullTotalPhys - stcMem.ullAvailPhys;

	DWORD dwPID[1000] = {};
	DWORD bufSize = sizeof(dwPID);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPID, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++) {
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, FALSE, dwPID[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
		CloseHandle(hProcess);
	}
	GlobalMemoryStatusEx(&stcMem);
	DWORDLONG afterCleanUsedMem =
		stcMem.ullTotalPhys - stcMem.ullAvailPhys;
	CString num;

	num.Format(L"占用率 %d %%", (int)stcMem.dwMemoryLoad);

	this->m_mem111.SetWindowText(num);
}

void CMyFiles::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码

	//创建线程A，准备S盘查杀
	h1 = AfxBeginThread(MyThreadProc1, this);
	
	//创建线程B，准备D盘查杀
	h2 = AfxBeginThread(MyThreadProc2, this);
}

//线程A----D盘
UINT CMyFiles::MyThreadProc1(LPVOID lpParam) {
	CMyFiles m;
	CMyFiles file;
	file.EnumFoldAndDeleteFile2(_T("S:"));
	::MessageBox(0, L"S盘搜索完毕", L"S", 0);
	return 0;
}

//线程B----S盘
UINT CMyFiles::MyThreadProc2(LPVOID lpParam) {
	CMyFiles file;
	file.EnumFoldAndDeleteFile(_T("D:\\7zip"));
	::MessageBox(0, L"D盘搜索完毕", L"D", 0);
	return 0;
}

//递归遍历
ULARGE_INTEGER CMyFiles::EnumFoldAndDeleteFile(WCHAR* lpFoldPath) {
	char* oldPMD5 = pMD5;
	//初始化路径字符串(在路径后要拼接一个*)
	WCHAR szFoldPath[MAX_PATH] = {};
	DWORD size = wcslen(lpFoldPath) * 2;
	memcpy(szFoldPath, lpFoldPath, size);
	//拼接*
	StringCchCat(szFoldPath, MAX_PATH, L"\\* ");

	//获取第一个文件信息
	ULARGE_INTEGER qwFileTotalSize = {};
	WIN32_FIND_DATA w32FindData = {};
	HANDLE hFindFile = FindFirstFile(szFoldPath, &w32FindData);
	//循环遍历获取当前目录中的文件信息
	do {
		//去掉特殊目录
		if ((!wcscmp(w32FindData.cFileName, L".")) ||
			(!wcscmp(w32FindData.cFileName, L".."))) {
			continue;
		}
		if (w32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//文件夹，继续往下走
			WCHAR szSubfoldPath[MAX_PATH];
			//追加反斜杠
			StringCchPrintf(szSubfoldPath, MAX_PATH, L"%s\\", lpFoldPath);
			//StringCchPrintf(szSubfoldPath, MAX_PATH, L"%s\\", lpFoldPath);
			//子目录名
			StringCchCat(szSubfoldPath, MAX_PATH, w32FindData.cFileName);
			qwFileTotalSize = EnumFoldAndDeleteFile(szSubfoldPath);
		}
		else {
			//获取文件md5
			LPWSTR FileSuffix = PathFindExtension(w32FindData.cFileName);

			//拼接一下全路径
			WCHAR szFullPath[MAX_PATH] = {};
			DWORD size = wcslen(lpFoldPath) * 2;
			memcpy(szFullPath, lpFoldPath, size);
			//拼接
			StringCchCat(szFullPath, MAX_PATH, _T("\\"));
			StringCchCat(szFullPath, MAX_PATH, w32FindData.cFileName);
			//获取md5
			CString md5 = calMD5(szFullPath);

			//循环比较md5值是否相同，相同则提示为病毒文件
			char* str = new char[33] {};
			for (int j = 0; j < 2; j++) {
				memcpy(str, pMD5, 32);
				CString tempstr;
				tempstr.Format(L"%S", str);
				if (md5 == tempstr) {
					CString out = _T("检测到病毒\r\n");
					out += szFullPath;
					//暂停5秒
					Sleep(5000);
					MessageBox(out);
					
					//DeleteFile(szFullPath);
				}

				//txtmd5.push_back(str);
				//因为后面有\r\n所以多了2个字节
				pMD5 += 34;
			}
			pMD5 = oldPMD5;
			if (str != NULL) {
				delete[] str;
				str = NULL;
			}		
		}

	} while (FindNextFile(hFindFile, &w32FindData));

	return qwFileTotalSize;
}


ULARGE_INTEGER CMyFiles::EnumFoldAndDeleteFile2(WCHAR* lpFoldPath) {
	char* oldPMD5 = pMD5;

	//初始化路径字符串(在路径后要拼接一个*)
	WCHAR szFoldPath[MAX_PATH] = {};
	DWORD size = wcslen(lpFoldPath) * 2;
	memcpy(szFoldPath, lpFoldPath, size);
	//拼接*
	StringCchCat(szFoldPath, MAX_PATH, L"\\* ");

	//获取第一个文件信息
	ULARGE_INTEGER qwFileTotalSize = {};
	WIN32_FIND_DATA w32FindData = {};
	HANDLE hFindFile = FindFirstFile(szFoldPath, &w32FindData);
	//循环遍历获取当前目录中的文件信息
	do {
		//去掉特殊目录
		if ((!wcscmp(w32FindData.cFileName, L".")) ||
			(!wcscmp(w32FindData.cFileName, L".."))) {
			continue;
		}
		if (w32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//文件夹，继续往下走
			WCHAR szSubfoldPath[MAX_PATH];
			//追加反斜杠
			StringCchPrintf(szSubfoldPath, MAX_PATH, L"%s\\", lpFoldPath);
			//StringCchPrintf(szSubfoldPath, MAX_PATH, L"%s\\", lpFoldPath);
			//子目录名
			StringCchCat(szSubfoldPath, MAX_PATH, w32FindData.cFileName);
			qwFileTotalSize = EnumFoldAndDeleteFile(szSubfoldPath);
		}
		else {
			//获取文件md5
			LPWSTR FileSuffix = PathFindExtension(w32FindData.cFileName);

			//拼接一下全路径
			WCHAR szFullPath[MAX_PATH] = {};
			DWORD size = wcslen(lpFoldPath) * 2;
			memcpy(szFullPath, lpFoldPath, size);
			//拼接
			StringCchCat(szFullPath, MAX_PATH, _T("\\"));
			StringCchCat(szFullPath, MAX_PATH, w32FindData.cFileName);
			//获取md5
			CString md5 = calMD5(szFullPath);

			//循环比较md5值是否相同，相同则提示为病毒文件
			char* str = new char[33] {};
			for (int j = 0; j < 2; j++) {
				memcpy(str, pMD5, 32);
				CString tempstr;
				tempstr.Format(L"%S", str);
				if (md5 == tempstr) {
					CString out = _T("检测到病毒\r\n");
					out += szFullPath;
					//暂停5秒
					Sleep(5000);
					MessageBox(out);
					//DeleteFile(szFullPath);
				}

				//txtmd5.push_back(str);
				//因为后面有\r\n所以多了2个字节
				pMD5 += 34;
			}
			pMD5 = oldPMD5;
			if (str != NULL) {
				delete[] str;
				str = NULL;
			}
		}

	} while (FindNextFile(hFindFile, &w32FindData));
	return qwFileTotalSize;
}

//线程暂停
void CMyFiles::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	if (h1 == 0 || h2 == 0) {
		MessageBox(_T("线程暂停失败或未开始"));
		return;
	}
	DWORD count1 = h1->SuspendThread();
	DWORD count2 = h2->SuspendThread();
	CString a, b;
	a.Format(_T("%d"), count1+1);
	b.Format(_T("%d"), count2+1);
	CString print = _T("A线程被挂起") + a + _T("次\r\n") +
		_T("B线程被挂起") + b + _T("次");
	MessageBox(print);
}

void CMyFiles::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD count1 = h1->ResumeThread();
	DWORD count2 = h2->ResumeThread();
	CString a, b;
	a.Format(_T("%d"), count1-1);
	b.Format(_T("%d"), count2-1);
	CString print = _T("A线程被挂起") + a + _T("次\r\n") +
		_T("B线程被挂起") + b + _T("次");
	MessageBox(print);
}
