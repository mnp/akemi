#ifndef WIN_H
#define WIN_H
#define WID_STRING_LENGTH 11

void xcb_init();
void xcb_cleanup();


int focused();
int exists(int wid);
int *list_windows();
int get_geom(int wid, int *width, int *height, int *x, int *y);
int set_geom(int wid, int widht, int height, int x, int y);

#endif
