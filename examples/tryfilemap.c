/*
 * This program opens or creates "./numbers", and fill it with "0123456789012345"
 * You can compile it with "gcc ./tryfilemap.c -L.. -lfilemap"
 *
 * Ce programme ouvre "./numbers" et écrit "0123456789012345" dans ce fichier
 * Compilez-vous avec "gcc ./tryfilemap.c -L.. -lfilemap"
*/

#include"filemap.h"

#include<fcntl.h>
#include<sys/mman.h>

int
main (void)
{
  struct filemap fm;
//_Bool (*open) (struct filemap * fmap, char * path, int fflag, int flags, int prot, int fmode, size_t hoped_size);
  filemap.open (&fm, "./numbers", O_RDWR|O_CREAT, MAP_SHARED, PROT_READ|PROT_WRITE,0755,16);
  filemap.resize (&fm, 16);
  for (int i = 0; i < 16; i++)
    fm.d[i] = '0' + i % 10;
  filemap.close (&fm);
  return 0;
}


