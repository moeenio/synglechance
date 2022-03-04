#!/bin/sh
set -e

cd `dirname $0`

# Colors
white="\033[0;37m"      # White - Regular
bold="\033[1;37m"       # White - Bold
cyan="\033[1;36m"       # Cyan - Bold
green="\033[1;32m"      # Green - Bold
color_reset="\033[0m"   # Reset Colors

ONESHOT_PATH=$HOME/.local/share/Steam/steamapps/common/OneShot

# Copy results
echo -e "-> ${cyan}Install OneShot apps to Steam directory...${color_reset}"
yes | cp -r dist/_______/* "$ONESHOT_PATH"
yes | cp oneshot "$ONESHOT_PATH"
yes | cp steamshim_parent/build/steamshim "$ONESHOT_PATH"
yes | cp build/bin/Data/xScripts.rxdata "$ONESHOT_PATH"

# Copy libraries
echo -e "-> ${cyan}Install OneShot libraries to Steam directory...${color_reset}"
mkdir libs
ldd oneshot | ruby libraries.rb
ldd steamshim_parent/build/steamshim | ruby libraries.rb
yes | cp libs/* "$ONESHOT_PATH"
rm "$ONESHOT_PATH/libgio-2.0.so.0"

echo -e "\n${green}Complete!  ${white}Please report any issues to https://github.com/GooborgStudios/synglechance/issues${color_reset}"