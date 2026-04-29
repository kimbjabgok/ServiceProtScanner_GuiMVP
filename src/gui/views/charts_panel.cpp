#include "charts_panel.hpp"

#include <QPainter>
#include <QVBoxLayout>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>

namespace sps::gui {

namespace {

bool is_probe_result(const core::ScanResult& r) {
    const auto& service = r.service.name;
    return service == "ssh" ||
           service == "http" ||
           service == "https" ||
           service == "ftp" ||
           service == "smtp";
}

QScatterSeries* make_series(const QString& name, const QColor& color) {
    auto* series = new QScatterSeries;
    series->setName(name);
    series->setMarkerSize(12.0);
    series->setColor(color);
    series->setBorderColor(QColor(255, 255, 255));
    return series;
}

QScatterSeries* series_for_risk(double risk,
                                QScatterSeries* critical,
                                QScatterSeries* high,
                                QScatterSeries* medium,
                                QScatterSeries* low) {
    if (risk >= 7.0) return critical;
    if (risk >= 3.0) return high;
    if (risk >= 1.0) return medium;
    return low;
}

} // namespace

ChartsPanel::ChartsPanel(QWidget* parent) : QWidget(parent) {
    matrix_ = new QChart;
    matrix_->setTitle("Risk Priority Matrix (CVSS x EPSS)");
    matrix_->setTheme(QChart::ChartThemeDark);
    matrix_->setAnimationOptions(QChart::SeriesAnimations);
    matrix_->setMargins(QMargins(12, 18, 18, 12));
    matrix_->legend()->setVisible(true);
    matrix_->legend()->setAlignment(Qt::AlignRight);

    matrix_view_ = new QChartView(matrix_);
    matrix_view_->setRenderHint(QPainter::Antialiasing);
    matrix_view_->setMinimumHeight(260);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(matrix_view_, 1);
}

void ChartsPanel::updateData(const std::vector<core::ScanResult>& results) {
    matrix_->removeAllSeries();
    for (auto* axis : matrix_->axes()) matrix_->removeAxis(axis);

    auto* critical = make_series("Critical", QColor(180, 30, 30));
    auto* high = make_series("High", QColor(200, 120, 20));
    auto* medium = make_series("Medium", QColor(180, 170, 30));
    auto* low = make_series("Low", QColor(80, 80, 80));

    for (const auto& r : results) {
        if (!is_probe_result(r) || r.cves.empty()) continue;

        auto* series = series_for_risk(r.max_risk(), critical, high, medium, low);
        series->append(r.max_epss(), r.max_cvss());
    }

    matrix_->addSeries(critical);
    matrix_->addSeries(high);
    matrix_->addSeries(medium);
    matrix_->addSeries(low);

    auto* axisX = new QValueAxis;
    axisX->setRange(0.0, 1.0);
    axisX->setTickCount(5);
    axisX->setLabelFormat("%.1f");
    axisX->setTitleText("EPSS (Exploit Probability)");
    matrix_->addAxis(axisX, Qt::AlignBottom);

    auto* axisY = new QValueAxis;
    axisY->setRange(0.0, 10.0);
    axisY->setTickCount(5);
    axisY->setLabelFormat("%.1f");
    axisY->setTitleText("CVSS Score");
    matrix_->addAxis(axisY, Qt::AlignLeft);

    for (auto* series : matrix_->series()) {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
}

} // namespace sps::gui
