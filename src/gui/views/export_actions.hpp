#pragma once

#include <QWidget>
#include <vector>
#include "core/result.hpp"

namespace sps::gui {

class ExportActions {
public:
    static void exportResults(QWidget* parent,
                              const std::vector<core::ScanResult>& results);
};

} // namespace sps::gui
