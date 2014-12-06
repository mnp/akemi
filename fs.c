#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdlib.h>
#include <stdio.h>
#include "fs.h"
#include "win.h"

static struct fuse_args args;
static char *mountpoint;
static struct fuse_chan *chan;
static struct fuse *sess;

static struct fuse_operations akemi_oper = {
	.getattr = NULL,
	.readdir = NULL,
	.open = NULL,
	.read = NULL,
};

int fuse_init(int argc, char **argv)
{
	args.argc = argc;
	args.argv = argv;
	args.allocated = 0;
	if((mountpoint = getenv("AKEMI_ROOT")) == NULL){
		if(fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != 0)
			return 1;
	}
	if((chan = fuse_mount(mountpoint, &args)) == NULL)
		return 1;

	if((sess = fuse_new(chan, &args, &akemi_oper, sizeof(&akemi_oper), NULL)) == NULL)
		return 1;

	if(fuse_set_signal_handlers(fuse_get_session(sess)) == -1)
		return 1;
	
	return fuse_loop(sess) ? 1 : 0;
}

void fuse_cleanup()
{
	if(mountpoint != NULL && chan != NULL)
		fuse_unmount(mountpoint, chan);
	if(sess != NULL){
		fuse_remove_signal_handlers(fuse_get_session(sess));
		fuse_destroy(sess);
	}
	fuse_opt_free_args(&args);
}
