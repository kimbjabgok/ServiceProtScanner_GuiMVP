#include "charts_panel.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include <algorithm>

namespace sps::gui {

ChartsPanel::ChartsPanel(QWidget* parent) : QWidget(parent) {
    chart_ = new QChart;
    chart_->setTitle("CVSS Score Distribution");
    chart_->setTheme(QChart::ChartThemeDark);
    chart_->setAnimationOptions(QChart::SeriesAnimations);

    chart_view_ = new QChartView(chart_);
    chart_view_->setRenderHint(QPainter::Antialiasing);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chart_view_, 1);
}

void ChartsPanel::updateData(const std::vector<core::ScanResult>& results) {
    chart_->removeAllSeries();
    for (auto* axis : chart_->axes()) chart_->removeAxis(axis);

    int critical = 0, high = 0, medium = 0, low = 0;
    for (const auto& r : results) {
        for (const auto& cve : r.cves) {
            if (cve.cvss_score >= 9.0f)      ++critical;
            else if (cve.cvss_score >= 7.0f)  ++high;
            else if (cve.cvss_score >= 4.0f)  ++medium;
            else if (cve.cvss_score > 0.0f)   ++low;
        }
    }

    auto* setCritical = new QBarSet("Critical (\xe2\x89\xa59.0)");
    setCritical->setColor(QColor(180, 30, 30));
    *setCritical << critical << 0 << 0 << 0;

    auto* setHigh = new QBarSet("High (\xe2\x89\xa57.0)");
    setHigh->setColor(QColor(200, 120, 20));
    *setHigh << 0 << high << 0 << 0;

    auto* setMedium = new QBarSet("Medium (\xe2\x89\xa54.0)");
    setMedium->setColor(QColor(180, 170, 30));
    *setMedium << 0 << 0 << medium << 0;

    auto* setLow = new QBarSet("Low (<4.0)");
    setLow->setColor(QColor(80, 80, 80));
    *setLow << 0 << 0 << 0 << low;

    auto* series = new QStackedBarSeries;
    series->append(setCritical);
    series->append(setHigh);
    series->append(setMedium);
    series->append(setLow);
    series->setBarWidth(0.6);
    chart_->addSeries(series);

    auto* axisX = new QBarCategoryAxis;
    axisX->append({"Critical", "High", "Medium", "Low"});
    chart_->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto* axisY = new QValueAxis;
    axisY->setTitleText("Count");
    int maxVal = std::max({critical, high, medium, low});
    axisY->setRange(0, maxVal + 1);
    axisY->setLabelFormat("%d");
    chart_->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart_->legend()->setVisible(true);
    chart_->legend()->setAlignment(Qt::AlignRight);
    chart_->legend()->setFont(QFont("sans", 9));
}

} // namespace sps::gui
