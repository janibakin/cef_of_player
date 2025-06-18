#!/usr/bin/env bash
set -euo pipefail

sudo apt update
sudo apt install -y build-essential cmake ninja-build git curl wget pkg-config \
  tar bzip2 unzip xz-utils \
  libgl1-mesa-dev libglu1-mesa-dev \
  libx11-dev libxinerama-dev libxcursor-dev libxi-dev \
  libxrandr-dev libxrender-dev libxfixes-dev libxtst-dev \
  libxcomposite-dev libxdamage-dev libx11-xcb-dev libxcb1-dev \
  libdrm-dev libgbm-dev mesa-utils \
  libpulse-dev libasound2-dev libudev1:i386 libudev-dev:i386 \
  libgtk-3-dev libnss3 libatk1.0-dev libatk-bridge2.0-dev \
  libcups2-dev libpango1.0-dev libssl-dev \
  patchelf zsync nlohmann-json3-dev libglew-dev libalut-dev \
  freeglut3-dev libglfw3-dev libgstreamer1.0-dev \
  libgstreamer-plugins-base1.0-dev \
  libopenal-dev libsndfile1-dev kissfft-tools libkissfft-dev\
  libfreetype6-dev libfontconfig1-dev librtaudio-dev \
  libfreeimage-dev libjpeg-dev libmpg123-dev libudev-dev\
  libcurl4-openssl-dev liburiparser-dev libfmt-dev \
  libfontconfig-dev libsndfile-dev libgtk2.0-dev \
  gstreamer1.0-libav gstreamer1.0-plugins-{base,good,bad,ugly}
echo "[✓] System dependencies installed."