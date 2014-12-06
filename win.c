#include <stdlib.h>
#include <stdio.h>
#include <xcb/xcb.h>
#include <err.h>
#include "win.h"
#include "akemi.h"

#define WID_STRING_LENGTH 10

static xcb_connection_t *conn;
static xcb_screen_t *scrn;

char **list_windows()
{
	xcb_query_tree_cookie_t tree_c = xcb_query_tree(conn, scrn->root);
	xcb_query_tree_reply_t *tree_r = xcb_query_tree_reply(conn, tree_c, NULL);

	if(tree_r == NULL){
		akemi_cleanup();
		errx(1, "Cannot find root window");
	}

	xcb_window_t *xcb_win_list = xcb_query_tree_children(tree_r);
	if(xcb_win_list == NULL){
		akemi_cleanup();
		errx(1, "Couldn't find the root window's children");
	}

	char **win_list = malloc(sizeof(char*)*(tree_r->children_len+1));
	int i;
	for (i=0; i<tree_r->children_len; i++) {
		win_list[i] = malloc(sizeof(char[WID_STRING_LENGTH]));
		sprintf(win_list[i], "0x%08x", xcb_win_list[i]);
	}
	win_list[tree_r->children_len] = NULL;
	return win_list;
}

void xcb_init()
{
        conn = xcb_connect(NULL, NULL);
        if(xcb_connection_has_error(conn))
                errx(1, "Cannot open display: %s", getenv("DISPLAY"));

        scrn = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
        if(scrn == NULL)
                errx(1, "Cannot retrieve screen information");
}

void xcb_cleanup()
{
        if(conn != NULL)
                xcb_disconnect(conn);
}
