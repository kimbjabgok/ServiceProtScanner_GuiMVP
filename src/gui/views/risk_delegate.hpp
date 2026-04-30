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
               const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;

private:
    static QColor risk_color(double score);
};

} // namespace sps::gui
