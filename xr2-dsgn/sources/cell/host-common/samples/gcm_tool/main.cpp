/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if defined(__linux__)
#include <unistd.h>
#endif
#include <iostream>
#include <fstream>
#include <string>
#include <gcm_tool.h>
#include "context.h"


using namespace std;
using namespace cell::Gcm;

#if defined(WIN32)
static int is_init = 0;
static int idx = 0;
static char *optarg = NULL;
static int optind = 0;
char getopt(int argc, char **argv, const char *opts)
{
    char	*cp = NULL;

    if (!is_init) {
	idx = 1;
	is_init = 1;
	optind = argc;
    }

    if (idx >= argc) {
	idx = 0;
	is_init = 0;
	return -1;
    }

    if (argv[idx][0] == '-') {
	/* found option */
	cp = (char *)memchr(opts, argv[idx][1], strlen(opts));
	if (cp != NULL) {
	    /* get argument */
	    if (cp[1] == ':') {
		optarg = argv[++idx];
	    } else {
		optarg = NULL;
	    }
	}
    } else {
	optind = idx;
    }
    idx++;
    return cp ? cp[0] : 0;
}
#endif

int options(int argc, char **argv)
{
	int ret = 0;

	int	c;
	while ((c = getopt(argc, argv, "h")) != -1){
		switch (c) {
		  case 'h':
		  case '?':
			  cerr << "create simple command\n";
			  cerr << "Usage:\t" << argv[0] << " [-h] [filename]\n";
			  ret = -1;
			  break;
		}
	}

	return ret;
}

// start entry point
int main(int argc, char **argv)
{
	// options
	if(options(argc, argv) != 0){
		return -1;
	}

	// set filename
	string str = "out.gcm";	// default
	if(argc > 1){
		str = argv[1];
	}

	// create instance
	CommandContext context;

	// open file
	context.open(str);

	// create commands
	context.create();

	// write file
	context.write();

	return 0;
}
