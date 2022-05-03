
#include <network/lwip/pbuf.h>
#include <network/lwip/apps/mqtt.h>
#include <network/lwip/apps/mqtt_priv.h>
#include <network/lwip/netif.h>

const ip_addr_t test_mqtt_remote_ip = IPADDR4_INIT_BYTES(192, 168, 1, 2);

static void test_mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    printk("test_mqtt_connection_cb\n");
    LWIP_UNUSED_ARG(client);
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(status);
}

void mqtt_demo_init(void)
{
    mqtt_client_t* client;
    struct netif netif;
    err_t err;

    struct mqtt_connect_client_info_t client_info = {
        "1",
        "testdev", "123456",
        10,
        NULL, NULL, 0, 0
    };

    err = mqtt_client_connect(client, &test_mqtt_remote_ip, 11884, test_mqtt_connection_cb, NULL, &client_info);

    printk("MQTT Connect result:%d\n", err);
}
