# ScanResult 구조체 초안 제안

## 배경
GUI Views (#6) 작업 시작하려는데 백엔드 데이터 스키마가 아직 미정이라,
먼저 초안 올립니다. 의견 주시면 수정할게요.

## 설계 원칙
- **Qt 비의존**: `src/core/`에 위치, Qt 헤더 include 없음 (기획안 원칙 준수)
- **단일 구조체**: 모든 모듈이 `ScanResult` 하나만 주고받음
- **POD 지향**: 직렬화(JSON/CSV) 쉽게

## 구조 요약

```
ScanResult
├── target_host / resolved_ip / port / protocol / state
├── ServiceInfo
│   ├── name        ("http", "ssh", "mysql", "redis")
│   ├── product     ("OpenSSH", "MySQL", "Redis")
│   ├── version     ("7.4", "5.7.32")
│   ├── extra_info  ("Ubuntu")
│   └── banner_raw  (8KB cap)
├── vector<CveInfo>
│   ├── cve_id / description / cvss_score / severity
│   ├── epss_score      ← FIRST EPSS API (무료)
│   └── nuclei_verified ← PoC 검증 여부 (선택)
└── timestamp / rtt
```

## 모듈별 책임

| 필드 | 채우는 모듈 |
|------|------------|
| target_host, port, protocol, state, rtt | Core Scanner (#1) |
| ServiceInfo (name, product, version, banner) | Probes (#3) |
| CveInfo (cve_id, cvss, description) | CVE Lookup (#4) |
| epss_score | CVE Lookup (#4) — FIRST EPSS API |
| nuclei_verified | 선택 (추후 논의) |
| timestamp | Orchestrator (#7) |

## EPSS 필드 추가 이유
- CVSS만으로는 "이론적 심각도"만 알 수 있음
- EPSS = "실제로 공격에 쓰일 확률" (0~1)
- GUI에서 CVSS × EPSS 산점도로 우선순위 매트릭스 시각화 예정
- API: https://api.first.org/data/v1/epss?cve=CVE-XXXX (무료, 인증 불필요)

## 헤더 파일
`src/core/result.hpp` 에 올려뒀습니다. 리뷰 부탁드립니다.

cc: Core(#1) / Probes(#3) / CVE(#4) 담당자
