/* Copyright (C) 1997-2001 Id Software, Inc.
   Copyright (C) 2004-2014 Serge Borodulin aka Berserker (tm)
   <http://berserker.quakegate.ru>

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#ifndef COMPAT_H
#define COMPAT_H

#ifndef _WIN32
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned char BYTE;
typedef unsigned char byte;
typedef unsigned short SHORT;

#define _stricmp strcasecmp
#define stricmp strcasecmp

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define _mkdir( x )	mkdir( x, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH )

#define socketError errno

#define EXPORT __attribute__((visibility("default")))
#else
#define ioctl ioctlsocket
#define close closesocket

#define socketError WSAGetLastError()

#define EWOULDBLOCK WSAEWOULDBLOCK
#define ECONNRESET WSAECONNRESET
#define EMSGSIZE WSAEMSGSIZE
#define EADDRNOTAVAIL WSAEADDRNOTAVAIL
#define EHOSTUNREACH WSAEHOSTUNREACH
#define ENETUNREACH WSAENETUNREACH
#define EAFNOSUPPORT WSAEAFNOSUPPORT

#define EXPORT __declspec(dllexport)
#endif

#endif
