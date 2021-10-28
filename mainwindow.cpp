#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <QtWidgets/QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);

    m_client = new QMqttClient(this);
    m_client->setHostname("192.168.147.1");
    m_client->setPort(1883);
    m_client->setProtocolVersion(QMqttClient::MQTT_3_1);

    connect(m_client, &QMqttClient::disconnected, this, &MainWindow::mqttDisconnected);
    connect(m_client, &QMqttClient::stateChanged, this, &MainWindow::mqttStateChange);
    connect(m_client, &QMqttClient::messageReceived, this, &MainWindow::mqttMessageReceived);

    mqttConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mqttConnect()
{
    if (m_client->state() == QMqttClient::Disconnected) {
        m_client->connectToHost();
    }
}

void MainWindow::mqttStateChange()
{
    if (m_client->state() == QMqttClient::Connected) {
        QMqttTopicFilter *topic1 = new QMqttTopicFilter(groundTxTopic);
        m_client->subscribe(*topic1);
        QMqttTopicFilter *topic2 = new QMqttTopicFilter(groundRxTopic);
        m_client->subscribe(*topic2);
    }
}

void MainWindow::mqttDisconnected()
{
    mqttConnect();
}

void MainWindow::mqttMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    auto msgDoc = QJsonDocument::fromJson(message);
    if (!msgDoc.isNull() && !msgDoc.isEmpty()) {
        auto msgObj = msgDoc.object();
        if (msgDoc.isObject()) {
          auto msgMap = msgObj.toVariantMap();
          if (topic.name() == groundTxTopic) {
              txPan = msgMap["pan"].toFloat() * 1000;
              txTilt = msgMap["tilt"].toFloat() * 1000;
              txBlkBtn = msgMap["blk"].toBool();
              txRedBtn = msgMap["red"].toBool();
              txStatus = msgMap["status"].toString();
              txRSSI = msgMap["TX_RSSI"].toString();
              txMsg = msgMap["msg"].toString();
          } else if (topic.name() == groundRxTopic) {
              rxStatus = msgMap["status"].toString();
              rxRSSI = msgMap["RX_RSSI"].toString();
              rxI1 = msgMap["I1"].toString();
              rxI2 = msgMap["I2"].toString();
              rxTC = msgMap["TC"].toString();
          }
          ui->labelStatus->setText("T:" + txStatus + ", R:" + rxStatus);
          ui->labelRSSI->setText("T:" + txRSSI + ", R:" + rxRSSI);
          ui->panSlider->setValue(txPan);
          ui->tiltSlider->setValue(txTilt);
          ui->blkCheckBox->setChecked(txBlkBtn);
          ui->redCheckBox->setChecked(txRedBtn);
          ui->labelMsg->setText(txMsg);
          ui->labelI1->setText(rxI1);
          ui->labelI2->setText(rxI2);
          ui->labelTC->setText(rxTC);
        }
    }
}

