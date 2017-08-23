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
    pmButton = new QPushButton(tr("Power Managment"));
    sleepButton = new QPushButton();
    shutdownButton = new QPushButton();
    connect(gyroButton, SIGNAL(released()), this, SLOT(handleGyroButton()));
    connect(accButton, SIGNAL(released()), this, SLOT(handleAccButton()));
    connect(magnetoButton, SIGNAL(released()), this, SLOT(handleMagnetoButton()));
    connect(pmButton, SIGNAL(released()), this, SLOT(handlePmButton()));
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
    pmButton->setFont(buttonFont);
    sleepButton->setFont(buttonFont);
    shutdownButton->setFont(buttonFont);

    gyroButton->setFixedHeight(40);
    accButton->setFixedHeight(40);
    magnetoButton->setFixedHeight(40);
    pmButton->setFixedHeight(40);

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

    pmButton->setAutoFillBackground(true);
    bPalette = pmButton->palette();
    bPalette.setColor(QPalette::Button, QRgb(0x00508c));
    bPalette.setColor(QPalette::ButtonText, Qt::white);
    pmButton->setPalette(bPalette);

    gyroButton->setContentsMargins(0,0,0,0);
    accButton->setContentsMargins(0,0,0,0);
    magnetoButton->setContentsMargins(0,0,0,0);
    pmButton->setContentsMargins(0,0,0,0);
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
    hboxLayout->addWidget(gyroButton);
    hboxLayout->addWidget(magnetoButton);
    hboxLayout->addWidget(pmButton);
    hgroupBox->setLayout(hboxLayout);
}

