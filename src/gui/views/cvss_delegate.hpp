#pragma once

#include <QPainter>
#include <QStyledItemDelegate>  //Qt의 테이블/리스트 셀 렌더링을 커스터마이즈할 때 쓰는 클래스
#include <algorithm>
#include "result_model.hpp"

namespace sps::gui {

class CvssDelegate : public QStyledItemDelegate {
    Q_OBJECT //Qt meta-object 기능을 쓰기 위한 매크로
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    //테이블의 셀 하나를 그릴 때 Qt가 호출됨.
    //index는 모델의 어느 셀인지, option은 셀의 위치/크기 등 스타일 정보, painter는 그리기 도구.
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override {

        //CVSS 열이 아니면 기본 delegat에게 넘김.
        if (index.column() != ResultModel::ColMaxCvss) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        const QVariant val = index.data(Qt::UserRole); //UserRole은 정렬/색상 계산용 원본 숫자를 위해 사용.
        //CVE가 없는 행이면 CVSS 값이 비어 있을 수 있으니까 기본 렌더링 처리.
        if (!val.isValid()) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        const float cvss = val.toFloat();
        const QColor bg = cvss_color(cvss); //cvss 점수에 따른 배경색 가져옴. 맨 아래에 정의.

        //painter로 그림.
        painter->save(); 
        painter->fillRect(option.rect, bg);
        painter->setPen(QColor(255, 255, 255));
        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
        painter->drawText(option.rect, Qt::AlignCenter,
                          QString::number(cvss, 'f', 1));
        painter->restore();
    }

    //셀의 권장 크기를 알려주는 함수.
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override {
        auto s = QStyledItemDelegate::sizeHint(option, index); //delegate가 계산한 크기를 가져옴.
        s.setWidth(std::max(s.width(), 60)); //너비가 너무 좁으면 최소 60으로 늘림.
        return s;
    }

private:
    //CVSS 점수에 따라 색상 반환하는 핼퍼 함수.
    //CVSS 9.0 이상은 빨강, 7.0 이상은 주황, 4.0 이상은 노랑, 0보다 크면 회색, 0이면 더 어두운 회색.
    static QColor cvss_color(float score) {
        if (score >= 9.0f) return QColor(180, 30, 30);
        if (score >= 7.0f) return QColor(200, 120, 20);
        if (score >= 4.0f) return QColor(180, 170, 30);
        if (score >  0.0f) return QColor(80, 80, 80);
        return QColor(50, 50, 50);
    }
};

} 