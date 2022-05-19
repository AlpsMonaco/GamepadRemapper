#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Windows.h>
#pragma comment(lib, "User32.Lib")

#include <functional>

class KeyboardHandler
{
public:
    using KeyCode = DWORD;
    using KeyStatus = WPARAM;
    using Handler = std::function<void(KeyCode, KeyStatus)>;

    static void Start();
    static void Stop();
    static void SetHandler(const Handler& handler);
    static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);

private:
    static Handler handler_;
    static HHOOK hook_;
};

#endif // KEYBOARD_H
