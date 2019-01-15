#include "applicationcallbacks.h"
#include <QtGui>
#include "avaspec.h"

#include "Logger.h"
#include "signalappender.h"

ApplicationCallbacks::ApplicationCallbacks(int &argc, char *argv[])
    : QApplication(argc, argv)
{
    SignalAppender* signalAppender = new SignalAppender;
    signalAppender->setFormat("[%{type:-7}] %{time}{dd-MM-yyyy, HH:mm}; Thread: %{threadid}; <%{Function}>; %{message}");
    signalAppender->setDetailsLevel(Logger::Trace);
    Logger::globalInstance()->registerAppender(signalAppender);

    QObject::connect(signalAppender, &SignalAppender::logMessage,
                     this, &ApplicationCallbacks::LogMessage);

}

// required because callback is static
void ApplicationCallbacks::emitsignal(int WParam, int LParam)
{
    emit SpectrumAvailable(WParam, LParam);
}

// required because callback is static
void ApplicationCallbacks::emitsignalConn(int WParam, int LParam)
{
    emit ConnStatus(WParam, LParam);
}


void ApplicationCallbacks::callback(AvsHandle *handle, int *result )
{
    if (*result >= 0) {
        ApplicationCallbacks *m = static_cast<ApplicationCallbacks*>(qApp);
        // qApp is a predefined pointer to the QApplication instance
        if (m != nullptr)
            m->emitsignal(*result, *handle);
    }
}
