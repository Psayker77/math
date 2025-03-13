#include "AuthWindow.h"
#include "ui_AuthWindow.h"

AuthWindow::AuthWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::AuthWindow) {
    mWindow = new MainWindow();
    connect(mWindow, &MainWindow::openWindow, this, &MainWindow::show);
    ui->setupUi(this);
    ui->passwordLabel_2->setVisible(false);
    ui->passwordTextEdit_2->setVisible(false);
}

AuthWindow::~AuthWindow() {
    delete ui;
}


void AuthWindow::on_pushButton_clicked() {
    QString loginString = ui->LoginTextEdit->text();
    QString passwordString = ui->PasswordTextEdit->text();
    // если мы в режиме регистрации
    if (ui->passwordLabel_2->isVisible()) {
        QString passwordVerifyString = ui->passwordTextEdit_2->text();
        if (passwordString != passwordVerifyString) {
            QMessageBox::critical(this, "Ошибка", "Пароли не совпадают");
            return;
        }
        bool status = register_account(loginString, passwordString);
        if (!status) {
            QMessageBox::information(this, "Регистрация", "Регистрация не удалась. Думай. Все вопросы к Набебину.");
            return;
        }
        else {
            QMessageBox::information(this, "Регистрация", "Вы успешно зарегистрировались!");
        }
    } else {
        bool status = login(loginString, passwordString);
        qDebug() << status;
        if (!status) {
            QMessageBox::information(this, "Авторизация", "Введён неправильный логин или пароль");
            return;
        }
        else {
            QMessageBox::information(this, "Авторизация", "Вы успешно авторизовались!");
        }
    }
    if (load_setting("isTeacher") == "true") {
        TeacherWindow teacherw;
        teacherw.show();
    }
    else {
        mWindow->update_data();
        mWindow->show();
    }
    this->close();
}

void AuthWindow::on_ChangeModeButton_clicked() {
    // если мы в режиме регистрации
    if (not ui->passwordLabel_2->isVisible()) {
        ui->passwordLabel_2->setVisible(true);
        ui->passwordTextEdit_2->setVisible(true);
        ui->ChangeModeButton->setText("Войти");
        ui->ChangeModeLabel->setText("Уже есть аккаунт?");
    }
    // если в режиме авторизации
    else {
        ui->passwordLabel_2->setVisible(false);
        ui->passwordTextEdit_2->setVisible(false);
        ui->ChangeModeButton->setText("Зарегистрироваться");
        ui->ChangeModeLabel->setText("Нет аккаунта?");
    }
}

