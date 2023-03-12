#include"filemap.h"

#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>

#include<err.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define __PRIVATE__ static
#define __PUBLIC__  static

__PUBLIC__ _Bool
filemap_openfile (struct filemap * fmap, char * path, int fflag, int flags, int prot, int fmode, size_t hoped_size);

__PUBLIC__ _Bool
filemap_newfile (struct filemap * fmap, char * path, int fflag, int flags, int prot, int fmode, size_t hoped_size);

__PUBLIC__ _Bool
filemap_newsize (struct filemap * fmap, int newsize);

__PUBLIC__ _Bool
filemap_close (struct filemap * fmap);

struct struct_filemap_functions filemap = {filemap_openfile, filemap_newfile, filemap_newsize, filemap_close};

__PRIVATE__ size_t
getmapsize (size_t needsize)
{
  static int pagesize = -1;
  if (pagesize == -1)
    pagesize = getpagesize ();
  size_t size;
  size = (needsize | (pagesize - 1)) + 1;
  size += logf (size);
  size = (size | (pagesize - 1)) + 1;
#ifdef DEBUG
  printf ("needsize : %d, pagesize : %d, finalsize %d\n", (int)needsize, (int)pagesize, (int)size);
#endif
  return size;
}

__PUBLIC__ _Bool
filemap_openfile (struct filemap * fmap, char * path, int fflag, int flags, int prot, int fmode, size_t hoped_size)
{
  int fd;
  if ((fd = open (path, fflag, fmode)) < 0)
  {
    warn ("fail to open %s", path);
    return 1;
  }

  fmap->fd = fd;

  if (fflag & O_CREAT)
  {
    if ( ftruncate (fmap->fd, hoped_size) )
    {
      warn ("fail ftruncate %s,size=%ld", fmap->path, hoped_size);
      return 1;
    }
	fmap->fsize = hoped_size;
  }
  else
  {
    fmap->fsize = lseek (fd, 0, SEEK_END);
  }

  fmap->msize = getmapsize (fmap->fsize);
  fmap->path = path;
  fmap->prot = prot;
  fmap->flags = flags;
  fmap->fflag = fflag;

  if ((fmap->d = mmap (NULL, fmap->msize, prot, flags, fd, 0)) == MAP_FAILED)
  {
    warn ("fail to mmap %s", path);
    return 1;
  }
  return 0;
}

__PUBLIC__ _Bool
filemap_newfile (struct filemap * fmap, char * path, int fflag, int flags, int prot, int fmode, size_t hoped_size)
{
  fprintf (stderr, "filemap.creat is deprecated\n");
  exit (1);
}
/*
__PUBLIC__ _Bool
filemap_newfile (struct filemap * fmap, char * path, int fflag, int flags, int prot, int fmode, size_t hoped_size)
{
  int fd;
  if ((fd = open (path, fflag, fmode)) < 0)
  {
    warn ("fail to create %s", path);
    return 1;
  }

  fmap->fsize = hoped_size;
  fmap->fd = fd;

  fmap->msize = getmapsize (fmap->fsize);
  fmap->path = path;
  fmap->prot = prot;
  fmap->flags = flags;
  fmap->fflag = fflag;

  if ((fmap->d = mmap (NULL, fmap->msize, prot, flags, fd, 0)) == MAP_FAILED)
  {
    warn ("fail to mmap %s", path);
    return 1;
  }
  return 0;
}
*/
__PUBLIC__ _Bool
filemap_newsize (struct filemap * fmap, int newsize)
{
  if ( ftruncate (fmap->fd, newsize) )
  {
    warn ("fail ftruncate %s,size=%d", fmap->path, newsize);
    return 1;
  }

  msync (fmap->d, fmap->msize, MS_ASYNC);

  const char * const path = fmap->path;
  const int prot = fmap->prot, flags = fmap->flags, fflag = fmap->fflag, fd = fmap->fd;

  if (newsize <= fmap->msize)
  {
    if (fmap->fsize > newsize)
      memset (fmap->d + newsize, 0, fmap->fsize - newsize);
    return 0;
  }

  fmap->fsize = newsize;

  if (fmap->flags & MAP_PRIVATE)
  {
    size_t newmsize = getmapsize (fmap->fsize);
	char * newd;
    if ((newd = mmap (NULL, newmsize, prot, flags, fd, 0)) == MAP_FAILED)
    {
      warn ("fail to mmap %s", path);
      return 1;
    }
	memcpy (newd, fmap->d, fmap->fsize);
	munmap (fmap->d, fmap->msize);
	fmap->d = newd;
	fmap->msize = newmsize;
	return 0;
  }


  size_t newmsize = getmapsize (fmap->fsize);

  munmap (fmap->d, fmap->msize);
  fmap->msize = newsize;

  if ((fmap->d = mmap (NULL, fmap->msize, prot, flags, fd, 0)) == MAP_FAILED)
  {
    warn ("fail to mmap %s", path);
    return 1;
  }
  return 0;
}

__PUBLIC__ _Bool
filemap_close (struct filemap * fmap)
{
  msync (fmap->d, fmap->msize, MS_ASYNC);
  const char * const path = fmap->path;
  fmap->path = 0;
  fmap->prot = 0;
  fmap->flags = 0;
  fmap->fflag = 0;
  fmap->fsize = 0;
  close (fmap->fd);
  fmap->fd = 0;
  if (munmap (fmap->d, fmap->msize) != 0)
  {
    warn ("fail to munmap %s",path);
    return 1;
  }
  fmap->d = NULL;
  fmap->msize = 0;
  return 0;
}

