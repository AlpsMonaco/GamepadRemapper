#include "gamepad.h"
#include "fmt/core.h"

#ifdef NDEBUG
void Gamepad::SetGamepadInfoHandler(const GamdpadInfoHandler&)
{
}
#define HANDLE_GAMEPAD_INFO(msg) ((void)0)
#else
Gamepad::GamdpadInfoHandler _handler = [](const char*) -> void {};
void Gamepad::SetGamepadInfoHandler(const GamdpadInfoHandler& handler)
{
    _handler = handler;
}

#define HANDLE_GAMEPAD_INFO(msg) \
    do                           \
    {                            \
        _handler(msg);           \
    } while (0)
#endif

Gamepad::Gamepad(Gamepad::SampleTime sampleTime,
                 unsigned long xInputIndex) : sampleTime_(sampleTime),
                                              handler_([](const Gamepad::GamepadState&) -> void {}),
                                              stop_(false),
                                              xInputIndex_(xInputIndex)
{
}

Gamepad::~Gamepad()
{
    Stop();
    Wait();
}

void Gamepad::SwitchIndex(unsigned long xInputIndex)
{
    HANDLE_GAMEPAD_INFO(
        fmt::format("switch controller index to {}", xInputIndex).c_str());
    if (xInputIndex == xInputIndex_)
        return;
    Stop();
    Wait();
    xInputIndex_ = xInputIndex;
    stop_ = false;
    Start();
}

void Gamepad::SetHandler(const Gamepad::Handler& handler) { handler_ = handler; }

void Gamepad::Start()
{
    thread_ = std::thread(
        [&]() -> void
        {
            HANDLE_GAMEPAD_INFO("gamepad thread start");
            XINPUT_STATE state { 0, { 0, 0, 0, 0, 0, 0, 0 } };
            DWORD lastPacketNumber = -1;
            for (;;)
            {
                if (stop_)
                    break;
                if (XInputGetState(xInputIndex_, &state) == ERROR_SUCCESS)
                {
                    if (lastPacketNumber != state.dwPacketNumber)
                    {
                        handler_(state.Gamepad);
                        lastPacketNumber = state.dwPacketNumber;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(sampleTime_));
            }
            HANDLE_GAMEPAD_INFO("gamepad thread finish");
        });
}

void Gamepad::Stop() { stop_ = true; }

void Gamepad::Wait()
{
    thread_.join();
}
