#include "lpdgraphs.h"
#include <QProcess>
#include <QDebug>

LpdGraphs::LpdGraphs(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    qsrand((uint) QTime::currentTime().msec());

    initialiseView();
}

void LpdGraphs::setupLogo()
{
    scene = new QGraphicsScene;
    view = new QGraphicsView(scene);
    item = new QGraphicsPixmapItem(QPixmap(":/images/toradexlogo.png"));
    scene->addItem(item);
    view->setFixedHeight(87);
    view->setFixedWidth(800);
    view->setContentsMargins(0,0,0,0);
    view->setAlignment(Qt::AlignTop);
    view->setVisible(true);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void LpdGraphs::setupButtons()
{
    gyroButton = new QPushButton(tr("Gyroscope"));
    accButton = new QPushButton(tr("Accelerometer"));
    magnetoButton = new QPushButton(tr("Magnetometer"));
    altiButton = new QPushButton(tr("Altimeter"));
    sleepButton = new QPushButton();
    shutdownButton = new QPushButton();
    connect(gyroButton, SIGNAL(released()), this, SLOT(handleGyroButton()));
    connect(accButton, SIGNAL(released()), this, SLOT(handleAccButton()));
    connect(magnetoButton, SIGNAL(released()), this, SLOT(handleMagnetoButton()));
    connect(altiButton, SIGNAL(released()), this, SLOT(handleAltiButton()));
    connect(sleepButton, SIGNAL(released()), this, SLOT(handleSleepButton()));
    connect(shutdownButton, SIGNAL(released()), this, SLOT(handleShutdownButton()));

    QPixmap sleepPixmap(":/images/sleep.png");
    QIcon sleepIcon(sleepPixmap);
    sleepButton->setIcon(sleepIcon);
    sleepButton->setIconSize(sleepPixmap.rect().size());

    QPixmap shutdownPixmap(":/images/shutdown.png");
    QIcon shutdownIcon(shutdownPixmap);
    shutdownButton->setIcon(shutdownIcon);
    shutdownButton->setIconSize(shutdownPixmap.rect().size());

    buttonFont.setPixelSize(16);
    gyroButton->setFont(buttonFont);
    accButton->setFont(buttonFont);
    magnetoButton->setFont(buttonFont);
    altiButton->setFont(buttonFont);
    sleepButton->setFont(buttonFont);
    shutdownButton->setFont(buttonFont);

    gyroButton->setFixedHeight(40);
    accButton->setFixedHeight(40);
    magnetoButton->setFixedHeight(40);
    altiButton->setFixedHeight(40);

    sleepButton->setFixedWidth(80);
    sleepButton->setFixedHeight(80);
    shutdownButton->setFixedWidth(80);
    shutdownButton->setFixedHeight(80);

    gyroButton->setAutoFillBackground(true);
    QPalette bPalette = gyroButton->palette();
    bPalette.setColor(QPalette::Button, QRgb(0x00508c));
    bPalette.setColor(QPalette::ButtonText, Qt::white);
    gyroButton->setPalette(bPalette);

    accButton->setAutoFillBackground(true);
    bPalette = accButton->palette();
    bPalette.setColor(QPalette::Button, QRgb(0x00508c));
    bPalette.setColor(QPalette::ButtonText, Qt::white);
    accButton->setPalette(bPalette);

    magnetoButton->setAutoFillBackground(true);
    bPalette = magnetoButton->palette();
    bPalette.setColor(QPalette::Button, QRgb(0x00508c));
    bPalette.setColor(QPalette::ButtonText, Qt::white);
    magnetoButton->setPalette(bPalette);

    altiButton->setAutoFillBackground(true);
    bPalette = altiButton->palette();
    bPalette.setColor(QPalette::Button, QRgb(0x00508c));
    bPalette.setColor(QPalette::ButtonText, Qt::white);
    altiButton->setPalette(bPalette);

    gyroButton->setContentsMargins(0,0,0,0);
    accButton->setContentsMargins(0,0,0,0);
    magnetoButton->setContentsMargins(0,0,0,0);
    altiButton->setContentsMargins(0,0,0,0);
    sleepButton->setContentsMargins(0,0,0,0);
    shutdownButton->setContentsMargins(0,0,0,0);
}

void LpdGraphs::setupHBoxLayout()
{
    hgroupBox = new QGroupBox;
    hboxLayout = new QHBoxLayout;
    hboxLayout->setSpacing(0);
    hboxLayout->setMargin(0);
    hboxLayout->setContentsMargins(0,0,0,0);
    hgroupBox->setContentsMargins(0,0,0,0);
    hgroupBox->setFixedHeight(40);
    hboxLayout->addWidget(accButton);
    /*hboxLayout->addWidget(altiButton);*/
    hboxLayout->addWidget(gyroButton);
    hboxLayout->addWidget(magnetoButton);
    hgroupBox->setLayout(hboxLayout);
}

void LpdGraphs::setupCharts()
{
    chart = new QChart(this);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    graphSeries1 = new QSplineSeries(chart);
    chart->addSeries(graphSeries1);
    graphSeries2 = new QSplineSeries(chart);
    chart->addSeries(graphSeries2);
    graphSeries3 = new QSplineSeries(chart);
    chart->addSeries(graphSeries3);

    axisX = new QValueAxis;
    chart->createDefaultAxes();
    chart->setAxisX(axisX, graphSeries1);
    chart->setAxisX(axisX, graphSeries2);
    chart->setAxisX(axisX, graphSeries3);
    axisX->setTickCount(X_RANGE_MAX);
    chart->axisX()->setRange(0, X_RANGE_MAX);
    chart->axisX()->hide();
    chart->axisY()->setRange(-100, 100);
    chart->setContentsMargins(0,0,0,0);
    chart->legend()->hide();

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setContentsMargins(0,0,0,0);

    hgroupChartBox = new QGroupBox;
    hboxChartLayout = new QHBoxLayout;
    vgroupChartBox = new QGroupBox;
    vboxChartLayout = new QVBoxLayout;

    chartLabel = new QTextEdit;
    chartLabel->setReadOnly(true);

    chartLabel->setFixedWidth(80);
    chartLabel->setFixedHeight(142);
    chartLabel->setContentsMargins(0,0,0,0);
    chartLabel->setAlignment(Qt::AlignBottom);

    QPalette pal = chartLabel->palette();
    pal.setColor(QPalette::Base, QRgb(0x414147));
    chartLabel->setPalette(pal);

    chartLabelFont.setPixelSize(16);
    chartLabelFont.setBold(true);
    chartLabel->setFont(chartLabelFont);

    vboxChartLayout->addWidget(chartLabel);
    vboxChartLayout->addWidget(sleepButton);
    vboxChartLayout->addWidget(shutdownButton);
    vboxChartLayout->setSpacing(0);
    vboxChartLayout->setMargin(0);
    vboxChartLayout->setContentsMargins(0,0,0,0);
    vgroupChartBox->setFixedWidth(80);
    vgroupChartBox->setContentsMargins(0,0,0,0);
    vgroupChartBox->setLayout(vboxChartLayout);

    hboxChartLayout->addWidget(vgroupChartBox);
    hboxChartLayout->addWidget(chartView);
    hboxChartLayout->setSpacing(0);
    hboxChartLayout->setMargin(0);
    hboxChartLayout->setContentsMargins(0,0,0,0);
    hgroupChartBox->setFixedHeight(360);
    hgroupChartBox->setLayout(hboxChartLayout);
}

void LpdGraphs::setupVBoxLayout()
{
    window = new QWidget;
    vboxLayout = new QVBoxLayout;
    vboxLayout->setSpacing(0);
    vboxLayout->setMargin(0);
    vboxLayout->setContentsMargins(0,0,0,0);
    vboxLayout->addWidget(view);
    vboxLayout->addWidget(hgroupBox);
    vboxLayout->addWidget(hgroupChartBox);
}

void LpdGraphs::setupMainWindow()
{
    QPalette pal = window->palette();
    pal.setColor(QPalette::Window, QRgb(0x121218));
    pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    chart->setTheme(QChart::ChartThemeDark);
    window->setPalette(pal);

    window->setLayout(vboxLayout);
    window->showFullScreen();
}

void LpdGraphs::initialiseView()
{
    setupLogo();
    setupButtons();
    setupHBoxLayout();
    setupCharts();
    setupVBoxLayout();
    setupMainWindow();
    openSleepFile();

    QMetaObject::invokeMethod(accButton, "released");
    m_x = 0;
}

LpdGraphs::~LpdGraphs()
{
    emit m4_sendCommand("close");
    if (sleepFile.isOpen())
        sleepFile.close();
}

void LpdGraphs::m4_processData(const QString &str)
{
    bool okx, oky, okz;
    /* Response from M4 will be a string of form "acc?x?y?z" */
    QStringList list = str.split(',');

    for (int i = 0; i < list.size() - 1; i = i + 4) {
        switch (index_graph) {
        case gyro:
            if (QString::compare(list[i + 0], "gyro") != 0)
                return;
            break;
        case accl:
            if (QString::compare(list[i + 0], "acc") != 0)
                return;
            break;
        case magneto:
            if (QString::compare(list[i + 0], "mag") != 0)
                return;
            break;
        case alti:
            if (QString::compare(list[i + 0], "alti") != 0)
                return;
            break;
        default:
            break;
        }

        qreal val1 = list.at(i + 1).toFloat(&okx);
        if (okx == false)
            return;

        qreal val2 = list.at(i + 2).toFloat(&oky);
        if (oky == false)
            return;

        qreal val3 = 0;
        if (index_graph != alti) {
            val3 = list.at(i + 3).toFloat(&okz);
            if (okz == false)
                return;
        }

        //emit server_sendData("");
        //qDebug().nospace() << str << "|" << val1 << " " << val2 << " " << val3;

        qreal y = (axisX->max() - axisX->min()) / axisX->tickCount();
        m_x += y;
        switch (index_graph) {
        case gyro:
            graphSeries1->append(m_x, val1);
            graphSeries2->append(m_x, val2);
            graphSeries3->append(m_x, val3);
            break;
        case accl:
            graphSeries1->append(m_x, val1);
            graphSeries2->append(m_x, val2);
            graphSeries3->append(m_x, val3);
            break;
        case magneto:
            graphSeries1->append(m_x, val1);
            graphSeries2->append(m_x, val2);
            graphSeries3->append(m_x, val3);
            break;
        case alti:
            graphSeries1->append(m_x, val1);
            graphSeries2->append(m_x, val2);
            break;
        default:
            break;
        }

        if (m_x > axisX->max()) {
            axisX->setMax(m_x);
            axisX->setMin(m_x - X_RANGE_COUNT);
        }

        if (graphSeries1->count() > X_RANGE_COUNT) {
            graphSeries1->remove(0);
            graphSeries2->remove(0);
            graphSeries3->remove(0);
        }
    }
}

void LpdGraphs::handleButtonColorChange(int button)
{
    QPalette bPalette;
    switch (button) {
    case accl:
        bPalette = accButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x004664));
        accButton->setPalette(bPalette);
        bPalette = gyroButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        gyroButton->setPalette(bPalette);
        bPalette = magnetoButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        magnetoButton->setPalette(bPalette);
        break;
    case gyro:
        bPalette = accButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        accButton->setPalette(bPalette);
        bPalette = gyroButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x004664));
        gyroButton->setPalette(bPalette);
        bPalette = magnetoButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        magnetoButton->setPalette(bPalette);
        break;
    case magneto:
        bPalette = accButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        accButton->setPalette(bPalette);
        bPalette = gyroButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        gyroButton->setPalette(bPalette);
        bPalette = magnetoButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x004664));
        magnetoButton->setPalette(bPalette);
        break;
    case alti:
        break;
    default:
        break;
    }
}

