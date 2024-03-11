# priv_
The Service Control Manager (SCM) is a remote procedure call (RPC) server. It is started at system boot and allows service configuration and service control programs to manipulate services on remote machines.

OpenProcessToken: This function is used to open a handle to the access token of a process. An access token is an object that holds information about a process's security context, including its privileges
OpenProcessToken: This function is used to open a handle to the access token of a process. An access token is an object that holds information about a process's security context, including its privileges.
Arguments:

GetCurrentProcess(): This retrieves a handle for the current process itself.

TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY:  This specifies the desired access rights for the opened token. Here, it combines two access rights using the bitwise OR operator (|):

TOKEN_ADJUST_PRIVILEGES: This allows modifying the privileges associated with the token. However, not all processes have permission to adjust privileges.

TOKEN_QUERY: This allows querying information from the token, such as the process's privileges.
&hToken: This is a pointer to a variable of type HANDLE. The function will store the handle to the opened token at this memory location if successful.

Functionality:
If the function call is successful, it opens a handle to the current process's access token and stores the handle in the hToken variable (pointed to by the address).
You can then use this handle with other functions to query information about the process's privileges or potentially adjust them (if the process has sufficient permissions).


****Additional notes****
<br>What is a handle?</br>


<b>HANDLE is a typedef that typically resolves to a void* pointer. This means it can hold the memory address of various resources depending on the context.</b>
<br> Some examples include <br>

<b>Socket Handle</b>
SOCKET hSocket = socket(AF_INET, SOCK_STREAM, 0);

<b>Registry Key Handle</b>
HKEY hKey;
RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MyApp", 0, KEY_READ, &hKey);
<b>File Mapping Handle</b>
HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4096, L"MyFileMapping");

<b>Semaphore Handle</b>
HANDLE hSemaphore = CreateSemaphore(NULL, 1, 1, L"MySemaphore");

<b>Event Handle</b>
HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, L"MyEvent");

<b>Mutex Handle</b>
HANDLE hMutex = CreateMutex(NULL, FALSE, L"MyMutex");

<b>Process Handle</b>
HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

<b>Thread Handle</b>
HANDLE hThread = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);

<b>File Handle</b>
HANDLE hFile = CreateFile(L"example.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
