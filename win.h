#ifndef WIN_H
#define WIN_H

#define WID_STRING_LENGTH 11
#define UNMAPPED 0
#define UNVIEWABLE 1
#define VIEWABLE 2

void xcb_init();
void xcb_cleanup();


int focused();
int exists(int wid);
int get_width(int wid);
int get_height(int wid);
int get_x(int wid);
int get_y(int wid);
int get_border_width(int wid);
int get_map_state(int wid);
int get_ignored(int wid);
char *get_title(int wid);
char **get_class(int wid);
int *list_windows();

#endif
