#pragma once
// ============================================================
//  tests/dummy_data.hpp  —  GUI prototype용 더미 데이터
//
//  백엔드 완성 전에 GUI 개발할 때 사용.
//  실제 Metasploitable2 스캔 결과와 비슷하게 만듦.
// ============================================================

#include "core/result.hpp"
#include <vector>

namespace sps::test {

inline std::vector<core::ScanResult> make_dummy_results() {
    using namespace core;
    std::vector<ScanResult> results;

    // ── 1. SSH (OpenSSH 7.4 — 유명한 user enum 취약점) ──
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 22;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(3);

        r.service.name       = "ssh";
        r.service.product    = "OpenSSH";
        r.service.version    = "7.4";
        r.service.extra_info = "Ubuntu 18.04";
        r.service.banner_raw = "SSH-2.0-OpenSSH_7.4";

        CveInfo cve;
        cve.cve_id          = "CVE-2018-15473";
        cve.description     = "OpenSSH user enumeration vulnerability";
        cve.cvss_score      = 5.3f;
        cve.severity        = Severity::Medium;
        cve.epss_score      = 0.85f;   // 실제 공격 활발
        cve.nuclei_verified = true;
        cve.nuclei_template_id = "CVE-2018-15473";
        r.cves.push_back(cve);

        results.push_back(std::move(r));
    }

    // ── 2. HTTP (Apache 2.4.29 — 다수 CVE) ──
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 80;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(1);

        r.service.name       = "http";
        r.service.product    = "Apache httpd";
        r.service.version    = "2.4.29";
        r.service.extra_info = "Ubuntu";
        r.service.banner_raw = "HTTP/1.1 200 OK\r\nServer: Apache/2.4.29 (Ubuntu)";

        CveInfo cve1;
        cve1.cve_id      = "CVE-2019-0211";
        cve1.description = "Apache privilege escalation via scoreboard manipulation";
        cve1.cvss_score  = 7.8f;
        cve1.severity    = Severity::High;
        cve1.epss_score  = 0.72f;
        r.cves.push_back(cve1);

        CveInfo cve2;
        cve2.cve_id      = "CVE-2021-41773";
        cve2.description = "Apache path traversal and RCE";
        cve2.cvss_score  = 9.8f;
        cve2.severity    = Severity::Critical;
        cve2.epss_score  = 0.97f;
        cve2.nuclei_verified = true;
        r.cves.push_back(cve2);

        results.push_back(std::move(r));
    }

    // ── 3. MySQL (5.7.32 — DB 외부 노출 시나리오) ──
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 3306;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(2);

        r.service.name       = "mysql";
        r.service.product    = "MySQL";
        r.service.version    = "5.7.32";
        r.service.extra_info = "Ubuntu 18.04";
        r.service.banner_raw = "5.7.32-0ubuntu0.18.04.1";

        CveInfo cve;
        cve.cve_id      = "CVE-2020-14812";
        cve.description = "MySQL Server Optimizer DoS vulnerability";
        cve.cvss_score  = 4.9f;
        cve.severity    = Severity::Medium;
        cve.epss_score  = 0.12f;
        r.cves.push_back(cve);

        results.push_back(std::move(r));
    }

    // ── 4. Redis (6.0.16 — 무인증 외부 노출!) ──
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 6379;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(1);

        r.service.name       = "redis";
        r.service.product    = "Redis";
        r.service.version    = "6.0.16";
        r.service.extra_info = "Linux 5.4.0, no auth";
        r.service.banner_raw = "# Server\r\nredis_version:6.0.16\r\nos:Linux 5.4.0";

        CveInfo cve;
        cve.cve_id          = "CVE-2022-0543";
        cve.description     = "Redis Lua sandbox escape leading to RCE";
        cve.cvss_score      = 10.0f;
        cve.severity        = Severity::Critical;
        cve.epss_score      = 0.94f;
        cve.nuclei_verified = true;
        cve.nuclei_template_id = "CVE-2022-0543";
        r.cves.push_back(cve);

        results.push_back(std::move(r));
    }

    // ── 5. FTP (vsftpd 2.3.4 — 백도어 내장 버전) ──
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 21;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(2);

        r.service.name       = "ftp";
        r.service.product    = "vsftpd";
        r.service.version    = "2.3.4";
        r.service.banner_raw = "220 (vsFTPd 2.3.4)";

        CveInfo cve;
        cve.cve_id          = "CVE-2011-2523";
        cve.description     = "vsftpd 2.3.4 backdoor command execution";
        cve.cvss_score      = 9.8f;
        cve.severity        = Severity::Critical;
        cve.epss_score      = 0.91f;
        cve.nuclei_verified = true;
        r.cves.push_back(cve);

        results.push_back(std::move(r));
    }

    // ── 6. HTTPS (443 — filtered) ──
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 443;
        r.protocol    = "tcp";
        r.state       = PortState::Filtered;
        r.rtt         = std::chrono::milliseconds(0);
        results.push_back(std::move(r));
    }

    // ── 7. Telnet (closed) ──
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 23;
        r.protocol    = "tcp";
        r.state       = PortState::Closed;
        r.rtt         = std::chrono::milliseconds(1);
        results.push_back(std::move(r));
    }

    return results;
}

} // namespace sps::test
