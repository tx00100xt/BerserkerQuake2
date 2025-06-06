/*
===========================================================================
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2004-2014 Serge Borodulin aka Berserker (tm)
                         <http://berserker.quakegate.ru>

This file is part of Berserker@Quake2 source code.

Berserker@Quake2 source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Berserker@Quake2 source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Berserker@Quake2 source code; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
===========================================================================
*/

#ifndef MAIN_H
#define MAIN_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#ifdef PLATFORM_UNIX
// ############ Unix variables ###########
extern int		sys_iSysPath;
extern int 		sys_iSysDataPath;
extern int 		sys_iHomeDataPath;
extern int 		sys_iCurrentDataPath;
extern char 	sys_strHomeDirData[2048];
extern char 	sys_strHomeDirBaseq2Data[2048];
extern char 	sys_strHomeDirRogueData[2048];
extern char 	sys_strHomeDirXatrixData[2048];
extern char 	sys_strCurrentDirData[2048];
// #######################################
#endif

#endif // MAIN_H

