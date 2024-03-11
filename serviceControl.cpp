#include <windows.h>
#include <string>

void DisplayErrorMessage(const wchar_t* action, DWORD errorCode) {
    LPVOID errorMsg;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        errorCode,
        0, // Default language
        (LPWSTR)&errorMsg,
        0,
        NULL
    );

    MessageBoxW(NULL, (std::wstring(action) + L" Error: " + (wchar_t*)errorMsg).c_str(), L"Error", MB_ICONERROR);

    LocalFree(errorMsg);
}

bool StopService(const wchar_t* serviceName) {
    SC_HANDLE serviceManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (serviceManager) {
        SC_HANDLE service = OpenServiceW(serviceManager, serviceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
        if (service) {
            SERVICE_STATUS serviceStatus;
            if (ControlService(service, SERVICE_CONTROL_STOP, &serviceStatus)) {
                MessageBoxW(NULL, L"The service has been stopped successfully!", L"Success", MB_ICONINFORMATION);
                CloseServiceHandle(service);
                CloseServiceHandle(serviceManager);
                return true;
            } else {
                DWORD error = GetLastError();
                DisplayErrorMessage(L"Failed to stop the service", error);
            }
            CloseServiceHandle(service);
        } else {
            DWORD error = GetLastError();
            DisplayErrorMessage(L"Failed to open the service", error);
        }
        CloseServiceHandle(serviceManager);
    } else {
        DWORD error = GetLastError();
        DisplayErrorMessage(L"Failed to open the service manager", error);
    }

    return false;
}

int main() {
    // Replace L"wuauserv" with the name of the service you want to stop
    const wchar_t* serviceName = L"wuauserv";

    if (StopService(serviceName)) {
        MessageBoxW(NULL, L"The program executed successfully.", L"Success", MB_ICONINFORMATION);
    } else {
        MessageBoxW(NULL, L"Failed to execute the program.", L"Error", MB_ICONERROR);
    }

    return 0;
}
