#include <Windows.h>
#include <iostream>

bool elevatePrivileges() {
    BOOL success = FALSE;
    HANDLE hToken = NULL;

    // Open the process token
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD dwSize;

        // Check if the process is running with administrator privileges
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
            if (elevation.TokenIsElevated == 0) {
                // Request UAC elevation
                SHELLEXECUTEINFO sei = { sizeof(sei) };
                sei.lpVerb = L"runas";
                sei.lpFile = L"your_program.exe";  // Replace with your program's executable
                sei.nShow = SW_SHOWNORMAL;

                success = ShellExecuteEx(&sei);

                if (success) {
                    // Wait for UAC window to appear
                    Sleep(1000);  // Adjust the sleep time as needed

                    // Find the UAC window
                    HWND hwndUAC = FindWindow(nullptr, L"User Account Control");

                    // Check if the UAC window is found
                    if (hwndUAC != nullptr) {
                        // Find the "Yes" button and send a click message
                        HWND hwndYesButton = FindWindowEx(hwndUAC, nullptr, L"Button", L"&Yes");
                        if (hwndYesButton != nullptr) {
                            SendMessage(hwndYesButton, BM_CLICK, 0, 0);
                        }
                    }
                }
            } else {
                // Process already has administrative privileges
                success = TRUE;
            }
        }

        // Close the token handle
        CloseHandle(hToken);
    }

    return success == TRUE;
}
