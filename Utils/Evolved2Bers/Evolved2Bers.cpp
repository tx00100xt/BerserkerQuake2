#include "stdafx.h"


#define	MAX_QPATH			64
#define	MAX_TOKEN_CHARS		512		// max length of an individual token

char	txtname[MAX_QPATH];
char	litname[MAX_QPATH];

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


int		lights;

char	_EOF[] = "\n\x0";
int		l_EOF;

char	_LIT[] = "\"classname\" \"light\"\x0";
int		l_LIT;

char	_KAV[] = "\"\x0";
int		l_KAV;

char	_SPC[] = " \x0";
int		l_SPC;

char	_NOSH[] = 
"\"noshadow\" \"1\"\n"
"\"noshadow2\" \"1\"\x0";
int		l_NOSH;

char	_AMB[] = "{\n\"classname\" \"worldspawn\"\n\"ambient\" \"0.01 0.01 0.01\"\n}\n\x0";
int		l_AMB;



/*
==============
COM_Parse
Parse a token out of a string
==============
*/
char	com_token[MAX_TOKEN_CHARS];
char *COM_Parse (char **data_p)
{
	int		c;
	int		len;
	char	*data;

	data = *data_p;
	len = 0;
	com_token[0] = 0;

	if (!data)
	{
		*data_p = NULL;
		return "";
	}

// skip whitespace
skipwhite:
	while ( (c = *data) <= ' ')
	{
		if (c == 0)
		{
			*data_p = NULL;
			return "";
		}
		data++;
	}

// skip // comments
	if (c=='/' && data[1] == '/')
	{
		while (*data && *data != '\n')
			data++;
		goto skipwhite;
	}

// handle quoted strings specially
	if (c == '\"')
	{
		data++;
		while (1)
		{
			c = *data++;
			if (c=='\"' || !c)
			{
				com_token[len] = 0;
				*data_p = data;
				return com_token;
			}
			if (len < MAX_TOKEN_CHARS)
			{
				com_token[len] = c;
				len++;
			}
		}
	}

// parse a regular word
	do
	{
		if (len < MAX_TOKEN_CHARS)
		{
			com_token[len] = c;
			len++;
		}
		data++;
		c = *data;
	} while (c>32);

	if (len == MAX_TOKEN_CHARS)
	{
		len = 0;
	}
	com_token[len] = 0;

	*data_p = data;
	return com_token;
}


int FS_filelength (FILE *f)
{
	int		pos;
	int		end;

	pos = ftell (f);
	fseek (f, 0, SEEK_END);
	end = ftell (f);
	fseek (f, pos, SEEK_SET);

	return end;
}


void Com_sprintf (char *dest, int size, char *fmt, ...)
{
	int		len;
	va_list	argptr;
	char	bigbuffer[0x10000];

	va_start (argptr,fmt);
	len = vsprintf (bigbuffer,fmt,argptr);
	va_end (argptr);
	if (len >= size)
		printf ("Com_sprintf: overflow of %i in %i\n", len, size);
	strncpy (dest, bigbuffer, size-1);
}


