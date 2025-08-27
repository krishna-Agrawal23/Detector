#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include "monitor.h"
#include "scanner.h"

// Include windows.h to hide the console
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // this helps in hiding the terminal window
    #ifdef _WIN32
    HWND stealth;
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);
    #endif

    std::string pathToWatch = "A:\\Downloads\\Chrome"; // For those who cloned the repositery:- Change this path to your browser's download folder path

    try {
        auto scanner = std::make_shared<Scanner>("signatures.db");
        Monitor monitor(pathToWatch, scanner);
        
        
        monitor.start();
    } catch (const std::exception& e) {
        // If an error occurs, we can show it in a message box
        MessageBoxA(NULL, e.what(), "Runtime Error", MB_OK | MB_ICONEXCLAMATION);
        return 1;
    }

    return 0;
}

// To hide the console and for it to work silently in the background....
