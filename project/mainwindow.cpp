#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
                                          ui_(std::make_unique<Ui::MainWindow>()),
                                          gamepad_(10),
                                          isHomeButtonPressed_(false),
                                          config_(),
                                          gamepadIndex_(0)
{
    ui_->setupUi(this);

    Gamepad::SetGamepadInfoHandler([&](const char* s) -> void
        { emit AppendText(QString("Gamepad:") + s); });
    connect(this, SIGNAL(AppendText(QString)),
            this, SLOT(OnAppendText(QString)));
    connect(ui_->showKbvCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(OnShowKbvCheckBoxStateChanged(int)));
    connect(ui_->showPadValueCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(OnShowPadValueCheckBoxStateChanged(int)));
    connect(ui_->comboBox, SIGNAL(activated(int)),
            this, SLOT(OnComboBoxActivated(int)));

    InitKeyboardHandler();
    InitGamepadHandler();
    config_.ReadConfig();
    if (config_.isShowKbvChecked)
        ui_->showKbvCheckBox->setChecked(true);
    if (config_.isShowPadValueChecked)
        ui_->showPadValueCheckBox->setChecked(true);
    ui_->comboBox->setCurrentIndex(config_.xInputIndex);
    emit ui_->comboBox->activated(config_.xInputIndex);
    gamepadIndex_ = config_.xInputIndex;
}

MainWindow::~MainWindow()
{
    KeyboardHandler::Stop();
    config_.SaveConfig();
}

void MainWindow::OnAppendText(const QString& string)
{
    ui_->plainTextEdit->appendPlainText(string);
}

void MainWindow::OnShowKbvCheckBoxStateChanged(int state)
{
    config_.isShowKbvChecked = bool(state);
}

void MainWindow::OnShowPadValueCheckBoxStateChanged(int state)
{
    config_.isShowPadValueChecked = bool(state);
}

inline QString GetGamepadStateText(const Gamepad::GamepadState& state)
{
    static QString str = "button pressed:%1\naxis:%2";
    static QString buttonStr;
    buttonStr.resize(0);
    if ((state.wButtons & XINPUT_GAMEPAD_DPAD_UP) == XINPUT_GAMEPAD_DPAD_UP)
        buttonStr.append("DPAD_UP ");
    if ((state.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) == XINPUT_GAMEPAD_DPAD_DOWN)
        buttonStr.append("DPAD_DOWN ");
    if ((state.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) == XINPUT_GAMEPAD_DPAD_LEFT)
        buttonStr.append("DPAD_LEFT ");
    if ((state.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) == XINPUT_GAMEPAD_DPAD_RIGHT)
        buttonStr.append("DPAD_RIGHT ");
    if ((state.wButtons & XINPUT_GAMEPAD_START) == XINPUT_GAMEPAD_START)
        buttonStr.append("START ");
    if ((state.wButtons & XINPUT_GAMEPAD_BACK) == XINPUT_GAMEPAD_BACK)
        buttonStr.append("BACK ");
    if ((state.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) == XINPUT_GAMEPAD_LEFT_THUMB)
        buttonStr.append("LEFT_THUMB ");
    if ((state.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) == XINPUT_GAMEPAD_RIGHT_THUMB)
        buttonStr.append("RIGHT_THUMB ");
    if ((state.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) == XINPUT_GAMEPAD_LEFT_SHOULDER)
        buttonStr.append("LEFT_SHOULDER ");
    if ((state.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) == XINPUT_GAMEPAD_RIGHT_SHOULDER)
        buttonStr.append("RIGHT_SHOULDER ");
    if ((state.wButtons & XINPUT_GAMEPAD_A) == XINPUT_GAMEPAD_A)
        buttonStr.append("A ");
    if ((state.wButtons & XINPUT_GAMEPAD_B) == XINPUT_GAMEPAD_B)
        buttonStr.append("B ");
    if ((state.wButtons & XINPUT_GAMEPAD_X) == XINPUT_GAMEPAD_X)
        buttonStr.append("X ");
    if ((state.wButtons & XINPUT_GAMEPAD_Y) == XINPUT_GAMEPAD_Y)
        buttonStr.append("Y ");
    return str.arg(buttonStr.size() > 0 ? buttonStr : "null",
                   QString("LX:%1 LY:%2 RX:%3 RY:%4 LT:%5 RT:%6")
                       .arg(state.sThumbLX)
                       .arg(state.sThumbLY)
                       .arg(state.sThumbRX)
                       .arg(state.sThumbRY)
                       .arg(state.bLeftTrigger)
                       .arg(state.bRightTrigger));
}

void MainWindow::InitGamepadHandler()
{
    gamepad_.SetHandler([&](const Gamepad::GamepadState& state) -> void {
        if (config_.isShowPadValueChecked)
            emit AppendText(GetGamepadStateText(state));
        if (!isHomeButtonPressed_)
            return;
        static bool isEventTriggered = false;
        if ((state.wButtons & XINPUT_GAMEPAD_DPAD_UP) == XINPUT_GAMEPAD_DPAD_UP)
            {
                if (!isEventTriggered)
                    {
                        isEventTriggered = true;
                        keybd_event(0, 0x3b, KEYEVENTF_SCANCODE, 0);
                        keybd_event(0, 0x3b, KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
                    }
            }
        else if ((state.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) == XINPUT_GAMEPAD_DPAD_DOWN)
            {
                if (!isEventTriggered)
                    {
                        isEventTriggered = true;
                        keybd_event(0, 0x3d, KEYEVENTF_SCANCODE, 0);
                        keybd_event(0, 0x3d, KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
                    }
            }
        else
            {
                if (isEventTriggered)
                    isEventTriggered = false;
            }
    });
    gamepad_.Start();
}

void MainWindow::InitKeyboardHandler()
{
    KeyboardHandler::SetHandler(
        [&](KeyboardHandler::KeyCode keyCode, KeyboardHandler::KeyState keyState) -> void {
            if (keyCode == 0x07)
                isHomeButtonPressed_ = keyState == KeyboardHandler::KeyPressed ? true : false;
            if (config_.isShowKbvChecked)
                {
                    static QString fmt = "keyboard:key_code:%1 key_state:%2";
                    emit AppendText(fmt.arg(keyCode).arg(
                        keyState == KeyboardHandler::KeyPressed ? "pressed" : "released"));
                }
        });
    KeyboardHandler::Start();
}

void MainWindow::OnComboBoxActivated(int index)
{
    config_.xInputIndex = index;
    gamepad_.SwitchIndex(index);
    emit AppendText(QString("select gamepad #%1").arg(index + 1));
}
