#include "csv_writer.hpp"
#include <sstream>
#include <iomanip>

namespace sps::report {

static std::string csv_escape(const std::string& s) {
    if (s.find_first_of(",\"\n\r") == std::string::npos) return s;
    std::string out = "\"";
    for (char c : s) {
        if (c == '"') out += "\"\"";
        else out += c;
    }
    out += '"';
    return out;
}

static const char* state_str(core::PortState s) {
    switch (s) {
        case core::PortState::Open:     return "open";
        case core::PortState::Closed:   return "closed";
        case core::PortState::Filtered: return "filtered";
        default:                        return "unknown";
    }
}

std::string CsvWriter::write(const std::vector<core::ScanResult>& results) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "Host,Port,Protocol,State,Service,Product,Version,RTT(ms),CVE Count,Max CVSS,Max EPSS\n";

    for (const auto& r : results) {
        ss << csv_escape(r.target_host) << ","
           << r.port << ","
           << csv_escape(r.protocol) << ","
           << state_str(r.state) << ","
           << csv_escape(r.service.name) << ","
           << csv_escape(r.service.product) << ","
           << csv_escape(r.service.version) << ","
           << r.rtt.count() << ","
           << r.cves.size() << ","
           << r.max_cvss() << ","
           << r.max_epss() << "\n";
    }

    return ss.str();
}

} // namespace sps::report
