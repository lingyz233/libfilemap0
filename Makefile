.PHONY: all shared static clean

all: libfilemap.a libfilemap.so
	@echo done

shared: libfilemap.so

static: libfilemap.a

libfilemap.a: filemap.o
	ar -rcs $@ $^

libfilemap.so: filemap.o
	gcc -shared $^ -o $@

filemap.o: filemap.c
	gcc -fPIC -c $^

clean:
	rm -f filemap.o libfilemap.a libfilemap.so