void LpdGraphs::setupCharts()
{
    chartAcc = new QChart(this);
    chartAcc->legend()->setVisible(true);
    chartAcc->legend()->setAlignment(Qt::AlignBottom);

    chartMag = new QChart(this);
    chartMag->legend()->setVisible(true);
    chartMag->legend()->setAlignment(Qt::AlignBottom);

    chartGyr = new QChart(this);
    chartGyr->legend()->setVisible(true);
    chartGyr->legend()->setAlignment(Qt::AlignBottom);

    chartPm = new QChart(this);
    chartPm->legend()->setVisible(true);
    chartPm->legend()->setAlignment(Qt::AlignBottom);

    graphSeriesAcc[0] = new QSplineSeries(chartAcc);
    graphSeriesAcc[1] = new QSplineSeries(chartAcc);
    graphSeriesAcc[2] = new QSplineSeries(chartAcc);

    graphSeriesMag[0] = new QSplineSeries(chartMag);
    graphSeriesMag[1] = new QSplineSeries(chartMag);
    graphSeriesMag[2] = new QSplineSeries(chartMag);

    graphSeriesGyr[0] = new QSplineSeries(chartGyr);
    graphSeriesGyr[1] = new QSplineSeries(chartGyr);
    graphSeriesGyr[2] = new QSplineSeries(chartGyr);

    graphSeriesPm[0] = new QSplineSeries(chartPm);
    graphSeriesPm[1] = new QSplineSeries(chartPm);
    graphSeriesPm[2] = new QSplineSeries(chartPm);

    chartAcc->addSeries(graphSeriesAcc[0]);
    chartAcc->addSeries(graphSeriesAcc[1]);
    chartAcc->addSeries(graphSeriesAcc[2]);

    chartMag->addSeries(graphSeriesMag[0]);
    chartMag->addSeries(graphSeriesMag[1]);
    chartMag->addSeries(graphSeriesMag[2]);

    chartGyr->addSeries(graphSeriesGyr[0]);
    chartGyr->addSeries(graphSeriesGyr[1]);
    chartGyr->addSeries(graphSeriesGyr[2]);

    chartPm->addSeries(graphSeriesPm[0]);
    chartPm->addSeries(graphSeriesPm[1]);
    chartPm->addSeries(graphSeriesPm[2]);

    axisXAcc = new QValueAxis;
    chartAcc->createDefaultAxes();
    chartAcc->setAxisX(axisXAcc, graphSeriesAcc[0]);
    chartAcc->setAxisX(axisXAcc, graphSeriesAcc[1]);
    chartAcc->setAxisX(axisXAcc, graphSeriesAcc[2]);
    axisXAcc->setTickCount(X_RANGE_MAX);
    chartAcc->axisX()->setRange(0, X_RANGE_MAX);
    chartAcc->axisX()->hide();
    chartAcc->setContentsMargins(0,0,0,0);
    chartAcc->legend()->hide();

    chartAccView = new QChartView(chartAcc);
    chartAccView->setRenderHint(QPainter::Antialiasing);
    chartAccView->setContentsMargins(0,0,0,0);

    axisXMag = new QValueAxis;
    chartMag->createDefaultAxes();
    chartMag->setAxisX(axisXMag, graphSeriesMag[0]);
    chartMag->setAxisX(axisXMag, graphSeriesMag[1]);
    chartMag->setAxisX(axisXMag, graphSeriesMag[2]);
    axisXMag->setTickCount(X_RANGE_MAX);
    chartMag->axisX()->setRange(0, X_RANGE_MAX);
    chartMag->axisX()->hide();
    chartMag->setContentsMargins(0,0,0,0);
    chartMag->legend()->hide();

    chartMagView = new QChartView(chartMag);
    chartMagView->setRenderHint(QPainter::Antialiasing);
    chartMagView->setContentsMargins(0,0,0,0);

    axisXGyr = new QValueAxis;
    chartGyr->createDefaultAxes();
    chartGyr->setAxisX(axisXGyr, graphSeriesGyr[0]);
    chartGyr->setAxisX(axisXGyr, graphSeriesGyr[1]);
    chartGyr->setAxisX(axisXGyr, graphSeriesGyr[2]);
    axisXGyr->setTickCount(X_RANGE_MAX);
    chartGyr->axisX()->setRange(0, X_RANGE_MAX);
    chartGyr->axisX()->hide();
    chartGyr->setContentsMargins(0,0,0,0);
    chartGyr->legend()->hide();

    chartGyrView = new QChartView(chartGyr);
    chartGyrView->setRenderHint(QPainter::Antialiasing);
    chartGyrView->setContentsMargins(0,0,0,0);

    axisXPm = new QValueAxis;
    chartPm->createDefaultAxes();
    chartPm->setAxisX(axisXPm, graphSeriesPm[0]);
    chartPm->setAxisX(axisXPm, graphSeriesPm[1]);
    chartPm->setAxisX(axisXPm, graphSeriesPm[2]);
    axisXPm->setTickCount(X_RANGE_MAX);
    chartPm->axisX()->setRange(0, X_RANGE_MAX);
    chartPm->axisX()->hide();
    chartPm->setContentsMargins(0,0,0,0);
    chartPm->legend()->hide();

    chartPmView = new QChartView(chartPm);
    chartPmView->setRenderHint(QPainter::Antialiasing);
    chartPmView->setContentsMargins(0,0,0,0);

    stackedWidget = new QStackedWidget;
    hgroupChartBox = new QGroupBox;
    hboxChartLayout = new QHBoxLayout;
    vgroupChartBox = new QGroupBox;
    vboxChartLayout = new QVBoxLayout;

    stackedWidget->insertWidget(accl, chartAccView);
    stackedWidget->insertWidget(magneto, chartMagView);
    stackedWidget->insertWidget(gyro, chartGyrView);
    stackedWidget->insertWidget(power, chartPmView);

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
    hboxChartLayout->addWidget(stackedWidget);
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
    chartAcc->setTheme(QChart::ChartThemeDark);
    chartMag->setTheme(QChart::ChartThemeDark);
    chartGyr->setTheme(QChart::ChartThemeDark);
    chartPm->setTheme(QChart::ChartThemeDark);
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

    QMetaObject::invokeMethod(pmButton, "released");
    m_x = 0;
}

