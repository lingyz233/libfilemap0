/*
 * This program opens or creates "./numbers", and fill it with "0123456789012345"
 * You can compile it with "gcc ./tryfilemap.c -L.. -lfilemap"
 *
 * Ce programme ouvre "./numbers" et ecrit "0123456789012345"
 * Compilez-vous avec "gcc ./tryfilemap.c -L.. -lfilemap"
*/

#include"filemap.h"

#include<fcntl.h>
#include<sys/mman.h>

int
main (void)
{
  struct filemap fm;
  filemap.open (&fm, "./numbers", O_RDWR|O_CREAT, MAP_SHARED, PROT_READ|PROT_WRITE); //if you use filemap.open with O_CREAT, the creation permission is 0755
  //or you can use filemap.creat (&fm, "./numbers", 0755, MAP_SHARED, PROT_READ|PROT_WRITE, 0);
  filemap.resize (&fm, 16);
  for (int i = 0; i < 16; i++)
    fm.d[i] = '0' + i % 10;
  filemap.close (&fm);
  return 0;
}


