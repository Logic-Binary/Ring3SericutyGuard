#include<Windows.h>
#include<stdio.h>


int main() {

	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,
		FALSE, L"PID");
	if (NULL == hMapFile)
		return FALSE;
	// 2. ����View
	PVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS,
		0, 0, 4);
	if (NULL == pBuf)
		return FALSE;
	// 3. ��ʾ��������
	CHAR text[10] = { 0 };
	DWORD temp = *((PDWORD)pBuf);
	sprintf_s(text, 10, "%d", temp);

	MessageBoxA(NULL, text, 0, MB_OK);
	// 4. �޸Ĺ�������
	*((PBYTE)pBuf) = 0;
	// 5. ȡ��Mapping���رվ��
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);

}