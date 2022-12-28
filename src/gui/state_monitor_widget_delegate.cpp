#include "state_monitor_widget_delegate.h"

void StateMonitorWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const {
    // TODO: Figure out how to draw icons with svgs instead of png
    switch (index.column()) {
        case 3: {
            QRect rect = option.rect;
            editIcon.paint(painter, rect, Qt::AlignCenter);
            break;
        }

        case 4: {
            QRect rect = option.rect;
            trashIcon.paint(painter, rect, Qt::AlignCenter);
            break;
        }

        default:
            QItemDelegate::paint(painter, option, index);
    }
}