bool SaveLIT (char *filename, char *s)
{
	FILE *f = fopen (filename, "wb");
	if (!f)
	{
		printf ("Couldn't create %s\n", filename);
		return false;
	}

	char	*token, *parm;
	char	str[MAX_TOKEN_CHARS+MAX_QPATH];

	fwrite (_AMB, 1, l_AMB, f);
	while (s)
	{
		token = COM_Parse (&s);
		if (!strcmp(token, "{"))
		{
			Com_sprintf (str, sizeof(str), "//Light %i\n", lights++);
			fwrite (str, 1, strlen(str), f);
			fwrite (token, 1, strlen(token), f);
			fwrite (_EOF, 1, l_EOF, f);
			fwrite (_LIT, 1, l_LIT, f);
			fwrite (_EOF, 1, l_EOF, f);
			continue;
		}
		if (!strcmp(token, "}"))
		{
			fwrite (token, 1, strlen(token), f);
			fwrite (_EOF, 1, l_EOF, f);
			continue;
		}
		if (!strcmp(token, "origin"))
		{
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (token, 1, strlen(token), f);
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (_SPC, 1, l_SPC, f);
			fwrite (_KAV, 1, l_KAV, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_SPC, 1, l_SPC, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_SPC, 1, l_SPC, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (_EOF, 1, l_EOF, f);
			continue;
		}
		if (!strcmp(token, "center"))	// ignore
		{
			parm = COM_Parse (&s);
			parm = COM_Parse (&s);
			parm = COM_Parse (&s);
			continue;
		}
		if (!strcmp(token, "radius"))
		{
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (token, 1, strlen(token), f);
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (_SPC, 1, l_SPC, f);
			fwrite (_KAV, 1, l_KAV, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_SPC, 1, l_SPC, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_SPC, 1, l_SPC, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (_EOF, 1, l_EOF, f);
			continue;
		}
		if (!strcmp(token, "style"))
		{
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (token, 1, strlen(token), f);
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (_SPC, 1, l_SPC, f);
			fwrite (_KAV, 1, l_KAV, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (_EOF, 1, l_EOF, f);
			continue;
		}
		if (!strcmp(token, "material"))		// ignore
		{
			parm = COM_Parse (&s);
			continue;
		}
		if (!strcmp(token, "color"))
		{
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (token, 1, strlen(token), f);
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (_SPC, 1, l_SPC, f);
			fwrite (_KAV, 1, l_KAV, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_SPC, 1, l_SPC, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_SPC, 1, l_SPC, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (_EOF, 1, l_EOF, f);
			continue;
		}
		if (!strcmp(token, "noShadows"))
		{
			fwrite (_NOSH, 1, l_NOSH, f);
			fwrite (_EOF, 1, l_EOF, f);
			continue;
		}
		if (!strcmp(token, "parallel"))		// ignore
		{
			continue;
		}
		if (!strcmp(token, "materialParm3"))		// ignore
		{
			parm = COM_Parse (&s);
			continue;
		}
		if (!strcmp(token, "angles"))
		{
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (token, 1, strlen(token), f);
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (_SPC, 1, l_SPC, f);
			fwrite (_KAV, 1, l_KAV, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_SPC, 1, l_SPC, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_SPC, 1, l_SPC, f);
			parm = COM_Parse (&s);
			fwrite (parm, 1, strlen(parm), f);
			fwrite (_KAV, 1, l_KAV, f);
			fwrite (_EOF, 1, l_EOF, f);
			continue;
		}






		if (token[0])
		{
			printf("Error: Token \"%s\"\n", token);
			fclose(f);
			return false;
		}
	}

	fclose (f);
	return true;
}


#define	MAX_READ	0x10000		// read in blocks of 64k
char *LoadTXT (char *filename)
{
	int		len;
	size_t	block, remaining;
	char	*txt, *buf;
	FILE	*f;

	// load the file
	f = fopen (filename, "rb");
	if (!f)
	{
		printf ("Couldn't open %s\n", filename);
		return NULL;
	}

	len = FS_filelength(f);
	buf = txt = (char*) malloc(len);
	remaining = len;
	while (remaining)
	{
		block = remaining;
		if (block > MAX_READ)
			block = MAX_READ;
		size_t read = fread(buf, 1, block, f);
		if (read <= 0)
		{
			printf("Couldn't read %s\n", filename);
			free(txt);
			return NULL;
		}
		remaining -= read;
		buf += read;
	}
	fclose(f);

	return txt;
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc==2)
	{
		size_t len = min(strlen(argv[1]), MAX_QPATH-1);
		if (len<4)
		{
			printf("Funny argument.\n");
			goto err;
		}
		memset(&txtname, 0, MAX_QPATH);
		memcpy(&txtname, argv[1], len);
		memcpy(&litname, argv[1], len);
		litname[len-3] = 'l';
		litname[len-2] = 'i';
		litname[len-1] = 't';
		litname[len] = 0;

		lights = 0;
		l_EOF = strlen(_EOF);
		l_LIT = strlen(_LIT);
		l_KAV = strlen(_KAV);
		l_SPC = strlen(_SPC);
		l_NOSH = strlen(_NOSH);
		l_AMB = strlen(_AMB);

		char *txt=LoadTXT(txtname);
		if (txt)
		{
			bool ok = SaveLIT(litname, txt);
			free(txt);
			if (ok)
				printf("Converted: %s -> %s\n", txtname, litname);
		}
	}
	else
	{
		printf("Q2E to Berserker@Quake2 reLight converter, Russia, 2007\n");
err:	printf("Usage: Evolved2Bers.exe <mapname.txt>\n");
	}
	return 0;
}

