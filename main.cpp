/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "lpdrpmsg.h"
#include "lpdgraphs.h"
#include "lpdserver.h"
#include <signal.h>
#include <unistd.h>

#include <QMessageBox>
#include <QtWidgets/QApplication>

void catchUnixSignals(const std::vector<int>& quitSignals,
                      const std::vector<int>& ignoreSignals = std::vector<int>())
{
    auto handler = [](int sig) ->void {
        QCoreApplication::quit();
    };

    for ( int sig : ignoreSignals )
        signal(sig, SIG_IGN);

    for ( int sig : quitSignals )
        signal(sig, handler);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSerialPort serialPort;

    catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP, SIGKILL});
    serialPort.setPortName("/dev/ttyRPMSG");
    if (!serialPort.open(QIODevice::ReadWrite)) {
        QMessageBox msgBox;
        msgBox.setText("Failed to open ttyRPMSG port");
        msgBox.exec();
        return 1;
    }

    LpdRpmsg LpdRpmsg(&serialPort);
    LpdGraphs LpdGraph;

    QCommandLineParser parser;
    parser.setApplicationDescription("QtWebSockets example: lpdserver");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Port for lpdserver [default: 1234]."),
            QCoreApplication::translate("main", "port"), QLatin1Literal("1234"));
    parser.addOption(portOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();

    LpdServer LpdServer(port, debug);
    QObject::connect(&LpdServer, &LpdServer::closed, &a, &QCoreApplication::quit);
    QObject::connect(&LpdRpmsg, &LpdGraphs::m4_dataAvailable, &LpdServer, &LpdServer::sendData);

    QObject::connect(&LpdRpmsg, &LpdRpmsg::m4_dataAvailable, &LpdGraph, &LpdGraphs::m4_processData);
    QObject::connect(&LpdGraph, &LpdGraphs::m4_sendCommand, &LpdRpmsg, &LpdRpmsg::sendCommand);

    /* Ask M4 to send Accelerometer data */
    emit LpdGraph.m4_sendCommand("acc");

    return a.exec();
}
