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

# Generate makefile and build
echo "-> ${cyan}Install dependencies...${color_reset}"
conan install .. --build=missing -o platform=$([ $with_steamshim == true ] && echo "steam" || echo "standalone") -o debug=$([ $debug == true ] && echo "True" || echo "False") -s arch=$([ $arm == true ] && echo "armv8" || echo "x86_64") -s os.version=$([ $arm == true ] && echo "11.0" || echo "10.13")
echo "-> ${cyan}Compile engine...${color_reset}"
conan build ..
cd ..

# Create app bundles
echo "-> ${cyan}Create app bundles...${color_reset}"
AppBundle="./build/bin/OneShot.app"
ContentsDir="$AppBundle/Contents"
LibrariesDir="$ContentsDir/Libraries"
ResourcesDir="$ContentsDir/Resources"

JournalAppBundle="./build/bin/_______.app"
JournalContentsDir="$JournalAppBundle/Contents"
JournalLibrariesDir="$JournalContentsDir/Libraries"
JournalResourcesDir="$JournalContentsDir/Resources"

# create directories in the @target@.app bundle
mkdir -p "$LibrariesDir"
mkdir -p "$ResourcesDir"
mkdir -p "$JournalLibrariesDir"
mkdir -p "$JournalResourcesDir"

# Steamshim
if [[ $with_steamshim == true ]]; then
	cp ./build/bin/steamshim $AppBundle/Contents/MacOS/steamshim
	install_name_tool -change @loader_path/libsteam_api.dylib "$( cd "$(dirname "$0")" ; pwd -P )"/steamworks/redistributable_bin/osx/libsteam_api.dylib $AppBundle/Contents/macOS/steamshim
fi

# Move files into proper locations
cp assets/icon.icns $ResourcesDir/icon.icns
cp assets/icon_journal.icns $JournalResourcesDir/icon_journal.icns
cp steam_appid.txt $ContentsDir/MacOS/steam_appid.txt
cp patches/mac/oneshot.sh $ContentsDir/MacOS/oneshot.sh

# Complete app bundles
cmake -DBUNDLE_NAME="OneShot.app" -P patches/mac/CompleteBundle.cmake
cmake -DBUNDLE_NAME="_______.app" -P patches/mac/CompleteBundle.cmake

# Fixup Qt plugins (XXX hopefully there's a better way to do this)
install_name_tool -change @rpath/libQt5PrintSupport.5.dylib @executable_path/../Libraries/libQt5PrintSupport.5.dylib ${JournalContentsDir}/Plugins/platforms/libqcocoa.dylib
install_name_tool -change @rpath/libQt5Widgets.5.dylib @executable_path/../Libraries/libQt5Widgets.5.dylib ${JournalContentsDir}/Plugins/platforms/libqcocoa.dylib
install_name_tool -change @rpath/libQt5Gui.5.dylib @executable_path/../Libraries/libQt5Gui.5.dylib ${JournalContentsDir}/Plugins/platforms/libqcocoa.dylib
install_name_tool -change @rpath/libQt5Core.5.dylib @executable_path/../Libraries/libQt5Core.5.dylib ${JournalContentsDir}/Plugins/platforms/libqcocoa.dylib

install_name_tool -change @rpath/libQt5Widgets.5.dylib @executable_path/../Libraries/libQt5Widgets.5.dylib ${JournalContentsDir}/Plugins/styles/libqmacstyle.dylib
install_name_tool -change @rpath/libQt5Gui.5.dylib @executable_path/../Libraries/libQt5Gui.5.dylib ${JournalContentsDir}/Plugins/styles/libqmacstyle.dylib
install_name_tool -change @rpath/libQt5Core.5.dylib @executable_path/../Libraries/libQt5Core.5.dylib ${JournalContentsDir}/Plugins/styles/libqmacstyle.dylib

echo "\n${green}Complete!  ${white}Please report any issues to https://github.com/GooborgStudios/synglechance/issues${color_reset}"
