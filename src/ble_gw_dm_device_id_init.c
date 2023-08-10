/**
 * @file ble_gw_dm_device_id_init.c
 *
 * Copyright (c) 2022 Laird Connectivity LLC
 *
 * SPDX-License-Identifier: LicenseRef-LairdConnectivity-Clause
 */

/**************************************************************************************************/
/* Includes                                                                                       */
/**************************************************************************************************/
#include <zephyr/zephyr.h>
#include <zephyr/init.h>
#include <zephyr/drivers/hwinfo.h>
#include <stdio.h>
#include <attr.h>

/**************************************************************************************************/
/* Local Function Prototypes                                                                      */
/**************************************************************************************************/
static int ble_gw_dm_device_id_init(const struct device *device);

/**************************************************************************************************/
/* Global Function Definitions                                                                    */
/**************************************************************************************************/
SYS_INIT(ble_gw_dm_device_id_init, APPLICATION, CONFIG_LCZ_BLE_GW_DM_DEVICE_ID_INIT_PRIORITY);

/**************************************************************************************************/
/* SYS INIT                                                                                       */
/**************************************************************************************************/
static int ble_gw_dm_device_id_init(const struct device *device)
{
	ARG_UNUSED(device);

	char id[sizeof(uint64_t) * 2 + 1];
	char *dev_id;
	uint8_t dev_id_buff[8];
	ssize_t device_id_size;

#if defined(CONFIG_BOARD_MG100) || defined(CONFIG_BOARD_PINNACLE_100_DVK) ||                       \
	defined(CONFIG_BOARD_BL5340_DVK_CPUAPP) || defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP)
	device_id_size = hwinfo_get_device_id(dev_id_buff, sizeof(dev_id_buff));
	(void)snprintf(id, sizeof(id), "%02x%02x%02x%02x%02x%02x%02x%02x", dev_id_buff[0],
		       dev_id_buff[1], dev_id_buff[2], dev_id_buff[3], dev_id_buff[4],
		       dev_id_buff[5], dev_id_buff[6], dev_id_buff[7]);
#else
#error "Unsupported board"
#endif

	dev_id = (char *)attr_get_quasi_static(ATTR_ID_device_id);
	if (strlen(dev_id) < 1) {
		attr_set_string(ATTR_ID_device_id, id, strlen(id));
	}

	return 0;
}
