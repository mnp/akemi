all:
	gcc -Wall akemi.c `pkg-config fuse --cflags --libs` `pkg-config xcb --cflags --libs` -o akemi
