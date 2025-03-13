#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->update_data();
    tWindow = new TaskForm();
    connect(tWindow, &TaskForm::openWindow, this, &TaskForm::show);
}

bool MainWindow::update_data() {
    ui->StatLabel->setText(getStat());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openTask(QVariantMap task) {
    int taskId = task["id"].toInt();
    QString taskText = task["text"].toString();
    if (tWindow != nullptr) {
        delete tWindow;
    }
    tWindow = new TaskForm();
    tWindow->initTask(taskText, taskId);
    tWindow->show();
}


void MainWindow::on_task1_clicked()
{
    QVariantMap task = getTask(1);
    openTask(task);
}


void MainWindow::on_task2_clicked()
{
    QVariantMap task = getTask(2);
    openTask(task);
}


void MainWindow::on_task3_clicked()
{
    QVariantMap task = getTask(3);
    openTask(task);
}


void MainWindow::on_task4_clicked()
{
    QVariantMap task = getTask(4);
    openTask(task);
}


void MainWindow::on_account_exit_triggered() {
    reset_settings();
    // перезапуск приложения
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}


