#ifndef TASKFORM_H
#define TASKFORM_H

#include <QWidget>
#include <clientfunctions.h>

namespace Ui {
class TaskForm;
}

class TaskForm : public QWidget
{
    Q_OBJECT

public:
    explicit TaskForm(QWidget *parent = nullptr);
    ~TaskForm();
    /*!
     * \brief Функция инциализации окна
     * \param[in] taskText Текст задания
     * \param[in] taskId Серверный ID задания
     */
    void initTask(QString taskText, int taskId);

private slots:
    /*!
     * \brief Функция обработки кнопку сдачи
     */
    void on_TryButton_clicked();
    /*!
     * \brief Функция закрытия окна
     */
    void on_LoseButton_clicked();

signals:
    void openWindow();

private:
    Ui::TaskForm *ui;
    int taskId;
};

#endif // TASKFORM_H
