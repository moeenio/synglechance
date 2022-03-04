#!/bin/sh
set -e

cd `dirname $0`

# User-configurable variables
mac_version="1.2.0"
with_steamshim=${STEAM:-true}
debug=${DEBUG:-false}
arm=${ARM:-false}

# Colors
white="\033[0;37m"      # White - Regular
bold="\033[1;37m"       # White - Bold
cyan="\033[1;36m"       # Cyan - Bold
green="\033[1;32m"      # Green - Bold
color_reset="\033[0m"   # Reset Colors



echo "${white}Compiling ${bold}SyngleChance v${mac_version} ${white}engine for macOS...${color_reset}\n"

mkdir -p build
cd build

# Set version number -- being replaced by CMake steps
echo "-> ${cyan}Set version number...${color_reset}"
m4 ../patches/mac/JournalInfo.plist.in -DONESHOTMACVERSION=$mac_version > ./JournalInfo.plist

# Generate makefile and build main
echo "-> ${cyan}Install dependencies...${color_reset}"
conan install .. --build=missing -o platform=$([ $with_steamshim == true ] && echo "steam" || echo "standalone") -o debug=$([ $debug == true ] && echo "True" || echo "False") -s arch=$([ $arm == true ] && echo "armv8" || echo "x86_64") -s os.version=$([ $arm == true ] && echo "11" || echo "10.10")
echo "-> ${cyan}Compile engine...${color_reset}"
conan build ..

# Build journal
echo "-> ${cyan}Compile journal...${color_reset}"
# cd ..
# pyinstaller journal/unix/journal.spec --onefile --windowed
# rm -rf build/_______
# rm -rf build/_______.app
# mv dist/* build
# rm -r dist
# rm -rf journal/unix/__pycache__

# Create app bundles
echo "-> ${cyan}Create app bundles...${color_reset}"
OSX_App="./build/bin/OneShot.app"
ContentsDir="$OSX_App/Contents"
LibrariesDir="$OSX_App/Contents/Libraries"
ResourcesDir="$OSX_App/Contents/Resources"

# create directories in the @target@.app bundle
mkdir -p "$LibrariesDir"
mkdir -p "$ResourcesDir"

# Steamshim
if [[ $with_steamshim == true ]]; then
	cp build/bin/steamshim $OSX_App/Contents/MacOS/steamshim
	install_name_tool -change @loader_path/libsteam_api.dylib "$( cd "$(dirname "$0")" ; pwd -P )"/steamworks/redistributable_bin/osx/libsteam_api.dylib $OSX_App/Contents/macOS/steamshim
fi

# Move files into proper locations
cp assets/icon.icns $OSX_App/Contents/Resources/icon.icns
# cp assets/icon_journal.icns build/_______.app/Contents/Resources/icon_journal.icns
cp steam_appid.txt $OSX_App/Contents/MacOS/steam_appid.txt
cp patches/mac/oneshot.sh $OSX_App/Contents/MacOS/oneshot.sh
cp -r build/lib/* $LibrariesDir
# rm -f build/_______.app/Contents/Info.plist
# cp build/JournalInfo.plist build/_______.app/Contents/Info.plist

# Complete OneShot bundle
cmake -P patches/mac/CompleteBundle.cmake

echo "\n${green}Complete!  ${white}Please report any issues to https://github.com/GooborgStudios/synglechance/issues${color_reset}"
