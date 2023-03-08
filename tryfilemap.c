#include"filemap.h"

#include<fcntl.h>
#include<sys/mman.h>

int
main (void)
{
  struct filemap fm;
  filemap.open (&fm, "./numbers", O_RDWR|O_CREAT, MAP_SHARED, PROT_READ|PROT_WRITE);
  filemap.resize (&fm, 16);
  for (int i = 0; i < 16; i++)
    fm.d[i] = '0' + i % 10;
  filemap.close (&fm);
  return 0;
}


