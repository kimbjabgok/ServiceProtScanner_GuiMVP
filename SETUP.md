# spscan 환경 셋업

## 사전 요구사항

| 도구 | 버전 | 설치 방법 |
|------|------|---------|
| Qt6 | 6.8+ | [Qt 온라인 설치기](https://www.qt.io/download-qt-installer) — Widgets, Charts 선택 |
| CMake | 3.25+ | 시스템 패키지 매니저 또는 cmake.org |
| vcpkg | latest | 아래 참조 |
| C++ 컴파일러 | C++20 지원 | GCC 12+ / MSVC 17+ / Clang 15+ |

## 1. vcpkg 설치 (1회)

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh   # Linux/Mac
cd vcpkg && bootstrap-vcpkg.bat    # Windows
```

환경변수 설정:

```bash
# Linux/Mac: ~/.bashrc 또는 ~/.zshrc
export VCPKG_ROOT=/path/to/vcpkg

# Windows: 시스템 환경변수에 VCPKG_ROOT=C:\vcpkg 추가
```

## 2. 빌드

### 방법 A: CMake Preset (추천)

```bash
cmake --preset default
cmake --build build/default --parallel
```

### 방법 B: Qt Creator

1. Qt Creator에서 `CMakeLists.txt` 열기
2. **첫 Configure 전에** Kit 설정 → CMake 변수 추가:
   - `CMAKE_TOOLCHAIN_FILE` = `%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake`
3. Configure 후에 추가하면 무시되므로, 이미 Configure 했다면 빌드 디렉터리 삭제 후 재시작
4. Build 버튼 클릭

### 방법 C: vcpkg 없이 (시스템 패키지만)

```bash
cmake --preset no-vcpkg
cmake --build build/no-vcpkg --parallel
```

이 경우 Boost, OpenSSL 등을 시스템에 직접 설치해야 함.

## 3. 실행

```bash
# Linux/Mac
./build/default/spscan-proto

# Windows (MinGW)
build\default\spscan-proto.exe

# Windows (MSVC — multi-config generator)
build\default\Debug\spscan-proto.exe
```

## 의존성 구조

| 라이브러리 | 관리 방식 | 용도 |
|-----------|---------|------|
| Qt6 (Widgets, Charts) | 시스템/Qt 설치기 | GUI |
| Boost.Asio | vcpkg | 비동기 네트워크 I/O |
| OpenSSL 3 | vcpkg | TLS 핑거프린트 (JA4) |
| nlohmann/json | vcpkg | JSON 파싱 |
| CLI11 | vcpkg | CLI 인자 파서 |
| Catch2 v3 | vcpkg | 테스트 |

## 트러블슈팅

### "Qt6 not found"

Qt 설치 경로를 CMake에 알려줘야 함. **컴파일러 ABI와 맞는 경로**를 사용:

```bash
# MinGW kit
cmake --preset default -DQt6_DIR=C:/Qt/6.11.0/mingw_64/lib/cmake/Qt6

# MSVC kit
cmake --preset default -DQt6_DIR=C:/Qt/6.11.0/msvc2022_64/lib/cmake/Qt6

# Linux (apt 설치)
cmake --preset default -DQt6_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt6
```

### "VCPKG_ROOT not set"

```bash
echo $VCPKG_ROOT        # Linux/Mac
echo %VCPKG_ROOT%       # Windows CMD
$env:VCPKG_ROOT         # Windows PowerShell
```