LpdGraphs::~LpdGraphs()
{
    emit m4_sendCommand("close");
    if (sleepFile.isOpen())
        sleepFile.close();
}

void LpdGraphs::m4_processData(const QByteArray &str)
{
    qreal acc[3], mag[3], gyr[3], power, current;
    int i = 0;
    //  qInfo("received %d bytes 7 %X 8 %X", str.count(), str.at(7), str.at(8));
    while(str.count() >= (24 + 24*i) && (unsigned char)str.at(0+24*i) == 0xFF){
        power = ((unsigned char)str.at(20 + 24*i) + ((unsigned char)str.at(21 + 24*i) << 8)) >> 3;
        current = ((unsigned char)str.at(22 + 24*i) + ((unsigned char)str.at(23 + 24*i) << 8)) * 0.0001;
        gyr[0] = ((unsigned char)str.at(1 + 24*i) + ((unsigned char)str.at(2 + 24*i) << 8));
        gyr[0] = (gyr[0] > 32767) ? (gyr[0] - 0x10000):gyr[0];
        gyr[0] *= 0.0625;
        gyr[1] = ((unsigned char)str.at(3 + 24*i) + ((unsigned char)str.at(4 + 24*i) << 8));
        gyr[1] = (gyr[1] > 32767) ? (gyr[1] - 0x10000):gyr[1];
        gyr[1] *= 0.0625;
        gyr[2] = ((unsigned char)str.at(5 + 24*i) + ((unsigned char)str.at(6 + 24*i) << 8));
        gyr[2] = (gyr[2] > 32767) ? (gyr[2] - 0x10000):gyr[2];
        gyr[2] *= 0.0625;

        acc[0] = ((unsigned char)str.at(7 + 24*i) + ((unsigned char)str.at(8 + 24*i) << 8) );
        acc[0] = (acc[0] > 32767) ? (acc[0] - 0x10000):acc[0];
        acc[0] /= 8192.0;
        acc[1] = ((unsigned char)str.at(9 + 24*i) + ((unsigned char)str.at(10 + 24*i) << 8));
        acc[1] = (acc[1] > 32767) ? (acc[1] - 0x10000):acc[1];
        acc[1] /= 8192.0;
        acc[2] = ((unsigned char)str.at(11 + 24*i) + ((unsigned char)str.at(12 + 24*i) << 8));
        acc[2] = (acc[2] > 32767) ? (acc[2] - 0x10000):acc[2];
        acc[2] /= 8192.0;

        mag[0] = ((unsigned char)str.at(13 + 24*i) + ((unsigned char)str.at(14 + 24*i) << 8));
        mag[0] = (mag[0] > 32767) ? (mag[0] - 0x10000):mag[0];
        mag[0] *= 0.1;
        mag[1] = ((unsigned char)str.at(15 + 24*i) + ((unsigned char)str.at(16 + 24*i) << 8));
        mag[1] = (mag[1] > 32767) ? (mag[1] - 0x10000):mag[1];
        mag[1] *= 0.1;
        mag[2] = ((unsigned char)str.at(17 + 24*i) + ((unsigned char)str.at(18 + 24*i) << 8));
        mag[2] = (mag[2] > 32767) ? (mag[2] - 0x10000):mag[2];
        mag[2] *= 0.1;

        qreal y = (axisXAcc->max() - axisXAcc->min()) / axisXAcc->tickCount();
        m_x += y;
        graphSeriesGyr[0]->append(m_x, gyr[0]);
        graphSeriesGyr[1]->append(m_x, gyr[1]);
        graphSeriesGyr[2]->append(m_x, gyr[2]);

        graphSeriesAcc[0]->append(m_x, acc[0]);
        graphSeriesAcc[1]->append(m_x, acc[1]);
        graphSeriesAcc[2]->append(m_x, acc[2]);

        graphSeriesMag[0]->append(m_x, mag[0]);
        graphSeriesMag[1]->append(m_x, mag[1]);
        graphSeriesMag[2]->append(m_x, mag[2]);

        graphSeriesPm[0]->append(m_x, power);

        if (m_x > axisXAcc->max()) {
            axisXAcc->setMax(m_x);
            axisXAcc->setMin(m_x - X_RANGE_COUNT);

            axisXGyr->setMax(m_x);
            axisXGyr->setMin(m_x - X_RANGE_COUNT);

            axisXMag->setMax(m_x);
            axisXMag->setMin(m_x - X_RANGE_COUNT);

            axisXPm->setMax(m_x);
            axisXPm->setMin(m_x - X_RANGE_COUNT);
        }

        if (graphSeriesGyr[0]->count() > X_RANGE_COUNT) {
            graphSeriesGyr[0]->remove(0);
            graphSeriesGyr[1]->remove(0);
            graphSeriesGyr[2]->remove(0);
        }

        if (graphSeriesAcc[0]->count() > X_RANGE_COUNT) {
            graphSeriesAcc[0]->remove(0);
            graphSeriesAcc[1]->remove(0);
            graphSeriesAcc[2]->remove(0);
        }

        if (graphSeriesMag[0]->count() > X_RANGE_COUNT) {
            graphSeriesMag[0]->remove(0);
            graphSeriesMag[1]->remove(0);
            graphSeriesMag[2]->remove(0);
        }

        if (graphSeriesPm[0]->count() > X_RANGE_COUNT) {
            graphSeriesPm[0]->remove(0);
        }
        i++;
    }
#if 0
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
        case power:
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
        if (index_graph != power) {
            val3 = list.at(i + 3).toFloat(&okz);
            if (okz == false)
                return;
        }

        //emit server_sendData("");
        //qDebug().nospace() << str << "|" << val1 << " " << val2 << " " << val3;

        qreal y = (axisXAcc->max() - axisXAcc->min()) / axisXAcc->tickCount();
        m_x += y;
        switch (index_graph) {
        case gyro:
            graphSeriesGyr[0]->append(m_x, val1);
            graphSeriesGyr[1]->append(m_x, val2);
            graphSeriesGyr[2]->append(m_x, val3);
            break;
        case accl:
            graphSeriesAcc[0]->append(m_x, val1);
            graphSeriesAcc[1]->append(m_x, val2);
            graphSeriesAcc[2]->append(m_x, val3);
            break;
        case magneto:
            graphSeriesMag[0]->append(m_x, val1);
            graphSeriesMag[1]->append(m_x, val2);
            graphSeriesMag[2]->append(m_x, val3);
            break;
        default:
            break;
        }

        if (m_x > axisXAcc->max()) {
            axisXAcc->setMax(m_x);
            axisXAcc->setMin(m_x - X_RANGE_COUNT);

            axisXGyr->setMax(m_x);
            axisXGyr->setMin(m_x - X_RANGE_COUNT);

            axisXMag->setMax(m_x);
            axisXMag->setMin(m_x - X_RANGE_COUNT);

            axisXPm->setMax(m_x);
            axisXPm->setMin(m_x - X_RANGE_COUNT);
        }

        if (graphSeriesGyr[0]->count() > X_RANGE_COUNT) {
            graphSeriesGyr[0]->remove(0);
            graphSeriesGyr[1]->remove(0);
            graphSeriesGyr[2]->remove(0);
        }

        if (graphSeriesAcc[0]->count() > X_RANGE_COUNT) {
            graphSeriesAcc[0]->remove(0);
            graphSeriesAcc[1]->remove(0);
            graphSeriesAcc[2]->remove(0);
        }

        if (graphSeriesMag[0]->count() > X_RANGE_COUNT) {
            graphSeriesMag[0]->remove(0);
            graphSeriesMag[1]->remove(0);
            graphSeriesMag[2]->remove(0);
        }

        if (graphSeriesPm[0]->count() > X_RANGE_COUNT) {
            graphSeriesPm[0]->remove(0);
            graphSeriesPm[1]->remove(0);
            graphSeriesPm[2]->remove(0);
        }
    }
