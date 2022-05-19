#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, SIGNAL(AppendText(QString)), this, SLOT(OnAppendText(QString)));
    InitKeyboardHandler();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnAppendText(const QString& string)
{
    ui->plainTextEdit->appendPlainText(string);
}

void MainWindow::OnShowKbvCheckBoxStateChanged(int state)
{
}

void MainWindow::OnShowPadValueCheckBoxStateChanged(int state)
{
}

void MainWindow::InitKeyboardHandler()
{
    KeyboardHandler::SetHandler([&](KeyboardHandler::KeyCode keyCode,
                                    KeyboardHandler::KeyStatus keyStatus) -> void {
        emit AppendText(QString("key pressed:") + QString::number(keyCode)
            + QString(" status:") + QString::number(keyStatus));
    });
    KeyboardHandler::Start();
}
