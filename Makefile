all: taggrepper

LDFLAGS += -lmagic -lid3tag -lvorbisfile -lpcre
CFLAGS += -ggdb -Wall

taggrepper: taggrepper.o files.o tagregexps.o

files.o: files.c files.h common.h tagregexps.h
taggrepper.o: taggrepper.c common.h tagregexps.h files.h
tagregexps.o: tagregexps.c tagregexps.h files.h common.h

clean:
	$(RM) -f files.o taggrepper.o tagregexps.o taggrepper

