#ifndef APPLICATIONCALLBACKS_H
#define APPLICATIONCALLBACKS_H

#include <QApplication>

class ApplicationCallbacks : public QApplication
{
    Q_OBJECT

public:        
    ApplicationCallbacks(int &argc, char *argv[]);
public:
    static void callback(long *handle, int *result );
    static void connCb(long *handle, int connStat);

    void emitsignal(int WParam, int LParam);
    void emitsignalConn(int WParam, int LParam);


signals:
    void SpectrumAvailable(int WParam, int LParam);
    void ConnStatus(int WParam, int LParam);
    void LogMessage(QString message);


};

#endif // APPLICATIONCALLBACKS_H
