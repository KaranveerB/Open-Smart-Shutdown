#pragma once

#include <QItemDelegate>

#include <QtSvg> // not currently used, but should be
#include <QSvgRenderer>

#include "main_state_monitor_widget.h"

class StateMonitorWidgetDelegate : public QItemDelegate {
Q_OBJECT
public:
    explicit StateMonitorWidgetDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    const QIcon editIcon = QIcon(":/icons/edit.png");
    const QIcon trashIcon = QIcon(":/icons/trash.png");
};

