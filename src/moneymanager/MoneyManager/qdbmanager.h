#ifndef QDBMANAGER_H
#define QDBMANAGER_H

#include <QObject>

class QDBManager : public QObject
{
    Q_OBJECT
public:
    explicit QDBManager(QObject *parent = nullptr);

private:
    void DatabaseConnect();
    void DatabaseInit();


signals:

public slots:
};

#endif // QDBMANAGER_H
