#include <stdlib.h>
#include <xcb/xcb.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
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

static xcb_get_property_reply_t *get_prop(int wid, xcb_atom_t property, xcb_atom_t type)
{
	xcb_get_property_cookie_t prop_c = xcb_get_property(conn, 0, wid, property, type, 0L, 32L);
	return xcb_get_property_reply(conn, prop_c, NULL);
}

static xcb_get_geometry_reply_t *get_geom(int wid)
{
	xcb_get_geometry_cookie_t geom_c = xcb_get_geometry(conn, wid);
	return xcb_get_geometry_reply(conn, geom_c, NULL);
}

static xcb_get_window_attributes_reply_t *get_attr(int wid)
{
	xcb_get_window_attributes_cookie_t attr_c = xcb_get_window_attributes(conn, wid);
	return xcb_get_window_attributes_reply(conn, attr_c, NULL);
}

int get_width(int wid)
{
	xcb_get_geometry_reply_t *geom_r = get_geom(wid);
	if(geom_r == NULL)
		return -1;

	int width = geom_r->width;
	free(geom_r);
	return width;
}

int get_height(int wid)
{
	xcb_get_geometry_reply_t *geom_r = get_geom(wid);
	if(geom_r == NULL)
		 return -1;

	int height = geom_r->height;
	free(geom_r);
	return height;
}

int get_x(int wid)
{
	xcb_get_geometry_reply_t *geom_r = get_geom(wid);
	if(geom_r == NULL)
		return -1;

	int x = geom_r->x;
	free(geom_r);
	return x;
}

int get_y(int wid)
{
	xcb_get_geometry_reply_t *geom_r = get_geom(wid);
	if(geom_r == NULL)
		return -1;

	int y = geom_r->y;
	free(geom_r);
	return y;
}

int get_border_width(int wid)
{
	xcb_get_geometry_reply_t *geom_r = get_geom(wid);
	if(geom_r == NULL)
		return -1;

	int bw = geom_r->border_width;
	free(geom_r);
	return bw;
}

int get_map_state(int wid)
{
	xcb_get_window_attributes_reply_t *attr_r = get_attr(wid);
	if(attr_r == NULL)
		return -1;

	int map_state = attr_r->map_state;
	free(attr_r);

	switch(map_state)
{
		case XCB_MAP_STATE_UNMAPPED:
			return UNMAPPED;
		case XCB_MAP_STATE_UNVIEWABLE:
			return UNVIEWABLE;
		case XCB_MAP_STATE_VIEWABLE:
			return VIEWABLE;
	}
	return -1;
}

int get_ignored(int wid)
{
	xcb_get_window_attributes_reply_t *attr_r = get_attr(wid);
	if(attr_r == NULL)
		return -1;

	int or = attr_r->override_redirect;
	free(attr_r);
	return or;
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

char *get_title(int wid)
{
	xcb_get_property_reply_t *prop_r = get_prop(wid, XCB_ATOM_WM_NAME, XCB_ATOM_STRING);
	if(prop_r == NULL)
		return NULL;
	
	char *title = (char *) xcb_get_property_value(prop_r);
	int len = xcb_get_property_value_length(prop_r);
	char *title_string=malloc(len+1);
	sprintf(title_string, "%.*s", len, title);
	free(prop_r);
	return title_string;
}

char **get_class(int wid)
{
	char **classes = malloc(sizeof(char*)*2);
	xcb_get_property_reply_t *prop_r = get_prop(wid, XCB_ATOM_WM_CLASS, XCB_ATOM_STRING);
	if(prop_r == NULL)
		return NULL;

	char *class; 
	class=(char *) xcb_get_property_value(prop_r);
	classes[0]=strdup(class);
	classes[1]=strdup(class+strlen(class)+1);

	free(prop_r);
	return classes;
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
		xcb_win_list[i] = XCB_WINDOW_NONE;
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
