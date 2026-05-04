/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/mbox.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(init_sync);

int init_sync_signal_stack_ready(void)
{
    const struct mbox_dt_spec tx_channel = MBOX_DT_SPEC_GET(DT_PATH(mbox_consumer), tx);
    int ret;

    ret = mbox_send_dt(&tx_channel, NULL);
    if(ret < 0) {
        LOG_ERR("Could not send (%d)", ret);
        return -1;
    }

    LOG_INF("signal sent");

    return ret;
}

#define INIT_DONE_EVENT BIT(1)

static struct k_event init_events;

static void callback(const struct device *dev, mbox_channel_id_t channel_id,
             void *user_data, struct mbox_msg *data)
{
    k_event_post((struct k_event *)user_data, INIT_DONE_EVENT);
}

int init_sync_wait_for_stack_ready(void)
{
    const struct mbox_dt_spec rx_channel = MBOX_DT_SPEC_GET(DT_PATH(mbox_consumer), rx);
    uint32_t events = 0;
    int ret;

    k_event_init(&init_events);

    ret = mbox_register_callback_dt(&rx_channel, callback, &init_events);
    if (ret < 0) {
        LOG_ERR("Could not register callback (%d)", ret);
        return -1;
    }

    ret = mbox_set_enabled_dt(&rx_channel, true);
    if (ret < 0) {
        LOG_ERR("Could not enable RX channel %d (%d)", rx_channel.channel_id, ret);
        return -1;
    }

    LOG_INF("setup done");

    while (events != INIT_DONE_EVENT) {
        events = k_event_wait(&init_events, INIT_DONE_EVENT, false, K_FOREVER);
    }

    LOG_INF("message received");

    return ret;
}
