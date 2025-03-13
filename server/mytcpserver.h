#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QByteArray>
#include <QDebug>
#include "serverfunction.h"

/*!
 * \brief Класс сервера
 */
class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

public slots:
    /*!
     * \brief Слот нового соединения
     */
    void slotNewConnection();
    /*!
     * \brief Слот, обрабатывающий отключение клиента
     */
    void slotClientDisconnected();
    /*!
     * \brief Слот, обрабатывающий приходящие запросы
     */
    void slotServerRead();

private:
    QTcpServer * mTcpServer;
    std::list<QTcpSocket*> TcpSocketList;
    int server_status;
};
#endif // MYTCPSERVER_H

