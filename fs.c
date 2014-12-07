#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "akemi.h"
#include "fs.h"
#include "win.h"

static int readdir_root (fuse_fill_dir_t filler, void *buf)
{
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

static int readdir_win(fuse_fill_dir_t filler, void *buf, int win)
{
	if(!exists(win))
		return -ENOENT;
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, "geometry", NULL, 0);
	return 0;
}

static int win_getattr(const char *path, struct stat *stbuf)
{
	if(strcmp(path, "/geometry")==0){
		stbuf->st_mode = S_IFREG | 0600;
		stbuf->st_nlink = 1;
		stbuf->st_size = (sizeof(char)*19);
		stbuf->st_uid = getuid();
		stbuf->st_gid = getgid();
		stbuf->st_atime = stbuf->st_mtime = stbuf->st_ctime = time(NULL);
		return 0;
	}
	return -ENOENT;
}

static int akemi_getattr(const char *path, struct stat *stbuf)
{
	memset(stbuf, 0, sizeof(struct stat));
	if(strcmp(path, "/") == 0){
		stbuf->st_mode = S_IFDIR | 0700;
		stbuf->st_nlink = 2;
		return 0;
	}
	if(strncmp(path, "/0x", 3) == 0){
		if(strlen(path)>(WID_STRING_LENGTH+3)){
			return win_getattr(path+WID_STRING_LENGTH+1, stbuf);
		}
		int wid;
		sscanf((path+3), "%x", &wid);
		if(!exists(wid))
			return -ENOENT;
		stbuf->st_mode = S_IFDIR | 0700;
		stbuf->st_nlink = 2;
		return 0;
	}
	return -ENOENT;
}

static int akemi_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;
	if (strcmp(path, "/") == 0)
		return readdir_root(filler, buf);

	if(strncmp(path, "/0x", 3) == 0){
		int wid;
		sscanf((path+3), "%x", &wid);
		return readdir_win(filler, buf, wid);
	}
	else
		return -ENOENT;
}

static int akemi_open(const char *path, struct fuse_file_info *fi)
{
	if (strcmp(path+WID_STRING_LENGTH+1, "/geometry") != 0)
		return -ENOENT;
	return 0;
}

static int akemi_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int wid;
	sscanf((path+3), "%x", &wid);
	char *geom = malloc(sizeof(char)*20);
	int width, height, x, y;
	get_geom(wid, &width, &height, &x, &y);
	sprintf(geom, "%dx%d+%d+%d\n", width, height, x, y);
	size_t len = strlen(geom);
	if(offset < len){
		if(offset + size > len)
			size = len - offset;
		memcpy(buf, geom + offset, size);
	}else
		size = 0;
	return size;	
}

static int akemi_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int wid, width, height, x, y;
	sscanf((path+3), "%x", &wid);
	sscanf(buf, "%dx%d+%d+%d", &width, &height, &x, &y);
	set_geom(wid, width, height, x, y);
	return size;
}

static int akemi_truncate(const char *path, off_t size)
{
	return 0;
}

static struct fuse_operations akemi_oper = {
	.destroy = akemi_cleanup,
	.truncate = akemi_truncate,
	.getattr = akemi_getattr,
	.readdir = akemi_readdir,
	.open = akemi_open,
	.read = akemi_read,
	.write = akemi_write,
};

int fuse_init(int argc, char **argv)
{
	return fuse_main(argc, argv, &akemi_oper, NULL);
}
