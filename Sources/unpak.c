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

#include "unpak.h"

int		PackFileOpen (zipfile_t *pf)
{
	uLong			x, max;
	unz_file_info	file_info;

	pf->uf = unzOpen (pf->pak_name);
	if (!pf->uf)
		return 0;
	unzGetGlobalInfo (pf->uf, &pf->gi);
	pf->fi = (fileinfo_t *)Z_Malloc (sizeof(fileinfo_t)*pf->gi.number_entry, false);
	if (!pf->fi)
	{
		unzClose (pf->uf);
		return 0;
	}

	pf->hash = Com_HashKey(pf->pak_name);
	max = pf->gi.number_entry;

	for (x=0; x<max; x++)
	{
		unzGetCurrentFileInfo (pf->uf,&file_info, pf->fi[x].name,sizeof(pf->fi[x].name),NULL,0,NULL,0);
		unzGetFilePos (pf->uf, &pf->fi[x].pos);
		pf->fi[x].attr = file_info.external_fa;
		pf->fi[x].size = file_info.uncompressed_size;
		pf->fi[x].hash = Com_HashKey(pf->fi[x].name);
		unzGoToNextFile (pf->uf);
	}
	return 1;
}

void	PackFileClose (zipfile_t *pf)
{
	if (pf->fi)
	{
		Z_Free (pf->fi);
		pf->fi = NULL;
	}
	if (pf->uf)
	{
		unzClose (pf->uf);
		pf->uf = NULL;
	}
}

int		PackFileGet (zipfile_t *pf, char *fname, char **buf, unsigned hash)
{
	uLong	num;
	int		err;
	for (num = 0; num<pf->gi.number_entry; num++)
		if (hash == pf->fi[num].hash)
			if (!b_stricmp(fname, pf->fi[num].name))
				break;

	if (num >= pf->gi.number_entry)
		return -1;

	*buf = (char *)Z_Malloc (pf->fi[num].size, true);

	unzGoToFilePos (pf->uf, &pf->fi[num].pos);

	unzOpenCurrentFile (pf->uf);
	err = unzReadCurrentFile (pf->uf,*buf,pf->fi[num].size);
	unzCloseCurrentFile (pf->uf);

	if (err!=pf->fi[num].size)
	{
		Z_Free (*buf);
		*buf = NULL;
		return -2;
	}

	return pf->fi[num].size;
}

int		PackFileGetFilesNumber (zipfile_t *pf)
{
	return pf->gi.number_entry;
}
/*
char	*PackFileGetFileName (zipfile_t *pf, int num)
{
	if (num >= (int)pf->gi.number_entry)
		return NULL;
	return pf->fi[num].name;
}
*/
int	PackFileSize (zipfile_t *pf, char *fname, unsigned hash)
{
	uLong num;
	for (num = 0; num<pf->gi.number_entry; num++)
		if (hash == pf->fi[num].hash)
			if (!b_stricmp(fname, pf->fi[num].name))
				return pf->fi[num].size;
	return -1;
}
