#include "keyboard.h"

KeyboardHandler::Handler KeyboardHandler::handler_ = [](KeyCode keyCode,
                                                        KeyState keyStatus) -> void {};

HHOOK KeyboardHandler::hook_ = NULL;

LRESULT CALLBACK KeyboardHandler::KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
    handler_(ks->vkCode, wParam);
    return CallNextHookEx(NULL, code, wParam, lParam);
}

void KeyboardHandler::Start()
{
    hook_ = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
}

void KeyboardHandler::Stop()
{
    if (hook_ != NULL)
        UnhookWindowsHookEx(hook_);
}

void KeyboardHandler::SetHandler(const Handler& handler)
{
    handler_ = handler;
}
