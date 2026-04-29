#pragma once
// ============================================================
//  src/core/result.hpp  —  ScanResult 구조체 (Qt 비의존 POD)
//
//  [제안] 김병옥 — GUI Views(#6) 모듈 담당
//  모든 모듈이 이 구조체를 공용으로 사용.
//  GUI model, CLI output, JSON/HTML/CSV export 모두
//  이 구조체 하나만 주고받음.
// ============================================================

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace sps::core {

// ── 포트 상태 ────────────────────────────────────
enum class PortState : uint8_t {
    Open,       // 응답 받음
    Closed,     // RST 받음
    Filtered,   // 타임아웃 / ICMP unreachable
    Unknown
};

// ── 심각도 (CVSS 기반) ───────────────────────────
enum class Severity : uint8_t {
    Critical,   // CVSS >= 9.0
    High,       // CVSS >= 7.0
    Medium,     // CVSS >= 4.0
    Low,        // CVSS <  4.0
    None
};

// ── CVE 정보 ─────────────────────────────────────
struct CveInfo {
    std::string cve_id;         // "CVE-2018-15473"
    std::string description;    // NVD 설명 (영문)
    float       cvss_score{0};  // 0.0 ~ 10.0
    Severity    severity{Severity::None};

    // ── 본인 제안 필드 ──
    float       epss_score{0};          // 0.0 ~ 1.0  (FIRST EPSS API)
    bool        nuclei_verified{false}; // nuclei PoC 검증 여부
    std::string nuclei_template_id;     // e.g. "CVE-2018-15473"
};

// ── 서비스 식별 결과 ─────────────────────────────
struct ServiceInfo {
    std::string name;       // "http", "ssh", "mysql", "redis", ...
    std::string product;    // "OpenSSH", "MySQL", "Redis", ...
    std::string version;    // "7.4", "5.7.32", "6.0.16", ...
    std::string extra_info; // OS 정보 등 부가 ("Ubuntu", "Debian")
    std::string banner_raw; // raw banner (8KB cap, truncated)
};

// ── 스캔 결과 (핵심 구조체) ──────────────────────
struct ScanResult {
    // ── 대상 정보 ──
    std::string target_host;    // IP 또는 hostname
    std::string resolved_ip;    // DNS resolve 결과 (hostname일 때)
    uint16_t    port{0};
    std::string protocol;       // "tcp" / "udp"
    PortState   state{PortState::Unknown};

    // ── 서비스 식별 (probe 결과) ──
    ServiceInfo service;

    // ── 취약점 정보 (CVE lookup 결과) ──
    std::vector<CveInfo> cves;

    // ── 타이밍 ──
    std::chrono::system_clock::time_point timestamp;
    std::chrono::milliseconds rtt{0};   // 응답 시간

    // ── 편의 메서드 ──
    [[nodiscard]] bool is_open() const noexcept {
        return state == PortState::Open;
    }

    [[nodiscard]] float max_cvss() const noexcept {
        float m = 0;
        for (const auto& c : cves) m = (c.cvss_score > m) ? c.cvss_score : m;
        return m;
    }

    [[nodiscard]] float max_epss() const noexcept {
        float m = 0;
        for (const auto& c : cves) m = (c.epss_score > m) ? c.epss_score : m;
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
