#include "gamepad.h"

Gamepad::Gamepad(Gamepad::SampleTime sampleTime)
    : sampleTime_(sampleTime)
    , handler_([](const Gamepad::GamepadState&) -> void {})
    , stop_(false)
{
}

Gamepad::~Gamepad()
{
    Stop();
    Wait();
}

void Gamepad::SetHandler(const Gamepad::Handler& handler) { handler_ = handler; }

void Gamepad::Start()
{
    thread_ = std::thread(
        [&]() -> void {
            XINPUT_STATE state { 0, { 0, 0, 0, 0, 0, 0, 0 } };
            DWORD lastPacketNumber = -1;
            for (;;)
                {
                    if (stop_)
                        break;
                    if (XInputGetState(0, &state) == ERROR_SUCCESS)
                        {
                            if (lastPacketNumber != state.dwPacketNumber)
                                handler_(state.Gamepad);
                        }
                    std::this_thread::sleep_for(std::chrono::milliseconds(sampleTime_));
                }
        });
}

void Gamepad::Stop() { stop_ = true; }

void Gamepad::Wait()
{
    thread_.join();
}
