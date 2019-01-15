#ifndef WIDGETSERIALPORT_H
#define WIDGETSERIALPORT_H

#include <QWidget>
class QSerialPort;

class WidgetSerialPort : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetSerialPort(QWidget *parent = nullptr);

signals:

public slots:
    void writeData(const QByteArray &data);
    void closeSerialPort();
    void readData();

private:
    QSerialPort * m_serial;
};

#endif // WIDGETSERIALPORT_H
