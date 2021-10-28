#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMqttClient>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void mqttDisconnected();
    void mqttConnect();
    void mqttStateChange();
    void mqttMessageReceived(const QByteArray &message, const QMqttTopicName &topic);

private:
    Ui::MainWindow *ui;
    QMqttClient *m_client;

    const QString groundTxTopic = "ground_tx";
    const QString groundRxTopic = "ground_rx";

    // TX PACKET
    QString txStatus;
    QString txRSSI;
    float   txPan;
    float   txTilt;
    bool    txBlkBtn;
    bool    txRedBtn;
    QString txMsg;
    // RX PACKET
    QString rxStatus;
    QString rxRSSI;
    QString rxI1;
    QString rxI2;
    QString rxTC;
};
#endif // MAINWINDOW_H
