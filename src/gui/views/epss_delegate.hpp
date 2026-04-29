#pragma once

#include <QPainter>
#include <QStyledItemDelegate>
#include <QtGlobal>
#include <algorithm>
#include "result_model.hpp"

namespace sps::gui {

class EpssDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override {
        if (index.column() != ResultModel::ColMaxEpss) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }
        const QVariant val = index.data(Qt::UserRole);
        if (!val.isValid()) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }
        const double epss = val.toDouble();
        QColor bg = epss_color(epss);
        painter->save();
        painter->fillRect(option.rect, bg);
        painter->setPen(QColor(255, 255, 255));
        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
        painter->drawText(option.rect, Qt::AlignCenter,
                          QString::number(qRound(epss * 100.0)) + "%");
        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override {
        auto s = QStyledItemDelegate::sizeHint(option, index);
        s.setWidth(std::max(s.width(), 60));
        return s;
    }

private:
    static QColor epss_color(double score) {
        if (score >= 0.50) return QColor(180, 30, 30);
        if (score >= 0.10) return QColor(180, 170, 30);
        return QColor(30, 140, 30);
    }
};

} // namespace sps::gui
