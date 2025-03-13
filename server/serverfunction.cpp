
#include "serverfunction.h"

QByteArray parse(QByteArray request) {
    // {"method": "register", "login": "qwer", "password": "pass"}
    // {"method": "authentication", "login": "qwer", "password": "pass"}
    // {"method": "getTask", "taskType": 1}
    // {"method": "checkTask", "taskId": 3, "answer": 1.0}
    // {"method": "getAllStat", "token": "10f6dc4fedbc5bedd4f6beff3171930d"}
    QJsonObject json_request = QJsonDocument::fromJson(request).object();
    qDebug() << json_request;
    QString response;
    QString method = json_request["method"].toString();
    if (method == QString("authentication")) {
        response = auth(json_request);
    }
    else if (method == QString("register")) {
        response = reg(json_request);
    }
    else if (method == QString("getTask")) {
        response = getTask(json_request);
    }
    else if (method == QString("checkTask")) {
        response = checkTask(json_request);
    }
    else if (method == QString("getUserStat")) {
        response = getUserStat(json_request);
    }
    else if (method == QString("getAllStat")) {
        response = getAllStat(json_request);
    }
    else {
        QVariantMap error_response;
        error_response["error"] = QString("Function is not defined");
        response = QJsonDocument::fromVariant(error_response).toJson(QJsonDocument::Compact);
    }
    return QByteArray((response + "\r\n").toUtf8());
}

QByteArray auth(QJsonObject request) {
    QVariantMap response;
    QString login = request["login"].toString();
    QString input_password = request["password"].toString();
    QString query = QString("SELECT * FROM Users WHERE login = \"%1\"").arg(login);
    QSqlQuery queryResult = Database::getInstance()->doSQLQuery(query);
    QSqlRecord record = queryResult.record();
    queryResult.next();
    QString dbPassword = queryResult.value(record.indexOf("password")).toString();
    if (dbPassword == input_password) {
        QString token = queryResult.value(record.indexOf("token")).toString();
        bool isTeacher = queryResult.value(record.indexOf("isTeacher")).toBool();
        response["status"] = "success";
        response["token"] = token;
        response["isTeacher"] = isTeacher;
    }
    else {
        response["status"] = "fail";
    }

    qDebug() << QString("Autherisation with login %1 and password %2").arg(login, input_password);
    return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
}

QByteArray reg(QJsonObject request) {
    QVariantMap response;
    QString login = request["login"].toString();
    QString password = request["password"].toString();
    QString token = QString(QCryptographicHash::hash(((login+password).toUtf8()), QCryptographicHash::Md5).toHex());
    QString query = QString("INSERT INTO Users(login, password, token, isTeacher) "
                            "VALUES (\"%1\", \"%2\", \"%3\", false)")
                            .arg(login, password, token);
    Database::getInstance()->doSQLQuery(query);
    response["token"] = token;
    response["status"] = "success";
    response["isTeacher"] = false;
    return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
}

QByteArray getAllStat(QJsonObject request) {
    QVariantMap response;
    if (!isTeacher(request["token"].toString())) {
        response["status"] = "fail";
        return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
    }
    QString query = QString("SELECT Users.login, Tasks.type, COUNT(*) AS solved_tasks "
                            "FROM Users "
                            "JOIN UsersTasks ON Users.userID = UsersTasks.userID "
                            "JOIN Tasks ON Tasks.taskID = UsersTasks.taskID "
                            "GROUP BY Users.login, Tasks.type"
    );
    QSqlQuery stat = Database::getInstance()->doSQLQuery(query);
    QMap<QString, QJsonObject> recordsMap;
    while(stat.next()) {
        QString login = stat.value("login").toString();
        int type = stat.value("type").toInt();
        int solved_tasks = stat.value("solved_tasks").toInt();

        if (!recordsMap.contains(login)) {
            QJsonObject recordObject;
            recordObject.insert("login", login);
            recordObject.insert("solved_tasks", QJsonObject());
            recordsMap.insert(login, recordObject);
        }

        QJsonObject recordObject = recordsMap.value(login);
        QJsonObject solvedTasksObject = recordObject["solved_tasks"].toObject();
        solvedTasksObject.insert(QString::number(type), solved_tasks);
        recordObject["solved_tasks"] = solvedTasksObject;
        recordsMap[login] = recordObject;
    }
    QJsonArray recordsArray;
    for (const QJsonObject &recordObject : recordsMap.values()) {
        recordsArray.push_back(recordObject);
    }
    response["status"] = "success";
    response["result"] = recordsArray;
    return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
}

