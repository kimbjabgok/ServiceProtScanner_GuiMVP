#include "main_window.hpp"
#include "ui_main_window.h"
#include "cvss_delegate.hpp"
#include "epss_delegate.hpp"
#include "risk_delegate.hpp"
#include "export_actions.hpp"
#include "dummy_data.hpp"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QPushButton>
#include <QVBoxLayout>

namespace sps::gui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    ui_->leftLayout->setStretch(0, 0);
    ui_->leftLayout->setStretch(1, 1);
    ui_->leftLayout->setStretch(2, 0);
    ui_->leftLayout->setStretch(3, 0);
    ui_->rightLayout->setStretch(0, 0);
    ui_->rightLayout->setStretch(1, 1);
    ui_->rightLayout->setStretch(2, 0);

    model_ = new ResultModel(this);
    proxy_ = new QSortFilterProxyModel(this);
    proxy_->setSourceModel(model_);
    proxy_->setSortRole(Qt::UserRole);

    ui_->tableView->setModel(proxy_);
    ui_->tableView->setSortingEnabled(true);
    ui_->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->tableView->setAlternatingRowColors(true);
    ui_->tableView->horizontalHeader()->setStretchLastSection(true);
    ui_->tableView->verticalHeader()->setDefaultSectionSize(28);

    detail_ = new DetailsPanel(this);
    auto* rightLayout = ui_->detailsText->parentWidget()->layout();
    rightLayout->replaceWidget(ui_->detailsText, detail_);
    delete ui_->detailsText;

    //ChartsPanel은 .ui 파일 안에 chartsContainer라는 QWidget으로 placeholder 되어 있음.
    //그 안에 ChartsPanel을 동적으로 생성하여 넣어줌. 실제 차트는 런타임에 코드로 넣음.
    charts_ = new ChartsPanel(this);
    auto* chartsLayout = new QVBoxLayout(ui_->chartsContainer);
    chartsLayout->setContentsMargins(0, 0, 0, 0);
    chartsLayout->addWidget(charts_);

    ui_->splitter->setSizes({800, 350});

    setup_delegates();
    setup_connections();
    load_dummy_data();
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::setup_delegates() {
    auto* cvss_del = new CvssDelegate(this);
    ui_->tableView->setItemDelegateForColumn(ResultModel::ColMaxCvss, cvss_del);
    auto* epss_del = new EpssDelegate(this);
    ui_->tableView->setItemDelegateForColumn(ResultModel::ColMaxEpss, epss_del);
    auto* risk_del = new RiskDelegate(this);
    ui_->tableView->setItemDelegateForColumn(ResultModel::ColRisk, risk_del);
    ui_->tableView->sortByColumn(ResultModel::ColRisk, Qt::DescendingOrder);
}

void MainWindow::setup_connections() {
    connect(ui_->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::on_row_selected);
    connect(ui_->btnExport, &QPushButton::clicked, this, [this]() {
        ExportActions::exportResults(this, model_->allResults());
    });
    //charts_pannle에서 차트 점 클릭 → DetailsPanel 갱신 → 테이블 행 선택까지 이어집니다.
    connect(charts_, &ChartsPanel::pointClicked, this, [this](int idx) {
        detail_->showResult(model_->resultAt(idx));
        QModelIndex proxyIdx = proxy_->mapFromSource(model_->index(idx, 0));
        if (proxyIdx.isValid()) ui_->tableView->setCurrentIndex(proxyIdx);
    });
}

void MainWindow::load_dummy_data() {
    model_->setResults(sps::test::make_dummy_results());
    charts_->updateData(model_->allResults());
    ui_->tableView->sortByColumn(ResultModel::ColRisk, Qt::DescendingOrder);
}

void MainWindow::on_row_selected(const QModelIndex& current) {
    if (!current.isValid()) return;
    QModelIndex src = proxy_->mapToSource(current);
    detail_->showResult(model_->resultAt(src.row()));
}

} // namespace sps::gui
