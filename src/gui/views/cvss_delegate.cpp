#include "cvss_delegate.hpp"

/*
이 코드의 존재 이유
Qt의 QTableView는 기본적으로 모델에서 받은 값을 그냥 텍스트로 보여줍니다.
그런데 CVSS 점수는 단순 숫자보다, 심각도별 색상으로 보여주는 게 훨씬 보기 좋습니다.
그래서 CvssDelegate가 QStyledItemDelegate를 상속해서 CVSS 셀만 직접 그립니다.


MainWindow에서 테이블의 CVSS 열에 CvssDelegate를 설정하면,
그 열의 셀들은 CvssDelegate::paint()가 호출되어서 CVSS 점수에 따라 색상이 칠해진 원형 배경과 함께 점수가 표시됩니다.


아래는 MainWindow에서 CvssDelegate를 설정하는 코드입니다.

auto* cvss_del = new CvssDelegate(this);
ui_->tableView->setItemDelegateForColumn(ResultModel::ColMaxCvss, cvss_del);

즉 ResultModel::ColMaxCvss 컬럼은 기본 렌더링 대신 CvssDelegate::paint()가 그립니다.
*/

namespace sps::gui {

void CvssDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const {

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

QSize CvssDelegate::sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const {
        auto s = QStyledItemDelegate::sizeHint(option, index); //delegate가 계산한 크기를 가져옴.
        s.setWidth(std::max(s.width(), 60)); //너비가 너무 좁으면 최소 60으로 늘림.
        return s;
    }

QColor CvssDelegate::cvss_color(float score) {
        if (score >= 9.0f) return QColor(180, 30, 30);
        if (score >= 7.0f) return QColor(200, 120, 20);
        if (score >= 4.0f) return QColor(180, 170, 30);
        if (score >  0.0f) return QColor(80, 80, 80);
        return QColor(50, 50, 50);
    }

} // namespace sps::gui
