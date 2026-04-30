#pragma once
// pragma는 헤더가 중복 include 되어도 한 번만 처리
// ============================================================
//  src/core/result.hpp -- ScanResult shared POD types
//
//  GUI model, CLI output, report exporters.
// ============================================================

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace sps::core {

// Port 상태
enum class PortState : uint8_t {
    Open,
    Closed,
    Filtered,
    Unknown
};

// CVE 심각도 등급. CVSS 점수에 따라 Critical, High, Medium, Low, None으로 분류
enum class Severity : uint8_t {
    Critical,   // CVSS >= 9.0
    High,       // CVSS >= 7.0
    Medium,     // CVSS >= 4.0
    Low,        // CVSS <  4.0
    None
};

// CVE 정보 구조체
struct CveInfo {
    std::string cve_id;         // "CVE-2018-15473"
    std::string description;    // NVD description
    float       cvss_score{0};  // 0.0 ~ 10.0
    Severity    severity{Severity::None};
    double      epss_score{0};          // 0.0 ~ 1.0  (EPSS API사용)
};

// 포트에서 식별된 서비스 정보
struct ServiceInfo {
    std::string name;       // "http", "ssh", ...
    std::string product;    // "OpenSSH", ...
    std::string version;    // "7.4", "5.7.32", "6.0.16", ...
    std::string extra_info; // OS details , 다른 service info
    std::string banner_raw; // 서버가 실제로 응답한 원본 배너 문자열
};

// 스캔 결과의 핵심 구조체 ; GUI 모델, CLI 출력, 보고서 생성 등에서 공통으로 사용
struct ScanResult {
    std::string target_host;    // IP or hostname
    std::string resolved_ip;    // DNS result
    uint16_t    port{0};
    std::string protocol;       // "tcp" / "udp"
    PortState   state{PortState::Unknown};

    ServiceInfo service;

    std::string ja4s;       // JA4S server fingerprint (TLS ServerHello 기반)
    std::string ja4x;       // JA4X certificate fingerprint (인증서 기반 지문)
    std::string cdn;        // CDN/WAF tag ("cloudflare", "akamai", ...)
    std::string os_guess;   // OS guess ("Linux 5.x", "Windows 10", ...)

    std::vector<CveInfo> cves; //CVE 목록 (취약점이 없는 경우 빈 벡터)

    std::chrono::system_clock::time_point timestamp; //스캔 시각
    std::chrono::milliseconds rtt{0}; // 응답 시간

    /*
    const는 함수가 객체를 수정하지 않음, noexcept는 예외를 던지지 않음,
    [[nodiscard]]는 반환값을 무시하지 말라는 뜻
    */

    //포트가 열려 있는지 확인
    [[nodiscard]] bool is_open() const noexcept {
        return state == PortState::Open;
    }

    //cvss 점수의 최대값을 계산 ; cve 목록 안에서 가장 높은 cvss 점수를 반환.
    [[nodiscard]] float max_cvss() const noexcept {
        float m = 0;
        for (const auto& c : cves) m = (c.cvss_score > m) ? c.cvss_score : m;
        return m;
    }

    //cve 목록 안에서 가장 높은 EPSS 점수를 반환.
    [[nodiscard]] double max_epss() const noexcept {
        double m = 0;
        for (const auto& c : cves) m = (c.epss_score > m) ? c.epss_score : m;
        return m;
    }

    /*
    Risk score = CVSS * EPSS
    CVSS: 0.0 ~ 10.0, EPSS: 0.0 ~ 1.0 => Risk: 0.0 ~ 10.0
    */

    // cve 목록 안에서 CVSS * EPSS의 최대값을 계산하여 반환.
    [[nodiscard]] double max_risk() const noexcept {
        double m = 0;
        for (const auto& c : cves) {
            double risk = static_cast<double>(c.cvss_score) * c.epss_score;
            if (risk > m) m = risk;
        }
        return m;
    }

    //  CVE 목록 중 하나라도 Critical 등급이 있으면 true를 반환
    [[nodiscard]] bool has_critical_cve() const noexcept {
        for (const auto& c : cves)
            if (c.severity == Severity::Critical) return true;
        return false;
    }
};

} 