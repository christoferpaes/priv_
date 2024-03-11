#include <windows.h>

int main() {
    SC_HANDLE serviceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (serviceManager) {
        SC_HANDLE service = OpenService(serviceManager, "wuauserv", SERVICE_START | SERVICE_STOP);
        if (service) {
            // Time to turn it off
            ControlService(service, SERVICE_CONTROL_STOP, new SERVICE_STATUS);

            // Do your rebellious explanation, then turn it back on
            ControlService(service, SERVICE_CONTROL_START, new SERVICE_STATUS);

            CloseServiceHandle(service);
        }
        CloseServiceHandle(serviceManager);
    }

    return 0;
}