QByteArray getUserStat(QJsonObject request) {
    QVariantMap response;
    QString userToken = request["token"].toString();
    QString query = QString("SELECT Tasks.type, COUNT(*) AS solved_tasks "
                            "FROM UsersTasks "
                            "JOIN Tasks ON Tasks.taskID = UsersTasks.taskID "
                            "WHERE UsersTasks.userID = (SELECT userID FROM Users WHERE token = \"%1\") "
                            "GROUP BY Tasks.type")
                            .arg(userToken);
    QSqlQuery stat = Database::getInstance()->doSQLQuery(query);
    QJsonArray recordsArray;
    while(stat.next()) {
        QJsonObject recordObject;
        for (int x = 0; x < stat.record().count(); x++) {
            recordObject.insert(stat.record().fieldName(x), QJsonValue::fromVariant(stat.value(x)));
        }
        recordsArray.push_back(recordObject);
    }
    response["status"] = "success";
    response["result"] = recordsArray;
    return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
}

QByteArray getTask(QJsonObject request) {
    QVariantMap response;
    int taskType = request["taskType"].toInt();
    QVariantMap task;
    switch (taskType) {
        case 1: {task = Task1(); break;}
        case 2: {task = Task2(); break;}
        case 3: {task = Task3(); break;}
        case 4: {task = Task4(); break;}
        default: {task = Task1(); break;}
    }
    QString taskText = task["text"].toString();
    QString taskAnswer = task["answer"].toString();
    QString query = QString("INSERT INTO Tasks(text, answer, type) "
                    "VALUES (\"%1\", \"%2\", %3)")
                    .arg(taskText).arg(taskAnswer).arg(taskType);
    QSqlQuery queryResult = Database::getInstance()->doSQLQuery(query);
    response["status"] = "success";
    response["taskId"] = queryResult.lastInsertId().toInt();
    response["taskText"] = taskText;
    return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
}

QByteArray checkTask(QJsonObject request) {
    QVariantMap response;
    QString userToken = request["token"].toString();
    int taskId = request["taskId"].toInt();
    QString answer = request["answer"].toString();
    QString query = QString("SELECT * FROM Tasks WHERE taskId = %1").arg(taskId);
    QSqlQuery queryResult = Database::getInstance()->doSQLQuery(query);
    QSqlRecord record = queryResult.record();
    queryResult.next();
    QString correctAnswer = queryResult.value(record.indexOf("answer")).toString();

    bool isCorrectAfterCheck = false;
    if (correctAnswer == "need_check") {
        int taskType = queryResult.value(record.indexOf("type")).toInt();
        QString taskText = queryResult.value(record.indexOf("text")).toString();
        switch (taskType) {
        case 1:
            isCorrectAfterCheck = isValidSRP(taskText, answer);
            qDebug() << isCorrectAfterCheck;
            break;
        case 3:
            isCorrectAfterCheck = hamiltonianCycleCheck(taskText, answer);
            qDebug() << isCorrectAfterCheck;
            break;
        default:
            break;
        }
    }

    if ((answer == correctAnswer) || isCorrectAfterCheck) {
        response["status"] = "success";
        QString query = QString("INSERT INTO UsersTasks(userID, taskid) "
                                "SELECT userID, %1 FROM Users WHERE token == \"%2\"")
                                .arg(taskId).arg(userToken);
        Database::getInstance()->doSQLQuery(query);
    }
    else {
        response["status"] = "fail";
    }
    return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
}

bool isTeacher(QString userToken) {
    QString query = QString("SELECT isTeacher FROM Users WHERE token == \"%2\"")
                            .arg(userToken);
    QSqlQuery queryResult = Database::getInstance()->doSQLQuery(query);
    QSqlRecord record = queryResult.record();
    queryResult.next();
    bool isTeacher = queryResult.value(record.indexOf("isTeacher")).toBool();
    return isTeacher;
}
