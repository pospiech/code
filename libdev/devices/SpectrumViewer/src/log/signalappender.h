#ifndef SIGNALAPPENDER_H
#define SIGNALAPPENDER_H

#include <AbstractStringAppender.h>

#include <QObject>

class SignalAppender : public QObject, public AbstractStringAppender
{
    Q_OBJECT
public:
    explicit SignalAppender(QObject * parent = nullptr);

    virtual QString format() const;
    void ignoreEnvironmentPattern(bool ignore);

signals:
    void logMessage(QString &);

protected:
    virtual void append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                        const char* function, const QString& category, const QString& message);

private:
    bool m_ignoreEnvPattern;

};

#endif // SIGNALAPPENDER_H
