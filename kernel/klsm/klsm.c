// Cenneo OS
// /kernel/klsm/klsm.c
// Kernel Log System Monitor

#include <stdio.h>
#include <kls.h>
#include <mpt.h>
#include <lib/fonts/font.h>
#include <GUI.h>

#define KLSM_WIN_WIDTH 480
#define KLSM_WIN_HEIGHT 320

static struct window *klsm_win;
static unsigned int x = 0, y = 0;
static struct font_info *font_info;

static void klsm_listener(const char level, const char *log)
{
    unsigned long i;
    unsigned int color=0xffffffff;
    
    // Different color for different level of logs
    if (level == KLS_LOGLEVEL_ERROR)      color = 0xffff0000;
    else if (level <= KLS_LOGLEVEL_FAULT) color = 0xff800000;
    else if (level <= KLS_LOGLEVEL_WARN)  color = 0xff400000;
    else if (level <= KLS_LOGLEVEL_NOTE)  color = 0xff0000e0;
    else if (level <= KLS_LOGLEVEL_DEBUG) color = 0xff000000;

    while (log[i])
    {
        window_put_char(klsm_win, color, font_info, x, y, log[i]);
        x += font_info->length;
        
        if (x >= KLSM_WIN_WIDTH)
        {
            x = 0;
            y += font_info->width;
        }

        i ++;
    }

    gui_update(klsm_win->layer);
}

int klsm_open(void)
{
    font_info = font("Standard Font");

    // Set name
    task_name("Kernel Log System Moniter");

    // Create window
    klsm_win = gui_window("Kernel Log System Moniter", WINDOW_NORMAL, 0, 0, \
    KLSM_WIN_WIDTH, KLSM_WIN_HEIGHT);

    gui_set_position(klsm_win->layer, 500, 360);

    // Update
    gui_update(klsm_win->layer);

    kls_set_intercept(klsm_listener);

    return 0;
}
