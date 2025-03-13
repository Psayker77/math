#include "taskform.h"
#include "ui_taskform.h"

TaskForm::TaskForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskForm)
{
    ui->setupUi(this);
    ui->taskText->setWordWrap(true);
}

void TaskForm::initTask(QString taskText, int taskId) {
    this->taskId = taskId;
    this->ui->taskText->setText(taskText);
}



TaskForm::~TaskForm()
{
    delete ui;
}

void TaskForm::on_TryButton_clicked()
{
    QString userAnswer = this->ui->AnswerLineEdit->text();
    bool status = checkTask(this->taskId, userAnswer);
    if (status) this->ui->statusLabel->setText("Правильный ответ");
        else this->ui->statusLabel->setText("Неправильный ответ");
}


void TaskForm::on_LoseButton_clicked()
{
    this->close();
}

