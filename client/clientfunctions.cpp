#include "clientfunctions.h"

bool login(QString login, QString password) {
    QVariantMap request;
    request["method"] = "authentication";
    request["login"] = login;
    request["password"] = password;
    QJsonObject response = Network::getInstance()->doRequest(request);
    QString status = response["status"].toString();
    if (status == "success") {
        QString token = response["token"].toString();
        QString isTeacher = response["isTeacher"].toBool() ? "true" : "false";
        save_setting("login", login);
        save_setting("token", token);
        save_setting("isTeacher", isTeacher);
        return true;
    }
    else {
        return false;
    }
}

bool register_account(QString login, QString password) {
    QVariantMap request;
    request["method"] = "register";
    request["login"] = login;
    request["password"] = password;
    QJsonObject response = Network::getInstance()->doRequest(request);
    QString status = response["status"].toString();
    if (status == "success") {
        QString token = response["token"].toString();
        QString isTeacher = response["isTeacher"].toBool() ? "true" : "false";
        save_setting("login", login);
        save_setting("token", token);
        save_setting("isTeacher", isTeacher);
        return true;
    }
    else {
        return false;
    }
}

QString getStat() {
    QVariantMap request;
    QString result_str;
    request["method"] = "getUserStat";
    request["token"] = load_setting("token");
    QJsonObject response = Network::getInstance()->doRequest(request);

    const auto stat = response["result"].toArray();
    for (const auto& item : stat) {
        const auto item_obj = item.toObject();
        const int type = item_obj["type"].toInt();
        const int solved_tasks = item_obj["solved_tasks"].toInt();
        result_str += QString("%1 - %2\n").arg(type).arg(solved_tasks);
    }
    //QJsonArray res = response.value("result").toArray();
    //return res;
    return result_str;
}

QJsonArray getAllStat() {
    QVariantMap request;
    request["method"] = "getAllStat";
    request["token"] = load_setting("token");
    QJsonObject response = Network::getInstance()->doRequest(request);
    /*
    const auto stat = response["result"].toArray();
    for (const auto& item : stat) {
        const auto item_obj = item.toObject();
        const int type = item_obj["type"].toInt();
        const int solved_tasks = item_obj["solved_tasks"].toInt();
        result_str += QString("%1 - %2\n").arg(type).arg(solved_tasks);
    }
    */
    QJsonArray res = response.value("result").toArray();
    qDebug() << res;
    //qDebug() << response["result"].toObject().toVariantMap();
    return res;
}

QVariantMap getTask(int taskType) {
    QVariantMap request;
    request["method"] = "getTask";
    request["token"] = load_setting("token");
    request["taskType"] = taskType;
    QJsonObject response = Network::getInstance()->doRequest(request);
    QString status = response["status"].toString();
    if (status == "success") {
        QVariantMap task;
        task["id"] = response["taskId"].toInt();
        task["text"] = response["taskText"].toString();
        return task;
    }
    else {
        return QVariantMap();
    }
}

bool checkTask(int taskId, QString answer) {
    QVariantMap request;
    request["method"] = "checkTask";
    request["token"] = load_setting("token");
    request["taskId"] = taskId;
    request["answer"] = answer;
    QJsonObject response = Network::getInstance()->doRequest(request);
    QString status = response["status"].toString();
    if (status == "success") return true;
    else return false;
}

bool save_setting(QString param_name, QString param_value) {
    QSettings settings("settings.conf", QSettings::IniFormat);
    settings.beginGroup("Settings");
    settings.setValue(param_name, param_value);
    return true;
}

QString load_setting(QString param_name) {
    QSettings settings("settings.conf", QSettings::IniFormat);
    settings.beginGroup("Settings");
    QString param = settings.value(param_name, -1 ).toString();
    return param;
}

bool reset_settings() {
    QSettings settings("settings.conf", QSettings::IniFormat);
    settings.clear();
    return true;
}
