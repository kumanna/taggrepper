all: taggrep

LDFLAGS += -lmagic -lid3tag -lvorbisfile -lpcre
CFLAGS += -ggdb -Wall

taggrep: taggrep.o files.o tagregexps.o

files.o: files.c files.h common.h tagregexps.h
taggrep.o: taggrep.c common.h tagregexps.h files.h
tagregexps.o: tagregexps.c tagregexps.h files.h common.h

clean:
	$(RM) -f files.o taggrep.o tagregexps.o taggrep

