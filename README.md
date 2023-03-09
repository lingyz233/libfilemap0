# libfilemap0
## compile
you can run ``make`` to get a libfilemap.so and a libfilemap.a <br />
or ``make static`` to get only libfilemap.a <br />
or ``make shared`` to get only libfilemap.so

## usage
there are 3 functions : `filemap.open, filemap.resize, filemap.close` <br />
they are defined as :
```
struct struct_filemap_functions {
  _Bool (*open) (struct filemap * fmap, char * path, int fflag, int flags, int prot, int fmode, size_t hoped_size);
  _Bool (*resize) (struct filemap * fmap, int newsize);
  _Bool (*close) (struct filemap * fmap);
} filemap;
```
#### filemap.open ()
arguments `fflag` and `fmode` is passed to open () : `open (path, fflag, fmode)`, <br />
arguments `flags` and `prot` is passed to `mmap ()`, <br />
argument `hoped_size` is ignored if there is no `O_CREAT` in argument `fflag`. If there is an `O_CREAT` flag in argument `fflag`, the `hoped_size` is passed to `ftruncate ()` to specify the file size. <br />

#### filemap.resize ()
argument `newsize` is passed to `ftruncate ()` to resize the file. <br />
If `newsize` is smaller than original size, the file will be cut off, if `newsize` is bigger than original size, the new space will be filled with zero.

#### filemap.close ()
close and unmap the file

