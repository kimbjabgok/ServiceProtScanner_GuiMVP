#pragma once

#include <QAction>
#include <QCursor>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QWidget>
#include <exception>
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
        QMenu menu(parent);
        QAction* html = menu.addAction("HTML Report");
        QAction* json = menu.addAction("JSON");
        QAction* csv = menu.addAction("CSV");

        QAction* selected = menu.exec(QCursor::pos());
        if (!selected) return;

        QString title;
        QString defaultName;
        QString filter;
        QString suffix;
        if (selected == html) {
            title = "Export HTML Report";
            defaultName = "scan_report.html";
            filter = "HTML (*.html)";
            suffix = ".html";
        } else if (selected == json) {
            title = "Export JSON";
            defaultName = "scan_report.json";
            filter = "JSON (*.json)";
            suffix = ".json";
        } else if (selected == csv) {
            title = "Export CSV";
            defaultName = "scan_report.csv";
            filter = "CSV (*.csv)";
            suffix = ".csv";
        } else {
            return;
        }

        QString path = QFileDialog::getSaveFileName(parent, title, defaultName, filter);
        if (path.isEmpty()) return;
        if (!path.endsWith(suffix, Qt::CaseInsensitive)) path += suffix;

        try {
            if (selected == html) {
                report::HtmlWriter::write(path.toStdString(), results);
            } else if (selected == json) {
                report::JsonWriter::write(path.toStdString(), results);
            } else {
                report::CsvWriter::write(path.toStdString(), results);
            }
        } catch (const std::exception& e) {
            QMessageBox::critical(parent, "Export Error",
                                  QString("Failed to write file:\n%1\n\n%2")
                                      .arg(path, QString::fromUtf8(e.what())));
            return;
        }

        QMessageBox::information(parent, "Export Complete",
                                 "Report exported to:\n" + path);
    }
};

} // namespace sps::gui
