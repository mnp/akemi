#include "fs.h"
#include "win.h"

int main(int argc, char** argv)
{
	xcb_init();

	int exit_status = fuse_init(argc, argv);
	fuse_cleanup();

	xcb_cleanup();
	return exit_status;
}
