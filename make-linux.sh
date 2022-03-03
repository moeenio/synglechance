#!/bin/sh
set -e

cd `dirname $0`

# User-configurable variables
linux_version="0.1.0"

# Colors
white="\033[0;37m"      # White - Regular
bold="\033[1;37m"       # White - Bold
cyan="\033[1;36m"       # Cyan - Bold
green="\033[1;32m"      # Green - Bold
color_reset="\033[0m"   # Reset Colors



echo -e "${white}Compiling ${bold}SyngleChance v${linux_version} ${white}engine for Linux...${color_reset}\n"

mkdir -p build
cd build

# Generate makefile and build main
echo "-> ${cyan}Install dependencies...${color_reset}"
conan install .. --build=missing -o platform=$([ $with_steamshim == true ] && echo "steam" || echo "standalone") -o debug=$([ $debug == true ] && echo "True" || echo "False") -s arch=x86_64
echo "-> ${cyan}Compile engine...${color_reset}"
conan build ..

# Build journal
echo -e "-> ${cyan}Compile journal...${color_reset}"
cd ..
pyinstaller journal/unix/journal.spec --windowed

# Compile scripts
echo -e "-> ${cyan}Compile xScripts.rxdata...${color_reset}"
mkdir -p build/Data
ruby rpgscript.rb ./scripts ./build

echo -e "\n${green}Complete!  ${white}Please report any issues to https://github.com/GooborgStudios/synglechance/issues${color_reset}"
