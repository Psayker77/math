#include <AuthWindow.h>
#include <mainwindow.h>
#include <teacherwindow.h>
#include <clientfunctions.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AuthWindow authw;
    MainWindow mainw;
    TeacherWindow teacherw;
    if (load_setting("login") == "-1") {  //
        authw.show();
    }
    else {
        if (load_setting("isTeacher") == "true") teacherw.show();
        else mainw.show();
    }

    return a.exec();
}
