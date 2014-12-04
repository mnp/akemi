CC=cc
LD=$(CC)
LIBS=-lfuse -lxcb
CFLAGS=-Wall -D_FILE_OFFSET_BITS=64 -c

SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)

all: akemi
	
$(OBJ): $(SRC)
	echo $(CC) $(CFLAGS) akemi.c
	$(CC) $(CFLAGS) akemi.c
	$(CC) $(CFLAGS) fs.c
	$(CC) $(CFLAGS) win.c

akemi: $(OBJ)
	$(LD) $(LIBS) akemi.o fs.o win.o -o akemi
