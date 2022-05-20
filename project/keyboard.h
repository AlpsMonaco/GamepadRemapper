#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Windows.h>
#pragma comment(lib, "User32.Lib")

#include <functional>

class KeyboardHandler
{
public:
    using KeyCode = DWORD;
    using KeyState = WPARAM;
    using Handler = std::function<void(KeyCode, KeyState)>;

    static constexpr KeyState KeyPressed = 256;
    static constexpr KeyState KeyReleased = 257;

    static void Start();
    static void Stop();
    static void SetHandler(const Handler& handler);
    static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);

private:
    static Handler handler_;
    static HHOOK hook_;
};

#endif // KEYBOARD_H
