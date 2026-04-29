#pragma once
// ============================================================
//  src/core/result.hpp -- ScanResult shared POD types
//
//  Used by GUI model, CLI output, and report exporters.
// ============================================================

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace sps::core {

enum class PortState : uint8_t {
    Open,
    Closed,
    Filtered,
    Unknown
};

enum class Severity : uint8_t {
    Critical,   // CVSS >= 9.0
    High,       // CVSS >= 7.0
    Medium,     // CVSS >= 4.0
    Low,        // CVSS <  4.0
    None
};

struct CveInfo {
    std::string cve_id;         // "CVE-2018-15473"
    std::string description;    // NVD description
    float       cvss_score{0};  // 0.0 ~ 10.0
    Severity    severity{Severity::None};

    double      epss_score{0};          // 0.0 ~ 1.0  (FIRST EPSS API사용)
    bool        nuclei_verified{false}; // proposed - unconfirmed
    std::string nuclei_template_id;     // proposed - unconfirmed
};

struct ServiceInfo {
    std::string name;       // "http", "ssh", ...
    std::string product;    // "OpenSSH", ...
    std::string version;    // "7.4", "5.7.32", "6.0.16", ...
    std::string extra_info; // OS details or other service info
    std::string banner_raw; // raw banner (8KB cap, truncated)
};

struct ScanResult {
    std::string target_host;    // IP or hostname
    std::string resolved_ip;    // DNS result
    uint16_t    port{0};
    std::string protocol;       // "tcp" / "udp"
    PortState   state{PortState::Unknown};

    ServiceInfo service;

    std::string ja4s;       // JA4S server fingerprint (TLS ServerHello)
    std::string ja4x;       // JA4X certificate fingerprint
    std::string cdn;        // CDN/WAF tag ("cloudflare", "akamai", ...)
    std::string os_guess;   // OS guess ("Linux 5.x", "Windows 10", ...)

    std::vector<CveInfo> cves;

    std::chrono::system_clock::time_point timestamp;
    std::chrono::milliseconds rtt{0};

    [[nodiscard]] bool is_open() const noexcept {
        return state == PortState::Open;
    }

    [[nodiscard]] float max_cvss() const noexcept {
        float m = 0;
        for (const auto& c : cves) m = (c.cvss_score > m) ? c.cvss_score : m;
        return m;
    }

    [[nodiscard]] double max_epss() const noexcept {
        double m = 0;
        for (const auto& c : cves) m = (c.epss_score > m) ? c.epss_score : m;
        return m;
    }

    [[nodiscard]] double max_risk() const noexcept {
        double m = 0;
        for (const auto& c : cves) {
            double risk = static_cast<double>(c.cvss_score) * c.epss_score;
            if (risk > m) m = risk;
        }
        return m;
    }

    [[nodiscard]] bool has_critical_cve() const noexcept {
        for (const auto& c : cves)
            if (c.severity == Severity::Critical) return true;
        return false;
    }

    [[nodiscard]] bool has_verified_cve() const noexcept {
        for (const auto& c : cves)
            if (c.nuclei_verified) return true;
        return false;
    }
};

} // namespace sps::core
