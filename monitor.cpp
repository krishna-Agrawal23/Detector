#include "monitor.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <algorithm> // Required for std::transform
#include <cctype>    // Required for std::tolower

#ifdef _WIN32
#include <windows.h>
#endif

std::string getFileType(const std::string& filename) {
    size_t dot_pos = filename.rfind('.');
    if (dot_pos == std::string::npos) {
        return "a file"; 
    }

    std::string ext = filename.substr(dot_pos);
    std::transform(ext.begin(), ext.end(), ext.begin(),
        [](unsigned char c){ return std::tolower(c); });

    if (ext == ".exe" || ext == ".msi" || ext == ".bat" || ext == ".com") {
        return "an Application";
    }
    if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".gif" || ext == ".bmp" || ext == ".svg") {
        return "an Image";
    }
    if (ext == ".mp4" || ext == ".avi" || ext == ".mkv" || ext == ".mov" || ext == ".wmv") {
        return "a Video";
    }
    if (ext == ".mp3" || ext == ".wav" || ext == ".aac" || ext == ".flac") {
        return "an Audio File";
    }
    if (ext == ".zip" || ext == ".rar" || ext == ".7z") {
        return "an Archive";
    }
    if (ext == ".pdf" || ext == ".docx" || ext == ".txt" || ext == ".pptx" || ext == ".xlsx") {
        return "a Document";
    }

    return "a FILE"; // Default case
}

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
        throw std::runtime_error("Failed to get handle to directory. Check path and permissions.");
    }
    const int buffer_size = 4096;
    std::vector<BYTE> buffer(buffer_size);
    DWORD bytes_returned = 0;
    while (true) {
        if (ReadDirectoryChangesW(
            hDir, buffer.data(), buffer_size, TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME,
            &bytes_returned, NULL, NULL
        )) {
            BYTE* pInfo = buffer.data();
            while (pInfo) {
                FILE_NOTIFY_INFORMATION& fni = *reinterpret_cast<FILE_NOTIFY_INFORMATION*>(pInfo);
                if (fni.Action == FILE_ACTION_ADDED || fni.Action == FILE_ACTION_RENAMED_NEW_NAME) {
                    std::wstring w_filename(fni.FileName, fni.FileNameLength / sizeof(WCHAR));
                    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &w_filename[0], (int)w_filename.size(), NULL, 0, NULL, NULL);
                    std::string filename(size_needed, 0);
                    WideCharToMultiByte(CP_UTF8, 0, &w_filename[0], (int)w_filename.size(), &filename[0], size_needed, NULL, NULL);

                    std::string extension = "";
                    size_t dot_pos = filename.rfind('.');
                    if (dot_pos != std::string::npos) {
                        extension = filename.substr(dot_pos);
                        std::transform(extension.begin(), extension.end(), extension.begin(),
                            [](unsigned char c){ return std::tolower(c); });
                    }

                    if (extension == ".crdownload" || extension == ".tmp") {
                        std::cout << "Ignoring temporary download file: " << filename << std::endl;
                        if (fni.NextEntryOffset == 0) break;
                        pInfo += fni.NextEntryOffset;
                        continue; 
                    }

                    std::string full_path = path_to_watch + "\\" + filename;
                    
                    std::cout << "Scanning new file: " << full_path << std::endl;
                    
                    Sleep(1000); // Wait for the file to be fully written

                    if (file_scanner->isMalware(full_path)) {
                       std::string message = "Looks like you downloaded " + fileType + " AND IT CONTAINS A MALWARE :\n\n" + filename;
                        MessageBoxA(NULL, message.c_str(), "!!! MALWARE ALERT !!!", MB_OK | MB_ICONERROR);
                    } else {
                        std::string message = "It looks like you downloaded " + fileType + " and it is safe from malwares.\n\nFile: " + filename;
                        MessageBoxA(NULL, message.c_str(), "File Scan Complete", MB_OK | MB_ICONINFORMATION);
                    }
                }
                if (fni.NextEntryOffset == 0) break;
                pInfo += fni.NextEntryOffset;
            }
        }
    }
    CloseHandle(hDir);
#else
    throw std::runtime_error("File monitoring is only implemented for Windows in this version.");
#endif
}


// Updated the whole code so that the user gets a "POP UP MESSAGE" instead the safe and virus detection alert happening in the terminal...
// Changed the main.cpp file to hide the console in the background and it can work silently in the user system...

