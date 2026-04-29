#pragma once

#include <QAbstractTableModel>
#include <vector>
#include "core/result.hpp"

namespace sps::gui {

class ResultModel : public QAbstractTableModel {
    Q_OBJECT
public:
    // 컬럼 정의
    enum Column {
        ColPort = 0,
        ColProtocol,
        ColState,
        ColService,
        ColProduct,
        ColVersion,
        ColCveCount,
        ColMaxCvss,
        ColMaxEpss,
        ColVerified,
        ColCount        // 총 컬럼 수
    };

    explicit ResultModel(QObject* parent = nullptr)
        : QAbstractTableModel(parent) {}

    // ── QAbstractTableModel 필수 구현 ──

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
            case ColVerified:  return "Verified";
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
                case ColVerified:  return r.has_verified_cve() ? "✓" : "";
                default:           return {};
            }
        }

        // UserRole: CVSS Delegate가 색상 결정에 사용
        if (role == Qt::UserRole && index.column() == ColMaxCvss) {
            return r.max_cvss();
        }
        if (role == Qt::UserRole && index.column() == ColMaxEpss) {
            return r.max_epss();
        }

        return {};
    }

    // ── 데이터 조작 ──

    // 한 행 추가 (백엔드 → GUI 실시간 갱신용)
    void appendResult(core::ScanResult result) {
        int row = static_cast<int>(results_.size());
        beginInsertRows({}, row, row);
        results_.push_back(std::move(result));
        endInsertRows();
    }

    // 배치 추가 (더미 데이터 로드용)
    void setResults(std::vector<core::ScanResult> results) {
        beginResetModel();
        results_ = std::move(results);
        endResetModel();
    }

    // 특정 행의 ScanResult 접근 (Details Panel용)
    const core::ScanResult& resultAt(int row) const {
        return results_.at(row);
    }

    // 전체 결과 접근 (Export / Charts용)
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
