#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <Windows.h>
#include <XInput.h>
#include <atomic>
#include <functional>
#include <thread>

#pragma comment(lib, "Xinput.lib")

class Gamepad
{
public:
    using SampleTime = unsigned long long;
    using GamepadState = XINPUT_GAMEPAD;
    using Handler = std::function<void(const GamepadState&)>;

    Gamepad(SampleTime sampleTime = 10, unsigned long xInputIndex = 0);
    ~Gamepad();

    void SwitchIndex(unsigned long xInputIndex);
    void SetHandler(const Handler& handler);
    void Start();
    void Stop();
    void Wait();

protected:
    SampleTime sampleTime_;
    Handler handler_;
    std::atomic_bool stop_;
    std::thread thread_;
    unsigned long xInputIndex_;
};

#endif // GAMEPAD_H
