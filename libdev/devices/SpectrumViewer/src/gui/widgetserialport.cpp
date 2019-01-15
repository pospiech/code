#include "widgetserialport.h"
#include <QSerialPort>

WidgetSerialPort::WidgetSerialPort(QWidget *parent)
    : QWidget(parent)
    , m_serial(new QSerialPort(this))
{

}

void WidgetSerialPort::writeData(const QByteArray &data)
{
    m_serial->write(data);
}

void WidgetSerialPort::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
//    m_console->setEnabled(false);
//    m_ui->actionConnect->setEnabled(true);
//    m_ui->actionDisconnect->setEnabled(false);
//    m_ui->actionConfigure->setEnabled(true);
//    showStatusMessage(tr("Disconnected"));
}


void WidgetSerialPort::readData()
{
    const QByteArray data = m_serial->readAll();
//    m_console->putData(data);
}

