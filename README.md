# SyngleChance

This is a specialized fork of [mkxp by Ancurio](https://github.com/Ancurio/mkxp) designed for [*OneShot*](http://oneshot-game.com/).

## Prepare

There are a few prerequesites you must install before proceeding.  They are:

- [Conan](https://conan.io/downloads.html)
- [CMake](https://cmake.org/)
- Windows only:
	- Visual Studio
- macOS only:
	- Xcode
- Linux only:
	- [linuxdeploy](https://github.com/linuxdeploy/linuxdeploy)
	- [AppImageTool](https://github.com/AppImage/AppImageKit)
	- `sudo apt install libgtk2.0-dev libxfconf-0-dev python3-venv libxcb-xinerama`

If you are compiling a version for Steam, you will also need to download the [Steamworks SDK](https://partner.steamgames.com/). Once you have downloaded it, place the folder in the project root and rename it to `steamworks`.

Once you have installed these dependencies, Conan will take care of the remaining ones. Run the following commands to configure Conan:

```sh
conan remote add eliza https://rkevin.jfrog.io/artifactory/api/conan/eliza
conan remote add queengooborg https://queengooborg.jfrog.io/artifactory/api/conan/default-conan
conan remote add bincrafters https://bincrafters.jfrog.io/artifactory/api/conan/public-conan
conan config set general.revisions_enabled=1
```

On Windows, additionally run the following:

```sh
setx CONAN_USE_ALWAYS_SHORT_PATHS 1
```

## Building

Building the engine is tested and supported on Windows, macOS, and Ubuntu Linux.

### Windows

Launch a command prompt that can run Visual Studio commands.  The easiest way to do this is to open Visual Studio and click `Tools` -> `Command Line` -> `Developer Command Prompt` (or `Developer PowerShell` if prefered).  Then, run the following commands:

```sh
mkdir build
cd build
conan install .. --build=missing
conan build ..
```

### macOS

All of the steps on macOS are automated by a single command.  Simply run the following:

```sh
./make-mac.command
```

### Linux

```sh
./make-linux.sh
./make-appimage.sh . build </path/to/game/files> /build/_______ /build/_______.AppImage`
```

## Installation

Simply copy the generated binaries in the `build/` folder to your OneShot game folder, available through Steam.  By design, the game will not run without a copy of the game through Steam.

Installation is automatic using the following command on macOS or Linux:
```sh
./install-mac.command # macOS
./install-linux.sh # Linux
```

## Running OneShot

These depend on the SDL auxiliary libraries. *OneShot* only makes use of bmp/png for images and oggvorbis/wav for audio.

To run *OneShot*, you should have a graphics card capable of at least **OpenGL (ES) 2.0** with an up-to-date driver installed.

## Configuration

*OneShot* reads configuration data from the file "oneshot.conf". The format is ini-style. Do *not* use quotes around file paths (spaces won't break). Lines starting with '#' are comments. See 'oneshot.conf.sample' for a list of accepted entries.

All option entries can alternatively be specified as command line options. Any options that are not arrays (eg. preloaded scripts) specified as command line options will override entries in oneshot.conf. Note that you will have to wrap values containing spaces in quotes (unlike in oneshot.conf).

The syntax is: `--<option>=<value>`

Example: `./oneshot --gameFolder="oneshot" --vsync=true`

## Credits

Please view `CREDITS.txt`.
