#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <clientfunctions.h>
#include <QApplication>
#include <QProcess>
#include <taskform.h>

namespace Ui {
class MainWindow;
}

/*!
 * \brief Класс окна учащегося
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор окна
     * \param parent
     */
    explicit MainWindow(QWidget *parent = nullptr);
    /*!
     * \brief Деструктор окна
     */
    ~MainWindow();
    /*!
     * \brief Функция обновления статистики
     * \return
     */
    bool update_data();

signals:
    void openWindow();

private slots:
    void on_task1_clicked();
    void on_task2_clicked();
    void on_task3_clicked();
    void on_task4_clicked();
    void on_account_exit_triggered();
    /*!
     * \brief Функция, которая создаёт окно TaskForm
     * \param[in] task Структура задания
     */
    void openTask(QVariantMap task);

private:
    Ui::MainWindow *ui;
    TaskForm *tWindow;
};

#endif // MAINWINDOW_H
