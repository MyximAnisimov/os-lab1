#include <commands.hpp>

#include <tchar.h>
#include <windows.h>

using namespace std;

void clear() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hConsole == INVALID_HANDLE_VALUE){
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)){
        return;
    }

    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD charsWritten;
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, {0, 0}, &charsWritten);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, {0, 0}, &charsWritten);
    SetConsoleCursorPosition(hConsole, {0, 0});
}

void pwd() {
    WCHAR buffer[MAX_PATH];
    if (GetCurrentDirectoryW(MAX_PATH, buffer) == 0) {
        wcerr << L"Error getting the current directory: " << GetLastError() << endl;
        return;
    }
    wcout << L"\nCurrent Directory:\n------------------\n" << buffer << L"\n" << endl;
}

void cd(const wstring &path) {
    if (path == L"..") {
        if (SetCurrentDirectoryW(L"..") == 0) {
            wcerr << L"Error changing to parent directory: " << GetLastError() << endl;
        }
        else {
            wcout << L"Changed to parent directory." << endl;
        }
        return;
    }

    if (path == L".") {
        wcout << L"Current directory remains the same." << endl;
        return;
    }

    if (SetCurrentDirectoryW(path.c_str()) == 0) {
        wcerr << L"Error changing directory to " << path << L": " << GetLastError() << endl;
    }
    else {
        wcout << L"Changed directory to " << path << endl;
    }
}

void ls() {
    WIN32_FIND_DATAW ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    WCHAR buffer[MAX_PATH];
    if (GetCurrentDirectoryW(MAX_PATH, buffer) == 0) {
        wcerr << L"Error getting the current directory: " << GetLastError() << endl;
        return;
    }

    wstring searchPath = wstring(buffer) + L"\\*"; // Исправление здесь: добавление обратной косой черты.
    hFind = FindFirstFileW(searchPath.c_str(), &ffd);

    if (hFind == INVALID_HANDLE_VALUE) {
        wcerr << L"Error finding files in the directory: " << GetLastError() << endl;
        return;
    }

    wcout << L"\nDirectory Listing:\n" << L"==========================================\n";
    do {
        if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0) {
            wcout << L"\nName       : " << ffd.cFileName << endl;

            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                wcout << L"Type       : Directory" << endl;
            }
            else {
                wcout << L"Type       : File" << endl;
                wcout << L"Size       : " << ffd.nFileSizeLow << L" bytes" << endl;
            }

            SYSTEMTIME systemTime;
            if (FileTimeToSystemTime(&ffd.ftCreationTime, &systemTime)) {
                wcout << L"Created On : "
                      << (systemTime.wDay < 10 ? L"0" : L"") << systemTime.wDay << L"/"
                      << (systemTime.wMonth < 10 ? L"0" : L"") << systemTime.wMonth << L"/"
                      << systemTime.wYear << L" "
                      << (systemTime.wHour < 10 ? L"0" : L"") << systemTime.wHour << L":"
                      << (systemTime.wMinute < 10 ? L"0" : L"") << systemTime.wMinute << L":"
                      << (systemTime.wSecond < 10 ? L"0" : L"") << systemTime.wSecond << endl;
            } else {
                wcerr << L"Error retrieving file creation time: " << GetLastError() << endl;
            }

            wcout << L"------------------------------------------" << endl;
        }
    } while (FindNextFileW(hFind, &ffd) != 0);

    wcout << L"==========================================\n" << endl;

    if (GetLastError() != ERROR_NO_MORE_FILES) {
        wcerr << L"Error reading files: " << GetLastError() << endl;
    }

    FindClose(hFind);
}

void mkdir(const wstring &path) {
    if (CreateDirectoryW(path.c_str(), nullptr)) {
        wcout << L"Directory created successfully: " << path << endl;
    } else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
            wcerr << L"Error: The directory already exists: " << path << endl;
        } else {
            wcerr << L"Error creating directory: " << path << L" (" << error << L")" << endl;
        }
    }
}

void rmdir(const wstring &path) {
    if (RemoveDirectoryW(path.c_str())) {
        wcout << L"Directory removed successfully: " << path << endl;
    } else {
        DWORD error = GetLastError();
        if (error == ERROR_DIR_NOT_EMPTY) {
            wcerr << L"Error: The directory is not empty: " << path << endl;
        } else if (error == ERROR_PATH_NOT_FOUND) {
            wcerr << L"Error: The specified path does not exist: " << path << endl;
        } else {
            wcerr << L"Error removing directory: " << path << L" (Error code: " << error << L")" << endl;
        }
    }
}
void echo(const string &word) {
    cout << word << endl;
}

void exitHell() {
    cout << "Exiting shell" << endl;
    exit(0);
}

void execute(const string& pathWithArgs) {
    STARTUPINFOA startupInfo = {0};
    PROCESS_INFORMATION processInfo = {nullptr};
    startupInfo.cb = sizeof(STARTUPINFOA);

    vector<char> commandLine(pathWithArgs.begin(), pathWithArgs.end());
    commandLine.push_back('\0');

    auto start = chrono::high_resolution_clock::now();

    if (!CreateProcessA(
            nullptr,
            commandLine.data(),
            nullptr,
            nullptr,
            false,
            0,
            nullptr,
            nullptr,
            &startupInfo,
            &processInfo
            )) {
        cerr << "Error creating process: " << GetLastError() << endl;
        return;
            }

    WaitForSingleObject(processInfo.hProcess, INFINITE);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedTime = end - start;

    DWORD exitCode;
    if (GetExitCodeProcess(processInfo.hProcess, &exitCode)) {
        cout << "Process exited with code: " << exitCode << endl;
    } else {
        cerr << "Failed to get process exit code: " << GetLastError() << endl;
    }

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    cout << "Execution time: " << elapsedTime.count() << " seconds" << endl;
}

