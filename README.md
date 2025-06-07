# Berserker@Quake2 Engine
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/tx00100xt/BerserkerQuake2)](https://github.com/tx00100xt/BerserkerQuake2/releases/tag/1.45.1-beta1)
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

### Linux and OpenBSD

#### Setting up the repository

Type this in your terminal:

```
git clone https://github.com/tx00100xt/BerserkeкQuake2.git
```

#### Copy official game data (optional)

If you have original Quake II CD, the data can be found in Install/Data/baseq2. In addition to  
the data on the CD, you will need the data from the official Quake II 3.20 patch, which you can get from  
http://deponie.yamagi.org/quake2/idstuff/q2-3.20-x86-full-ctf.exe  
The content can be extracted using any utility for working with ZIP files.  

If you have access to a copy of the game (either by CD or through Steam),  
you can copy the **pak0-2.pak** files and **video** directory from the game   
directory to the repository (put it in the **baseq2 repository directory**).  

#### NOTE:
**pak0-2.pak** and **video** directory must have lowercase names.

#### Building

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

### Extra data game content

Then you will have to get some extra data that comes with this engine. You can get it from  
[Release section](https://github.com/tx00100xt/BerserkerQuake2/releases). The content itself is identical to those hosted at official website and ModDB,  
except all file names were converted  to lower case. The contents of the *.fx and *.sp3 files  
were edited so that the paths to the textures files were in lower case  and references to them,  
as well as paths have been fixed to use slashes instead of backslashes. Each **pk2** archive   
contains a GPL-2.0 license file copied from the official archive on ModDB.  

### Running the game
After building and placing the game data, you can launch the games.  
Type this in your terminal:  

```
cd BerserkerQuake2
./berserkerq2
```

#### NOTE:
Non-standard screen resolution can be specified in the **modelist.txt** file.

Supported Architectures
----------------------
* `x86`

Supported OS
-----------
* `Linux`
* `OpenBSD`


License
-------

* BerserkerQuake2 is licensed under the GNU GPL v2 (see LICENSE file).
* BerserkerQuake2 extra data game content is licensed under the GNU GPL v2 (see LICENSE file).

