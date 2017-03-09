#ifndef LPDRPMSG_H
#define LPDRPMSG_H

#include <QtCore/QObject>
#include <QtCore/QTextStream>
#include <QtSerialPort/QSerialPort>

class LpdRpmsg : public QObject
{
    Q_OBJECT
public:
    explicit LpdRpmsg(QSerialPort *serialPort, QObject *parent = nullptr);
    ~LpdRpmsg();

public slots:
    void sendCommand(const QByteArray& arg);

signals:
    void m4_dataAvailable(const QString& arg);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serialPort;
};

#endif // LPDRPMSG_H
