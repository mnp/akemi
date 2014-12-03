#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>

static void xcb_init(xcb_connection_t *conn, xcb_screen_t *scrn);
static void xcb_cleanup(xcb_connection_t *conn);

static  xcb_connection_t *conn;
static xcb_screen_t *scrn;

static void xcb_init(xcb_connection_t *conn, xcb_screen_t *scrn)
{
	conn = xcb_connect(NULL, NULL);
	if(xcb_connection_has_error(conn))
		errx(1, "Cannot open display: %s", getenv("DISPLAY"));

	scrn = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
	if(scrn == NULL)
		errx(1, "Cannot retrieve screen information");
}

static void xcb_cleanup(xcb_connection_t *conn)
{
	if(conn != NULL)
		xcb_disconnect(conn);
}

int main(int argc, char** argv)
{
	xcb_init(conn, scrn);
	xcb_cleanup(conn);

	return 0;
}