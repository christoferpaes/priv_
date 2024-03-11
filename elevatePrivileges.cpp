#include <Windows.h>
#include <Shellapi.h>

bool elevatePrivileges() {
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD dwSize;
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
            if (elevation.TokenIsElevated == 0) {
                SHELLEXECUTEINFOA sei = { sizeof(sei) };  // Use ANSI version
                sei.lpVerb = "runas";  // Use ANSI string
                sei.lpFile = "serviceControl.exe";  // Replace with your program's executable
                sei.nShow = SW_SHOWNORMAL;

                if (ShellExecuteExA(&sei)) {
                    Sleep(1000);
                    HWND hwndUAC = FindWindowA(nullptr, "User Account Control");  // Use ANSI version
                    if (hwndUAC != nullptr) {
                        HWND hwndYesButton = FindWindowExA(hwndUAC, nullptr, "Button", "&Yes");  // Use ANSI version
                        if (hwndYesButton != nullptr) {
                            SendMessageA(hwndYesButton, BM_CLICK, 0, 0);  // Use ANSI version
                        }
                    }

                    // Close the console window
                    FreeConsole();

                    // Terminate the program
                    ExitProcess(0);
                } else {
                    // Handle the error
                    DWORD error = GetLastError();
                    MessageBoxA(nullptr, "Failed to execute elevated process.", "Error", MB_ICONERROR);
                    // ...
                }
            } else {
                CloseHandle(hToken);
                return TRUE;
            }
        }
        CloseHandle(hToken);
    }
    return TRUE;
}

int main() {
    if (elevatePrivileges()) {
        MessageBoxA(nullptr, "The program is running with elevated privileges.", "Success", MB_ICONINFORMATION);
    } else {
        MessageBoxA(nullptr, "Failed to elevate privileges.", "Error", MB_ICONERROR);
    }

    return 0;
}
