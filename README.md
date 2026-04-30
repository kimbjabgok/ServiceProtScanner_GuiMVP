# spscan

C++20과 Qt6 기반의 서비스 포트 스캐너 GUI 프로토타입입니다.

스캔 결과를 테이블, 상세 패널, CVSS/EPSS 기반 위험도 차트로 확인하고,
CSV, JSON, HTML 형식으로 내보내는 것을 목표로 합니다.

## 주요 기능

- Qt6 Widgets 기반 데스크톱 GUI
- 포트, 서비스, CVE 중심의 결과 모델 (`ResultModel`)
- CVSS, EPSS, Risk Score 시각화 delegate
- nuclei 검증 여부를 표시하는 Verified 컬럼
- JA4S, JA4X, CDN, OS 추정 정보 컬럼
- Risk Priority Matrix 차트
- CSV, JSON, HTML 리포트 export
- Qt Designer `.ui` 파일 기반 레이아웃 관리
- vcpkg manifest 기반 의존성 관리

## 의존성 정책

Qt6는 vcpkg로 관리하지 않고 시스템 설치 또는 Qt 온라인 설치기를 사용합니다.
나머지 백엔드/테스트 의존성은 `vcpkg.json`으로 관리합니다.

| 라이브러리 | 관리 방식 | 용도 |
|-----------|---------|------|
| Qt6 Widgets, Charts | 시스템/Qt 설치기 | GUI |
| Boost.Asio | vcpkg | 비동기 네트워크 I/O |
| OpenSSL | vcpkg | TLS 핑거프린트 |
| nlohmann/json | vcpkg | JSON 파싱 |
| CLI11 | vcpkg | CLI 인자 파서 |
| Catch2 | vcpkg | 테스트 |

## 빌드

자세한 환경 구성은 [SETUP.md](SETUP.md)를 참고하세요.

Qt GUI 개발 중에는 Qt Creator 방식이 가장 편합니다.

1. Qt Creator에서 `CMakeLists.txt`를 엽니다.
2. 첫 Configure 전에 Kit 설정에 `CMAKE_TOOLCHAIN_FILE`을 추가합니다.
3. Build 버튼으로 빌드합니다.

터미널 또는 CI에서는 CMake Preset을 사용할 수 있습니다.

```bash
cmake --preset default
cmake --build build/default --parallel
```

vcpkg 없이 시스템 패키지만 사용할 수도 있습니다.

```bash
cmake --preset no-vcpkg
cmake --build build/no-vcpkg --parallel
```

## 실행

```bash
# Linux/Mac
./build/default/spscan-proto

# Windows MinGW
build\default\spscan-proto.exe

# Windows MSVC multi-config
build\default\Debug\spscan-proto.exe
```

## 프로젝트 구조

```text
spscan/
├── CMakeLists.txt
├── CMakePresets.json
├── vcpkg.json
├── SETUP.md
├── src/
│   ├── main.cpp
│   ├── core/
│   │   └── result.hpp
│   ├── gui/views/
│   │   ├── main_window.hpp/.cpp/.ui
│   │   ├── result_model.hpp/.cpp
│   │   ├── details_panel.hpp/.cpp/.ui
│   │   ├── charts_panel.hpp/.cpp
│   │   ├── cvss_delegate.hpp/.cpp
│   │   ├── epss_delegate.hpp/.cpp
│   │   ├── risk_delegate.hpp/.cpp
│   │   └── export_actions.hpp/.cpp
│   └── report/
│       ├── csv_writer.hpp/.cpp
│       ├── html_writer.hpp/.cpp
│       └── json_writer.hpp/.cpp
└── tests/
    └── dummy_data.hpp
```

## 현재 범위

이 저장소는 GUI MVP/prototype 단계입니다. 현재는 더미 데이터를 기반으로
결과 테이블, 상세 패널, 차트, export 흐름을 검증합니다. 실제 스캐너,
서비스 probe, CVE lookup, TLS fingerprint 모듈은 이후 백엔드 모듈로
연결될 예정입니다.
