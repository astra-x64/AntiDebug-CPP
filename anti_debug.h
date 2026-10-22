#include <Windows.h>
#include <winternl.h>


void ckc1() {
    typedef NTSTATUS(WINAPI* pfnNtQueryInformationProcess)(
        HANDLE ProcessHandle,
        PROCESSINFOCLASS ProcessInformationClass,
        PVOID ProcessInformation,
        ULONG ProcessInformationLength,
        ULONG ReturnLength
        );

    pfnNtQueryInformationProcess NtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
    HANDLE hProcess = GetCurrentProcess();
    ULONG ReturnLength = 0;
    PROCESS_BASIC_INFORMATION Information;
    NTSTATUS status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &Information, sizeof(Information), ReturnLength);

    if (NT_SUCCESS(status)) {
        unsigned char* pPeb = (unsigned char*)Information.PebBaseAddress;
        
        if (pPeb[2] == 1) { exit(1); }

        #ifdef _WIN64
        DWORD ntGlobalFlag = *(DWORD*)(pPeb + 0xBC);
        #else
        DWORD ntGlobalFlag = *(DWORD*)(pPeb + 0x68);
        #endif

        if (*(DWORD*)(pPeb + 0xBC) & 0x70) { exit(1); }
    }
}

void ckc2() {
    HANDLE hThread = GetCurrentThread();
    CONTEXT ctx = { 0 };
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    BOOL sub = GetThreadContext(hThread, &ctx);
    if (!sub){ exit(0); }
    if(ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0 || ctx.Dr7 != 0){
        exit(0);
    }
}

void ckc3(){
    typedef NTSTATUS(WINAPI* pfnNtSetInformationThread)(
        HANDLE ThreadHandle,
        ULONG ThreadInformationClass,
        PVOID ThreadInformation,
        ULONG ThreadInformationLength
    );
    HANDLE hThread2 = GetCurrentThread();
    pfnNtSetInformationThread NtSetInformationThread = (pfnNtSetInformationThread)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtSetInformationThread");
    PVOID ThreadInformation = NULL;
    NTSTATUS status2 = NtSetInformationThread(hThread2, 0x11, NULL, 0);
}

void RunAll() {
    ckc1();
    ckc2();
    ckc3();
}