void LpdGraphs::handleChartLabelText(int graph)
{
    chartLabel->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    chartLabel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    switch (graph) {
    case accl:
        chartLabel->clear();
        /* Use HTML hackery since use of superscript is not possible with plain text */
        chartLabel->append("<p style=\"line-height: 50\"><font size=\"3\">"
                           "<font color=#38ad6b><br>&nbsp;&nbsp;&nbsp;X m/s<sup>2<br><br><br><br>");
        chartLabel->append("<font size=\"3\"><font color=#3c84a7>&nbsp;&nbsp;&nbsp;Y m/s<sup>2<br><br><br>");
        chartLabel->append("<font size=\"3\"><font color=#eb8817>&nbsp;Z m/s<sup>2</p>");

        break;
    case gyro:
        chartLabel->clear();
        chartLabel->setTextColor(graphSeries1->pen().color());
        chartLabel->setText("\n   X °/s");
        chartLabel->setTextColor(graphSeries2->pen().color());
        chartLabel->append("\n   Y °/s");
        chartLabel->setTextColor(graphSeries3->pen().color());
        chartLabel->append("\n   Z °/s");
        break;
    case magneto:
        chartLabel->clear();
        chartLabel->setTextColor(graphSeries1->pen().color());
        chartLabel->setText("\n     X  °");
        chartLabel->setTextColor(graphSeries2->pen().color());
        chartLabel->append("\n     Y  °");
        chartLabel->setTextColor(graphSeries3->pen().color());
        chartLabel->append("\n     Z  °");
        break;
    case alti:
        break;
    default:
        break;
    }
    chartLabel->setAlignment(Qt::AlignHCenter);
}

