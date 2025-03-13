#include "network.h"
#include "Qcoreapplication.h"
#include "QMessageBox"

Network::Network()
{
    this->initTCP();
}

Network::~Network()
{
    this->tcpInstance->close();
}

void Network::initTCP() {
    this->tcpInstance = new QTcpSocket(this);
    this->tcpInstance->connectToHost("127.0.0.1", 33333);
    if (this->tcpInstance->waitForConnected(1000)) {
        qDebug() << "Connected!";
        tcpInstance->waitForReadyRead(1000);
        this->tcpInstance->readAll();
    }
    else {
        qDebug() << "no Connected!";
    }
}


QJsonObject Network::doRequest(QVariantMap request) {
    qDebug() << QJsonDocument::fromVariant(request).toJson();
    this->tcpInstance->write(QJsonDocument::fromVariant(request).toJson());
    this->tcpInstance->waitForBytesWritten(1000);
    this->tcpInstance->waitForReadyRead(2000);
    QByteArray response = this->tcpInstance->readAll();
    qDebug() << QJsonDocument::fromJson(response);
    return QJsonDocument::fromJson(response).object();
}

Network *Network::pInstance = nullptr;
QTcpSocket *Network::tcpInstance;
NetworkDestroyer Network::destroyer;
