cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /TcInject.c /link /OUT:Inject.exe /SUBSYSTEM:CONSOLE /MACHINE:x64
cl.exe /O2 /D_USRDLL /D_WINDLL MessageBoxDLL.c /MT /link /DLL /OUT:MessageBoxDLL.dll