#!/bin/bash

if [[ $OSTYPE == msys ]]; then 
    echo "* MinGW-w64 detected."
    echo "* Installing dependencies..."
    pacman -S pactoys --noconfirm
    pacboy -S libtool: autoconf: automake: git: gcc:p make:p cmake:p bison: doxygen:p ruby:p \
    SDL2:p SDL2_image:p SDL2_ttf:p openal:p vim: freetype:p \
    physfs:p pixman:p libwebp:p zlib:p meson:p clang:p \
    bzip2:p libvorbis:p libogg:p zeromq:p libsigc++:p \
    boost:p libpng:p libjpeg-turbo:p libtiff:p harfbuzz:p --noconfirm
     
else 
    if [[ "$(cat /etc/issue)" == Debian* || "$(cat /etc/issue)" == Ubuntu* ]]; then
        echo "* Debian Linux detected."
        echo "* Installing dependencies..."
        sudo apt install -y gcc make cmake bison doxygen ruby \
         libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libopenal-dev \
         libphysfs-dev libpixman-1-dev libwebp-dev libbz2-dev \
         libvorbis-dev libogg-dev libsodium-dev libboost-dev libpng-dev \
         libjpeg-dev libtiff-dev libsigc++-2.0-dev meson vim libgtk2.0-dev \
         libxfconf-0-dev libfreetype-dev libharfbuzz-dev \

    fi

    if [[ "$(cat /etc/issue)" == Manjaro* ]]; then
        echo "* Manjaro detected."
        echo "* Installing dependencies..."
        sudo pacman --noconfirm -S gcc make cmake bison doxygen ruby \
        sdl2 openal pixman libwebp bzip2 libvorbis libogg libsodium \
        libpng libjpeg libtiff zeromq mm-common base-devel vim gtk2
        echo "* Installing dependencies with pamac..."
        sudo pamac install sdl2_image sdl2_ttf physfs boost boost-libs \
        libsigc++ sdl_sound m4 meson freetype harfbuzz --no-confirm
    fi

    if [[ $(cat /etc/redhat-release) ]]; then
        echo "* RedHat/Fedora Linux detected."
        echo "* Installing dependencies..."
        sudo dnf install gcc make m4 cmake bison doxygen ruby mm-common \
	    SDL2 SDL2-devel SDL2_image SDL2_tff SDL2_ttf-devel SDL2_image-devel \
        bzip2-devel libwebp libwebp-devel libvorbis libvorbis-devel \
        libpng libpng-devel libjpeg-turbo libjpeg-turbo-devel libogg \
	    libogg-devel libtiff libtiff-devel libsodium libsodium-devel \
	    zeromq zeromq-devel physfs physfs-devel pixman pixman-devel \
        bzip2 openal-soft speex speex-devel libmodplug libmodplug-devel \
        boost boost-devel openal-soft-devel xfconf xfconf-devel gtk2 gtk2-devel \
        vim meson libsigc++-devel harfbuzz-devel
    fi
fi