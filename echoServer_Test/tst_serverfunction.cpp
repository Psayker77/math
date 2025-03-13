#include <QtTest>
#include <vector>
#include <../server/tasks.cpp>
#include <../server/serverfunction.h>

// add necessary includes here

class serverfunction : public QObject
{
    Q_OBJECT

public:
    serverfunction();
    ~serverfunction();

private slots:
    void test_case_1();
    void test_case_2();
    void test_case_3();
    void test_case_4();
};

serverfunction::serverfunction() {

}

serverfunction::~serverfunction() {

}

void serverfunction::test_case_1() {
    QVERIFY(isTeacher("admin_token") == true);
    QVERIFY(isTeacher("not_admin_token") == false);
}

void serverfunction::test_case_2() {
    QList<QList<int>> graph = {
        {0, 1, 0, 1},
        {1, 0, 1, 1},
        {0, 1, 0, 1},
        {1, 1, 1, 0}
    };
    QVERIFY(isBipartite(graph) == false);
    graph = {
        {0, 1, 0, 1},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 1, 0}
    };
    QVERIFY(isBipartite(graph) == true);
}

void serverfunction::test_case_3() {
    QString task = "Задан следующий граф:\n0 1 0 1 1 \n1 0 1 1 0 \n0 1 0 1 1 \n1 1 1 0 1 \n1 0 1 1 0 \nНайдите гамильтонов цикл в этом графе (укажите вершины через -).";
    QString answer = "0-1-2-3-4-0";
    QVERIFY(hamiltonianCycleCheck(task, answer) == true);
    answer = "0-1-2-3-2-1";
    QVERIFY(hamiltonianCycleCheck(task, answer) == false);
}

void serverfunction::test_case_4() {
    QList<QList<QPair<int, int>>> graph = {
        {QPair<int, int>(1, 10), QPair<int, int>(2, 5)},
        {QPair<int, int>(0, 10), QPair<int, int>(2, 9)},
        {QPair<int, int>(0, 5), QPair<int, int>(1, 9)}
    };
    QVERIFY(shortestPathWeight(graph, 3, 2, 1) == 9);
}

QTEST_APPLESS_MAIN(serverfunction)

#include "tst_serverfunction.moc"
