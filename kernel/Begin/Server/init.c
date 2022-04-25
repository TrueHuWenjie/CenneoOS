// Cenneo OS
// /kernel/Begin/Server/init.c
// Kernel Initialization for Server OS

#include "../begin.h"
#include <mmu.h>
// Start in Server mode
void begin_server_mode(void)
{
    char *path = "/ccdevddd/pci/network/rtl8139/d/c/af";
    char *test = "abc";
    URL_PRASE path_prase;
    

    //run("CONSOLE.O", NULL, 0);


    urm_print("/");
    printk("Apple Apple\n");
    return;
    path_prase = url_praser(path);

    printk("scheme:%s, hostname:%s, path:%s.\n", path_prase.scheme, path_prase.hostname, path_prase.path);


    return;

    unsigned int i = 0, len = 0;
    url_next_node(path, &i, &len);
    printk("%.*s\n", len, &path[i]);
    url_next_node(path, &i, &len);
    printk("%.*s, i:%d, len:%d\n", len, &path[i], i, len);
    url_next_node(path, &i, &len);
    printk("%.*s\n", len, &path[i]);
    
    // console_loop();
}
