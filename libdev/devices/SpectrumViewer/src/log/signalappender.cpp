#include "signalappender.h"

SignalAppender::SignalAppender(QObject * parent)
    : QObject(parent),
      AbstractStringAppender(),
      m_ignoreEnvPattern(false)
{
    setFormat("[%{type:-7}] <%{function}> %{message}\n");
}

QString SignalAppender::format() const
{
  const QString envPattern = QString::fromLocal8Bit(qgetenv("QT_MESSAGE_PATTERN"));
  return (m_ignoreEnvPattern || envPattern.isEmpty()) ? AbstractStringAppender::format() : (envPattern + "\n");
}


void SignalAppender::ignoreEnvironmentPattern(bool ignore)
{
  m_ignoreEnvPattern = ignore;
}


//! Sends the log record as a signal
/**
 * \sa AbstractStringAppender::format()
 */
void SignalAppender::append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                             const char* function, const QString& category, const QString& message)
{
   QString messageStr = formattedString(timeStamp, logLevel, file, line, function, category, message);
    emit logMessage(messageStr);
}

