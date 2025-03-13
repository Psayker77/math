#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

class Network;


class NetworkDestroyer
{
    private:
        Network* pInstance;
    public:
        ~NetworkDestroyer() {
            delete this->pInstance;
        }
        void initialize(Network * p) {
            this->pInstance = p;
        }
};

/*!
 * \brief Синглтон-класс для работы с сетью
 */
class Network: public QObject {
    private:
        static Network* pInstance;
        void initTCP();
        static QTcpSocket* tcpInstance;
        static NetworkDestroyer destroyer;
    protected:
        /*!
         * \brief Конструктор класса, фукнция инициализации TCP-соединения.
         */
        Network();
        Network(const Network&) = delete;
        Network& operator = (Network &) = delete;
        /*!
         * \brief Деструктор класса Network
         */
        ~Network();
        friend class NetworkDestroyer;
    public:
        static Network* getInstance() {
            if (!pInstance) {
                pInstance = new Network();
                destroyer.initialize(pInstance);
            }
            return pInstance;
        }
        /*!
         * \brief Фукнция выполнения JSON-запроса
         * \param[in] Передаваемые данные
         * \return Ответ сервера в JSON-формате
         */
        QJsonObject doRequest(QVariantMap request);

};

#endif // NETWORK_H
