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
git clone https://github.com/tx00100xt/Berserker@Quake2.git
```

#### Copy official game data (optional)

If you have original Quake II CD, the data can be found in Install/Data/baseq2. In addition to  
the data on the CD, you will need the data from the official Quake II 3.20 patch, which you can get from  
http://deponie.yamagi.org/quake2/idstuff/q2-3.20-x86-full-ctf.exe  
The content can be extracted using any utility for working with ZIP files.  

If you have access to a copy of the game (either by CD or through Steam),  
you can copy the **pak0-2.pak** files and **player,video** from the game   
directory to the repository (put it in the **baseq2 repository directory**).  

#### NOTE:
**pak0-2.pak** must have lowercase names.

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

You can view additional commands for make by typing the make command in the terminal without arguments.

### Running the game

Then you will have to get some extra data that comes with this engine. You can get it from  
[Release section](https://github.com/tx00100xt/BerserkerQuake2/releases). Extract the archive in  your BerserkerQuake2 directory. The content itself  
is identical to those hosted at official website and ModDB, except nedeed file names were converted  
to lower case and the contents of the *.fx and *.sp3 files were edited so that the paths to the textures  
files were in lower case, so hopefully there isn't any problem with file  
access on Linux and BSD systems.  

After this run game.  
Type this in your terminal:  

```
cd BerserkerQuake2
./berserkerq2
```
#### NOTE:
Non-standard screen resolution can be specified in the modelist.txt file.

Supported Architectures
----------------------
* `x86`

Supported OS
-----------
* `Linux`
* `OpenBSD`

TODO:
-----
* `Perform a normal installation on the system.`
* `Use the home directory ~/berserkerq2. for a system installation.`
* `Fix code to correct multiple compiler warnings.`
* `Just fix the code in some places`

