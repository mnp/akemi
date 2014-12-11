#include <stdlib.h>
#include <xcb/xcb.h>
#include <err.h>
#include "win.h"
#include "akemi.h"

static xcb_connection_t *conn;
static xcb_screen_t *scrn;

int focused()
{
	xcb_get_input_focus_cookie_t focus_c;
	xcb_get_input_focus_reply_t *focus_r;

	focus_c = xcb_get_input_focus(conn);
	focus_r = xcb_get_input_focus_reply(conn, focus_c, NULL);

	if(focus_r == NULL)
		return -1;

	int focused = focus_r->focus;
	free(focus_r);
	return focused;
}

int exists(int wid)
{
	xcb_get_window_attributes_cookie_t attr_c = xcb_get_window_attributes(conn, wid);
	xcb_get_window_attributes_reply_t *attr_r = xcb_get_window_attributes_reply(conn, attr_c, NULL);

	if(attr_r == NULL)
		return 0;

	free(attr_r);
	return 1;
}

int set_geom(int wid, int width, int height, int x, int y)
{
	if(!exists(wid))
		return -1;

	uint32_t geom[4] = {x, y, width, height};
	uint32_t mask = XCB_CONFIG_WINDOW_WIDTH 
		| XCB_CONFIG_WINDOW_HEIGHT 
		| XCB_CONFIG_WINDOW_X 
		| XCB_CONFIG_WINDOW_Y;
	xcb_configure_window(conn, wid, mask, geom);
	xcb_flush(conn);
	return 0;
}

int get_geom(int wid, int *width, int *height, int *x, int *y)
{
	if(!exists(wid))
		return -1;

	xcb_get_geometry_cookie_t geom_c = xcb_get_geometry(conn, wid);
	xcb_get_geometry_reply_t *geom_r = xcb_get_geometry_reply(conn, geom_c, NULL);

	*width = geom_r->width;
	*height = geom_r->height;
	*x = geom_r->x;
	*y = geom_r->y;
	return 0;
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
