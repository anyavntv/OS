// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <string>
void IntRead(HANDLE proc, int& intRead);
void StringRead(HANDLE proc, std::string& stringRead);
void CharArrayRead(HANDLE proc, char charArrayRead[]);

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
HHOOK keyboardHook;
HWND prevWindow;

int main()
{
    int PID = 0;
    int intRead = 0;
    std::string stringRead;
    char charArrayRead[50];
    std::cout << "PID: ";
    std::cin >> PID;
    std::cout << std::endl;
    HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
    if (proc == NULL) return 1;
    std::cout << "INT: " << std::endl;
    IntRead(proc, intRead);
    std::cout << "int = " << intRead << std::endl;
    StringRead(proc, stringRead);
    std::cout << "string = " << stringRead << std::endl;
    CharArrayRead(proc, charArrayRead);
    std::cout << "chararr = " << charArrayRead << std::endl;


    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, 0, 0);
    MSG msg{ 0 };
    while (GetMessage(&msg, NULL, 0, 0));
    UnhookWindowsHookEx(keyboardHook);
    return 0;
}
void IntRead(HANDLE proc, int& intRead)
{
    uintptr_t adrr;
    std::cout << "Address: 0x";
    std::cin >> std::hex >> adrr;
    bool Ok = ReadProcessMemory(proc, (LPCVOID)adrr, &intRead, sizeof(int), NULL);
    if (Ok == 0)exit(EXIT_FAILURE);
}
void StringRead(HANDLE proc, std::string& stringRead)
{
    uintptr_t adrr;
    std::cout << "Address: 0x";
    std::cin >> std::hex >> adrr;
    bool Ok = ReadProcessMemory(proc, (LPCVOID)adrr, &stringRead, sizeof(std::string), NULL);
    if (Ok == 0)exit(EXIT_FAILURE);
}
void CharArrayRead(HANDLE proc, char charArrayRead[])
{
    uintptr_t adrr;
    std::cout << "Address: 0x";
    std::cin >> std::hex >> adrr;
    bool Ok = ReadProcessMemory(proc, (LPCVOID)adrr, (LPVOID)charArrayRead, 50, NULL);
    if (Ok == 0)exit(EXIT_FAILURE);
}


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    wchar_t title[256];
    HWND fwindow = GetForegroundWindow();
    HWND hwnd = FindWindowA(NULL,
        "C:\\Users\\annav\\source\\repos\\ConsoleApplication1\\x64\\Debug\\ConsoleApplication1.exe");
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
    if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
    {

        if (hwnd == fwindow) {
            if (key->vkCode != VK_RETURN)
                std::cout << (char)key->vkCode;
            else
                std::cout << std::endl;
        }


    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
