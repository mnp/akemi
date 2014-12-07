#ifndef WIN_H
#define WIN_H
#define WID_STRING_LENGTH 10

void xcb_init();
void xcb_cleanup();

int exists(int wid);
int hidden(int wid);
int *list_windows();
void get_geom(int wid, int *width, int *height, int *x, int *y);
void set_geom(int wid, int widht, int height, int x, int y);

#endif
