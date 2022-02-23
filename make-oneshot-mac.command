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

# Set version number
echo "-> ${cyan}Set version number...${color_reset}"
rm -f OneShot.app/Contents/Info.plist
rm -f _______.app/Contents/Info.plist
mkdir -p dist
m4 patches/mac/Info.plist.in -DONESHOTMACVERSION=$mac_version > ./dist/Info.plist
m4 patches/mac/JournalInfo.plist.in -DONESHOTMACVERSION=$mac_version > ./dist/JournalInfo.plist

# Generate makefile and build main + journal
if [[ $use_qmake == true ]]
	then
	echo "-> ${cyan}Generate makefile...${color_reset}"
	MRIVERSION=2.7 qmake -spec macx-xcode
	echo "-> ${cyan}Compile engine...${color_reset}"
	xcodebuild
	if [[ $with_steamshim == true ]]
		then
		echo "-> ${cyan}Compile steamshim...${color_reset}"
		cd steamshim_parent
		if [ ! -e build ]
			then mkdir build
		fi
		cd build
		cmake ..
		make -j${make_threads}
		cd ../..
	fi
else
	echo "-> ${cyan}Install dependencies...${color_reset}"
	if [ ! -e build ]
		then mkdir build
	fi
	cd build
	conan install .. --build=missing -o platform=$([ $with_steamshim == true ] && echo "steam" || echo "standalone")
	echo "-> ${cyan}Compile engine...${color_reset}"
	conan build ..
	cd ..
fi
echo "-> ${cyan}Compile journal...${color_reset}"
pyinstaller journal/unix/journal.spec --onefile --windowed

# Create app bundles
echo "-> ${cyan}Create app bundles...${color_reset}"
OSX_App="./Release/oneshot.app"
ContentsDir="$OSX_App/Contents"
LibrariesDir="$OSX_App/Contents/Libraries"
ResourcesDir="$OSX_App/Contents/Resources"

# create directories in the @target@.app bundle
if [ ! -e $LibrariesDir ]
	then
	mkdir -p "$LibrariesDir"
fi

if [ ! -e $ResourcesDir ]
	then
	mkdir -p "$ResourcesDir"
fi

rm -rf ./OneShot.app
mv ./Release/oneshot.app ./OneShot.app

# Steamshim
if [[ $with_steamshim ]]
	then
	cp steamshim_parent/build/steamshim ./OneShot.app/Contents/MacOS/steamshim
	install_name_tool -change @loader_path/libsteam_api.dylib "$( cd "$(dirname "$0")" ; pwd -P )"/steamworks/redistributable_bin/osx/libsteam_api.dylib ./OneShot.app/Contents/macOS/steamshim
fi

# Move files into proper locations
cp -f journal/unix/macOS/Python dist/_______.app/Contents/MacOS/Python
cmake -P patches/mac/CompleteBundle.cmake
cp assets/icon.icns ./OneShot.app/Contents/Resources/icon.icns
cp assets/icon_journal.icns dist/_______.app/Contents/Resources/icon_journal.icns
cp steam_appid.txt ./OneShot.app/Contents/MacOS/steam_appid.txt
cp patches/mac/oneshot.sh ./OneShot.app/Contents/MacOS/oneshot.sh
rm -rf _______.app
cp -r dist/_______.app _______.app
rm -f ./OneShot.app/Contents/Info.plist
rm -f ./_______.app/Contents/Info.plist
cp ./dist/Info.plist ./OneShot.app/Contents/Info.plist
cp ./dist/JournalInfo.plist ./_______.app/Contents/Info.plist

# Compile scripts
echo "-> ${cyan}Compile xScripts.rxdata...${color_reset}"
ruby rpgscript.rb ./scripts "$ONESHOT_PATH"
cp "$ONESHOT_PATH/Data/xScripts.rxdata" .

echo "-> ${cyan}Install OneShot apps to Steam directory...${color_reset}"
cp -rf "./OneShot.app" "$ONESHOT_PATH"
cp -rf "./_______.app" "$ONESHOT_PATH"

# Cleanup
echo "-> ${cyan}Cleanup files...${color_reset}"
# make clean
rm -rf journal/unix/__pycache__
rm -rf build
rm -rf dist

echo "\n${green}Complete!  ${white}Please report any issues to https://github.com/GooborgStudios/synglechance/issues${color_reset}"