#endif
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
        bPalette = pmButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        pmButton->setPalette(bPalette);
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
        bPalette = pmButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        pmButton->setPalette(bPalette);
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
        bPalette = pmButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        pmButton->setPalette(bPalette);
        break;
    case power:
        bPalette = accButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        accButton->setPalette(bPalette);
        bPalette = gyroButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        gyroButton->setPalette(bPalette);
        bPalette = magnetoButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x00508c));
        magnetoButton->setPalette(bPalette);
        bPalette = pmButton->palette();
        bPalette.setColor(QPalette::Button, QRgb(0x004664));
        pmButton->setPalette(bPalette);
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
        chartLabel->setTextColor(graphSeriesGyr[0]->pen().color());
        chartLabel->setText("\n   X °/s");
        chartLabel->setTextColor(graphSeriesGyr[1]->pen().color());
        chartLabel->append("\n   Y °/s");
        chartLabel->setTextColor(graphSeriesGyr[2]->pen().color());
        chartLabel->append("\n   Z °/s");
        break;
    case magneto:
        chartLabel->clear();
        chartLabel->setTextColor(graphSeriesMag[0]->pen().color());
        chartLabel->setText("\n     X  °");
        chartLabel->setTextColor(graphSeriesMag[1]->pen().color());
        chartLabel->append("\n     Y  °");
        chartLabel->setTextColor(graphSeriesMag[2]->pen().color());
        chartLabel->append("\n     Z  °");
        break;
    case power:
        chartLabel->clear();
        chartLabel->setTextColor(graphSeriesMag[0]->pen().color());
        chartLabel->setText("\n     mW  ");
        /*   chartLabel->setTextColor(graphSeriesMag[1]->pen().color());
        chartLabel->append("\n      mAh");
        chartLabel->setTextColor(graphSeriesMag[2]->pen().color());
        chartLabel->append("\n      A  ");*/
        break;
    default:
        break;
    }
    chartLabel->setAlignment(Qt::AlignHCenter);
}

