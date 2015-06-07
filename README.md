# Dictation Helper

![Screenshot](http://lzh9102.github.io/dictation-helper/screenshots/1.png)

This utility is a combination of an audio player and a text editor. Keyboard
shortcuts are provided to control playback of the audio player while typing.
The audio player contains the A-B points functionality to make it easier to
repeat a sentence over and over again.

## Key Features

- keyboard shortcuts for forward, backward, play/pause
- A-B loop
- insert A-B loop timestamps to editor

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

## Known Issues

- seeking is not accurate for video files (e.g. mp4)

## Acknowledgement

This project uses [qmpwidget](http://qmpwidget.sourceforge.net/#about), written
by Jonas Gehring, to interface with mplayer.

## License

This work is released under the GNU General Public License version 3. See
[COPYING](COPYING) for details.
