#ifndef serverfunction_h
#define serverfunction_h

#include <QCoreApplication>
#include <QString>
#include <stdio.h>
#include "database.h"
#include "tasks.h"
#include <QDebug>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QCryptographicHash>

/*
parsing — функция, которая обрабатывает сообщения клиента: выделяет команду и аргументы, а затем вызывает соответствующую функцию
Аргумент data — сообщение, пришедшее от клиента
Возвращаемое значение — ответ соответствующей функции
*/
QByteArray parse(QByteArray request);
QByteArray auth(QJsonObject request);
QByteArray reg(QJsonObject request);
QByteArray getAllStat(QJsonObject request);
QByteArray getUserStat(QJsonObject request);
QByteArray getTask(QJsonObject request);
QByteArray checkTask(QJsonObject request);

bool isTeacher(QString userToken);
#endif
