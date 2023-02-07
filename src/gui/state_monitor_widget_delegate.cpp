#include "state_monitor_widget_delegate.h"

void StateMonitorWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const {
    // TODO: draw icons using svg instead of png
    switch (index.column()) {
        case 4: {
            QRect rect = option.rect;
            trashIcon.paint(painter, rect, Qt::AlignCenter);
            break;
        }

        default:
            QItemDelegate::paint(painter, option, index);
    }
}
