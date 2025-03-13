#ifndef TEACHERWINDOW_H
#define TEACHERWINDOW_H
#include <clientfunctions.h>
#include <QMainWindow>
#include <QProcess>

namespace Ui {
class TeacherWindow;
}

class TeacherWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeacherWindow(QWidget *parent = nullptr);
    ~TeacherWindow();

private slots:
    void on_action_triggered();

private:
    Ui::TeacherWindow *ui;
    void UpdateWindow();
};

#endif // TEACHERWINDOW_H
