#ifndef LPDGRAPHS_H
#define LPDGRAPHS_H

#include <QtWidgets>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QTimer>
#include <QtCore/QTextStream>

QT_CHARTS_USE_NAMESPACE

class LpdGraphs : public QChart
{
    Q_OBJECT
public:
    LpdGraphs(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~LpdGraphs();

public slots:
    void m4_processData(const QString& arg);
    void handleGyroButton();
    void handleAccButton();
    void handleMagnetoButton();
    void handleAltiButton();
    void handleSleepButton();
    void handleShutdownButton();

signals:
    void m4_sendCommand(const QByteArray& arg);

private:
    enum graph_index {gyro, accl, magneto, alti};
    volatile quint8 index_graph;

    QFont buttonFont;
    QFont chartLabelFont;
    QWidget *window;
    QChart *chart;
    QSplineSeries *graphSeries1;
    QSplineSeries *graphSeries2;
    QSplineSeries *graphSeries3;
    QValueAxis *axisX;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPixmapItem *item;

    QPushButton *gyroButton;
    QPushButton *accButton;
    QPushButton *magnetoButton;
    QPushButton *altiButton;
    QPushButton *sleepButton;
    QPushButton *shutdownButton;
    QGroupBox *hgroupBox;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QChartView *chartView;

    QGroupBox *hgroupChartBox;
    QHBoxLayout *hboxChartLayout;
    QGroupBox *vgroupChartBox;
    QVBoxLayout *vboxChartLayout;
    QTextEdit *chartLabel;

    const uint8_t X_RANGE_COUNT = 50;
    const uint8_t X_RANGE_MAX = X_RANGE_COUNT - 1;
    qreal m_x;

    void setupLogo();
    void setupButtons();
    void setupHBoxLayout();
    void setupCharts();
    void setupVBoxLayout();
    void setupMainWindow();
    void initialiseView();
    void handleButtonColorChange(int);
    void handleChartLabelText(int);

    QFile sleepFile;
    bool sleepFileActive;
    void openSleepFile();
};

#endif // LPDGRAPHS_H
