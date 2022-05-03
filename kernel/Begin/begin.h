

#ifndef BEGIN_H_
#define BEGIN_H_

#include <stdarg.h>
#include <stdlib.h>
#include <types.h>
#include <lib/string.h>
#include <lib/gpl.h>
#include <lib/font.h>
#include <time.h>
#include <../FileSystem/fat32/fat32.h>
#include <gui.h>
#include <mpt/mpt.h>
#include <arch.h>
#include <drivers/i8042.h>
#include <kmm.h>
#include <kls.h>
#include <graphics.h>
#include <kvi.h>
#include <lib/url.h>
#include <network/lwip/init.h>
#include <Console.h>

// Start in Embedded mode
void begin_embedded_mode(void);

// Start in Server mode
void begin_server_mode(void);

// Start in Desktop mode
void begin_desktop_mode(void);

#endif
