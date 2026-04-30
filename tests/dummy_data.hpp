#pragma once
//  tests/dummy_data.hpp -> GUI prototype demo data 용도

#include "core/result.hpp"
#include <vector>

namespace sps::test {

inline std::vector<core::ScanResult> make_dummy_results() {
    using namespace core;
    std::vector<ScanResult> results;

    // 1. SSH probe
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 22;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(3);
        r.os_guess    = "Linux 5.4";

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
        cve.epss_score      = 0.85;
        r.cves.push_back(cve);

        results.push_back(r);
    }

    // 2. HTTP probe
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 80;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(1);
        r.cdn         = "cloudflare";
        r.os_guess    = "Linux 5.4";

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
        cve1.epss_score  = 0.72;
        r.cves.push_back(cve1);

        CveInfo cve2;
        cve2.cve_id      = "CVE-2021-41773";
        cve2.description = "Apache path traversal and RCE";
        cve2.cvss_score  = 9.8f;
        cve2.severity    = Severity::Critical;
        cve2.epss_score  = 0.97;
        r.cves.push_back(cve2);

        results.push_back(r);
    }

    // 3. TLS probe
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 443;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(2);
        r.ja4s        = "t13d3112h2_e8f1e7e78f70_6bebaf5329ac";
        r.ja4x        = "a1b2c3d4e5f6a7b8_c32a5380a08cec8d_4f48e4e00304ed25";
        r.cdn         = "cloudflare";
        r.os_guess    = "Linux 5.4";

        r.service.name       = "https";
        r.service.product    = "nginx";
        r.service.version    = "1.18.0";
        r.service.extra_info = "Ubuntu";
        r.service.banner_raw = "HTTP/1.1 200 OK\r\nServer: nginx/1.18.0";

        CveInfo cve;
        cve.cve_id      = "CVE-2021-23017";
        cve.description = "nginx DNS resolver off-by-one heap write";
        cve.cvss_score  = 9.4f;
        cve.severity    = Severity::Critical;
        cve.epss_score  = 0.04;
        r.cves.push_back(cve);

        results.push_back(r);
    }

    // 4. SMTP probe
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 25;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(4);
        r.os_guess    = "Linux 5.4";

        r.service.name       = "smtp";
        r.service.product    = "Postfix";
        r.service.version    = "3.3.0";
        r.service.extra_info = "Ubuntu 18.04";
        r.service.banner_raw = "220 mail.local ESMTP Postfix";

        CveInfo cve;
        cve.cve_id      = "CVE-2019-3462";
        cve.description = "Postfix SMTP relay open relay misconfiguration vector";
        cve.cvss_score  = 3.7f;
        cve.severity    = Severity::Low;
        cve.epss_score  = 0.82;
        r.cves.push_back(cve);

        results.push_back(r);
    }

    // 5. FTP probe
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 21;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(2);
        r.os_guess    = "Linux 5.4";

        r.service.name       = "ftp";
        r.service.product    = "vsftpd";
        r.service.version    = "2.3.4";
        r.service.banner_raw = "220 (vsFTPd 2.3.4)";

        CveInfo cve;
        cve.cve_id          = "CVE-2011-2523";
        cve.description     = "vsftpd 2.3.4 backdoor command execution";
        cve.cvss_score      = 9.8f;
        cve.severity        = Severity::Critical;
        cve.epss_score      = 0.91;
        r.cves.push_back(cve);

        results.push_back(r);
    }
    // GUI 테스트용 더미 데이터. 실제 probe 구현 여부와 무관하게
    // 다양한 서비스/상태 조합을 포함하여 UI 렌더링 검증용으로 사용.

    // 6. Service diversity
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 3306;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(2);
        r.os_guess    = "Linux 5.4";

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
        cve.epss_score  = 0.12;
        r.cves.push_back(cve);

        results.push_back(r);
    }

    // 7. Service diversity
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 6379;
        r.protocol    = "tcp";
        r.state       = PortState::Open;
        r.rtt         = std::chrono::milliseconds(1);
        r.os_guess    = "Linux 5.4";

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
        cve.epss_score      = 0.94;
        r.cves.push_back(cve);

        results.push_back(r);
    }

    // 8. State diversity
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 8443;
        r.protocol    = "tcp";
        r.state       = PortState::Filtered;
        r.rtt         = std::chrono::milliseconds(0);
        results.push_back(r);
    }

    // 9. State diversity
    {
        ScanResult r;
        r.target_host = "192.168.122.10";
        r.resolved_ip = "192.168.122.10";
        r.port        = 23;
        r.protocol    = "tcp";
        r.state       = PortState::Closed;
        r.rtt         = std::chrono::milliseconds(1);
        results.push_back(r);
    }

    return results;
}

} // namespace sps::test
