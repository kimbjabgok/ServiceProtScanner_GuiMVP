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