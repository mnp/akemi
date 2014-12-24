#include "win_oper.h"
#include "win.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *dir_read(int wid)
{
	errno = -EISDIR;
	return NULL;
}

int dir_write(int wid, const char *buf)
{
	return -EISDIR;
}

static const int regdir = 0700;
static const int regfile = 0600;

int win_mode(int wid)
{
	return regdir;
}

int border_mode(int wid)
{
	return regdir;
}

int border_color_mode(int wid)
{
	return 0200;
}

char *border_color_read(int wid)
{
	errno = -EACCES;
	return NULL;
}

int border_color_write(int wid, const char *buf)
{
	return 0;
}

int border_width_mode(int wid)
{
	return regfile;
}

char *border_width_read(int wid)
{
	int bw = get_border_width(wid);
	if(bw == -1){
		errno = -ENOENT;
		return NULL;
	}
	char *border_width_string=malloc(snprintf(NULL, 0, "%d\n", bw)+1);
	sprintf(border_width_string, "%d\n", bw);
	return border_width_string;
}

int border_width_write(int wid, const char *buf)
{
	return 0;
}

int geometry_mode(int wid)
{
	return regdir;
}

char *geometry_width_read(int wid)
{
	int w=get_width(wid);
	if(w == -1){
		errno = -ENOENT;
		return NULL;
	}
	char *width_string = malloc(snprintf(NULL, 0, "%d\n", w)+1);
	sprintf(width_string, "%d\n", w);
	return width_string;
}

int geometry_width_mode(int wid)
{
	return regfile;
}

int geometry_width_write(int wid, const char *buf)
{
	return 0;
}

char *geometry_height_read(int wid)
{
	int h=get_height(wid);
	if(h == -1){
		errno = -ENOENT;
		return NULL;
	}
	char *height_string=malloc(snprintf(NULL, 0, "%d\n", h)+1);
	sprintf(height_string, "%d\n", h);
	return height_string;
}

int geometry_height_mode(int wid)
{
	return regfile;
}

int geometry_height_write(int wid, const char *buf)
{
	return 0;
}

int geometry_x_mode(int wid)
{
	return regfile;
}

char *geometry_x_read(int wid)
{
	int x=get_x(wid);
	if(x == -1){
		errno = -ENOENT;
		return NULL;
	}
	char *x_string=malloc(snprintf(NULL, 0, "%d\n", x)+1);
	sprintf(x_string, "%d\n", x);
	return x_string;
}

int geometry_x_write(int wid, const char *buf)
{
	return 0;
}

int geometry_y_mode(int wid)
{
	return regfile;
}

char *geometry_y_read(int wid)
{
	int y=get_y(wid);
	if(y == -1){
		errno = -ENOENT;
		return NULL;
	}
	char *y_string=malloc(snprintf(NULL, 0, "%d\n", y)+1);
	sprintf(y_string, "%d\n", y);
	return y_string;
}

int geometry_y_write(int wid, const char *buf)
{
	return 0;
}

int mapstate_mode(int wid)
{
	return regfile;
}

char *mapstate_read(int wid)
{
	int ms = get_map_state(wid);
	if(ms == -1){
		errno = -ENOENT;
		return NULL;
	}
	switch(ms){
		case VIEWABLE:
			return strdup("viewable\n");
		case UNVIEWABLE:
			return strdup("unviewable\n");
		case UNMAPPED:
			return strdup("unmapped\n");
	}
	return NULL;
}

int mapstate_write(int wid, const char *buf)
{
	return 0;
}

int ignored_mode(int wid)
{
	return regfile;
}

char *ignored_read(int wid)
{
	int i=get_ignored(wid);
	if(i == -1){
		errno = -ENOENT;
		return NULL;
	}
	if(i)
		return strdup("true\n");
	else
		return strdup("false\n");
}

int ignored_write(int wid, const char *buf)
{
	return 0;
}

int stack_mode(int wid)
{
	return 0200;
}

char *stack_read(int wid)
{
	errno = -EACCES;
	return NULL;
}

int stack_write(int wid, const char *buf)
{
	return 0;
}

int title_mode(int wid)
{
	return 0400;
}

char *title_read(int wid)
{
	char *title=get_title(wid);
	if(title == NULL){
		errno = -ENOENT;
		return NULL;
	}
	char *title_string=malloc(strlen(title)+2);
	sprintf(title_string, "%s\n", title);
	free(title);
	return title_string;
}

int title_write(int wid, const char *buf)
{
	return -EACCES;
}

int class_mode(int wid)
{
	return 0400;
}

char *class_read(int wid)
{
	char **classes = get_class(wid);
	char* class_string=malloc(strlen(classes[0])+strlen(classes[1])+4);
	sprintf(class_string, "%s\n%s\n", classes[0], classes[1]);
	free(classes[0]);
	free(classes[1]);
	free(classes);
	return class_string;
}

int class_write(int wid, const char *buf)
{
	return -EACCES;
}
