#pragma once

#include <QObject>
#include <QString>
#include <utility>

class StateQObject : public QObject {
Q_OBJECT
    QString name;
    bool state = false;
    explicit StateQObject(QString name) : name{std::move(name)} {};

    friend class StateMonitorManager;
};
