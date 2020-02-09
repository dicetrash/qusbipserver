// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * command structure borrowed from udev
 * (git://git.kernel.org/pub/scm/linux/hotplug/udev.git)
 *
 * Copyright (C) 2011 matt mooney <mfm@muteddisk.com>
 *               2005-2007 Takahiro Hirofuchi
 */

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>
#include <syslog.h>

#include "usbip_common.h"
#include "usbip_network.h"
#include "usbip.h"
#include "../config.h"

static int usbip_help(int argc, char *argv[]);
static int usbip_version(int argc, char *argv[]);

static const char usbip_version_string[] = PACKAGE_STRING;

static const char usbip_usage_string[] =
	"usbip [--debug] [--log] [--tcp-port PORT] [version]\n"
	"             [help] <command> <args>\n";

static void usbip_usage(void)
{
	printf("usage: %s", usbip_usage_string);
}

struct command {
	const char *name;
	int (*fn)(int argc, char *argv[]);
	const char *help;
	void (*usage)(void);
};

static const struct command cmds[] = {
	{
		.name  = "help",
		.fn    = usbip_help,
		.help  = NULL,
		.usage = NULL
	},
	{
		.name  = "version",
		.fn    = usbip_version,
		.help  = NULL,
		.usage = NULL
	},
	{
		.name  = "attach",
		.fn    = usbip_attach,
		.help  = "Attach a remote USB device",
		.usage = usbip_attach_usage
	},
	{
		.name  = "detach",
		.fn    = usbip_detach,
		.help  = "Detach a remote USB device",
		.usage = usbip_detach_usage
	},
	{
		.name  = "list",
		.fn    = usbip_list,
		.help  = "List exportable or local USB devices",
		.usage = usbip_list_usage
	},
	{
		.name  = "bind",
		.fn    = usbip_bind,
		.help  = "Bind device to " USBIP_HOST_DRV_NAME ".ko",
		.usage = usbip_bind_usage
	},
	{
		.name  = "unbind",
		.fn    = usbip_unbind,
		.help  = "Unbind device from " USBIP_HOST_DRV_NAME ".ko",
		.usage = usbip_unbind_usage
	},
	{
		.name  = "port",
		.fn    = usbip_port_show,
		.help  = "Show imported USB devices",
		.usage = NULL
	},
	{ NULL, NULL, NULL, NULL }
};

static int usbip_help(int argc, char *argv[])
{
	const struct command *cmd;
	int i;
	int ret = 0;

	if (argc > 1 && argv++) {
		for (i = 0; cmds[i].name != NULL; i++)
			if (!strcmp(cmds[i].name, argv[0]) && cmds[i].usage) {
				cmds[i].usage();
				goto done;
			}
		ret = -1;
	}

	usbip_usage();
	printf("\n");
	for (cmd = cmds; cmd->name != NULL; cmd++)
		if (cmd->help != NULL)
			printf("  %-10s %s\n", cmd->name, cmd->help);
	printf("\n");
done:
	return ret;
}

static int usbip_version(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	printf(PROGNAME " (%s)\n", usbip_version_string);
	return 0;
}

static int run_command(const struct command *cmd, int argc, char *argv[])
{
	dbg("running command: `%s'", cmd->name);
	return cmd->fn(argc, argv);
}
