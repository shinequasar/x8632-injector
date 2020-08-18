#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

DWORD FindProcessID(LPCTSTR szProcessName);
BOOL InjectDll(DWORD dwPID, LPCTSTR szDllName);

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Injector.exe [ProgramPath] [Dll Path]");
        return 1;
    }

    DWORD dwPID = 0xFFFFFFFF;

    // CreateProcess 함수
    STARTUPINFO si = { 0, };    //구조체 선언, 초기화
    PROCESS_INFORMATION pi;

    si.cb = sizeof(si);
    si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
    //Flags 값들이 여러가지가 있음.
    //STARTF_USEPOSITION :: dwX, dwY 위치 사용
    //STARTF_USESIZE     :: dwXSize, dwYSize 사용
    //Flags 값이 주어지지 않는 si구조체 값은
    //새로 만들어지는 프로세스에 영향을 주지 않음.
    si.dwX = 100;
    si.dwY = 100;
    si.dwXSize = 300;
    si.dwYSize = 300;  //dw ~ 는 사실 잘 쓰이지 않음
    //si.lpTitle = TCHAR*<" Child process! ">;

    BOOL state;

    //find process

    TCHAR* file_path = argv[1];
    LPCTSTR DEF_DLL_PATH = argv[2];


    state = CreateProcess(
        NULL, //이름
        file_path, // 경로를 System에 해줬기 때문에 notepad가 열림
        NULL, NULL,
        TRUE, //부모프로세스중 상속가능한 핸들 상속
        CREATE_SUSPENDED, // SUSPENDED 상태로 프로세스 생성
        NULL, NULL,
        &si, //위에서 만들어둔 STARTUPINFO 구조체 정보
        &pi  //이전 프로세스의 정보를 가져올때 이 구조체를 사용
    );

    //Sleep(1000);
    //dwPID = FindProcessID("notepad.exe");
    dwPID = pi.dwProcessId;
    if (dwPID == 0xFFFFFFFF)
    {
        printf("There is no <%s> process!\n", argv[1]);
        return 1;
    }

    // 프로세스의 스레드 핸들러 가져오기
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
    HANDLE hThread;
    hThread = pi.hThread;

    //inject dll
    InjectDll(dwPID, DEF_DLL_PATH);

    // suspend 상태를 해제한다.
    ResumeThread(hThread);
    system("PAUSE");
    return EXIT_SUCCESS;
}


/*
    목적 프로세스에 가상메모리를 할당하여 DLL 삽입
    성공시 TRUE return
*/
BOOL InjectDll(DWORD dwPID, LPCTSTR szDllName)
{
    int ForError = 0;
    HANDLE hProcess, hThread;
    HMODULE hMod;
    LPVOID pRemoteBuf;
    DWORD dwBufSize = lstrlen(szDllName) + 1;
    LPTHREAD_START_ROUTINE pThreadProc;

    // dwPID 를 이용하여 대상 프로세스의 HANDLE을 구함
    if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))  //실패시 NULL 성공시 프로세스핸들 
        return FALSE;

    // 대상 프로세스메모리에 szDllName(dwBufSize) 크기만큼 메모리를 할당 
    pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE); //성공시 할당된 메모리 주소(대상 프로세스 가상메모리) 

    // 할당 받은 메모리에 myhack.dll 경로를 씀
    ForError = WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllName, dwBufSize, NULL);
    if (!ForError)
        return FALSE;


    // LoadLibraryA() API 주소를 구함 
    hMod = GetModuleHandle("kernel32.dll");
    pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryA");

    // 대상프로세스에 스레드를 실행 
    hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    CloseHandle(hProcess);

    printf("모두 끝남");

    return TRUE;
}
/*
    (목적 프로세스의 PID값을 찾는 함수, 성공시 PID return)
    하지만 현재 이 코드에선 FindProcessID 함수로 PID를 받아오는 대신 
    CreateProcess 함수에서 쓰이는  PROCESS_INFORMATION pi구조체의 
    dwProcessId를 이용해 현재 실행시킨 프로세스의 PID를 직접 받아옴.
*/

/*
DWORD FindProcessID(LPCTSTR szProcessName)
{
    DWORD dwPID = 0xFFFFFFFF;
    HANDLE hSnapShot = INVALID_HANDLE_VALUE;
    PROCESSENTRY32 pe;

    // Get the snapshot of the system
    pe.dwSize = sizeof(PROCESSENTRY32);
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

    //find process
    Process32First(hSnapShot, &pe);
    do
    {
        if (!_stricmp(szProcessName, pe.szExeFile))
        {
            dwPID = pe.th32ProcessID;
            break;
        }
    } while (Process32Next(hSnapShot, &pe));

    CloseHandle(hSnapShot);

    return dwPID;
}
*/