void LpdGraphs::handleGyroButton()
{
    index_graph = gyro;
    graphSeries1->clear();
    graphSeries2->clear();
    graphSeries3->clear();
    handleButtonColorChange(gyro);
    handleChartLabelText(gyro);
    gyroButton->setDown(true);
    accButton->setDown(false);
    magnetoButton->setDown(false);
    altiButton->setDown(false);
    chart->axisY()->setRange(-180, 180);
    /* Ask M4 to send Gyroscope data */
    emit m4_sendCommand("gyro");
}

void LpdGraphs::handleAccButton()
{
    index_graph = accl;
    graphSeries1->clear();
    graphSeries2->clear();
    graphSeries3->clear();
    handleButtonColorChange(accl);
    handleChartLabelText(accl);
    gyroButton->setDown(false);
    accButton->setDown(true);
    magnetoButton->setDown(false);
    altiButton->setDown(false);
    chart->axisY()->setRange(-4, 4);
    /* Ask M4 to send Accelerometer data */
    emit m4_sendCommand("acc");
}

void LpdGraphs::handleMagnetoButton()
{
    index_graph = magneto;
    graphSeries1->clear();
    graphSeries2->clear();
    graphSeries3->clear();
    handleButtonColorChange(magneto);
    handleChartLabelText(magneto);
    gyroButton->setDown(false);
    accButton->setDown(false);
    magnetoButton->setDown(true);
    altiButton->setDown(false);
    chart->axisY()->setRange(-90, 90);
    /* Ask M4 to send Magnetometer data */
    emit m4_sendCommand("mag");
}

void LpdGraphs::handleAltiButton()
{
    index_graph = alti;
    graphSeries1->clear();
    graphSeries2->clear();
    graphSeries3->clear();
    gyroButton->setDown(false);
    accButton->setDown(false);
    magnetoButton->setDown(false);
    altiButton->setDown(true);
    chart->axisY()->setRange(0, 1000);
    /* Ask M4 to send Altimeter data */
    emit m4_sendCommand("alti");
}

void LpdGraphs::handleSleepButton()
{
    if (sleepFileActive) {
        QByteArray command("mem");
        sleepFile.write(command);
        sleepFile.flush();
    }
}

void LpdGraphs::handleShutdownButton()
{
    QString programName("poweroff");
    QProcess::execute(programName);
}

void LpdGraphs::openSleepFile()
{
    sleepFileActive = false;
    if (sleepFile.isOpen())
        sleepFile.close();

    sleepFile.setFileName("/sys/power/state");
    if (!sleepFile.open(QIODevice::WriteOnly))
        qDebug().nospace() << "Failed to open /sys/power/state" << endl;
    else
        sleepFileActive = true;
}
