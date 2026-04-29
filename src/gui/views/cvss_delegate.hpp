#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include "result_model.hpp"

namespace sps::gui {

class CvssDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override {

        // CVSS 컬럼만 색상 처리
        if (index.column() != ResultModel::ColMaxCvss) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        QVariant val = index.data(Qt::UserRole);
        if (!val.isValid()) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        float cvss = val.toFloat();
        QColor bg  = cvss_color(cvss);

        // 배경 칠하기
        painter->save();
        painter->fillRect(option.rect, bg);

        // 텍스트 (점수)
        painter->setPen(QColor(255, 255, 255));
        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
        painter->drawText(option.rect, Qt::AlignCenter,
                          QString::number(cvss, 'f', 1));
        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override {
        auto s = QStyledItemDelegate::sizeHint(option, index);
        s.setWidth(std::max(s.width(), 60));
        return s;
    }

private:
    // CVSS 점수별 색상 (보안 도구 표준 컬러)
    static QColor cvss_color(float score) {
        if (score >= 9.0f) return QColor(180, 30, 30);    // 빨강 (Critical)
        if (score >= 7.0f) return QColor(200, 120, 20);   // 주황 (High)
        if (score >= 4.0f) return QColor(180, 170, 30);   // 노랑 (Medium)
        if (score >  0.0f) return QColor(80, 80, 80);     // 회색 (Low)
        return QColor(50, 50, 50);                         // 어두운 회색 (None)
    }
};

} // namespace sps::gui
