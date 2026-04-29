#pragma once

#include <QWidget>
#include <QtCharts/QChartView>
#include <vector>
#include "core/result.hpp"

namespace sps::gui {

class ChartsPanel : public QWidget {
    Q_OBJECT
public:
    explicit ChartsPanel(QWidget* parent = nullptr);
    void updateData(const std::vector<core::ScanResult>& results);

private:
    QChartView* matrix_view_ = nullptr;
    QChart*     matrix_      = nullptr;
};

} // namespace sps::gui
