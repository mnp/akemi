#ifndef WIN_H
#define WIN_H
#define WID_STRING_LENGTH 10

void xcb_init();
void xcb_cleanup();

int exists(int wid);
int hidden(int wid);
int *list_windows();

#endif
