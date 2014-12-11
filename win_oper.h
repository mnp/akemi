#ifndef WIN_OPER_H
#define WIN_OPER_H
#include <fuse.h>
int win_getattr(int wid, struct stat *stbuf);

int border_getattr(int wid, struct stat *stbuf);
int border_color_getattr(int wid, struct stat *stbuf);
int border_color_read(int wid, char *buf, size_t size, off_t offset);
int border_color_write(int wid, const char *buf, size_t size, off_t offset);
int border_width_getattr(int wid, struct stat *stbuf);
int border_width_read(int wid, char *buf, size_t size, off_t offset);
int border_width_write(int wid, const char *buf, size_t size, off_t offset);

int geometry_getattr(int wid, struct stat *stbuf);
int geometry_width_getattr(int wid, struct stat *stbuf);
int geometry_width_read(int wid, char *buf, size_t size, off_t offset);
int geometry_width_write(int wid, const char *buf, size_t size, off_t offset);
int geometry_height_getattr(int wid, struct stat *stbuf);
int geometry_height_read(int wid, char *buf, size_t size, off_t offset);
int geometry_height_write(int wid, const char *buf, size_t size, off_t offset);
int geometry_x_getattr(int wid, struct stat *stbuf);
int geometry_x_read(int wid, char *buf, size_t size, off_t offset);
int geometry_x_write(int wid, const char *buf, size_t size, off_t offset);
int geometry_y_getattr(int wid, struct stat *stbuf);
int geometry_y_read(int wid, char *buf, size_t size, off_t offset);
int geometry_y_write(int wid, const char *buf, size_t size, off_t offset);

int mapstate_getattr(int wid, struct stat *stbuf);
int mapstate_read(int wid, char *buf, size_t size, off_t offset);
int mapstate_write(int wid, const char *buf, size_t size, off_t offset);

int ignored_getattr(int wid, struct stat *stbuf);
int ignored_read(int wid, char *buf, size_t size, off_t offset);
int ignored_write(int wid, const char *buf, size_t size, off_t offset);

int stack_getattr(int wid, struct stat *stbuf);
int stack_write(int wid, const char *buf, size_t size, off_t offset);

int title_getattr(int wid, struct stat *stbuf);
int title_read(int wid, char *buf, size_t size, off_t offset);

int class_getattr(int wid, struct stat *stbuf);
int class_read(int wid, char *buf, size_t size, off_t offset);
#endif
