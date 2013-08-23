/*
 * This file Copyright (C) Mnemosyne LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 * $Id: daemon.c 14084 2013-06-07 23:31:26Z jordan $
 */

#include <errno.h>
#include <stdio.h> /* printf */
#include <stdlib.h> /* exit, atoi */
#include <libintl.h>	/* For translation */

#include <fcntl.h> /* open */
#include <signal.h>
#ifdef HAVE_SYSLOG
#include <syslog.h>
#endif
#include <unistd.h> /* daemon */

#include <event2/buffer.h>

#include <config.h>
#include <libtransmission/tr-getopt.h>

#define MY_NAME "SibSync"

static const char *
getUsage (void)
{
    return "SibSync " VERSION
           "\n"
           MY_NAME " is a test program\n"
           "that actualy does nothing.\n"
           "\n"
           "Usage: " MY_NAME " [options]";
}

static const struct tr_option options[] =
{
    { 'e', "logfile", "Dump the log messages to this filename", "e", 1, "<filename>" },
    { 0, NULL, NULL, NULL, 0, NULL }
};

static void
showUsage (void)
{
    tr_getopt_usage (MY_NAME, getUsage (), options);
    exit (0);
}

main(int argc, char ** argv)
{
    int c;
    const char * optarg;

    printf("Hello World!\n");

    /* overwrite settings from the comamndline */
    tr_optind = 1;
    while ((c = tr_getopt (getUsage (), argc, (const char**)argv, options, &optarg))) {
        switch (c) {
            case 'e': printf("Do the logfile thing\n");
                      break;
            default:  showUsage ();
                      break;
        }
    }

    printf("Worked?!\n");
    return 0;
}
