#include "result_model.hpp"

#include <utility>

namespace sps::gui {

ResultModel::ResultModel(QObject* parent)
        : QAbstractTableModel(parent) {}

int ResultModel::rowCount(const QModelIndex&) const {
        return static_cast<int>(results_.size());
    }

int ResultModel::columnCount(const QModelIndex&) const {
        return ColCount;
    }

QVariant ResultModel::headerData(int section, Qt::Orientation orientation,
                        int role) const {
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
            return {};

        switch (static_cast<Column>(section)) {
            case ColPort:      return "Port";
            case ColService:   return "Service";
            case ColProduct:   return "Product";
            case ColRisk:      return "Risk";
            case ColMaxCvss:   return "CVSS";
            case ColMaxEpss:   return "EPSS";
            case ColCveCount:  return "CVEs";
            case ColVersion:   return "Version";
            case ColJa4s:      return "JA4S";
            case ColJa4x:      return "JA4X";
            case ColCdn:       return "CDN";
            case ColOs:        return "OS";
            default:           return {};
        }
    }

QVariant ResultModel::data(const QModelIndex& index, int role) const {
        if (!index.isValid() || index.row() >= static_cast<int>(results_.size()))
            return {};

        const auto& r = results_[index.row()];

        if (role == Qt::DisplayRole) {
            switch (static_cast<Column>(index.column())) {
                case ColPort:      return r.port;
                case ColService:   return QString::fromStdString(r.service.name);
                case ColProduct:   return QString::fromStdString(r.service.product);
                case ColRisk:      return r.cves.empty() ? QVariant{} : QVariant{QString::number(r.max_risk(), 'f', 2)};
                case ColMaxCvss:   return r.cves.empty() ? QVariant{} : QVariant{r.max_cvss()};
                case ColMaxEpss:   return r.cves.empty() ? QVariant{} : QVariant{r.max_epss()};
                case ColCveCount:  return static_cast<int>(r.cves.size());
                case ColVersion:   return QString::fromStdString(r.service.version);
                case ColJa4s:      return QString::fromStdString(r.ja4s);
                case ColJa4x:      return QString::fromStdString(r.ja4x);
                case ColCdn:       return QString::fromStdString(r.cdn);
                case ColOs:        return QString::fromStdString(r.os_guess);
                default:           return {};
            }
        }

        // CVSS 컬럼의 UserRole 요청이 들어오면 r.max_cvss()를 반환. epss와 risk도 마찬가지.
        if (role == Qt::UserRole && index.column() == ColMaxCvss) {
            return r.max_cvss();
        }
        if (role == Qt::UserRole && index.column() == ColMaxEpss) {
            return r.max_epss();
        }
        if (role == Qt::UserRole && index.column() == ColRisk) {
            return r.max_risk();
        }

        return {};
    }

void ResultModel::appendResult(core::ScanResult result) {
        int row = static_cast<int>(results_.size());
        beginInsertRows({}, row, row);
        results_.push_back(std::move(result));
        endInsertRows();
    }

void ResultModel::setResults(std::vector<core::ScanResult> results) {
        beginResetModel();
        results_ = std::move(results);
        endResetModel();
    }

const core::ScanResult& ResultModel::resultAt(int row) const {
        return results_.at(row);
    }

const std::vector<core::ScanResult>& ResultModel::allResults() const {
        return results_;
    }

void ResultModel::clear() {
        beginResetModel();
        results_.clear();
        endResetModel();
    }

QString ResultModel::state_str(core::PortState s) {
        switch (s) {
            case core::PortState::Open:     return "open";
            case core::PortState::Closed:   return "closed";
            case core::PortState::Filtered: return "filtered";
            default:                        return "unknown";
        }
    }

} // namespace sps::gui
