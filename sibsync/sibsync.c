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
#include <libtransmission/transmission.h>
#include <libtransmission/tr-getopt.h>
#include <libtransmission/log.h>

// I guess it is necessary for OS with systemd like Fedora
#ifdef USE_SYSTEMD_DAEMON
 #include <systemd/sd-daemon.h>
#else
 static void sd_notify (int status UNUSED, const char * str UNUSED) { }
 static void sd_notifyf (int status UNUSED, const char * fmt UNUSED, ...) { }
#endif

#define MY_NAME "SibSync"

#define MEM_K 1024
#define MEM_K_STR "KiB"
#define MEM_M_STR "MiB"
#define MEM_G_STR "GiB"
#define MEM_T_STR "TiB"

#define DISK_K 1000
#define DISK_B_STR  "B"
#define DISK_K_STR "kB"
#define DISK_M_STR "MB"
#define DISK_G_STR "GB"
#define DISK_T_STR "TB"

#define SPEED_K 1000
#define SPEED_B_STR  "B/s"
#define SPEED_K_STR "kB/s"
#define SPEED_M_STR "MB/s"
#define SPEED_G_STR "GB/s"
#define SPEED_T_STR "TB/s"

#define LOGFILE_MODE_STR "a+"

static const char *logfileName = NULL;
static FILE *logfile = NULL;

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

static void
printMessage (FILE * logfile, int level, const char * name, const char * message, const char * file, int line)
{
    if (logfile != NULL)
    {
        char timestr[64];
        tr_logGetTimeStr (timestr, sizeof (timestr));
        if (name)
            fprintf (logfile, "[%s] %s %s (%s:%d)\n", timestr, name, message, file, line);
        else
            fprintf (logfile, "[%s] %s (%s:%d)\n", timestr, message, file, line);
    }
#ifdef HAVE_SYSLOG
    else /* daemon... write to syslog */
    {
        int priority;

        /* figure out the syslog priority */
        switch (level) {
            case TR_LOG_ERROR: priority = LOG_ERR; break;
            case TR_LOG_DEBUG: priority = LOG_DEBUG; break;
            default:           priority = LOG_INFO; break;
        }

        if (name)
            syslog (priority, "%s %s (%s:%d)", name, message, file, line);
        else
            syslog (priority, "%s (%s:%d)", message, file, line);
    }
#endif
}

static void
pumpLogMessages (FILE * logfile)
{
    const tr_log_message * l;
    tr_log_message * list = tr_logGetQueue ();

    for (l=list; l!=NULL; l=l->next)
        printMessage (logfile, l->level, l->name, l->message, l->file, l->line);

    if (logfile != NULL)
        fflush (logfile);

    tr_logFreeQueue (list);
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
            case 'e': logfile = fopen (optarg, LOGFILE_MODE_STR);
                      if (logfile)
                          logfileName = optarg;
                      else
                          fprintf (stderr, "Couldn't open \"%s\": %s\n", optarg, tr_strerror (errno));
                      break;
            default:  showUsage ();
                      break;
        }
    }

    printMessage (logfile, TR_LOG_INFO, MY_NAME, "Starting...", __FILE__, __LINE__);
    
    printf("Worked?!\n");
    return 0;
}
