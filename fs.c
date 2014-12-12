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
#include "win_oper.h"

struct win_oper
{
    char *path;
	int (*getattr)(int wid, struct stat *stbuf);
    int (*read)(int wid, char *buf, size_t size, off_t offset);
    int (*write)(int wid, const char *buf, size_t size, off_t offset);
};

static const struct win_oper akemi_win_oper[] = {
	{"", win_getattr, dir_read, dir_write},
	{"/border", border_getattr, dir_read, dir_write},
		{"/border/color", border_color_getattr, border_color_read, border_color_write},
		{"/border/width", border_width_getattr, border_width_read, border_width_write},
	{"/geometry", geometry_getattr, dir_read, dir_write},
		{"/geometry/width", geometry_width_getattr, geometry_width_read, geometry_width_write},
		{"/geometry/height", geometry_height_getattr, geometry_height_read, geometry_height_write},
		{"/geometry/x", geometry_x_getattr, geometry_x_read, geometry_x_write},
		{"/geometry/y", geometry_y_getattr, geometry_y_read, geometry_y_write},
	{"/mapstate", mapstate_getattr, mapstate_read, mapstate_write},
	{"/ignored", ignored_getattr, ignored_read, ignored_write},
	{"/stack", stack_getattr, stack_read, stack_write},
	{"/title", title_getattr, title_read, title_write},
	{"/class", class_getattr, class_read, class_write},
};

static int get_winid(const char *path)
{
	int wid = 0;
	if(strncmp(path, "/0x", 3) == 0)
		sscanf(path, "/0x%08x", &wid);

	if(strncmp(path, "/focused", 8) == 0)
		wid = focused();

	if(!exists(wid))
		return -1;

	return wid;
}

static const char *get_winpath(const char *path)
{
	const char *winpath = strchr(path+1, '/');
	if(winpath==NULL)
		winpath="";
	return winpath;
}

static int akemi_getattr(const char *path, struct stat *stbuf)
{
	memset(stbuf, 0, sizeof(struct stat));
	if(strcmp(path, "/") == 0){
		stbuf->st_mode = S_IFDIR | 0700;
		stbuf->st_nlink = 2;
		return 0;
	}

	int wid = get_winid(path);
	if(wid == -1)
		return -ENOENT;

	const char *winpath = get_winpath(path);

	int i;
	for(i=0;i<sizeof(akemi_win_oper)/sizeof(struct win_oper); i++){
		if(strcmp(winpath, akemi_win_oper[i].path) == 0){
			return akemi_win_oper[i].getattr(wid, stbuf);
		}
	}
	return -ENOENT;
}

static int akemi_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	if(strcmp(path, "/") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "focused", NULL, 0);

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

	const char *winpath = get_winpath(path);
	
	int exists, dir = 0;
	int i;
	for(i=0;i<sizeof(akemi_win_oper)/sizeof(struct win_oper); i++){
		if(strcmp(winpath, akemi_win_oper[i].path) == 0)
			exists = 1;
		if((strncmp(winpath, akemi_win_oper[i].path, strlen(winpath)) == 0) 
				&& (strlen(akemi_win_oper[i].path) > strlen(winpath))
				&& (strchr(akemi_win_oper[i].path+strlen(winpath)+1, '/') == NULL)){
			dir = 1;
			filler(buf, akemi_win_oper[i].path+strlen(winpath)+1, NULL, 0);
		}
	}
	if(dir){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
	}
	else
		return -ENOTDIR;
	if(!exists)
		return -ENOENT;
	return 0;
}

static int akemi_open(const char *path, struct fuse_file_info *fi)
{
	return 0;
}

static int akemi_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int wid = get_winid(path);
	const char *winpath = get_winpath(path);
	int i;
	for(i=0;i<sizeof(akemi_win_oper)/sizeof(struct win_oper); i++){
		if(strcmp(winpath, akemi_win_oper[i].path) == 0){
			return akemi_win_oper[i].read(wid, buf, size, offset);
		}
	}
	return -ENOENT;	
}

static int akemi_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int wid = get_winid(path);
	const char *winpath = get_winpath(path);
	int i;
	for(i=0;i<sizeof(akemi_win_oper)/sizeof(struct win_oper); i++){
		if(strcmp(winpath, akemi_win_oper[i].path) == 0){
			return akemi_win_oper[i].write(wid, buf, size, offset);
		}
	}
	return -ENOENT;
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
