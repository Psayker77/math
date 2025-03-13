#include "tasks.h"


QVector<QVector<int>> generateRandomGraph(int minSize, int maxSize)
{
    int size = QRandomGenerator::global()->bounded(minSize, maxSize + 1);

    // Создаем граф, который сначала является циклом
    QVector<QVector<int>> graph(size, QVector<int>(size, 0));
    for (int i = 0; i < size; ++i)
    {
        graph[i][(i + 1) % size] = graph[(i + 1) % size][i] = 1;
    }

    // Добавляем дополнительные ребра случайным образом
    for (int i = 0; i < size; ++i)
    {
        for (int j = i + 2; j < size; ++j)
        {
            if (j != (i + 1) % size && j != (i - 1 + size) % size) // исключаем вершины, уже соединенные с i
            {
                int edge = QRandomGenerator::global()->bounded(2); // 0 или 1
                graph[i][j] = graph[j][i] = edge;
            }
        }
    }

    return graph;
}




QVector<QVector<int>> stringToGraph(const QString& taskDescription)
{
    QVector<QVector<int>> graph;

    int graphStart = taskDescription.indexOf("\n") + 1;
    int graphEnd = taskDescription.indexOf("Найдите");

    QString graphString = taskDescription.mid(graphStart, graphEnd - graphStart).trimmed();

    QStringList rows = graphString.split("\n");
    for (const auto& row : rows)
    {
        QStringList values = row.trimmed().split(' ');
        QVector<int> graphRow;
        for (const auto& value : values)
        {
            graphRow.push_back(value.toInt());
        }
        graph.push_back(graphRow);
    }

    return graph;
}

using Edge = QPair<int, QPair<int, int>>;
QVector<QVector<Edge>> stringToGraph2(QString taskDescription) {
    QVector<QVector<Edge>> adj;

    // Удаляем все лишнее из описания задачи
    int graphStart = taskDescription.indexOf("\n") + 1;
    int graphEnd = taskDescription.indexOf("Найдите");

    QString graphString = taskDescription.mid(graphStart, graphEnd - graphStart).trimmed();

    // Разбиваем строку на отдельные строки
    QStringList edges = graphString.split("\n");

    int maxVertexIndex = 0;
    for (const QString& edge : edges) {
        if (edge[0] != '(') continue;
        QString processedEdge = edge;
        processedEdge.remove('(').remove(')').remove(' ');
        // Извлекаем вершины и вес ребра из строки
        QStringList parts = processedEdge.split("->");
        int src = parts[0].toInt();
        QStringList destAndWeight = parts[1].split(',');
        int dest = destAndWeight[0].toInt();
        int weight = destAndWeight[1].toInt();

        // Обновляем максимальный индекс вершины
        maxVertexIndex = qMax(maxVertexIndex, qMax(src, dest));

        // Проверяем, достаточно ли места в списке смежности для текущего источника
        if (src >= adj.size())
            adj.resize(src + 1);

        // Добавляем ребро в список смежности
        adj[src].push_back(Edge(dest, QPair<int, int>(weight, 0)));
    }

    // Убеждаемся, что в списке смежности достаточно места для всех вершин
    if (maxVertexIndex + 1 > adj.size())
        adj.resize(maxVertexIndex + 1);

    return adj;
}




// Функция для парсинга строки вида "{(0, 1), (1, 2), (2, 0)}" в QMap
QMap<int, int> parseSRPAnswer(QString input) {
    QMap<int, int> srp;
    // Удаляем скобки и пробелы
    input.remove("{").remove("}").remove(" ");
    // Разбиваем строку на пары вершин по запятым
    QStringList pairs = input.split("),(");
    for (auto& pair : pairs) {
        pair.remove("(").remove(")");
        // Разбиваем пару вершин на две вершины по запятой
        QStringList vertices = pair.split(',');
        int u = vertices[0].toInt(), v = vertices[1].toInt();
        srp.insert(u, v);
    }
    return srp;
}

