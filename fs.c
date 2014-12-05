#define FUSE_USE_VERSION 30

#include <fuse_lowlevel.h>
#include <stdlib.h>
#include <err.h>
#include "fs.h"
#include "win.h"

static struct fuse_args args;
static char *mountpoint;
static struct fuse_chan *chan;
static struct fuse_session *sess;

static struct fuse_lowlevel_ops akemi_oper = {
	.lookup = NULL,
	.getattr = NULL,
	.readdir = NULL,
	.open = NULL,
	.read = NULL,
};

int fuse_init(int argc, char **argv){
	args.argc = argc;
	args.argv = argv;
	args.allocated = 0;
	if((mountpoint = getenv("AKEMI_ROOT")) == NULL){
		if(fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != 0)
			return 1;
	}
	if((chan = fuse_mount(mountpoint, &args)) == NULL)
		return 1;

	if((sess = fuse_lowlevel_new(&args, &akemi_oper, sizeof(&akemi_oper), NULL)) == NULL)
		return 1;
	
	if(fuse_set_signal_handlers(sess) == -1)
		return 1;

	fuse_session_add_chan(sess, chan);

	return fuse_session_loop(sess) ? 1 : 0;
}

void fuse_cleanup(){
	fuse_remove_signal_handlers(sess);
	fuse_session_remove_chan(chan);
	fuse_session_destroy(sess);
	fuse_unmount(mountpoint, chan);
	fuse_opt_free_args(&args);
}
