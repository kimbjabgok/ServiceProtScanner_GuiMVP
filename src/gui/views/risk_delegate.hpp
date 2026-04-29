#pragma once

#include <QPainter>
#include <QStyledItemDelegate>
#include <algorithm>
#include "result_model.hpp"

namespace sps::gui {

class RiskDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override {
        if (index.column() != ResultModel::ColRisk) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }
        const QVariant val = index.data(Qt::UserRole);
        if (!val.isValid()) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }
        const double risk = val.toDouble();
        QColor bg = risk_color(risk);
        painter->save();
        painter->fillRect(option.rect, bg);
        painter->setPen(QColor(255, 255, 255));
        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
        painter->drawText(option.rect, Qt::AlignCenter, QString::number(risk, 'f', 2));
        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override {
        auto s = QStyledItemDelegate::sizeHint(option, index);
        s.setWidth(std::max(s.width(), 70));
        return s;
    }

private:
    static QColor risk_color(double score) {
        if (score >= 7.0) return QColor(180, 30, 30);
        if (score >= 3.0) return QColor(200, 120, 20);
        if (score >= 1.0) return QColor(180, 170, 30);
        return QColor(80, 80, 80);
    }
};

} // namespace sps::gui
