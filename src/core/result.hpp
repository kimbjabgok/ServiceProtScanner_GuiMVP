#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace sps::core {

//포트 상태를 나타냄. Open, Closed, Filtered, Unknown 네 가지 상태가 있음.
enum class PortState : uint8_t {
    Open,
    Closed,
    Filtered,
    Unknown
};

//취약점 심각도를 나타냄. Critical, High, Medium, Low, None 다섯 단계가 있음.
enum class Severity : uint8_t {
    Critical,
    High,
    Medium,
    Low,
    None
};

//각각의 CVE에 대한 정보. CVE ID, 설명, CVSS 점수, 심각도, EPSS 점수, Nuclei 검증 여부 등을 포함함.
struct CveInfo {
    std::string cve_id;
    std::string description;
    float       cvss_score{0};
    Severity    severity{Severity::None};
    double      epss_score{0};
    bool        nuclei_verified{false};
};

//서비스 정보. 서비스 이름, 제품명, 버전, 추가 정보, 배너 원문 등을 포함함.
struct ServiceInfo {
    std::string name;
    std::string product;
    std::string version;
    std::string extra_info;
    std::string banner_raw;
};


//포트 스캔 결과를 나타내는 구조체. 각 포트마다 하나의 ScanResult가 생성됨. 
//대상 호스트, IP, 포트 번호, 프로토콜, 서비스 정보, CVE 목록 등을 포함함.
struct ScanResult {
    std::string target_host;
    std::string resolved_ip;
    uint16_t    port{0};
    std::string protocol;
    PortState   state{PortState::Unknown};

    ServiceInfo service;

    std::string ja4s;
    std::string ja4x;
    std::string cdn;
    std::string os_guess;

    std::vector<CveInfo> cves;

    //스캔이 수행된 시간과 RTT(왕복 시간) 정보를 포함함. 
    std::chrono::system_clock::time_point timestamp; //스캔이 완료된 시점
    std::chrono::milliseconds rtt{0};     //포트에 대한 응답 시간 

    //포트가 열려 있는지 여부를 반환하는 헬퍼 함수. Open 상태인 경우에만 true를 반환.
    [[nodiscard]] bool is_open() const noexcept {
        return state == PortState::Open;
    }

    //CVE 목록에서 가장 높은 CVSS 점수를 반환하는 함수. CVE가 없는 경우 0을 반환.
    [[nodiscard]] float max_cvss() const noexcept {
        float m = 0;
        for (const auto& c : cves) m = (c.cvss_score > m) ? c.cvss_score : m;
        return m;
    }

    //CVE 목록에서 가장 높은 EPSS 점수를 반환하는 함수. CVE가 없는 경우 0을 반환.
    [[nodiscard]] double max_epss() const noexcept {
        double m = 0;
        for (const auto& c : cves) m = (c.epss_score > m) ? c.epss_score : m;
        return m;
    }

    //CVE 목록에서 가장 높은 Risk 점수를 반환하는 함수. Risk는 CVSS × EPSS로 계산됨. CVE가 없는 경우 0을 반환.
    [[nodiscard]] double max_risk() const noexcept {
        double m = 0;
        for (const auto& c : cves) {
            const double risk = static_cast<double>(c.cvss_score) * c.epss_score;
            if (risk > m) m = risk;
        }
        return m;
    }

    //CVE 목록에 심각도가 Critical인 CVE가 하나라도 있는지 여부를 반환하는 함수.
    [[nodiscard]] bool has_critical_cve() const noexcept {
        for (const auto& c : cves)
            if (c.severity == Severity::Critical) return true;
        return false;
    }

    //CVE 목록에서 검증된 CVE의 개수를 반환하는 함수. nuclei_verified가 true인 CVE의 개수를 셈.
    [[nodiscard]] int verified_cve_count() const noexcept {
        int count = 0;
        for (const auto& c : cves)
            if (c.nuclei_verified) ++count;
        return count;
    }

    //CVE 목록에 검증된 CVE가 하나라도 있는지 여부를 반환하는 함수.
    //verified_cve_count()가 0보다 큰 경우 true를 반환.
    [[nodiscard]] bool has_verified_cve() const noexcept {
        return verified_cve_count() > 0;
    }
};

} 
