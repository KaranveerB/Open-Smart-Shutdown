#pragma once

#include <QApplication>
#include <QItemDelegate>
#include <QPushButton>

#include "main_state_monitor_widget.h"

class StateMonitorWidgetDelegate : public QItemDelegate {
Q_OBJECT
public:
    explicit StateMonitorWidgetDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        switch (index.column()) {
            case 3: {
                QStyleOptionButton btnOption;
                btnOption.features = QStyleOptionButton::DefaultButton;
                btnOption.state = QStyle::State_Enabled | QStyle::State_Sunken;
                btnOption.text = "Edit";
                btnOption.rect = option.rect.adjusted(1,1,-1,-1);
                QApplication::style()->drawControl(QStyle::CE_PushButton, &btnOption, painter);
                break;
            }

            case 4: {
                QStyleOptionButton btnOption;
                btnOption.features = QStyleOptionButton::DefaultButton;
                btnOption.state = QStyle::State_Enabled | QStyle::State_Sunken;
                btnOption.text = "Delete";
                btnOption.rect = option.rect.adjusted(1,1,-1,-1);
                QApplication::style()->drawControl(QStyle::CE_PushButton, &btnOption, painter);
                break;
            }

            default:
                QItemDelegate::paint(painter, option, index);
        }
    }
};
