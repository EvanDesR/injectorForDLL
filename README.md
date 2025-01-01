# injectorForDLL

DLL Injection! 

Requires, a DLL to inject, for this I made calcDLL, and included shellcode to launch calc.exe from the remote prcoess, the remote process in my case being notepad.exe.

### Usage 

Change DLLpath before compiling, soon I hope to implement something a more dynamic, solution.

Change target[], if you want to attempt injection on a specific process. 



## WCHAR vs CHAR, and W vs A WinAPI funtions

Written, using UNICODE for all strings, and UNICODE WinAPI functions, as windows converts ANSI (8bit chars) to UTF-16 (wchar_t) anyways. This is because, char only supports 0-0255, and wchar_t supports 0-65535, which is needed to support all glpyhs(symbols) from all languages. If only ANSI was supported, then windows would have to convert between language glyphs. The smart folks on CPlusPlus.com, gave me the basic heuristic of: If working with window API, or any sysinterals, just use wchar_t , and L"string" for all strings, as programs working heavily with winAPI should strive to follow the intended implementation guidelines.
