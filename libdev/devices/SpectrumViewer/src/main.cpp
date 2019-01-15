#include "mainwindow.h"
#include "applicationcallbacks.h"

int main(int argc, char *argv[])
{
    ApplicationCallbacks a(argc, argv);
    a.setStyle("plastique");

    MainWindow w;
    w.show();

    return a.exec();
}
