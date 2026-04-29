#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QLabel>
#include <QSortFilterProxyModel>

#include "result_model.hpp"
#include "cvss_delegate.hpp"
#include "charts_panel.hpp"
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

        charts_ = new ChartsPanel(this);
        charts_->setMinimumHeight(250);

        auto* topWidget = new QWidget;
        auto* topLayout = new QVBoxLayout(topWidget);
        topLayout->setContentsMargins(0, 0, 0, 0);
        topLayout->addWidget(new QLabel("Scan Results"));
        topLayout->addWidget(table_, 1);

        auto* splitter = new QSplitter(Qt::Vertical);
        splitter->addWidget(topWidget);
        splitter->addWidget(charts_);
        splitter->setStretchFactor(0, 3);
        splitter->setStretchFactor(1, 2);

        setCentralWidget(splitter);
    }

    void load_dummy_data() {
        auto data = sps::test::make_dummy_results();
        model_->setResults(data);
        charts_->updateData(data);
    }
};

} // namespace sps::gui
