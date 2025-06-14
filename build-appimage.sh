#!/bin/sh

          rm -fr src
          cp -axv Sources src

          sudo apt-get update -qq
          sudo apt-get install -qq -y libsdl2-dev libogg-dev libvorbis-dev libsdl2-image-dev &&
          sudo apt-get install -qq -y zlib1g-dev libglew-dev libjpeg-dev libpng-dev libminizip-dev zlib1g-dev
          echo Remove the echo after 2025-04-01. The Ubuntu 20.04 Actions runner will be fully unsupported by 2025-04-01
          echo sudo add-apt-repository universe
          echo sudo apt install libfuse2 

          patch --verbose -p1 < AppImage/patches/0001-berserkerq2-AppImage.patch
          cd src
          make release
          make install.local
          cd ..

          cd AppImage
          cp -axv App AppDir
          cp -r ../berserkerq2 ../AppImage/AppDir/usr/bin
          cp -r ../baseq2/libgame.so ../AppImage/AppDir/usr/bin/baseq2

          wget -nv -c https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
          chmod +x linuxdeploy-x86_64.AppImage

          SIGN=1 ./linuxdeploy-x86_64.AppImage --executable AppDir/usr/bin/berserkerq2 --desktop-file AppDir/usr/share/applications/io.github.tx00100xt.BerserkerQuake2.desktop --icon-file AppDir/usr/share/icons/hicolor/128x128/apps/berserkerq2.png --appdir AppDir --output appimage

          mv -f BerserkerQuake2-x86_64.AppImage BerserkerQuake2-1.45.1-glibc-2.23-x86_64.AppImage

