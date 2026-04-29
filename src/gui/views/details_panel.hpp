#pragma once

#include <QTextEdit>
#include <QString>
#include "core/result.hpp"

namespace sps::gui {

class DetailsPanel : public QTextEdit {
    Q_OBJECT
public:
    explicit DetailsPanel(QWidget* parent = nullptr);
    void showResult(const core::ScanResult& r);

private:
    static QString cvss_html_color(float score);
};

} // namespace sps::gui
