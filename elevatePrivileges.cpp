#include <Windows.h>
#include <Shellapi.h>
#include <string>

void DisplayErrorMessage(const wchar_t* action, DWORD error) {
    LPVOID errorMsg;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL,
                   error,
                   0, // Default language
                   (LPWSTR)&errorMsg,
                   0,
                   NULL);

    MessageBoxW(NULL, (LPCWSTR)errorMsg, action, MB_ICONERROR);

    LocalFree(errorMsg);
}

bool StopService(const wchar_t* serviceName) {
    SC_HANDLE serviceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (serviceManager) {
        SC_HANDLE service = OpenServiceW(serviceManager, serviceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
        if (service) {
            SERVICE_STATUS serviceStatus;
            if (ControlService(service, SERVICE_CONTROL_STOP, &serviceStatus)) {
                MessageBoxW(NULL, L"The service has been stopped successfully!", L"Success", MB_ICONINFORMATION);
            } else {
                DWORD error = GetLastError();
                DisplayErrorMessage(L"Failed to stop the service", error);
            }

            CloseServiceHandle(service);
        } else {
            MessageBoxW(NULL, L"Failed to open the service.", L"Error", MB_ICONERROR);
        }
        CloseServiceHandle(serviceManager);
    } else {
        MessageBoxW(NULL, L"Failed to open the service manager.", L"Error", MB_ICONERROR);
    }

    return false;
}

bool IsRunningAsAdmin() {
    BOOL result;
    PSID adminSid;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminSid)) {
        if (CheckTokenMembership(NULL, adminSid, &result) == FALSE) {
            result = FALSE;
        }

        FreeSid(adminSid);
    }

    return result;
}

void RelaunchAsAdmin() {
    SHELLEXECUTEINFOW sei = { sizeof(sei) };
    sei.lpVerb = L"runas";
    sei.lpFile = L"serviceControl.exe";  // Replace with your program's executable
    sei.nShow = SW_SHOWNORMAL;

    if (ShellExecuteExW(&sei)) {
        // Sleep to allow UAC prompt to appear
        Sleep(1000);
        HWND hwndUAC = FindWindowW(NULL, L"User Account Control");
        if (hwndUAC != nullptr) {
            HWND hwndYesButton = FindWindowExW(hwndUAC, nullptr, L"Button", L"&Yes");
            if (hwndYesButton != nullptr) {
                SendMessageW(hwndYesButton, BM_CLICK, 0, 0);
            }
        }

        // Close the console window
        FreeConsole();

        // Terminate the program
        ExitProcess(0);
    } else {
        DWORD error = GetLastError();
        DisplayErrorMessage(L"Failed to execute elevated process", error);
    }
}

int main() {
    if (!IsRunningAsAdmin()) {
        RelaunchAsAdmin();
    } else {
        MessageBoxW(NULL, L"The program is running with elevated privileges.", L"Success", MB_ICONINFORMATION);
    }

    return 0;
}
