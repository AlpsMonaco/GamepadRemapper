#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "keyboard.h"
#include <QMainWindow>

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

private:
    Ui::MainWindow* ui;
    void InitKeyboardHandler();
};
#endif // MAINWINDOW_H
