.PHONY: all shared static clean install install_lib_so install_head
TARGET=libfilemap.a libfilemap.so
C_HEAD=filemap.h

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

CFLAGS=-O2 -mavx -mfma --function-sections

all: libfilemap.a libfilemap.so
	@echo done

install : install_lib_so install_head

install_lib_so : all
	install -d $(DESTDIR)$(PREFIX)/lib64/
	install -m 644 $(TARGET) $(DESTDIR)$(PREFIX)/lib64/

install_head : all
	install -d $(DESTDIR)$(PREFIX)/include/filemap/
	install -m 644 $(C_HEAD) $(DESTDIR)$(PREFIX)/include/filemap/


shared: libfilemap.so

static: libfilemap.a

libfilemap.a: filemap.o
	ar -rcs $@ $^

libfilemap.so: filemap.o
	gcc -shared $^ -o $@

filemap.o: filemap.c
	gcc -fPIC -c $^ $(CFLAGS)

clean:
	rm -f filemap.o libfilemap.a libfilemap.so
