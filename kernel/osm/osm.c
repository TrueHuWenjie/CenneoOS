// Cenneo OS
// /kernel/osm/osm.c
// Operating System Moniter

#include <stdio.h>
#include <mmu.h>
#include <GUI.h>

#define OSM_WIN_WIDTH 480
#define OSM_WIN_HEIGHT 320

struct window *osm_win;

void osm_display(void)
{
    struct layer *layer = osm_win->layer;
    char buf[32];

    GUI_put_square(layer, 0xffd1eeee, 245, 32, 240, 320);

    // All the strings
    window_string(osm_win, 8, 8, "Performance:");
    window_string(osm_win, (OSM_WIN_WIDTH / 2) + 8, 8, "Threads:");
    window_string(osm_win, 8, 32, "Memory:");
    window_string(osm_win, 8, (OSM_WIN_HEIGHT / 2) + 8, "CPU:");
    sprintf(buf, "user:%.1fMB", 0.0);
    window_string(osm_win, 8, 132, buf);
    sprintf(buf, "kernel:%.1fMB", 256.0 - ((double)kmm_info_free() / 1048576.0));
    window_string(osm_win, 8, 148, buf);
    sprintf(buf, "free:%.1fMB", (double)pmb_info_free() / 256.0);
    window_string(osm_win, 124, 132, buf);

    // Memory usage
    GUI_put_square(layer, 0xdd0000cd, 20, 96, 200 - \
        (200 * pmb_info_free() / pmb_info_total()), 60);
    GUI_put_square(layer, 0xddffa500, 220 - (200 * pmb_info_free() / \
    pmb_info_total()), 96, 200 * pmb_info_free() / pmb_info_total(), 60);

    // CPU usage
    
}

void osm_loop(void)
{
    osm_display();
}

int osm_open(void)
{
    // Create window
    osm_win = GUI_window("Operating System Moniter", WINDOW_NORMAL, 0, 0, \
    OSM_WIN_WIDTH, OSM_WIN_HEIGHT);

    //
    osm_loop();
}
