#include "mainwindow.h"
#include <QApplication>
#include <thread>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    std::thread([&]() -> void {
        for (;;)
            {
                std::this_thread::sleep_for(std::chrono::seconds(3));
                emit w.AppendText("Hello!");
            }
    }).detach();
    return a.exec();
}
