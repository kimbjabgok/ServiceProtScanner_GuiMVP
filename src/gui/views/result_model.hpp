#pragma once

#include <QAbstractTableModel>
#include <vector>
#include "core/result.hpp"

namespace sps::gui {

class ResultModel : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Column {
        ColPort = 0, ColProtocol, ColState, ColService, ColProduct, ColVersion,
        ColCveCount, ColMaxCvss, ColMaxEpss, ColRisk,
        ColJa4s, ColCdn, ColOs,
        ColCount
    };

    explicit ResultModel(QObject* parent = nullptr)
        : QAbstractTableModel(parent) {}

    int rowCount(const QModelIndex& = {}) const override {
        return static_cast<int>(results_.size());
    }

    int columnCount(const QModelIndex& = {}) const override {
        return ColCount;
    }

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
            return {};

        switch (static_cast<Column>(section)) {
            case ColPort:      return "Port";
            case ColProtocol:  return "Proto";
            case ColState:     return "State";
            case ColService:   return "Service";
            case ColProduct:   return "Product";
            case ColVersion:   return "Version";
            case ColCveCount:  return "CVEs";
            case ColMaxCvss:   return "CVSS";
            case ColMaxEpss:   return "EPSS";
            case ColRisk:      return "Risk";
            case ColJa4s:      return "JA4S";
            case ColCdn:       return "CDN";
            case ColOs:        return "OS";
            default:           return {};
        }
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= static_cast<int>(results_.size()))
            return {};

        const auto& r = results_[index.row()];

        if (role == Qt::DisplayRole) {
            switch (static_cast<Column>(index.column())) {
                case ColPort:      return r.port;
                case ColProtocol:  return QString::fromStdString(r.protocol);
                case ColState:     return state_str(r.state);
                case ColService:   return QString::fromStdString(r.service.name);
                case ColProduct:   return QString::fromStdString(r.service.product);
                case ColVersion:   return QString::fromStdString(r.service.version);
                case ColCveCount:  return static_cast<int>(r.cves.size());
                case ColMaxCvss:   return r.cves.empty() ? QVariant{} : QVariant{r.max_cvss()};
                case ColMaxEpss:   return r.cves.empty() ? QVariant{} : QVariant{r.max_epss()};
                case ColRisk:      return r.cves.empty() ? QVariant{} : QVariant{QString::number(r.max_risk(), 'f', 2)};
                case ColJa4s:      return QString::fromStdString(r.ja4s);
                case ColCdn:       return QString::fromStdString(r.cdn);
                case ColOs:        return QString::fromStdString(r.os_guess);
                default:           return {};
            }
        }

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

    void appendResult(core::ScanResult result) {
        int row = static_cast<int>(results_.size());
        beginInsertRows({}, row, row);
        results_.push_back(std::move(result));
        endInsertRows();
    }

    void setResults(std::vector<core::ScanResult> results) {
        beginResetModel();
        results_ = std::move(results);
        endResetModel();
    }

    const core::ScanResult& resultAt(int row) const {
        return results_.at(row);
    }

    const std::vector<core::ScanResult>& allResults() const {
        return results_;
    }

    void clear() {
        beginResetModel();
        results_.clear();
        endResetModel();
    }

private:
    std::vector<core::ScanResult> results_;

    static QString state_str(core::PortState s) {
        switch (s) {
            case core::PortState::Open:     return "open";
            case core::PortState::Closed:   return "closed";
            case core::PortState::Filtered: return "filtered";
            default:                        return "unknown";
        }
    }
};

} // namespace sps::gui
