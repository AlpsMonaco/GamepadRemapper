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
        {                        \
            _handler(msg);       \
        }                        \
    while (0)
#endif

Gamepad::Gamepad(Gamepad::SampleTime sampleTime) : sampleTime_(sampleTime),
                                                   handler_([](unsigned long, const Gamepad::GamepadState&) -> void {}),
                                                   stop_(false)
{
}

Gamepad::~Gamepad()
{
    Stop();
    Wait();
}

// void Gamepad::SwitchIndex(unsigned long xInputIndex)
//{
//     HANDLE_GAMEPAD_INFO(
//         fmt::format("switch controller index to {}", xInputIndex).c_str());
//     if (xInputIndex == xInputIndex_)
//         return;
//     Stop();
//     Wait();
//     xInputIndex_ = xInputIndex;
//     stop_ = false;
//     Start();
// }

void Gamepad::SetHandler(const Gamepad::Handler& handler) { handler_ = handler; }

void Gamepad::Start()
{
    thread_ = std::thread(
        [&]() -> void {
            HANDLE_GAMEPAD_INFO("gamepad thread start");
            XINPUT_STATE state { 0, { 0, 0, 0, 0, 0, 0, 0 } };
            static constexpr DWORD maxPacketNumber = (~0);
            DWORD lastPacketNumberList[XUSER_MAX_COUNT];
            for (size_t i = 0; i < XUSER_MAX_COUNT; i++)
                lastPacketNumberList[i] = maxPacketNumber;
            XInputIndex xInputIndex = 0;
            for (;;)
                {
                    if (stop_)
                        break;
                    for (xInputIndex = 0; xInputIndex < XUSER_MAX_COUNT; xInputIndex++)
                        {
                            if (XInputGetState(xInputIndex, &state) == ERROR_SUCCESS)
                                {
                                    if (lastPacketNumberList[xInputIndex] != state.dwPacketNumber)
                                        {
                                            handler_(xInputIndex, state.Gamepad);
                                            lastPacketNumberList[xInputIndex] = state.dwPacketNumber;
                                        }
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
