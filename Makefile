all: taggrep

LDFLAGS += -lmagic -lid3tag -lvorbisfile
CFLAGS += -ggdb

taggrep: taggrep.o files.o

files.o: files.c files.h taggrep.h
taggrep.o: taggrep.c taggrep.h

clean:
	$(RM) -f files.o taggrep.o taggrep
