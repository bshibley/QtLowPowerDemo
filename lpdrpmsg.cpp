#include "lpdrpmsg.h"
#include <QByteArray>
#include <QMessageBox>

LpdRpmsg::LpdRpmsg(QSerialPort *serialPort, QObject *parent)
    : QObject(parent)
    , m_serialPort(serialPort)
{
    connect(m_serialPort, &QSerialPort::readyRead, this, &LpdRpmsg::handleReadyRead);
    connect(m_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
                this, &LpdRpmsg::handleError);
}

LpdRpmsg::~LpdRpmsg()
{
    m_serialPort->close();
}

void LpdRpmsg::handleReadyRead()
{
    emit m4_dataAvailable(m_serialPort->readAll());
}

void LpdRpmsg::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ReadError) {
        QMessageBox msgBox;
        msgBox.setText("IO error occured");
        msgBox.exec();
    }
}

void LpdRpmsg::sendCommand(const QByteArray &arg)
{
    m_serialPort->write(arg, arg.length());
}
