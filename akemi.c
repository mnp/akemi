#include <stdlib.h>
#include "fs.h"
#include "win.h"

static struct fuse_operations akemi_oper = 
{
	.getattr = NULL,
	.readdir = NULL,
	.open = NULL,
	.read = NULL,
};

int main(int argc, char** argv)
{
	xcb_init();
	xcb_cleanup();

	return fuse_main(argc, argv, &akemi_oper, NULL);
}
