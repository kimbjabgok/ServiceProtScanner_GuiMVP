#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <vector>
#include "core/result.hpp"
#include "report/html_writer.hpp"
#include "report/json_writer.hpp"
#include "report/csv_writer.hpp"

namespace sps::gui {

class ExportActions {
public:
    static void exportResults(QWidget* parent,
                              const std::vector<core::ScanResult>& results)
    {
        QString path = QFileDialog::getSaveFileName(
            parent, "Export Report", "scan_report",
            "HTML (*.html);;JSON (*.json);;CSV (*.csv)");

        if (path.isEmpty()) return;

        std::string content;
        if (path.endsWith(".json"))      content = report::JsonWriter::write(results);
        else if (path.endsWith(".csv"))  content = report::CsvWriter::write(results);
        else {
            if (!path.endsWith(".html")) path += ".html";
            content = report::HtmlWriter::write(results);
        }

        std::ofstream out(path.toStdString());
        if (!out) {
            QMessageBox::critical(parent, "Export Error",
                                  "Failed to write file:\n" + path);
            return;
        }
        out << content;
        QMessageBox::information(parent, "Export Complete",
                                 "Report exported to:\n" + path);
    }
};

} // namespace sps::gui
