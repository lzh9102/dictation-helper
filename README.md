# Dictation Helper

This utility is a combination of an audio player and a text editor. Keyboard
shortcuts are provided to control playback of the audio player while typing.
The audio player contains the A-B points functionality to make it easier to
repeat a sentence over and over again.

## Dependencies

The following packages are required to build this program:

- Qt4 or Qt5
- cmake

This program uses [mplayer](https://www.mplayerhq.hu/design7/news.html) as its
audio player. You need to have mplayer installed to run this program.

## Building

Open the source directory in a terminal and type the following commands:

```
mkdir build
cd build
cmake ..
make
```
