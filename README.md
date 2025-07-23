# 🎥 cef_of_player

Simple demo application, integrating **Chromium Embedded Framework** and **openFrameworks** under **CMake/Ninja**.
It draws a bouncing ball animation on the top of the window using Cef, and renders a video on the bottom of the screen,
using openFrameworks.

---
## 0 Prerequisites

```
Tested on x86_64
* Ubuntu 24.04 - Kubuntu installed natively
* On Windows 11 - WSL2 Ubuntu 24.04
* Raspbian OS 12 (Bookworm) - Raspberry Pi 4B - 2GB RAM
```

## 1  Clone and build

```
git clone https://github.com/janibakin/cef_of_player.git
cd cef_of_player
```
## 2 Download openFrameworks binary libs
```
mkdir -p third_party/openFrameworks
cd third_party/openFrameworks

For x86_64 Linux:
wget -c https://github.com/openframeworks/openFrameworks/releases/download/0.12.1/of_v0.12.1_linux64_gcc6_release.tar.gz
tar xvf of_v0.12.1_linux64_gcc6_release.tar.gz --strip-components=1
rm -rf of_v0.12.1_linux64_gcc6_release.tar.gz
cd ../../

For ARM64 Linux:
wget -c https://github.com/openframeworks/openFrameworks/releases/download/0.12.1/of_v0.12.1_linuxaarch64_release.tar.gz
tar xvf of_v0.12.1_linuxaarch64_release.tar.gz --strip-components=1
rm -rf of_v0.12.1_linuxaarch64_release.tar.gz
cd ../../
```

## 3 Download CEF binary libs
```
mkdir -p third_party/cef_binary
cd third_party/cef_binary

# For x86_64 Linux:
wget -c https://cef-builds.spotifycdn.com/cef_binary_137.0.17%2Bgf354b0e%2Bchromium-137.0.7151.104_linux64_minimal.tar.bz2
tar xvf cef_binary_137.0.17+gf354b0e+chromium-137.0.7151.104_linux64_minimal.tar.bz2
mv cef_binary_137.0.17+gf354b0e+chromium-137.0.7151.104_linux64_minimal/* .
rm -rf cef_binary_137.0.17+gf354b0e+chromium-137.0.7151.104_linux64_minimal*
cd ../../

# For ARM64 Linux:
wget -c https://cef-builds.spotifycdn.com/cef_binary_137.0.17+gf354b0e+chromium-137.0.7151.104_linuxarm64_minimal.tar.bz2
tar xvf cef_binary_137.0.17+gf354b0e+chromium-137.0.7151.104_linuxarm64_minimal.tar.bz2
mv cef_binary_137.0.17+gf354b0e+chromium-137.0.7151.104_linuxarm64_minimal/* .
rm -rf cef_binary_137.0.17+gf354b0e+chromium-137.0.7151.104_linuxarm64_minimal*
cd ../../
```


## 3 Install build dependencies (Ubuntu 24.04)
```
./scripts/install_deps.sh
```
## 4 Build & Run
```
# For x86_64 Linux:
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
# For ARM64 Linux:
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DPROJECT_ARCH=arm64

cmake --build build -j$(nproc)
./build/bin/simpleapp
```

## 5 Repo layout
```
cef_of_player/
├─ CMakeLists.txt
├─ README.md
├─ simpleapp/
├─ scripts/
└─ third_party/
   └─ openFrameworks/
   └─ cef_binary/
```