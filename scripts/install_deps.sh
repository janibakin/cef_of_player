#!/usr/bin/env bash
set -euo pipefail

ARCH=$(uname -m)

COMMON_PACKAGES="build-essential cmake ninja-build git curl wget pkg-config \
  tar bzip2 unzip xz-utils \
  libgl1-mesa-dev libglu1-mesa-dev \
  libx11-dev libxinerama-dev libxcursor-dev libxi-dev \
  libxrandr-dev libxrender-dev libxfixes-dev libxtst-dev \
  libxcomposite-dev libxdamage-dev libx11-xcb-dev libxcb1-dev \
  libdrm-dev libgbm-dev mesa-utils \
  libpulse-dev libasound2-dev \
  libgtk-3-dev libnss3 libatk1.0-dev libatk-bridge2.0-dev \
  libcups2-dev libpango1.0-dev libssl-dev \
  patchelf zsync nlohmann-json3-dev libglew-dev libalut-dev \
  freeglut3-dev libglfw3-dev libgstreamer1.0-dev \
  gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
  gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly \
  libopenal-dev libsndfile1-dev kissfft-tools libkissfft-dev \
  libfreetype6-dev libfontconfig1-dev librtaudio-dev \
  libfreeimage-dev libjpeg-dev libmpg123-dev libudev-dev \
  libcurl4-openssl-dev liburiparser-dev libfmt-dev \
  libfontconfig-dev libsndfile-dev libgtk2.0-dev \
  gstreamer1.0-libav"

if [[ "ARCH" == "x86_64" ]]; then
  EXTRA_PACKAGES="libudev1:i386 libudev-dev:i386"
elif [[ "ARCH" == "aarch64" ]]; then
  EXTRA_PACKAGES="libpugixml-dev"
else
  EXTRA_PACKAGES=""
fi

sudo apt update
sudo apt install -y $COMMON_PACKAGES $EXTRA_PACKAGES
echo "[✓] System dependencies installed."
