#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>

void launchProcess(const std::string& command, const std::vector<std::string>& args) {
    std::string commandLine = command;
    for (const auto& arg : args) {
        commandLine += " " + arg;
    }

    char commandLineCStr[2048];
    strcpy_s(commandLineCStr, commandLine.c_str());

    STARTUPINFOA si = {0};
    si.cb = sizeof(STARTUPINFOA);
    PROCESS_INFORMATION pi = {0};

    if (!CreateProcessA(
            nullptr,
            commandLineCStr,
            nullptr,
            nullptr,
            FALSE,
            0,
            nullptr,
            nullptr,
            &si,
            &pi
    )) {
        std::cerr << "Failed to start process: " << GetLastError() << std::endl;
        return;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

int main(int argc, char* argv[]) {
    if (argc < 8) {
        std::cerr << "Usage: " << argv[0] << " <count> <command> <arg1> <arg2> <arg3> <arg4> <arg5> <arg6>\n";
        return 1;
    }

    int count = std::stoi(argv[1]);
    std::string command = argv[2];
    std::vector<std::string> args = {argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]};

    for (int i = 0; i < count; ++i) {
        launchProcess(command, args);
    }

    return 0;
}
// // #include <windows.h>
// // #include <vector>
// // #include <string>
// // #include <iostream>
// // #include <winternl.h>
// //
// // #pragma comment(lib, "ntdll.lib")  // Не забудьте подключить ntdll.lib
// // #define THREAD_CREATE_FLAGS_CREATE_SUSPENDED 0x00000001
// // #define PROCESS_CREATE_FLAGS_PROTECTED_PROCESS 0x00000040
// // #define STATUS_SUCCESS 0x00000000
// //
// // typedef NTSTATUS(WINAPI* NtCreateUserProcessPtr)(
// //     PHANDLE ProcessHandle,
// //     PHANDLE ThreadHandle,
// //     ACCESS_MASK ProcessDesiredAccess,
// //     ACCESS_MASK ThreadDesiredAccess,
// //     POBJECT_ATTRIBUTES ProcessObjectAttributes,
// //     POBJECT_ATTRIBUTES ThreadObjectAttributes,
// //     ULONG CreateFlags,
// //     ULONG NumberOfProcessIdsToReserve,
// //     ULONG NumberOfThreadIdsToReserve,
// //     PVOID ProcessParameters
// // );
// //
// // typedef VOID(WINAPI* RtlInitUnicodeStringPtr)(PUNICODE_STRING DestinationString, PCWSTR SourceString);
// //
// // struct PROCESS_PARAMETERS {
// //     ULONG Length;
// //     HANDLE Section;
// //     HANDLE DebugPort;
// //     HANDLE ExceptionPort;
// //     UNICODE_STRING ImagePathName;
// //     UNICODE_STRING CommandLine;
// //     // Другие параметры могут быть добавлены по мере необходимости
// // };
// //
// // void launchProcess(const std::string& command, const std::vector<std::string>& args) {
// //     HANDLE hProcess = nullptr;
// //     HANDLE hThread = nullptr;
// //
// //     // Инициализация объектных атрибутов
// //     UNICODE_STRING imageName;
// //     HMODULE ntdll = GetModuleHandleA("ntdll.dll");
// //     RtlInitUnicodeStringPtr RtlInitUnicodeString = (RtlInitUnicodeStringPtr)GetProcAddress(ntdll, "RtlInitUnicodeString");
// //
// //     if (!RtlInitUnicodeString) {
// //         std::cerr << "Failed to load RtlInitUnicodeString." << std::endl;
// //         return;
// //     }
// //
// //     RtlInitUnicodeString(&imageName, std::wstring(command.begin(), command.end()).c_str());
// //
// //     UNICODE_STRING commandLine;
// //     std::string fullCommandLine = command;
// //     for (const auto& arg : args) {
// //         fullCommandLine += " " + arg;
// //     }
// //     RtlInitUnicodeString(&commandLine, std::wstring(fullCommandLine.begin(), fullCommandLine.end()).c_str());
// //
// //     PROCESS_PARAMETERS processParameters;
// //     processParameters.Length = sizeof(PROCESS_PARAMETERS);
// //     processParameters.ImagePathName = imageName;
// //     processParameters.CommandLine = commandLine;
// //
// //     // Инициализация объектных атрибутов
// //     OBJECT_ATTRIBUTES processAttributes;
// //     InitializeObjectAttributes(&processAttributes, nullptr, 0, nullptr, nullptr);
// //
// //     OBJECT_ATTRIBUTES threadAttributes;
// //     InitializeObjectAttributes(&threadAttributes, nullptr, 0, nullptr, nullptr);
// //
// //     NtCreateUserProcessPtr NtCreateUserProcess =
// //         reinterpret_cast<NtCreateUserProcessPtr>(GetProcAddress(ntdll, "NtCreateUserProcess"));
// //
// //     if (NtCreateUserProcess) {
// //         NTSTATUS status = NtCreateUserProcess(
// //             &hProcess,
// //             &hThread,
// //             PROCESS_ALL_ACCESS,
// //             THREAD_ALL_ACCESS,
// //             &processAttributes,
// //             &threadAttributes,
// //             FALSE,
// //             PROCESS_CREATE_FLAGS_PROTECTED_PROCESS,
// //             THREAD_CREATE_FLAGS_CREATE_SUSPENDED,
// //             &processParameters
// //         );
// //
// //         if (status != STATUS_SUCCESS) {
// //             std::cerr << "Failed to start process: " << std::hex << status << std::endl;
// //             return;
// //         }
// //
// //         CloseHandle(hThread);
// //         CloseHandle(hProcess);
// //     } else {
// //         std::cerr << "Failed to load NtCreateUserProcess." << std::endl;
// //     }
// // }
// //
// // int main(int argc, char* argv[]) {
// //     if (argc < 9) {
// //         std::cerr << "Usage: " << argv[0] << " <count> <command> <arg1> <arg2> <arg3> <arg4> <arg5> <arg6>\n";
// //         return 1;
// //     }
// //
// //     int count = std::stoi(argv[1]);
// //     std::string command = argv[2];
// //     std::vector<std::string> args = { argv[3], argv[4], argv[5], argv[6], argv[7], argv[8] };
// //
// //     for (int i = 0; i < count; ++i) {
// //         launchProcess(command, args);
// //     }
// //
// //     return 0;
// // }
// #include <windows.h>
// #include <vector>
// #include <string>
// #include <iostream>
// #include <winternl.h>
//
// #pragma comment(lib, "ntdll.lib")
// #define THREAD_CREATE_FLAGS_CREATE_SUSPENDED 0x00000001
// #define PROCESS_CREATE_FLAGS_PROTECTED_PROCESS 0x00000040
// #define STATUS_SUCCESS 0x00000000
//
// typedef NTSTATUS(WINAPI* NtCreateUserProcessPtr)(
//     PHANDLE ProcessHandle,
//     PHANDLE ThreadHandle,
//     ACCESS_MASK ProcessDesiredAccess,
//     ACCESS_MASK ThreadDesiredAccess,
//     POBJECT_ATTRIBUTES ProcessObjectAttributes,
//     POBJECT_ATTRIBUTES ThreadObjectAttributes,
//     ULONG CreateFlags,
//     ULONG NumberOfProcessIdsToReserve,
//     ULONG NumberOfThreadIdsToReserve,
//     PVOID ProcessParameters
// );
//
// typedef VOID(WINAPI* RtlInitUnicodeStringPtr)(PUNICODE_STRING DestinationString, PCWSTR SourceString);
//
// struct PROCESS_PARAMETERS {
//     ULONG Length;
//     HANDLE Section; // Should be initialized to NULL
//     HANDLE DebugPort; // Should be initialized to NULL
//     HANDLE ExceptionPort; // Should be initialized to NULL
//     UNICODE_STRING ImagePathName;
//     UNICODE_STRING CommandLine;
// };
//
// void launchProcess(const std::string& command, const std::vector<std::string>& args) {
//     HANDLE hProcess = nullptr;
//     HANDLE hThread = nullptr;
//
//     UNICODE_STRING imageName;
//     HMODULE ntdll = GetModuleHandleA("ntdll.dll");
//     RtlInitUnicodeStringPtr RtlInitUnicodeString = (RtlInitUnicodeStringPtr)GetProcAddress(ntdll, "RtlInitUnicodeString");
//
//     if (!RtlInitUnicodeString) {
//         std::cerr << "Failed to load RtlInitUnicodeString." << std::endl;
//         return;
//     }
//
//     RtlInitUnicodeString(&imageName, std::wstring(command.begin(), command.end()).c_str());
//
//     UNICODE_STRING commandLine;
//     std::string fullCommandLine = command;
//     for (const auto& arg : args) {
//         fullCommandLine += " " + arg;
//     }
//     RtlInitUnicodeString(&commandLine, std::wstring(fullCommandLine.begin(), fullCommandLine.end()).c_str());
//
//     PROCESS_PARAMETERS processParameters = {};
//     processParameters.Length = sizeof(PROCESS_PARAMETERS);
//     processParameters.Section = nullptr; // Initialize to NULL
//     processParameters.DebugPort = nullptr; // Initialize to NULL
//     processParameters.ExceptionPort = nullptr; // Initialize to NULL
//     processParameters.ImagePathName = imageName;
//     processParameters.CommandLine = commandLine;
//
//     // Инициализация объектных атрибутов
//     OBJECT_ATTRIBUTES processAttributes;
//     InitializeObjectAttributes(&processAttributes, nullptr, 0, nullptr, nullptr);
//
//     OBJECT_ATTRIBUTES threadAttributes;
//     InitializeObjectAttributes(&threadAttributes, nullptr, 0, nullptr, nullptr);
//
//     NtCreateUserProcessPtr NtCreateUserProcess =
//         reinterpret_cast<NtCreateUserProcessPtr>(GetProcAddress(ntdll, "NtCreateUserProcess"));
//
//     if (NtCreateUserProcess) {
//         NTSTATUS status = NtCreateUserProcess(
//             &hProcess,
//             &hThread,
//             PROCESS_ALL_ACCESS,
//             THREAD_ALL_ACCESS,
//             &processAttributes,
//             &threadAttributes,
//             FALSE,
//             PROCESS_CREATE_FLAGS_PROTECTED_PROCESS,
//             THREAD_CREATE_FLAGS_CREATE_SUSPENDED,
//             &processParameters
//         );
//
//         if (status != STATUS_SUCCESS) {
//             std::cerr << "Failed to start process: " << std::hex << status << std::endl;
//             return;
//         }
//
//
//         CloseHandle(hThread);
//         CloseHandle(hProcess);
//     } else {
//         std::cerr << "Failed to load NtCreateUserProcess." << std::endl;
//     }
// }
//
// int main(int argc, char* argv[]) {
//     if (argc < 9) {
//         std::cerr << "Usage: " << argv[0] << " <count> <command> <arg1> <arg2> <arg3> <arg4> <arg5> <arg6>\n";
//         return 1;
//     }
//
//     int count = std::stoi(argv[1]);
//     std::string command = argv[2];
//     std::vector<std::string> args = { argv[3], argv[4], argv[5], argv[6], argv[7], argv[8] };
//
//     for (int i = 0; i < count; ++i) {
//         launchProcess(command, args);
//     }
//
//     return 0;
// }