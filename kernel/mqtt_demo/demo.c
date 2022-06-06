
#include <network/lwip/pbuf.h>
#include <network/lwip/apps/mqtt.h>
#include <network/lwip/apps/mqtt_priv.h>
#include <network/lwip/netif.h>

const ip_addr_t test_mqtt_remote_ip = IPADDR4_INIT_BYTES(120, 232, 252, 10);

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
    struct pbuf *p;
    err_t err;

    unsigned char rxbuf[] = {0x20, 0x02, 0x00, 0x00};

    struct mqtt_connect_client_info_t client_info = {
        "1",
        "testdev", "123456",
        10,
        NULL, NULL, 0, 0
    };

    client = mqtt_client_new();

    err = mqtt_client_connect(client, &test_mqtt_remote_ip, 11884, test_mqtt_connection_cb, NULL, &client_info);
    // -4
    printk("MQTT Connect result:%d\n", err);

    //client->conn->connected(client->conn->callback_arg, client->conn, ERR_OK);
    //p = pbuf_alloc(PBUF_RAW, sizeof(rxbuf), PBUF_REF);
}


int iot_panel_open(void)
{
    // Set name
    task_name("IoT Panel");

    // Create window
    //osm_win = gui_window("Operating System Moniter", WINDOW_NORMAL, 0, 0, \
    //OSM_WIN_WIDTH, OSM_WIN_HEIGHT);
}
