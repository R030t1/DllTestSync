CC=x86_64-w64-mingw32-gcc
CF=-municode -mwindows -pedantic
BIN=TestSync.exe

.phony: clean

TestSync.exe: TestSync.c TestSync.h
	${CC} ${CF} -L. -lSync -o $@ $<

Sync.dll: Sync.c Sync.h
	${CC} -lkernel32 -municode -pedantic -shared -o $@ $<

clean:
	del ${BIN}