// Cenneo OS
// /kernel/osm/osm.c
// Operating System Moniter

#include <stdio.h>
#include <mmu.h>
#include <gui.h>
#include <kmm.h>
#include <mpt/mpt.h>
#include "../Architectures/x86/include/i8254.h"

#define OSM_WIN_WIDTH 480
#define OSM_WIN_HEIGHT 320

//static struct window *osm_win;
struct window *osm_win;
static float *cpu_usage;
static int cpu_update_index;
static int invalid;
static union thread *osm_id;

void osm_display(void)
{
    struct layer *layer = osm_win->layer;
    char buf[32];
    char *status;
    static union thread *entry;
    float usage;

    gui_put_square(layer, 0xffd1eeee, 245, 32, 240, 320);

    // All the strings
    window_put_string(osm_win, 8, 8, "Performance:");
    window_put_string(osm_win, (OSM_WIN_WIDTH / 2) + 8, 8, "Threads:");
    window_put_string(osm_win, (OSM_WIN_WIDTH / 2) + 8, 24, \
    "Name        Status  CPU  tid");
    window_put_string(osm_win, 8, 32, "Memory:");
    window_put_string(osm_win, 8, (OSM_WIN_HEIGHT / 2) + 8, "CPU:");
    gui_put_square(layer, 0xffffffff, 13, 164, 213, 32);
    sprintf(buf, "user:%.1fMB", 0.0);
    window_put_string(osm_win, 8, 132, buf);
    sprintf(buf, "kernel:%.1fMB", 256.0 - ((double)kmm_info_free() / 1048576.0));
    window_put_string(osm_win, 8, 148, buf);
    sprintf(buf, "free:%.1fMB", (double)pmb_info_free() / 256.0);
    window_put_string(osm_win, 124, 132, buf);

    // Memory usage
    gui_put_square(layer, 0xdd8470ff, 20, 96, 200 - \
        (200 * pmb_info_free() / pmb_info_total()), 60);
    gui_put_square(layer, 0x80000000, 220 - (200 * pmb_info_free() / \
    pmb_info_total()), 96, 200 * pmb_info_free() / pmb_info_total(), 60);
    gui_line(layer, 0xff191970, 20, 96, 220, 96);
    gui_line(layer, 0xff191970, 20, 156, 220, 156);
    gui_line(layer, 0xff191970, 20, 96, 20, 156);
    gui_line(layer, 0xff191970, 220, 96, 220, 157);

    // CPU usage
    gui_line(layer, 0xff191970, 20, 246, 220, 246);
    gui_line(layer, 0xff191970, 20, 306, 220, 306);
    gui_line(layer, 0xff191970, 20, 246, 20, 306);
    gui_line(layer, 0xff191970, 220, 246, 220, 307);

    // Thread info
    entry = current;
    for (int i = 0; i < 16; i ++)
    {
        switch (entry->info.state)
        {
            case THREAD_HIGH: status = "Run";
                            break;
            case THREAD_SLEEP: status = "Sleep";
            case THREAD_ZOMBIE: status = "Zomble";
        }

        usage = (float)(entry->info.runtime - entry->info.lastsecond) / 10.0;
        sprintf(buf, "%-12.12s %s   %4.1f%%  %d", entry->info.name, status, usage, entry->info.tid);
        entry->info.lastsecond = entry->info.runtime;
        if (buf[11] != 0x20)
        {
            buf[9] = '.';
            buf[10] = '.';
            buf[11] = '.';
        }
        window_put_string(osm_win, (OSM_WIN_WIDTH / 2) + 8, 40 + i * 16, buf);

        entry = entry->info.next;
        if (entry == current) break;
    }

    // Update
    gui_update(layer);
}

void osm_update(void)
{
    cpu_update_index ++;

    if (cpu_update_index >= 200) cpu_update_index %= 200;

    invalid = 1;

    wakeup(osm_id);
}

void osm_loop(void)
{
    while (1)
    {
        if (invalid)
        {
            invalid = 0;
            osm_display();
            sleep();
        }
    }
}

int osm_open(void)
{
    osm_id = current;

    // Set name
    task_name("OS Moniter");

    // Create window
    osm_win = gui_window("Operating System Moniter", WINDOW_NORMAL, 0, 0, \
    OSM_WIN_WIDTH, OSM_WIN_HEIGHT);


    cpu_usage = kmalloc (200 * sizeof(float), 0);
    for (int i = 0; i < 100; i ++)
    {
        cpu_usage[i] = i;
        cpu_usage[i + 100] = i;
    }

    cpu_update_index = 0;
    settimer(&osm_update, 1000, 0);

    invalid = 1;

    gui_set_position(osm_win->layer, 10, 360);

    //
    osm_loop();
}