bool isValidSRP(QString taskText, QString answer) {
    // парсим строку условия в QVector<QVector<int>>
    qDebug() << taskText;
    QVector<QVector<Edge>> adj = stringToGraph2(taskText);
    qDebug() << adj;
    // Парсим строку с СРП в QMap
    QMap<int, int> srp = parseSRPAnswer(answer);
    qDebug() << srp;
    // Создаем QSet для отслеживания уникальности вершин
    QSet<int> seenVertices;
    // Переменная для отслеживания предыдущей вершины
    int prevVertex = -1;
    for (auto& pair : srp.toStdMap()) {
        // Извлекаем вершины из пары
        int u = pair.first, v = pair.second;
        // Проверяем, является ли пара вершин ребром в графе
        bool edgeExists = false;
        for (const Edge& edge : adj[u]) {
            if (edge.first == v) {
                edgeExists = true;
                break;
            }
        }
        // Если ребро не существует или если u не равно предыдущей вершине и u уже было встречено,
        // то возвращаем false
        if (!edgeExists || (u != prevVertex && seenVertices.contains(u)))
            return false;
        // Если текущая вершина уникальна, добавляем ее в QSet
        seenVertices.insert(u);
        // Устанавливаем текущую вершину как предыдущую для следующей итерации
        prevVertex = v;
    }
    // Если все проверки прошли успешно, возвращаем true
    return true;
}




// Функция для преобразования QString в QVector<int>
QVector<int> stringToUserPath(const QString& userPathString)
{
    QVector<int> userPath;
    QStringList values = userPathString.split('-');
    for (const auto& value : values)
    {
        userPath.push_back(value.toInt());
    }
    return userPath;
}


bool isBipartite(QVector<QVector<int>>& graph)
{
    QVector<int> colorArr(graph.size(), -1);

    for (int i = 0; i < graph.size(); i++)
    {
        if (colorArr[i] != -1)
            continue;

        colorArr[i] = 1;
        QQueue<int> q;
        q.enqueue(i);

        while (!q.isEmpty())
        {
            int u = q.dequeue();

            for (int v = 0; v < graph.size(); ++v)
            {
                if (graph[u][v])
                {
                    if (colorArr[v] == -1)
                    {
                        colorArr[v] = 1 - colorArr[u];
                        q.enqueue(v);
                    }
                    else if (colorArr[v] == colorArr[u])
                        return false;
                }
            }
        }
    }

    return true;
}




// Функция для проверки корректности решения
bool hamiltonianCycleCheck(const QString& graphString, const QString& userPathString)
{
    QVector<QVector<int>> graph = stringToGraph(graphString);
    qDebug() << graph;
    QVector<int> userPath = stringToUserPath(userPathString);

    int size = graph.size();
    if (userPath.size() != size + 1)
        return false;

    if (userPath[0] != userPath[size])
        return false;

    QVector<int> visitCount(size, 0);
    for (int i = 0; i < size; ++i)
    {
        if (graph[userPath[i]][userPath[i + 1]] == 0)
            return false;
        visitCount[userPath[i]]++;
    }

    for (int i = 0; i < size; ++i)
    {
        if (visitCount[i] != 1)
            return false;
    }

    return true;
}



// функции задачи 4 (найти кратчайший путь между двумя вершинами)

// Функция для добавления ребра в граф
void addEdge(QVector<QVector<iPair>>& adj, int u, int v, int w) {
    adj[u].push_back(qMakePair(v, w));
    adj[v].push_back(qMakePair(u, w));
}

// Функция для генерации графа
QVector<QVector<iPair>> generateGraphTask4(int V) {
    QVector<QVector<iPair>> adj(V);

    // Генерация ребер между вершинами
    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            int distance = QRandomGenerator::global()->bounded(10) + 1;
            addEdge(adj, i, j, distance);
        }
    }

    return adj;
}

