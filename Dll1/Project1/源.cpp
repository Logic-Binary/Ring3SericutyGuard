#include<Windows.h>
#include<stdio.h>
#define PATH L"C:\\Users\\�޼�\\Desktop\\111\\Dll1\\Debug\\Dll1.dll"

int main() { 

    DWORD size = (wcslen(PATH) + 1) * 2;
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 31424);
    LPVOID buff = VirtualAllocEx(hProc, NULL, size, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(hProc, buff, PATH, size, NULL);
    HANDLE hTheread = CreateRemoteThread(
        hProc, NULL, NULL,
        (LPTHREAD_START_ROUTINE)LoadLibrary,
        buff, NULL, NULL);
    WaitForSingleObject(hTheread, -1);

    if (buff != 0) {
        VirtualFreeEx(hProc, buff, 0, MEM_RELEASE);
        buff = NULL;
    }

    // 1. �����������ļ�ӳ��
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,
        NULL, PAGE_READWRITE, 0,
        4, L"PID");
    if (NULL == hMapFile || INVALID_HANDLE_VALUE == hMapFile)
        return FALSE;
    // 2. ����View
    PVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 4);
    if (NULL == pBuf)
        return FALSE;
    // 3. ���������ݸ��Ƶ��ļ�ӳ����
    DWORD id = 37072;

    memcpy_s(pBuf, 4, &id, 4);
    char text[10] = {};
    int temp = *((PDWORD)pBuf);
    sprintf_s(text, 10, "%d", id);




    //MessageBox(0, L"�����ر�", 0, 0);
    // 5. ȡ��Mapping���رվ��
    CloseHandle(hProc);
    CloseHandle(hTheread);
    while (1);
    return 0;
}