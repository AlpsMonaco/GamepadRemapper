#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include "gamepad.h"
#include "keyboard.h"
#include <QMainWindow>
#include <atomic>
#include <fstream>
#include <memory>

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
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void AppendText(const QString& string);

private slots:
    void OnAppendText(const QString& string);
    void OnShowKbvCheckBoxStateChanged(int state);
    void OnShowPadValueCheckBoxStateChanged(int state);
    void OnComboBoxActivated(int index);

private:
    using UIPointer = std::unique_ptr<Ui::MainWindow>;
    UIPointer ui_;
    Gamepad gamepad_;
    std::atomic_bool isHomeButtonPressed_;
    Config config_;
    std::atomic_ulong gamepadIndex_;
    void InitKeyboardHandler();
    void InitGamepadHandler();
};
#endif // MAINWINDOW_H
