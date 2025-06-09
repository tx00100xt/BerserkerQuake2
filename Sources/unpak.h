/*
===========================================================================
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 1998-2000 Vitaly Ovtchinnikov, Damir Sagidullin
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

#ifndef UNPAK_H
#define	UNPAK_H

#include <stdbool.h>
#include <minizip/unzip.h>

#ifdef _WIN32
#define MAX_OSPATH			256		// max length of a filesystem pathname (same as MAX_PATH)
#else
#define MAX_OSPATH			4096	// max length of a filesystem pathname
#endif

void *Z_Malloc (int size, bool crash);
void Z_Free (void *ptr);
unsigned Com_HashKey (const char *string);

typedef struct
{
	char			name[MAX_OSPATH];
	uLong			attr;
	unz_file_pos	pos;
	uLong			size;
	unsigned		hash;	// hash of name
} fileinfo_t;

typedef struct
{
	char			pak_name[MAX_OSPATH];
	unzFile			uf;
	unz_global_info	gi;
	fileinfo_t		*fi;
	unsigned		hash;	// hash of pak_name
} zipfile_t;

int		PackFileOpen (zipfile_t *pf);
void	PackFileClose (zipfile_t *pf);
int		PackFileGet (zipfile_t *pf, char *fname, char **buf, unsigned hash);
int		PackFileGetFilesNumber (zipfile_t *pf);
///char	*PackFileGetFileName (zipfile_t *pf, int num);
bool	b_stricmp(char *str1, char *str2);
char	b_chrt(char sym);
int		PackFileSize (zipfile_t *pf, char *fname, unsigned hash);

#endif
