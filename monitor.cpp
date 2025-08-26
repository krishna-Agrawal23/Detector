#include "monitor.h"
#include <iostream>
#include <vector>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

Monitor::Monitor(const std::string& path, std::shared_ptr<Scanner> scanner)
    : path_to_watch(path), file_scanner(scanner) {}

void Monitor::start() {
#ifdef _WIN32
    HANDLE hDir = CreateFileA(
        path_to_watch.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to get handle to directory.");
    }

    const int buffer_size = 1024;
    std::vector<BYTE> buffer(buffer_size);
    DWORD bytes_returned = 0;

    while (true) {
        if (ReadDirectoryChangesW(
            hDir,
            buffer.data(),
            buffer_size,
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME,
            &bytes_returned,
            NULL,
            NULL
        )) {
            BYTE* pInfo = buffer.data();
            while (pInfo) {
                FILE_NOTIFY_INFORMATION& fni = *reinterpret_cast<FILE_NOTIFY_INFORMATION*>(pInfo);
                
                if (fni.Action == FILE_ACTION_ADDED || fni.Action == FILE_ACTION_RENAMED_NEW_NAME) {
                    std::wstring w_filename(fni.FileName, fni.FileNameLength / sizeof(WCHAR));
                    
                    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &w_filename[0], (int)w_filename.size(), NULL, 0, NULL, NULL);
                    std::string filename(size_needed, 0);
                    WideCharToMultiByte(CP_UTF8, 0, &w_filename[0], (int)w_filename.size(), &filename[0], size_needed, NULL, NULL);

                    std::string full_path = path_to_watch + "\\" + filename;

                    Sleep(1000);

                    if (file_scanner->isMalware(full_path)) {
                        std::cout << "!!! MALWARE DETECTED !!! -> " << full_path << std::endl;
                    } else {
                        std::cout << "File is clean: " << full_path << std::endl;
                    }
                }

                if (fni.NextEntryOffset == 0) {
                    break;
                }
                pInfo += fni.NextEntryOffset;
            }
        }
    }
    CloseHandle(hDir);
#else
    throw std::runtime_error("File monitoring is only implemented for Windows in this version.");
#endif
}