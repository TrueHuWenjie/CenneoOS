// Cenneo OS
// /kernel/klsm/klsm.c
// Kernel Log System Monitor

#include <stdio.h>
#include <mpt.h>
#include <GUI.h>

#define KLSM_WIN_WIDTH 480
#define KLSM_WIN_HEIGHT 320

static struct window *klsm_win;

int klsm_open(void)
{
    // Set name
    task_name("Kernel Log System Moniter");

    // Create window
    klsm_win = gui_window("Kernel Log System Moniter", WINDOW_NORMAL, 0, 0, \
    KLSM_WIN_WIDTH, KLSM_WIN_HEIGHT);


}
