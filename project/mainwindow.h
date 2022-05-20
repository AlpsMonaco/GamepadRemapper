#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gamepad.h"
#include "keyboard.h"
#include <QMainWindow>
#include <atomic>
#include <fstream>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    struct Config
    {
        bool isShowKbvChecked;
        bool isShowPadValueChecked;

        Config()
            : isShowKbvChecked(false)
            , isShowPadValueChecked(false)
        {
        }
        ~Config() = default;

        static constexpr char fileName[] = "config.bin";
        static constexpr char isShowKbvCheckedBit = 0b00000001;
        static constexpr char isShowPadValueCheckedBit = 0b00000010;
        void ReadConfig()
        {
            std::ifstream ifs(fileName, std::ios_base::binary);
            if (!ifs.is_open())
                return;
            char c;
            size_t size = ifs.read(&c, 1).gcount();
            if (size == 0)
                return;
            if ((isShowKbvCheckedBit & c) == isShowKbvCheckedBit)
                isShowKbvChecked = true;
            if ((isShowPadValueCheckedBit & c) == isShowPadValueCheckedBit)
                isShowPadValueChecked = true;
        }

        void SaveConfig()
        {
            std::ofstream ofs(fileName, std::ios_base::binary | std::ios_base::trunc);
            if (!ofs.is_open())
                return;
            char c = 0;
            if (isShowKbvChecked)
                c = (c | isShowKbvCheckedBit);
            if (isShowPadValueChecked)
                c = (c | isShowPadValueCheckedBit);
            ofs << c;
        }
    };

    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void AppendText(const QString& string);

private slots:
    void OnAppendText(const QString& string);
    void OnShowKbvCheckBoxStateChanged(int state);
    void OnShowPadValueCheckBoxStateChanged(int state);

private:
    Ui::MainWindow* ui;
    Gamepad gamepad_;
    std::atomic_bool isHomeButtonPressed_;
    Config config_;
    void InitKeyboardHandler();
    void InitGamepadHandler();
};
#endif // MAINWINDOW_H
