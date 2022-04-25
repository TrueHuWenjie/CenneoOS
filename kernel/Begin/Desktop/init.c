// Cenneo OS
// /kernel/Begin/Desktop/init.c
// Kernel Initialization for Desktop OS

#include "../begin.h"

// Start in Desktop mode
void begin_desktop_mode(void)
{
    // Graphical User Interface
	init_gui();

	// Operating System Moniter
	extern int osm_open(void);
	task(&osm_open, NULL);

	// Kernel Log System Monitor
	extern int klsm_open(void);
	klsm_open();
}
