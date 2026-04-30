#include "csv_writer.hpp"

#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace sps::report {

namespace {

std::string csv_escape(const std::string& s) {
    bool needs_quotes = s.find_first_of(",\"\r\n") != std::string::npos;
    if (!needs_quotes) return s;

    std::string out = "\"";
    for (char c : s) {
        if (c == '"') out += "\"\"";
        else out += c;
    }
    out += '"';
    return out;
}

const char* state_str(core::PortState s) {
    switch (s) {
        case core::PortState::Open:     return "open";
        case core::PortState::Closed:   return "closed";
        case core::PortState::Filtered: return "filtered";
        default:                        return "unknown";
    }
}

} // namespace

void CsvWriter::write(const std::string& path,
                      const std::vector<core::ScanResult>& results) {
    std::ofstream out(path);
    if (!out) throw std::runtime_error("failed to open CSV export path");
    out << std::fixed << std::setprecision(2);
    out << "Port,Protocol,State,Service,Product,Version,CVE_Count,"
        << "Max_CVSS,Max_EPSS,Max_Risk,JA4S,CDN,OS\n";

    for (const auto& r : results) {
        out << r.port << ","
            << csv_escape(r.protocol) << ","
            << state_str(r.state) << ","
            << csv_escape(r.service.name) << ","
            << csv_escape(r.service.product) << ","
            << csv_escape(r.service.version) << ","
            << r.cves.size() << ","
            << r.max_cvss() << ","
            << r.max_epss() << ","
            << static_cast<double>(r.max_cvss()) * r.max_epss() << ","
            << csv_escape(r.ja4s) << ","
            << csv_escape(r.cdn) << ","
            << csv_escape(r.os_guess) << "\n";
    }
}

} // namespace sps::report
