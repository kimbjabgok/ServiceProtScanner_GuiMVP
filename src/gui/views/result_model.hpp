#pragma once

#include <QAbstractTableModel>
#include <vector>
#include "core/result.hpp"

namespace sps::gui {

class ResultModel : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Column {
        ColPort = 0,
        ColService,
        ColProduct,
        ColRisk,
        ColMaxCvss,
        ColMaxEpss,
        ColCveCount,
        ColVersion,
        ColJa4s,
        ColJa4x,
        ColCdn,
        ColOs,
        ColCount
    };

    explicit ResultModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& = {}) const override;

    int columnCount(const QModelIndex& = {}) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void appendResult(core::ScanResult result);

    void setResults(std::vector<core::ScanResult> results);

    const core::ScanResult& resultAt(int row) const;

    const std::vector<core::ScanResult>& allResults() const;

    void clear();

private:
    std::vector<core::ScanResult> results_;

    static QString state_str(core::PortState s);
};

} // namespace sps::gui
