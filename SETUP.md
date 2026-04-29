# Qt6 Prototype 환경 셋업

## Ubuntu (22.04 / 24.04)

```bash
# 1. Qt6 + Charts 설치
sudo apt update
sudo apt install -y \
    qt6-base-dev \
    qt6-charts-dev \
    cmake \
    build-essential \
    g++-12

# 2. 빌드
cd schema_proposal
cmake -B build -S .
cmake --build build -j$(nproc)

# 3. 실행
./build/spscan-proto
```

## vcpkg 방식 (팀 프로젝트 공식 방식)

```bash
# 1. vcpkg 설치 (이미 있으면 스킵)
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh
export VCPKG_ROOT=$(pwd)

# 2. vcpkg.json에 의존성 명시 (프로젝트 루트에)
cat > vcpkg.json << 'EOF'
{
    "name": "spscan",
    "version": "0.1.0",
    "dependencies": [
        "qtbase",
        "qtcharts"
    ]
}
EOF

# 3. 빌드 (vcpkg 자동 설치)
cmake -B build -S . \
    -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build -j$(nproc)
```

## 주의: 어떤 방식 선택?

| 방식 | 장점 | 단점 |
|------|------|------|
| apt (시스템 Qt) | 설치 3분 | 팀 버전과 다를 수 있음 |
| vcpkg | 팀 통일 | 첫 빌드 Qt 컴파일 30분+ |

### 추천: prototype은 apt로 빠르게 시작 → 팀 통합 시 vcpkg로 전환

prototype은 Qt API만 쓰고 팀 빌드시스템과 독립적이라
apt로 빠르게 돌려보고, 나중에 팀 repo에 머지할 때 vcpkg 전환해도 코드 변경 없음.

## 트러블슈팅

### "Qt6Charts not found"
```bash
# Ubuntu 22.04는 qt6-charts-dev 패키지명 확인
apt search qt6 | grep chart
```

### "CMAKE_AUTOMOC requires Qt"
```bash
# Qt6_DIR 직접 지정
cmake -B build -S . -DQt6_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt6
```

### GUI 안 뜰 때 (SSH 원격 환경)
```bash
# X11 포워딩
ssh -X user@host
# 또는 DISPLAY 환경변수
export DISPLAY=:0
```
