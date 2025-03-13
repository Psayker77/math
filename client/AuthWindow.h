#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H
#include <clientfunctions.h>
#include <QMainWindow>
#include <QMessageBox>
#include "mainwindow.h"
#include "teacherwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class AuthWindow; }
QT_END_NAMESPACE

class AuthWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();

private slots:
    void on_pushButton_clicked();

    void on_ChangeModeButton_clicked();

private:
    Ui::AuthWindow *ui;
    MainWindow *mWindow;
};
#endif // AUTHWINDOW_H
