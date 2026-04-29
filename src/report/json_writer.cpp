#include "json_writer.hpp"
#include <sstream>
#include <iomanip>

namespace sps::report {

static std::string json_escape(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:
                if (static_cast<unsigned char>(c) < 0x20)
                    out += ' ';
                else
                    out += c;
        }
    }
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

std::string JsonWriter::write(const std::vector<core::ScanResult>& results) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "[\n";

    for (size_t i = 0; i < results.size(); ++i) {
        const auto& r = results[i];
        ss << "  {\n"
           << "    \"host\": \"" << json_escape(r.target_host) << "\",\n"
           << "    \"resolved_ip\": \"" << json_escape(r.resolved_ip) << "\",\n"
           << "    \"port\": " << r.port << ",\n"
           << "    \"protocol\": \"" << json_escape(r.protocol) << "\",\n"
           << "    \"state\": \"" << state_str(r.state) << "\",\n"
           << "    \"service\": \"" << json_escape(r.service.name) << "\",\n"
           << "    \"product\": \"" << json_escape(r.service.product) << "\",\n"
           << "    \"version\": \"" << json_escape(r.service.version) << "\",\n"
           << "    \"rtt_ms\": " << r.rtt.count() << ",\n"
           << "    \"max_cvss\": " << r.max_cvss() << ",\n"
           << "    \"max_epss\": " << r.max_epss() << ",\n"
           << "    \"max_risk\": " << r.max_risk() << ",\n"
           << "    \"ja4s\": \"" << json_escape(r.ja4s) << "\",\n"
           << "    \"ja4x\": \"" << json_escape(r.ja4x) << "\",\n"
           << "    \"cdn\": \"" << json_escape(r.cdn) << "\",\n"
           << "    \"os_guess\": \"" << json_escape(r.os_guess) << "\",\n"
           << "    \"cves\": [";

        for (size_t j = 0; j < r.cves.size(); ++j) {
            const auto& cve = r.cves[j];
            ss << "\n      {"
               << "\"id\": \"" << json_escape(cve.cve_id) << "\", "
               << "\"cvss\": " << cve.cvss_score << ", "
               << "\"epss\": " << cve.epss_score << ", "
               << "\"verified\": " << (cve.nuclei_verified ? "true" : "false") << ", "
               << "\"description\": \"" << json_escape(cve.description) << "\""
               << "}";
            if (j + 1 < r.cves.size()) ss << ",";
        }

        ss << "\n    ]\n  }";
        if (i + 1 < results.size()) ss << ",";
        ss << "\n";
    }

    ss << "]\n";
    return ss.str();
}

} // namespace sps::report
