#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class Database;

class DatabaseDestroyer
{
    private:
        Database * pInstance;
    public:
        ~DatabaseDestroyer() {
            delete this->pInstance;
        }
        void initialize(Database *p) {
            this->pInstance = p;
        }
};

/*!
 * \brief Синглтон-класс для работы с базой данных
 */
class Database {
    private:
        static Database * pInstance;
        /*!
         * \brief Функция открытия базы данных
         */
        void initDB();
        static QSqlDatabase dbInstance;
        static DatabaseDestroyer destroyer;
    protected:
        /*!
         * \brief Конструктор класса
         */
        Database();
        Database(const Database&) = delete;
        Database& operator = (Database &)=delete;
        /*!
         * brief Деструктор класса
        */
        ~Database();
        friend class DatabaseDestroyer;
    public:
        static Database* getInstance() {
            if (!pInstance) {
                pInstance = new Database();
                destroyer.initialize(pInstance);
            }
            return pInstance;
        }
        /*!
         * \brief Функция осуществления SQL-запроса
         * \param[in] stringQuery SQL-запрос строкой
         * \return Объект типа QSqlQuery
         */
        QSqlQuery doSQLQuery(QString stringQuery);
};



#endif // DATABASE_H
