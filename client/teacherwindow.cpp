#include "teacherwindow.h"
#include "ui_teacherwindow.h"

TeacherWindow::TeacherWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::TeacherWindow) {
    QStringList header;
    header << QString("Логин")
           << QString("Задание 1")
           << QString("Задание 2")
           << QString("Задание 3")
           << QString("Задание 4");
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setHorizontalHeaderLabels(header);
    this->UpdateWindow();
}

void TeacherWindow::UpdateWindow() {
    QJsonArray results = getAllStat();
    int counter = 0;
    for (auto user: results) {
        ui->tableWidget->insertRow(counter);
        ui->tableWidget->setItem(counter, 0, new QTableWidgetItem(user.toObject()["login"].toString()));
        QJsonObject tasks = user.toObject()["solved_tasks"].toObject();
        qDebug() << tasks;
        for (int i = 1; i < 5; i++) {
            //qDebug() << QString::number(i);
            //qDebug() << tasks[QString::number(i)].toInt();
            QString taskRes = QString::number(tasks[QString::number(i)].toInt());
            qDebug() << taskRes;
            ui->tableWidget->setItem(counter, i, new QTableWidgetItem(taskRes));

        }
        counter += 1;

    }
    //qDebug() << results;
    ui->tableWidget->resizeColumnsToContents();
    return;
}

TeacherWindow::~TeacherWindow()
{
    delete ui;
}


void TeacherWindow::on_action_triggered()
{
    reset_settings();
    // перезапуск приложения
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

