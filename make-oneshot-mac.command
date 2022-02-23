#!/bin/sh
set -e

cd `dirname $0`

# User-configurable variables
mac_version="1.2.0"
make_threads=8
ONESHOT_PATH=$HOME/Library/Application\ Support/Steam/steamapps/common/OneShot
# Colors
white="\033[0;37m"      # White - Regular
bold="\033[1;37m"       # White - Bold
cyan="\033[1;36m"       # Cyan - Bold
green="\033[1;32m"      # Green - Bold
color_reset="\033[0m"   # Reset Colors

use_qmake=false
with_steamshim=true


echo "${white}Compiling ${bold}SyngleChance v${mac_version} ${white}engine for macOS...${color_reset}\n"

if [ ! -e build ]; then
	mkdir build
fi
cd build

# Set version number -- being replaced by CMake steps
echo "-> ${cyan}Set version number...${color_reset}"
if [[ $use_qmake == true ]]; then m4 ../patches/mac/Info.plist.in -DONESHOTMACVERSION=$mac_version > ./Info.plist; fi
m4 ../patches/mac/JournalInfo.plist.in -DONESHOTMACVERSION=$mac_version > ./JournalInfo.plist

# Generate makefile and build main + journal
if [[ $use_qmake == true ]]; then
	cd ..
	echo "-> ${cyan}Generate makefile...${color_reset}"
	MRIVERSION=2.7 qmake -spec macx-xcode
	echo "-> ${cyan}Compile engine...${color_reset}"
	xcodebuild
	mkdir build/bin
	mv "./Release/oneshot.app" "./build/bin/OneShot.app"
	rm -rf Release
	if [[ $with_steamshim == true ]]; then
		echo "-> ${cyan}Compile steamshim...${color_reset}"
		cd build
		if [ ! -e steamshim ]; then
			mkdir steamshim
		fi
		cd steamshim
		cmake ../../steamshim_parent
		make -j${make_threads}
		cd ..
	fi
else
	echo "-> ${cyan}Install dependencies...${color_reset}"
	conan install .. --build=missing -o platform=$([ $with_steamshim == true ] && echo "steam" || echo "standalone")
	echo "-> ${cyan}Compile engine...${color_reset}"
	conan build ..
fi
echo "-> ${cyan}Compile journal...${color_reset}"
cd ..
pyinstaller journal/unix/journal.spec --onefile --windowed
rm -rf build/_______
rm -rf build/_______.app
mv dist/* build
rm -r dist
rm -rf journal/unix/__pycache__

# Create app bundles
echo "-> ${cyan}Create app bundles...${color_reset}"
OSX_App="./build/bin/OneShot.app"
ContentsDir="$OSX_App/Contents"
LibrariesDir="$OSX_App/Contents/Libraries"
ResourcesDir="$OSX_App/Contents/Resources"

# create directories in the @target@.app bundle
if [ ! -e $LibrariesDir ]; then
	mkdir -p "$LibrariesDir"
fi

if [ ! -e $ResourcesDir ]; then
	mkdir -p "$ResourcesDir"
fi

# Steamshim
if [[ $with_steamshim == true ]]; then
	cp build/$([ $use_qmake == true ] && echo "steamshim" || echo "bin")/steamshim $OSX_App/Contents/MacOS/steamshim
	install_name_tool -change @loader_path/libsteam_api.dylib "$( cd "$(dirname "$0")" ; pwd -P )"/steamworks/redistributable_bin/osx/libsteam_api.dylib $OSX_App/Contents/macOS/steamshim
fi

# Complete OneShot bundle
if [[ $use_qmake == true ]]; then
	cmake -P patches/mac/CompleteBundle.cmake
fi

# Move files into proper locations
cp -f journal/unix/macOS/Python build/_______.app/Contents/MacOS/Python
cp assets/icon.icns $OSX_App/Contents/Resources/icon.icns
cp assets/icon_journal.icns build/_______.app/Contents/Resources/icon_journal.icns
cp steam_appid.txt $OSX_App/Contents/MacOS/steam_appid.txt
cp patches/mac/oneshot.sh $OSX_App/Contents/MacOS/oneshot.sh

if [[ $use_qmake == true ]]; then
	rm -f $OSX_App/Contents/Info.plist
	cp build/Info.plist $OSX_App/Contents/Info.plist
fi
rm -f build/_______.app/Contents/Info.plist
cp build/JournalInfo.plist build/_______.app/Contents/Info.plist

# Compile scripts
echo "-> ${cyan}Compile xScripts.rxdata...${color_reset}"
ruby rpgscript.rb ./scripts "$ONESHOT_PATH"
cp "$ONESHOT_PATH/Data/xScripts.rxdata" ./build

echo "-> ${cyan}Install OneShot apps to Steam directory...${color_reset}"
cp -rf "$OSX_App" "$ONESHOT_PATH"
cp -rf "build/_______.app" "$ONESHOT_PATH"

echo "\n${green}Complete!  ${white}Please report any issues to https://github.com/GooborgStudios/synglechance/issues${color_reset}"