void LpdGraphs::handleGyroButton()
{
    index_graph = gyro;
    handleButtonColorChange(gyro);
    handleChartLabelText(gyro);
    gyroButton->setDown(true);
    accButton->setDown(false);
    magnetoButton->setDown(false);
    pmButton->setDown(false);
    chartGyr->axisY()->setRange(-180, 180);
    stackedWidget->setCurrentIndex(index_graph);
    /* Ask M4 to send Gyroscope data */
    emit m4_sendCommand("gyro");
}

void LpdGraphs::handleAccButton()
{
    index_graph = accl;
    handleButtonColorChange(accl);
    handleChartLabelText(accl);
    gyroButton->setDown(false);
    accButton->setDown(true);
    magnetoButton->setDown(false);
    pmButton->setDown(false);
    chartAcc->axisY()->setRange(-4, 4);
    stackedWidget->setCurrentIndex(index_graph);
    /* Ask M4 to send Accelerometer data */
    emit m4_sendCommand("acc");
}

void LpdGraphs::handleMagnetoButton()
{
    index_graph = magneto;
    handleButtonColorChange(magneto);
    handleChartLabelText(magneto);
    gyroButton->setDown(false);
    accButton->setDown(false);
    magnetoButton->setDown(true);
    pmButton->setDown(false);
    chartMag->axisY()->setRange(-50, 50);
    stackedWidget->setCurrentIndex(index_graph);
    /* Ask M4 to send Magnetometer data */
    emit m4_sendCommand("mag");
}

void LpdGraphs::handlePmButton()
{
    index_graph = power;
    handleButtonColorChange(power);
    handleChartLabelText(power);
    gyroButton->setDown(false);
    accButton->setDown(false);
    magnetoButton->setDown(false);
    pmButton->setDown(true);
    chartPm->axisY()->setRange(0, 600);
    stackedWidget->setCurrentIndex(index_graph);
    /* Ask M4 to send Altimeter data */
    emit m4_sendCommand("alti");
}

void LpdGraphs::handleSleepButton()
{
    openSleepFile();
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
