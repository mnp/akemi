#include "win_oper.h"
static void regdir(struct stat *stbuf)
{
	stbuf->st_mode = S_IFDIR | 0700;
	stbuf->st_nlink = 2;
}

static void regfile(struct stat *stbuf)
{
	stbuf->st_mode = S_IFREG | 0600;
	stbuf->st_nlink = 1;
}

int win_getattr(int wid, struct stat *stbuf)
{
	regdir(stbuf);
	return 0;
}

int border_getattr(int wid, struct stat *stbuf)
{
	regdir(stbuf);
	return 0;	
}


int border_color_getattr(int wid, struct stat *stbuf)
{
	regfile(stbuf);
	return 0;
}

int border_color_read(int wid, char *buf, size_t size, off_t offset)
{
	return 0;
}

int border_color_write(int wid, const char *buf, size_t size, off_t offset)
{
	return 0;
}

int border_width_getattr(int wid, struct stat *stbuf)
{
	regfile(stbuf);
	return 0;
}

int border_width_read(int wid, char *buf, size_t size, off_t offset)
{
	return 0;
}

int border_width_write(int wid, const char *buf, size_t size, off_t offset)
{
	return 0;
}

int geometry_getattr(int wid, struct stat *stbuf)
{
	regdir(stbuf);
	return 0;
}

int geometry_width_getattr(int wid, struct stat *stbuf)
{
	regfile(stbuf);
	return 0;
}

int geometry_width_read(int wid, char *buf, size_t size, off_t offset)
{
	return 0;
}

int geometry_width_write(int wid, const char *buf, size_t size, off_t offset)
{
	return 0;
}

int geometry_height_getattr(int wid, struct stat *stbuf)
{
	regfile(stbuf);
	return 0;
}

int geometry_height_read(int wid, char *buf, size_t size, off_t offset)
{
	return 0;
}

int geometry_height_write(int wid, const char *buf, size_t size, off_t offset)
{
	return 0;
}

int geometry_x_getattr(int wid, struct stat *stbuf)
{
	regfile(stbuf);
	return 0;
}

int geometry_x_read(int wid, char *buf, size_t size, off_t offset)
{
	return 0;
}

int geometry_x_write(int wid, const char *buf, size_t size, off_t offset)
{
	return 0;
}

int geometry_y_getattr(int wid, struct stat *stbuf)
{
	regfile(stbuf);
	return 0;
}

int geometry_y_read(int wid, char *buf, size_t size, off_t offset)
{
	return 0;
}

int geometry_y_write(int wid, const char *buf, size_t size, off_t offset)
{
	return 0;
}

int mapstate_getattr(int wid, struct stat *stbuf)
{
	regfile(stbuf);
	return 0;
}

int mapstate_read(int wid, char *buf, size_t size, off_t offset)
{
	return 0;
}

int mapstate_write(int wid, const char *buf, size_t size, off_t offset)
{
	return 0;
}

int ignored_getattr(int wid, struct stat *stbuf)
{
	regfile(stbuf);
	return 0;
}

int ignored_read(int wid, char *buf, size_t size, off_t offset)
{
	return 0;
}

int ignored_write(int wid, const char *buf, size_t size, off_t offset)
{
	return 0;
}

int stack_getattr(int wid, struct stat *stbuf)
{
	stbuf->st_mode = S_IFREG | 0200;
	stbuf->st_nlink = 1;
	return 0;
}

int stack_write(int wid, const char *buf, size_t size, off_t offset)
{
	return 0;
}

int title_getattr(int wid, struct stat *stbuf)
{
	stbuf->st_mode= S_IFREG | 0400;
	stbuf->st_nlink = 1;
	return 0;
}

int title_read(int wid, char *buf, size_t size, off_t offset)
{
	return 0;
}

int class_getattr(int wid, struct stat *stbuf)
{
	stbuf->st_mode= S_IFREG | 0400;
	stbuf->st_nlink = 1;
	return 0;
}

int class_read(int wid, char *buf, size_t size, off_t offset)
{
	return 0;
}
