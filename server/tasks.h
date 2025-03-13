#pragma once
#ifndef TASKS_H
#define TASKS_H
#include <QVariant>
#include <QRandomGenerator>
#include <QtMath>
#include <QVector>
#include <QQueue>
#include <QPair>
#include <QMap>
#include <limits>

/*!
 * \brief Класс заданий
*/

/*!
 * \brief Функция, генерирующее задание на тему построения системы представителей двудольного графа
 * \return Структуру задания, содержащую поля text и answer
 */
QVariantMap Task1();
/*!
 * \brief Функция, генерирующее задание на тему проверки двудольного графа
 * \return Структуру задания, содержащую поля text и answer
 */
QVariantMap Task2();
/*!
 * \brief Функция, генерирующее задание на тему построения гамильтонова графа
 * \return Структуру задания, содержащую поля text и answer
 */
QVariantMap Task3();
/*!
 * \brief Функция, генерирующее задание на хер знает какую тему
 * \return Структуру задания, содержащую поля text и answer
 */
QVariantMap Task4();

typedef QPair<int, int> iPair;
bool hamiltonianCycleCheck(const QString& graphString, const QString& userPathString);
bool isValidSRP(QString taskText, QString answer);

#endif // TASKS_H
