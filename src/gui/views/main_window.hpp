#pragma once

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include "result_model.hpp"
#include "details_panel.hpp"
#include "charts_panel.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace sps::gui {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    ResultModel* resultModel() const { return model_; }

private:
    Ui::MainWindow*         ui_     = nullptr;
    ResultModel*            model_  = nullptr;
    QSortFilterProxyModel*  proxy_  = nullptr;
    DetailsPanel*           detail_ = nullptr;
    ChartsPanel*            charts_ = nullptr;

    void setup_delegates();
    void setup_connections();
    void load_dummy_data();
    void on_row_selected(const QModelIndex& current);
};

} // namespace sps::gui
