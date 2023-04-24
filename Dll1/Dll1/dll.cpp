#include "pch.h"
#include "dll.h"
#include <stdio.h>

UCHAR newOpcode[4] = {};
UCHAR oldOpcode[5] = {};

BOOL WINAPI MyTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
	
	//打开mapping接收pid
	// 1. 打开文件Mapping
	DWORD id = GetProcessId(hProcess);

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,
		FALSE, L"PID");
	if (NULL == hMapFile)
		return FALSE;
	// 2. 创建View
	PVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS,
		0, 0, 4);
	if (NULL == pBuf)
		return FALSE;
	// 3. 显示共享数据
	CHAR text[10] = {0};
	DWORD temp = *((PDWORD)pBuf);
	sprintf_s(text, 10, "%d", temp);

	MessageBoxA(NULL, text, 0, MB_OK);
	// 4. 修改共享数据
	
	//*((PBYTE)pBuf) = 0;
	//UnmapViewOfFile(pBuf);
	//CloseHandle(hMapFile);

	closeHook();
	if (id == temp) {
		//openHook();
		MessageBox(0, L"无法关闭", 0, 0);
		openHook();
		return NULL;
	}
	BOOL result = TerminateProcess(hProcess,uExitCode);
	openHook();
	return result;
}

void initHook() {
	// newOpcode  = 新地址-旧地址
	*((PDWORD)newOpcode) = (DWORD)MyTerminateProcess - (DWORD)TerminateProcess - 5;
	//保存 oldOpcode
	for (int i = 0; i < 5; i++) {
		oldOpcode[i] = ((PCHAR)TerminateProcess)[i];
	}
	
}

void openHook() {
	//更改保护权限
	DWORD oldProtect = 0;
	VirtualProtect(TerminateProcess, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

	//将旧地址的opcode更改
	((PCHAR)TerminateProcess)[0] = 0xe9;
	for (int i = 0; i < 4; i++) {
		((PCHAR)TerminateProcess)[i + 1] = newOpcode[i];
	}
	VirtualProtect(TerminateProcess, 5, oldProtect, &oldProtect);

	//closeHook();
}

void closeHook() {
	DWORD oldProtect = 0;
	VirtualProtect(TerminateProcess, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	for (int i = 0; i < 5; i++) {
		((PUCHAR)TerminateProcess)[i] = oldOpcode[i];
	}
	VirtualProtect(TerminateProcess, 5, oldProtect, &oldProtect);
	//TerminateProcess(0, -1);
}


75C3B000
