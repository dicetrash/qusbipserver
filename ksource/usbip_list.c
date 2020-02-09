// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2011 matt mooney <mfm@muteddisk.com>
 *               2005-2007 Takahiro Hirofuchi
 * Copyright (C) 2015-2016 Samsung Electronics
 *               Igor Kotrasinski <i.kotrasinsk@samsung.com>
 *               Krzysztof Opasiak <k.opasiak@samsung.com>
 * Modified By
 * 	2020 dice14u - striped console and made it return a result
 */

#include <sys/types.h>
#include <libudev.h>

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>
#include <netdb.h>
#include <unistd.h>

#include <dirent.h>

#include <linux/usb/ch9.h>

#include "usbip_common.h"

struct usbip_external_list {
  char* busid;
  char* idVendor;
  char* idProduct;
  char* product_name;

  struct usbip_external_list* next;
};

void usbip_external_list_free(struct usbip_external_list* device) {
    free(device->product_name);
    free(device->busid);
    free(device->idVendor);
    free(device->idProduct);
    free(device);
};

struct usbip_external_list* list_devices()
{
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;
	struct usbip_external_list* last = NULL;
	struct usbip_external_list* current = NULL;
	struct usbip_external_list* first = NULL;
	const char *path;
	const char *idVendor;
	const char *idProduct;
	const char *bConfValue;
	const char *bNumIntfs;
	const char *busid;
	char product_name[128];
	const char *devpath;

	/* Create libudev context. */
	udev = udev_new();

	/* Create libudev device enumeration. */
	enumerate = udev_enumerate_new(udev);

	/* Take only USB devices that are not hubs and do not have
	 * the bInterfaceNumber attribute, i.e. are not interfaces.
	 */
	udev_enumerate_add_match_subsystem(enumerate, "usb");
	udev_enumerate_add_nomatch_sysattr(enumerate, "bDeviceClass", "09");
	udev_enumerate_add_nomatch_sysattr(enumerate, "bInterfaceNumber", NULL);
	udev_enumerate_scan_devices(enumerate);

	devices = udev_enumerate_get_list_entry(enumerate);

	/* Show information about each device. */
	udev_list_entry_foreach(dev_list_entry, devices) {

		current = (struct usbip_external_list*)malloc(sizeof(struct usbip_external_list));
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, path);

		/* Ignore devices attached to vhci_hcd */
		devpath = udev_device_get_devpath(dev);
		if (strstr(devpath, USBIP_VHCI_DRV_NAME)) {
			dbg("Skip the device %s already attached to %s\n",
			    devpath, USBIP_VHCI_DRV_NAME);
			continue;
		}

		/* Get device information. */
		idVendor = udev_device_get_sysattr_value(dev, "idVendor");
		idProduct = udev_device_get_sysattr_value(dev, "idProduct");
		bConfValue = udev_device_get_sysattr_value(dev,
				"bConfigurationValue");
		bNumIntfs = udev_device_get_sysattr_value(dev,
				"bNumInterfaces");
		busid = udev_device_get_sysname(dev);
		if (!idVendor || !idProduct || !bConfValue || !bNumIntfs) {
			err("problem getting device attributes: %s",
			    strerror(errno));
			goto err_out;
		}

		/* Get product name. */
		usbip_names_get_product(product_name, sizeof(product_name),
					strtol(idVendor, NULL, 16),
					strtol(idProduct, NULL, 16));

		current->next = NULL;
		current->busid = strdup(busid);
		current->product_name = strdup(product_name);
		current->idVendor = strdup(idVendor);
		current->idProduct = strdup(idProduct);

		if (last != NULL) {
		    last->next = current;
		}
		if (first == NULL) {
		    first = current;
		}
		last = current;

		udev_device_unref(dev);
	}

err_out:
	udev_enumerate_unref(enumerate);
	udev_unref(udev);

	// usbip_names_free(); -- bug out on a server
	return first;
}
