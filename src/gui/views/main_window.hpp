#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSortFilterProxyModel>

#include "result_model.hpp"
#include "cvss_delegate.hpp"
#include "epss_delegate.hpp"
#include "risk_delegate.hpp"
#include "details_panel.hpp"
#include "charts_panel.hpp"
#include "export_actions.hpp"
#include "dummy_data.hpp"

namespace sps::gui {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        setup_ui();
        load_dummy_data();
    }

    ResultModel* resultModel() const { return model_; }

private:
    ResultModel*            model_  = nullptr;
    QSortFilterProxyModel*  proxy_  = nullptr;
    QTableView*             table_  = nullptr;
    DetailsPanel*           detail_ = nullptr;
    ChartsPanel*            charts_ = nullptr;

    void setup_ui() {
        model_ = new ResultModel(this);
        proxy_ = new QSortFilterProxyModel(this);
        proxy_->setSourceModel(model_);
        proxy_->setSortRole(Qt::UserRole);

        table_ = new QTableView;
        table_->setModel(proxy_);
        table_->setSortingEnabled(true);
        table_->setSelectionBehavior(QAbstractItemView::SelectRows);
        table_->setAlternatingRowColors(true);
        table_->horizontalHeader()->setStretchLastSection(true);
        table_->verticalHeader()->setDefaultSectionSize(28);

        auto* delegate = new CvssDelegate(this);
        table_->setItemDelegateForColumn(ResultModel::ColMaxCvss, delegate);
        auto* epss_del = new EpssDelegate(this);
        table_->setItemDelegateForColumn(ResultModel::ColMaxEpss, epss_del);
        auto* risk_del = new RiskDelegate(this);
        table_->setItemDelegateForColumn(ResultModel::ColRisk, risk_del);
        table_->sortByColumn(ResultModel::ColRisk, Qt::DescendingOrder);

        connect(table_->selectionModel(), &QItemSelectionModel::currentRowChanged,
                this, &MainWindow::on_row_selected);

        detail_ = new DetailsPanel(this);
        charts_ = new ChartsPanel(this);

        auto* btn_export = new QPushButton("Export Report");
        btn_export->setMinimumHeight(40);
        connect(btn_export, &QPushButton::clicked, this, [this]() {
            ExportActions::exportResults(this, model_->allResults());
        });

        auto* left = new QWidget;
        auto* leftLayout = new QVBoxLayout(left);
        leftLayout->setContentsMargins(0, 0, 0, 0);
        leftLayout->addWidget(new QLabel("Scan Results"));
        leftLayout->addWidget(table_, 1);
        leftLayout->addWidget(new QLabel("Risk Priority Matrix (CVSS x EPSS)"));
        leftLayout->addWidget(charts_, 0);

        auto* right = new QWidget;
        auto* rightLayout = new QVBoxLayout(right);
        rightLayout->addWidget(new QLabel("Details"));
        rightLayout->addWidget(detail_, 1);
        rightLayout->addWidget(btn_export);

        auto* splitter = new QSplitter(Qt::Horizontal);
        splitter->addWidget(left);
        splitter->addWidget(right);
        splitter->setStretchFactor(0, 3);
        splitter->setStretchFactor(1, 1);

        setCentralWidget(splitter);
    }

    void load_dummy_data() {
        model_->setResults(sps::test::make_dummy_results());
        charts_->updateData(model_->allResults());
        table_->sortByColumn(ResultModel::ColRisk, Qt::DescendingOrder);
    }

    void on_row_selected(const QModelIndex& current) {
        if (!current.isValid()) return;
        QModelIndex src = proxy_->mapToSource(current);
        detail_->showResult(model_->resultAt(src.row()));
    }
};

} 
