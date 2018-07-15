#include "gui/mainwindow.h"
#include <QApplication>

#include "Logger.h"
#include "ConsoleAppender.h"
#include "FileAppender.h"

int main(int argc, char *argv[])
{
    // init logger console
    ConsoleAppender* consoleAppender = new ConsoleAppender;
    consoleAppender->setFormat("[%{type:-7}] %{time}{dd-MM-yyyy, HH:mm}; Thread: %{threadid}; <%{Function}>; %{message}\n");
    consoleAppender->setDetailsLevel(Logger::Trace);
    Logger::globalInstance()->registerAppender(consoleAppender);

    // init logger file log
    FileAppender* fileAppender = new FileAppender;
    fileAppender->setFormat("[%{type:-7}] %{time}{dd-MM-yyyy, HH:mm:ss.zzz}; Thread: %{threadid}; <%{Function}>; %{message}\n");
    fileAppender->setFileName("CameraViewerLog.txt");
    fileAppender->setDetailsLevel(Logger::Debug);
    Logger::globalInstance()->registerAppender(fileAppender);

    QApplication a(argc, argv);
    LOG_INFO() << "Starting the application";
    MainWindow w;
    w.show();

    return a.exec();
}
