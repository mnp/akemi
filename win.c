#include <stdlib.h>
#include <stdio.h>
#include <xcb/xcb.h>
#include <err.h>
#include "win.h"
#include "akemi.h"

static xcb_connection_t *conn;
static xcb_screen_t *scrn;

int exists(int wid)
{
	xcb_get_window_attributes_cookie_t attr_c = xcb_get_window_attributes(conn, wid);
	xcb_get_window_attributes_reply_t *attr_r = xcb_get_window_attributes_reply(conn, attr_c, NULL);
	
	if(attr_r == NULL)
		return 0;

	free(attr_r);
	return 1;
}

int hidden(int wid)
{
	xcb_get_window_attributes_cookie_t attr_c = xcb_get_window_attributes(conn, wid);
	xcb_get_window_attributes_reply_t *attr_r = xcb_get_window_attributes_reply(conn, attr_c, NULL);

	if(attr_r == NULL)
		return 0;

	int map_state = attr_r->map_state;
	int override_redirect = attr_r->override_redirect;

	free(attr_r);
	return (map_state != XCB_MAP_STATE_VIEWABLE) || override_redirect;
}

int *list_windows()
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

	int *win_list = malloc(sizeof(int)*(tree_r->children_len+1));
	int i;
	for (i=0; i<tree_r->children_len; i++) {
		win_list[i] = xcb_win_list[i];
	}
	free(tree_r);
	win_list[i] = 0;
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
