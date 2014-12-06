#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "akemi.h"
#include "fs.h"
#include "win.h"

static int akemi_getattr(const char *path, struct stat *stbuf)
{
	memset(stbuf, 0, sizeof(struct stat));
	if(strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	}else
		return -ENOENT;
	return 0;
}

static int akemi_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;
	if (strcmp(path, "/") != 0)
		return -ENOSYS;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	int *wins = list_windows();
	int i;
	for(i=0; wins[i]; i++){
		int win = wins[i];

		char *win_string;
		win_string = malloc(sizeof(char)*(WID_STRING_LENGTH));

		sprintf(win_string, "0x%08x", win);
		filler(buf, win_string, NULL, 0);

		free(win_string);
	}
	free(wins);
		
	return 0;
}

static struct fuse_operations akemi_oper = {
	.destroy = akemi_cleanup,
	.getattr = akemi_getattr,
	.readdir = akemi_readdir,
	.open = NULL,
	.read = NULL,
};

int fuse_init(int argc, char **argv)
{
	return fuse_main(argc, argv, &akemi_oper, NULL);
}