// Функция для нахождения веса кратчайшего пути используя алгоритм Дейкстры
int shortestPathWeight(QVector<QVector<iPair>> &adj, int V, int src, int dest) {
    // Создаем минимальную кучу и добавляем исходную вершину.
    // У исходной вершины расстояние до себя 0
    QMap<int, iPair> minHeap;
    minHeap.insert(0, std::make_pair(src, -1));

    QVector<int> parent(V, -1);
    QVector<int> dist(V, std::numeric_limits<int>::max());

    dist[src] = 0;
    while (!minHeap.empty()) {
        int u = minHeap.begin().value().first;
        minHeap.erase(minHeap.begin());

        for (auto x : adj[u]) {
            int v = x.first;
            int weight = x.second;

            if (dist[v] > dist[u] + weight) {
                if (dist[v] != std::numeric_limits<int>::max()) {
                    minHeap.remove(dist[v]);
                }
                dist[v] = dist[u] + weight;
                minHeap[dist[v]] = std::make_pair(v, u);
                parent[v] = u;
            }
        }
    }

    return dist[dest];
}

// (src->dest, weight)
QString graphToStringTask4(const QVector<QVector<iPair>>& graph) {
    QString result;

    for (int i = 0; i < graph.size(); ++i) {
        for (auto edge : graph[i]) {
            result += "(" + QString::number(i) + "->" + QString::number(edge.first)
                    + ", " + QString::number(edge.second) + ")\n";
        }
    }

    return result;
}


QVariantMap Task1() {
    QVariantMap task;
    int length = QRandomGenerator::global()->bounded(1, 3) + 2;  // Количество вершин в графе
    QVector<QVector<iPair>> gr = generateGraphTask4(length);
    QString text = "Двудольный граф:\n";
    text += graphToStringTask4(gr) + "\n";
    text += "Найдите систему различных представителей. Ответ запишите в формате {(0, 1), (1, 2), (2, 0)}";
    task["text"] = text;
    task["answer"] = "need_check";
    return task;
}


QVariantMap Task2() {
    QVariantMap task;
    QVector<QVector<int>> graph = generateRandomGraph(4, 6);
    qDebug() << graph;
    QString text = "Задан следующий граф:\n";
        for (const auto& row : graph)
        {
            for (int val : row)
            {
                text += QString::number(val) + ' ';
            }
            text += '\n';
        }
    text += "Является ли он двудольным? В ответ напишите 'да' или 'нет'";
    QString answer = isBipartite(graph) ? "да" : "нет";
    task["text"] = text;
    task["answer"] = answer;
    return task;
}

QVariantMap Task3() {
    QVariantMap task;
    QVector<QVector<int>> graph = generateRandomGraph(4, 6);
    QString text = "Задан следующий граф:\n";
        for (const auto& row : graph)
        {
            for (int val : row)
            {
                text += QString::number(val) + ' ';
            }
            text += '\n';
        }
        text += "Найдите гамильтонов цикл в этом графе (укажите вершины через -).";
    task["text"] = text;
    task["answer"] = "need_check";
    return task;
}

QVariantMap Task4() {
    QVariantMap task;
    int length = QRandomGenerator::global()->bounded(1, 3) + 2;  // Количество вершин в графе
    QVector<QVector<iPair>> gr = generateGraphTask4(length);

    int src = QRandomGenerator::global()->bounded(length);
    int dest;
    do {
        dest = QRandomGenerator::global()->bounded(length);
    } while (src == dest);

    int answer = shortestPathWeight(gr, length, src, dest);
    QString text = "Задан следующий граф:\n";
    text += graphToStringTask4(gr) + "\n";
    text += QString("Найти кратчайший путь между вершинами %1 и %2. В ответ укажите его вес.").arg(src).arg(dest);
    task["text"] = text;
    task["answer"] = answer;

    return task;
}
