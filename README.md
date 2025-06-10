# Berserker@Quake2 Engine
[![Build status](https://github.com/tx00100xt/SeriousSamClassic/actions/workflows/cibuild.yml/badge.svg)](https://github.com/tx00100xt/BerserkerQuake2/actions/)
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/tx00100xt/BerserkerQuake2)](https://github.com/tx00100xt/BerserkerQuake2/releases/tag/1.45.1)
[![Github downloads](https://img.shields.io/github/downloads/tx00100xt/BerserkerQuake2/total.svg?logo=github&logoColor=white&style=flat-square&color=E75776)](https://github.com/tx00100xt/BerserkerQuake2/releases/)

Berserker@Quake2 engine that modernizes some elements of the Quake 2 graphical system.  
This Berserker engine supports real-time per-pixel lighting and shadow volumes, parallax,  
mirrors, fog, decal system, high resolution textures, and more.

![Screenshot 00](https://raw.githubusercontent.com/tx00100xt/BerserkerQuake2/master/Images/image-1.png)
![Screenshot 01](https://raw.githubusercontent.com/tx00100xt/BerserkerQuake2/master/Images/image-2.png)
![Screenshot 02](https://raw.githubusercontent.com/tx00100xt/BerserkerQuake2/master/Images/image-3.png)
![Screenshot 03](https://raw.githubusercontent.com/tx00100xt/BerserkerQuake2/master/Images/image-4.png)
![Screenshot 04](https://raw.githubusercontent.com/tx00100xt/BerserkerQuake2/master/Images/image-5.png)
![Screenshot 05](https://raw.githubusercontent.com/tx00100xt/BerserkerQuake2/master/Images/image-6.png)

Berserker@Quake2 has been around since about 2005, but only supported on Windows. Russian  
developer behind this engine has finally published his code under GPL-2 license in hopes of  
someone porting it to Linux. Game resources for this engine pulished under GPL-2 license.  
See COPYING.txt  file in root folder of the game engine resources archive.

### Links:
- Original source code published to [SourceForge.net](http://sourceforge.net/projects/quake2berserker/).
- Game engine resources published to [ModDB](https://www.moddb.com/mods/berserkerquake2/downloads/berserkerquake2-145-full).
- Original project hosted at: [Quake Gate](http://berserker.quakegate.ru/).

**This is a port of Berserker@Quake2 engine (1.45) to SDL2 library.**

Building
--------
CMakeLists.txt and the makefile in the Sources directory should create the executable  
file **berserkereq2** and the library **libgame.so**, provided that you have all the necessary  
libraries installed.

### Depends

* `libjpeg (v8)`
* `libpng`
* `libogg`
* `libvorbis`
* `libvorbisfile`
* `SDL2 (v2.0.4 or newer recommended)`
* `zlib`
* `libminizip`

#### NOTE:
On some Linux, like Gentoo, you can use libjpeg (v62). 
In Debian based Linux use libjpeg (v8).

For Debian based linux type this in your terminal:
```
sudo apt-get update
sudo apt-get install libsdl2-dev libogg-dev libvorbis-dev
sudo apt-get install libjpeg-dev libpng-dev libminizip-dev zlib1g-dev
```

#### Setting up the repository

Type this in your terminal:

```
git clone https://github.com/tx00100xt/BerserkerQuake2.git
```

#### Copy official game data (optional)

If you have original Quake II CD, the data can be found in Install/Data/baseq2. In addition to  
the data on the CD, you will need the data from the official Quake II 3.20 patch, which you can get from  
http://deponie.yamagi.org/quake2/idstuff/q2-3.20-x86-full-ctf.exe  
The content can be extracted using any utility for working with ZIP files.  

If you have access to a copy of the game (either by CD or through Steam),  
you can copy the (**pak0.pak-pak2.pak**) files and (**player,video**) directory from the game   
directory to the repository (put it in the **baseq2 repository directory**).  

#### NOTE:
The (**pak0.pak-pak2.pak**) files and (**player,video**) directory must have lowercase names.

#### Building with CMakeLists.txt

Type this in your terminal:

```
cd BerserkerQuake2/Sources
mkdir build
cd build
cmake ..
make -j4
make install
```
If you prefer **ninja**, then add key **-GNinja** to the cmake. And replace the **make** command with **ninja**.

#### Building with Makefile

Type this in your terminal:

```
cd BerserkerQuake2/Sources

make release
make install.local
```

#### Building and install to the system

Type this in your terminal:

```
cd BerserkerQuake2/Sources
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
make -j4
sudo make install
```
or
```
cd BerserkerQuake2/Sources

make release
sudo make install
```

You can view additional commands for make by typing the make command in the terminal without arguments.

### Extra data game content

Then you will have to get some extra data that comes with this engine. You can get it from  
[Release section](https://github.com/tx00100xt/BerserkerQuake2/releases). The content itself is identical to those hosted at official website and ModDB,  
except all file names were converted  to lower case. The contents of the *.fx and *.sp3 files  
were edited so that the paths to the textures files were in lower case  and references to them,  
as well as paths have been fixed to use slashes instead of backslashes. Each **pk2** archive   
contains a GPL-2.0 license file copied from the official archive on ModDB.  

### Location of game content

Game content can be located in three places.

 - In the directory ~/.berserkerq2
 - Anywhere with portable installation

The file tree should be like this:
```
.
├── baseq2
│   ├── bsq2-01.pk2
│   ├── bsq2-02.pk2
│   ├── bsq2-03.pk2
│   ├── bsq2-04.pk2
│   ├── bsq2-05.pk2
│   ├── bsq2-06.pk2
│   ├── bsq2-07.pk2
│   ├── bsq2-08.pk2
│   ├── bsq2-09.pk2
│   ├── bsq2-10.pk2
│   ├── pak0.pak
│   ├── pak1.pak
│   ├── pak2.pak
│   ├── purepaks.lst
│   ├── cache
│   ├── player
│   └── video
│       ├── end.cin
│       ├── eou1_.cin
│       ├── eou2_.cin
│       ├── eou3_.cin
│       ├── eou4_.cin
│       ├── eou5_.cin
│       ├── eou6_.cin
│       ├── eou7_.cin
│       ├── eou8_.cin
│       ├── idlog.cin
│       └── ntro.cin
├── modelist.txt
```
The file **purepaks.lst** contains the names of the packs that will be used in the game.  
Without it, the game will not start.  
The file **modelist.txt** contains a list of allowed screen resolutions. You can edit it and add your own non-standard resolutions.  

The configuration file (**q2b_config.cfg**), as well as save and cache files, will always be located in directory **~/.berserkerq2**.  

Additional missions in the Linux/BSD version are not supported, as they require the presence of an external q2dedicated.exe must be exist in Quake2 folder.

#### NOTE:
All file and directory names must be in lower case.

### Running the game
Before the first launch, it is recommended to unpack the **cache** directory  
from the **bsq2-10.pk2** archive into **baseq2** game data directory.

```
cd <place where bsq2-10.pk2>
unzip -LL "bsq2-10.pk2" "cache/*" -d ~/.berserkerq2/baseq2
```
After building and placing the game data, you can launch the games.  
Type this in your terminal:  

```
cd BerserkerQuake2
./berserkerq2
```
or or just type in the terminal:   
```
berserkerq2
```
if you installing the game on the system.

#### NOTE:
Non-standard screen resolution can be specified in the **modelist.txt** file.

Supported Architectures
----------------------
* `x86`
* `aarch64`

Supported OS
-----------
* `Linux`
* `FreeBSD`
* `Raspberry PI OS`

### Build status
|CI|Platform|Compiler|Configurations|Platforms|Status|
|---|---|---|---|---|---|
|GitHub Actions|Ubuntu, FreeBSD, Alpine, Raspberry PI OS Lite|GCC, Clang|Release|x86, x64, armv7l, aarch64, riscv64, ppc64le, s390x, mipsel, loongarch64|![GitHub Actions Build Status](https://github.com/tx00100xt/BerserkerQuake2/actions/workflows/cibuild.yml/badge.svg)

You can download a the automatically build based on the latest commit.  
To do this, go to the [Actions tab], select the top workflows, and then Artifacts.

License
-------

* BerserkerQuake2 is licensed under the GNU GPL v2 (see LICENSE file).
* BerserkerQuake2 extra data game content is licensed under the GNU GPL v2 (see LICENSE file).

[Actions tab]: https://github.com/tx00100xt/BerserkerQuake2/actions "Download Artifacts"

