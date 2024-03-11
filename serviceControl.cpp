#include <windows.h>

int main() {
    SC_HANDLE serviceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (serviceManager) {
        SC_HANDLE service = OpenService(serviceManager, "wuauserv", SERVICE_START | SERVICE_STOP);
        if (service) {
            SERVICE_STATUS serviceStatus;

            // Time to turn it off/ this turns off the trustedInstaller 
            if (ControlService(service, SERVICE_CONTROL_STOP, &serviceStatus)) {
                MessageBoxW(NULL, L"The service has been stopped successfully!", L"Success", MB_ICONINFORMATION);
                // Do your rebellious explanation, then turn it back on
                // ControlService(service, SERVICE_CONTROL_START, &serviceStatus);
            } else {
                // Handle the error, e.g., print an error message
                DWORD error = GetLastError();
                MessageBoxW(NULL, L"Failed to stop the service.", L"Error", MB_ICONERROR);
                // ...

                // Note: You might want to handle the error appropriately based on your application's requirements
            }

            CloseServiceHandle(service);
        } else {
            MessageBoxW(NULL, L"Failed to open the service.", L"Error", MB_ICONERROR);
        }
        CloseServiceHandle(serviceManager);
    } else {
        MessageBoxW(NULL, L"Failed to open the service manager.", L"Error", MB_ICONERROR);
    }

    return 0;
}
