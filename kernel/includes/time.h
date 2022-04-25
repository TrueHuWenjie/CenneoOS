/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Time Definition
 * Explorer/arch/x86/kernel/time.h
 * version:Aplha
 * 1/18/2014 12:12 PM
 */

#ifndef TIME_H_
#define TIME_H_

#include <types.h>

// Kernel time
struct ktime_def
{
    ulong century;
    ulong year, month, day;
    ulong hour, mintus, second;
    ulong week_day;
    ulong ms;
    ulong system_runtime;
};

extern struct ktime_def ktime;

#include <time.h>

#endif