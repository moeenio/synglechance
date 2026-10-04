# SyngleChance

This is a specialized fork of [mkxp by Ancurio](https://github.com/Ancurio/mkxp) designed for [*OneShot*](http://oneshot-game.com/).

## Building & Installation
This branch only explicitly supports building with on Linux, with Meson, in
[a specific Flatpak environment](https://codeberg.org/moeenio/notcatbox/src/branch/main/org.colescaut.notcatbox.yml).
Conan doesn't work, and Linux distribution packages do not have the required Ruby version. I don't know about Windows and Mac.

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
