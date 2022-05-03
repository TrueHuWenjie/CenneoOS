// Cenneo OS
// /kernel/Begin/Server/init.c
// Kernel Initialization for Server OS

#include "../begin.h"
#include <mmu.h>
// Start in Server mode
void begin_server_mode(void)
{
    //run("CONSOLE.O", NULL, 0);

    urm_print("/");

    mqtt_demo_init();

    // console_loop();
    
    return;
}
