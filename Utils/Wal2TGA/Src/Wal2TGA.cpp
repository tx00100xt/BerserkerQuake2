#include "stdafx.h"

#define	MAX_QPATH	64
char	walname[MAX_QPATH];
char	tganame[MAX_QPATH];

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define	MIPLEVELS	4
typedef struct miptex_s
{
	char		name[32];
	unsigned	width, height;
	unsigned	offsets[MIPLEVELS];		// four mip maps stored
	char		animname[32];			// next frame in animation chain
	int			flags;
	int			contents;
	int			value;
} miptex_t;

miptex_t	*mt;

unsigned char	r_q2pal[] =
{
	#include "q2pal.h"
};


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


bool SaveTGA (char *filename, unsigned char *pic, unsigned char *pal, int width, int height)
{
	int	i, j, p, t;
	unsigned char	*buffer;
	int c = 18+width*height*3;
	buffer = (unsigned char *) malloc(c);
	memset (buffer, 0, 18);
	buffer[2] = 2;
	buffer[12] = width&255;
	buffer[13] = width>>8;
	buffer[14] = height&255;
	buffer[15] = height>>8;
	buffer[16] = 24;

	// copy from indexed to rgb (with flip horizontal/vertical/bgr)
	for (i=18, j=height, t=width ; i<c ; i+=3)
	{
		p = pic[j*width-t];
		buffer[i] = pal[p*3+2];
		buffer[i+1] = pal[p*3+1];
		buffer[i+2] = pal[p*3];
		t--;
		if (!t)
		{
			t=width;
			j--;
		}
	}

	FILE *f = fopen (filename, "wb");
	if (!f)
	{
		printf ("Couldn't create %s\n", filename);
		free (buffer);
		return false;
	}
	fwrite (buffer, 1, c, f);
	fclose (f);
	free (buffer);
	return true;
}


#define	MAX_READ	0x10000		// read in blocks of 64k
void LoadWal (char *filename, unsigned char **pic, int *width, int *height)
{
	int				block, remaining, ofs;
	unsigned char	*out, *buf, *buf_;

	*pic = NULL;

	// load the file
	FILE	*f;
	f = fopen (filename, "rb");
	if (!f)
	{
		printf ("Couldn't open %s\n", filename);
		return;
	}

	int len = FS_filelength(f);
	buf_ = buf = (unsigned char*) malloc(len);
	remaining = len;
	while (remaining)
	{
		block = remaining;
		if (block > MAX_READ)
			block = MAX_READ;
		int read = fread(buf, 1, block, f);
		if (read <= 0)
		{
			printf("Couldn't read %s\n", filename);
			free(buf);
			return;
		}
		remaining -= read;
		buf += read;
	}
	fclose(f);

	// parse the WAL file
	mt = (miptex_t *)buf_;

	*width = mt->width;
	*height = mt->height;

	ofs = mt->offsets[0];
	out = (unsigned char*) malloc ( mt->width * mt->height );

	*pic = out;
	memcpy (out, (unsigned char *)mt + ofs, mt->width * mt->height);
	free(buf_);
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc==2)
	{
		int len = min(strlen(argv[1]), MAX_QPATH-1);
		if (len<4)
		{
			printf("Funny argument size.\n");
			goto err;
		}
		memset(&walname, 0, MAX_QPATH);
		memcpy(&walname, argv[1], len);
		memcpy(&tganame, argv[1], len);
		tganame[len-3] = 't';
		tganame[len-2] = 'g';
		tganame[len-1] = 'a';
		tganame[len] = 0;

		int				width, height;
		unsigned char	*pix;
		LoadWal(walname, &pix, &width, &height);
		if (pix)
		{
			bool ok = SaveTGA(tganame, pix, r_q2pal, width, height);
			free(pix);
			if (ok)
				printf("Converted: %s -> %s\n", walname, tganame);
		}
	}
	else
	{
		printf("Wal2TGA by Berserker, Russia, 2006.10.21\n");
err:	printf("Usage: Wal2TGA.exe <texture.wal>\n");
	}
	return 0;
}

