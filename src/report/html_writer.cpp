#include "html_writer.hpp"
#include <sstream>

namespace sps::report {

static const char* state_str(core::PortState s) {
    switch (s) {
        case core::PortState::Open:     return "open";
        case core::PortState::Closed:   return "closed";
        case core::PortState::Filtered: return "filtered";
        default:                        return "unknown";
    }
}

static std::string html_escape(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '&':  out += "&amp;";  break;
            case '<':  out += "&lt;";   break;
            case '>':  out += "&gt;";   break;
            case '"':  out += "&quot;"; break;
            default:   out += c;
        }
    }
    return out;
}

std::string HtmlWriter::write(const std::vector<core::ScanResult>& results) {
    std::ostringstream ss;
    ss << "<!DOCTYPE html>\n<html><head><meta charset='UTF-8'>\n"
       << "<title>SPS Scan Report</title>\n"
       << "<style>\n"
       << "body { font-family: 'Segoe UI', sans-serif; background: #1e1e1e; color: #ddd; padding: 20px; }\n"
       << "h1 { border-bottom: 2px solid #0078d7; padding-bottom: 8px; }\n"
       << "table { border-collapse: collapse; width: 100%; margin-top: 16px; }\n"
       << "th, td { border: 1px solid #444; padding: 8px; text-align: left; }\n"
       << "th { background: #333; }\n"
       << "tr:nth-child(even) { background: #2a2a2a; }\n"
       << ".critical { color: #ff4444; font-weight: bold; }\n"
       << ".high     { color: #ff8800; font-weight: bold; }\n"
       << ".medium   { color: #cccc00; }\n"
       << ".low      { color: #888; }\n"
       << ".cve-block { margin: 4px 0; padding: 6px; border-radius: 4px; }\n"
       << ".cve-critical { background: #4a1010; }\n"
       << ".cve-high     { background: #4a2a08; }\n"
       << ".cve-medium   { background: #3a3508; }\n"
       << ".cve-low      { background: #2a2a2a; }\n"
       << "</style></head><body>\n"
       << "<h1>Service Port Scanner &mdash; Report</h1>\n"
       << "<p>Total hosts scanned: results contain " << results.size() << " entries</p>\n"
       << "<table>\n<tr>"
       << "<th>Port</th><th>Proto</th><th>State</th><th>Service</th>"
       << "<th>Product</th><th>Version</th><th>CVEs</th><th>Max CVSS</th>"
       << "<th>EPSS</th><th>Risk</th><th>CDN</th><th>OS</th><th>Vulnerabilities</th>"
       << "</tr>\n";

    for (const auto& r : results) {
        float cvss = r.max_cvss();
        std::string css;
        if (cvss >= 9.0f)      css = "critical";
        else if (cvss >= 7.0f) css = "high";
        else if (cvss >= 4.0f) css = "medium";
        else                   css = "low";

        ss << "<tr>"
           << "<td>" << r.port << "</td>"
           << "<td>" << html_escape(r.protocol) << "</td>"
           << "<td>" << state_str(r.state) << "</td>"
           << "<td>" << html_escape(r.service.name) << "</td>"
           << "<td>" << html_escape(r.service.product) << "</td>"
           << "<td>" << html_escape(r.service.version) << "</td>"
           << "<td>" << r.cves.size() << "</td>"
           << "<td class='" << css << "'>" << cvss << "</td>"
           << "<td>" << r.max_epss() << "</td>"
           << "<td>" << r.max_risk() << "</td>"
           << "<td>" << html_escape(r.cdn) << "</td>"
           << "<td>" << html_escape(r.os_guess) << "</td>"
           << "<td>";

        for (const auto& cve : r.cves) {
            std::string cve_css;
            if (cve.cvss_score >= 9.0f)      cve_css = "cve-critical";
            else if (cve.cvss_score >= 7.0f)  cve_css = "cve-high";
            else if (cve.cvss_score >= 4.0f)  cve_css = "cve-medium";
            else                              cve_css = "cve-low";

            ss << "<div class='cve-block " << cve_css << "'>"
               << "<b>" << html_escape(cve.cve_id) << "</b> "
               << "(CVSS: " << cve.cvss_score << " | EPSS: "
               << static_cast<int>(cve.epss_score * 100) << "%)"
               << "</div>";
        }

        ss << "</td></tr>\n";
    }

    ss << "</table>\n</body></html>";
    return ss.str();
}

} // namespace sps::report
