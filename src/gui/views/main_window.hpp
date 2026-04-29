#pragma once

#include <QMainWindow>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QLabel>
#include <QSortFilterProxyModel>

#include "result_model.hpp"
#include "cvss_delegate.hpp"
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
    ResultModel*            model_ = nullptr;
    QSortFilterProxyModel*  proxy_ = nullptr;
    QTableView*             table_ = nullptr;

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

        auto* central = new QWidget;
        auto* layout = new QVBoxLayout(central);
        layout->addWidget(new QLabel("Scan Results"));
        layout->addWidget(table_, 1);
        setCentralWidget(central);
    }

    void load_dummy_data() {
        model_->setResults(sps::test::make_dummy_results());
    }
};

} // namespace sps::gui
